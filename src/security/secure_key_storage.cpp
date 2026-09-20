// BSD 3-Clause License
// Copyright (c) 2025, 🍀☀🌕🌥 🌊
// See the LICENSE file in the project root for full license information.

#include <kcenon/logger/security/secure_key_storage.h>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <aclapi.h>
#include <limits>
#include <memory>

namespace kcenon::logger::security {
namespace {

struct close_handle {
    void operator()(void* value) const {
        if (value && value != INVALID_HANDLE_VALUE) {
            CloseHandle(value);
        }
    }
};
using native_handle = std::unique_ptr<void, close_handle>;

struct free_local {
    void operator()(void* value) const { LocalFree(value); }
};
using local_buffer = std::unique_ptr<void, free_local>;

struct current_user {
    std::vector<uint8_t> storage;

    bool query() {
        HANDLE raw = nullptr;
        if (!OpenThreadToken(GetCurrentThread(), TOKEN_QUERY, TRUE, &raw)) {
            if (GetLastError() != ERROR_NO_TOKEN ||
                !OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &raw)) {
                return false;
            }
        }
        native_handle token(raw);
        DWORD size = 0;
        GetTokenInformation(token.get(), TokenUser, nullptr, 0, &size);
        if (size == 0) {
            return false;
        }
        storage.resize(size);
        return GetTokenInformation(token.get(), TokenUser, storage.data(), size, &size) != FALSE;
    }

    PSID sid() const {
        return reinterpret_cast<const TOKEN_USER*>(storage.data())->User.Sid;
    }
};

bool has_private_dacl(HANDLE file, PSID user) {
    PACL acl = nullptr;
    PSECURITY_DESCRIPTOR raw = nullptr;
    const auto status = GetSecurityInfo(file, SE_FILE_OBJECT, DACL_SECURITY_INFORMATION,
                                       nullptr, nullptr, &acl, nullptr, &raw);
    local_buffer descriptor(raw);
    // A null DACL grants access to everyone, unlike an empty DACL.
    if (status != ERROR_SUCCESS || !acl || !IsValidAcl(acl)) {
        return false;
    }
    bool can_read = false;
    for (DWORD index = 0; index < acl->AceCount; ++index) {
        void* entry = nullptr;
        if (!GetAce(acl, index, &entry)) {
            return false;
        }
        const auto* header = static_cast<const ACE_HEADER*>(entry);
        if (header->AceType == ACCESS_DENIED_ACE_TYPE) {
            continue;
        }
        // Fail closed for other principals and unrecognized ACE forms.
        if (header->AceType != ACCESS_ALLOWED_ACE_TYPE) {
            return false;
        }
        auto* allowed = static_cast<ACCESS_ALLOWED_ACE*>(entry);
        if (!EqualSid(&allowed->SidStart, user)) {
            return false;
        }
        can_read |= (allowed->Mask & (FILE_READ_DATA | GENERIC_READ | GENERIC_ALL)) != 0;
    }
    return can_read;
}

} // namespace

common::VoidResult secure_key_storage::save_key_windows(
    const secure_key& key, const std::filesystem::path& path) {
    current_user user;
    if (!user.query()) {
        return make_logger_void_result(logger_error_code::file_permission_denied,
                                       "Failed to identify the Windows key owner");
    }

    EXPLICIT_ACCESSW access{};
    access.grfAccessPermissions = FILE_ALL_ACCESS;
    access.grfAccessMode = SET_ACCESS;
    access.grfInheritance = NO_INHERITANCE;
    access.Trustee.TrusteeForm = TRUSTEE_IS_SID;
    access.Trustee.TrusteeType = TRUSTEE_IS_USER;
    access.Trustee.ptstrName = reinterpret_cast<LPWSTR>(user.sid());
    PACL raw_acl = nullptr;
    const auto acl_status = SetEntriesInAclW(1, &access, nullptr, &raw_acl);
    local_buffer acl(raw_acl);
    SECURITY_DESCRIPTOR descriptor{};
    if (acl_status != ERROR_SUCCESS ||
        !InitializeSecurityDescriptor(&descriptor, SECURITY_DESCRIPTOR_REVISION) ||
        !SetSecurityDescriptorDacl(&descriptor, TRUE, raw_acl, FALSE) ||
        !SetSecurityDescriptorControl(&descriptor, SE_DACL_PROTECTED, SE_DACL_PROTECTED)) {
        return make_logger_void_result(logger_error_code::file_permission_denied,
                                       "Failed to create a private Windows key DACL");
    }

    SECURITY_ATTRIBUTES attributes{sizeof(SECURITY_ATTRIBUTES), &descriptor, FALSE};
    native_handle file(CreateFileW(path.c_str(), GENERIC_WRITE | READ_CONTROL | WRITE_DAC,
                                   0, &attributes, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr));
    if (file.get() == INVALID_HANDLE_VALUE) {
        return make_logger_void_result(logger_error_code::file_write_failed,
                                       "Failed to open the Windows key file for writing");
    }
    // Creation uses the private DACL atomically; existing files are restricted
    // before any key bytes are written. Do not inherit the directory's grants.
    if (SetSecurityInfo(file.get(), SE_FILE_OBJECT,
                        DACL_SECURITY_INFORMATION | PROTECTED_DACL_SECURITY_INFORMATION,
                        nullptr, nullptr, raw_acl, nullptr) != ERROR_SUCCESS ||
        !has_private_dacl(file.get(), user.sid())) {
        return make_logger_void_result(logger_error_code::file_permission_denied,
                                       "Failed to enforce the Windows key DACL");
    }

    DWORD written = 0;
    if (key.size() > std::numeric_limits<DWORD>::max() || !SetEndOfFile(file.get()) ||
        !WriteFile(file.get(), key.data().data(), static_cast<DWORD>(key.size()), &written, nullptr) ||
        written != key.size() || !FlushFileBuffers(file.get())) {
        return make_logger_void_result(logger_error_code::file_write_failed,
                                       "Failed to write the Windows key file");
    }
    return common::ok();
}

result<secure_key> secure_key_storage::load_key_windows(
    const std::filesystem::path& path, size_t expected_size) {
    native_handle file(CreateFileW(path.c_str(), GENERIC_READ | READ_CONTROL, FILE_SHARE_READ,
                                   nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr));
    if (file.get() == INVALID_HANDLE_VALUE) {
        return {logger_error_code::file_open_failed, "Failed to open the Windows key file"};
    }
    current_user user;
    if (!user.query() || !has_private_dacl(file.get(), user.sid())) {
        return {logger_error_code::insecure_permissions,
                "Windows key DACL must grant access only to the current user"};
    }

    LARGE_INTEGER size{};
    if (!GetFileSizeEx(file.get(), &size)) {
        return {logger_error_code::file_read_failed, "Failed to get the Windows key file size"};
    }
    if (expected_size > std::numeric_limits<DWORD>::max() ||
        size.QuadPart != static_cast<LONGLONG>(expected_size)) {
        return {logger_error_code::invalid_key_size, "Invalid Windows key file size"};
    }

    secure_key key(expected_size);
    DWORD read = 0;
    // Read from the same handle whose DACL and size were verified.
    if (!ReadFile(file.get(), key.mutable_data().data(), static_cast<DWORD>(expected_size),
                  &read, nullptr) || read != expected_size) {
        return {logger_error_code::file_read_failed, "Failed to read the Windows key file"};
    }
    return result<secure_key>(std::move(key));
}

} // namespace kcenon::logger::security
#endif
