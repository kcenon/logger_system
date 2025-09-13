/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include "plugin_dependency_resolver.h"
#include <algorithm>
#include <queue>
#include <stack>

namespace logger_module {

void plugin_dependency_resolver::add_plugin(const std::string& plugin_name,
                                           const std::vector<std::string>& dependencies) {
    // Add to adjacency list
    if (adjacency_list_.find(plugin_name) == adjacency_list_.end()) {
        adjacency_list_[plugin_name] = std::unordered_set<std::string>();
        reverse_adjacency_list_[plugin_name] = std::unordered_set<std::string>();
    }

    for (const auto& dep : dependencies) {
        adjacency_list_[plugin_name].insert(dep);

        // Ensure dependency exists in the graph
        if (adjacency_list_.find(dep) == adjacency_list_.end()) {
            adjacency_list_[dep] = std::unordered_set<std::string>();
            reverse_adjacency_list_[dep] = std::unordered_set<std::string>();
        }

        reverse_adjacency_list_[dep].insert(plugin_name);
    }
}

void plugin_dependency_resolver::add_dependency(const dependency_edge& edge) {
    // Add to adjacency lists
    adjacency_list_[edge.from].insert(edge.to);
    reverse_adjacency_list_[edge.to].insert(edge.from);

    // Store edge details
    edge_details_[{edge.from, edge.to}] = edge;

    // Ensure both nodes exist
    if (adjacency_list_.find(edge.to) == adjacency_list_.end()) {
        adjacency_list_[edge.to] = std::unordered_set<std::string>();
        reverse_adjacency_list_[edge.to] = std::unordered_set<std::string>();
    }
}

void plugin_dependency_resolver::remove_plugin(const std::string& plugin_name) {
    // Remove from adjacency lists
    adjacency_list_.erase(plugin_name);

    // Remove edges pointing to this plugin
    for (auto& [node, neighbors] : adjacency_list_) {
        neighbors.erase(plugin_name);
    }

    // Remove from reverse adjacency list
    reverse_adjacency_list_.erase(plugin_name);

    for (auto& [node, neighbors] : reverse_adjacency_list_) {
        neighbors.erase(plugin_name);
    }

    // Remove edge details
    auto it = edge_details_.begin();
    while (it != edge_details_.end()) {
        if (it->first.first == plugin_name || it->first.second == plugin_name) {
            it = edge_details_.erase(it);
        } else {
            ++it;
        }
    }
}

std::optional<std::vector<std::string>> plugin_dependency_resolver::detect_circular_dependencies() const {
    std::unordered_set<std::string> visited;
    std::unordered_set<std::string> rec_stack;
    std::vector<std::string> path;

    for (const auto& [node, _] : adjacency_list_) {
        if (visited.find(node) == visited.end()) {
            if (dfs_detect_cycle(node, visited, rec_stack, path)) {
                return path;
            }
        }
    }

    return std::nullopt;
}

plugin_dependency_resolver::resolution_result plugin_dependency_resolver::resolve_loading_order(
    const std::vector<std::string>& plugins) const {
    resolution_result result;

    // Check for circular dependencies first
    auto circular = detect_circular_dependencies();
    if (circular.has_value()) {
        result.success = false;
        result.circular_dependencies = circular.value();
        result.error_message = "Circular dependency detected";
        return result;
    }

    // Perform topological sort
    std::unordered_set<std::string> visited;
    std::vector<std::string> stack;

    // Determine which plugins to process
    std::vector<std::string> to_process = plugins;
    if (to_process.empty()) {
        for (const auto& [node, _] : adjacency_list_) {
            to_process.push_back(node);
        }
    }

    // Topological sort
    for (const auto& plugin : to_process) {
        if (visited.find(plugin) == visited.end()) {
            dfs_topological_sort(plugin, visited, stack);
        }
    }

    // Reverse to get correct loading order
    std::reverse(stack.begin(), stack.end());
    result.loading_order = stack;

    // Check for conflicts
    result.conflicts = find_conflicts();

    return result;
}

std::vector<std::string> plugin_dependency_resolver::get_dependencies(const std::string& plugin_name,
                                                                     bool recursive) const {
    std::vector<std::string> result;

    auto it = adjacency_list_.find(plugin_name);
    if (it == adjacency_list_.end()) {
        return result;
    }

    if (!recursive) {
        result.assign(it->second.begin(), it->second.end());
    } else {
        std::unordered_set<std::string> visited;
        std::unordered_set<std::string> deps;
        get_dependencies_recursive(plugin_name, visited, deps);
        result.assign(deps.begin(), deps.end());
    }

    return result;
}

std::vector<std::string> plugin_dependency_resolver::get_dependents(const std::string& plugin_name,
                                                                   bool recursive) const {
    std::vector<std::string> result;

    auto it = reverse_adjacency_list_.find(plugin_name);
    if (it == reverse_adjacency_list_.end()) {
        return result;
    }

    if (!recursive) {
        result.assign(it->second.begin(), it->second.end());
    } else {
        std::unordered_set<std::string> visited;
        std::unordered_set<std::string> deps;

        // Similar to get_dependencies_recursive but using reverse adjacency list
        std::queue<std::string> q;
        q.push(plugin_name);
        visited.insert(plugin_name);

        while (!q.empty()) {
            std::string current = q.front();
            q.pop();

            auto rev_it = reverse_adjacency_list_.find(current);
            if (rev_it != reverse_adjacency_list_.end()) {
                for (const auto& dependent : rev_it->second) {
                    if (visited.find(dependent) == visited.end()) {
                        visited.insert(dependent);
                        deps.insert(dependent);
                        q.push(dependent);
                    }
                }
            }
        }

        result.assign(deps.begin(), deps.end());
    }

    return result;
}

bool plugin_dependency_resolver::depends_on(const std::string& plugin_a, const std::string& plugin_b,
                                           bool transitive) const {
    if (!transitive) {
        auto it = adjacency_list_.find(plugin_a);
        if (it != adjacency_list_.end()) {
            return it->second.find(plugin_b) != it->second.end();
        }
        return false;
    }

    // Check transitive dependencies
    auto deps = get_dependencies(plugin_a, true);
    return std::find(deps.begin(), deps.end(), plugin_b) != deps.end();
}

std::vector<plugin_dependency_resolver::dependency_conflict> plugin_dependency_resolver::find_conflicts() const {
    std::vector<dependency_conflict> conflicts;

    // For simplicity, we'll check for version conflicts
    // This is a placeholder implementation
    // In a real system, you'd need version comparison logic

    return conflicts;
}

std::vector<std::string> plugin_dependency_resolver::suggest_conflict_resolution(
    const std::vector<dependency_conflict>& conflicts) const {
    std::vector<std::string> suggestions;

    for (const auto& conflict : conflicts) {
        std::string suggestion = "Consider updating " + conflict.plugin_a +
                               " to use version " + conflict.version_b +
                               " of " + conflict.conflicting_dependency;
        suggestions.push_back(suggestion);
    }

    return suggestions;
}

std::string plugin_dependency_resolver::export_as_dot() const {
    std::stringstream ss;

    ss << "digraph PluginDependencies {\n";
    ss << "  rankdir=LR;\n";
    ss << "  node [shape=box];\n\n";

    // Add nodes
    for (const auto& [node, _] : adjacency_list_) {
        ss << "  \"" << node << "\";\n";
    }

    ss << "\n";

    // Add edges
    for (const auto& [from, neighbors] : adjacency_list_) {
        for (const auto& to : neighbors) {
            ss << "  \"" << from << "\" -> \"" << to << "\"";

            // Add edge attributes if available
            auto edge_key = std::make_pair(from, to);
            auto edge_it = edge_details_.find(edge_key);
            if (edge_it != edge_details_.end()) {
                if (edge_it->second.optional) {
                    ss << " [style=dashed]";
                }
            }

            ss << ";\n";
        }
    }

    ss << "}\n";

    return ss.str();
}

std::string plugin_dependency_resolver::export_as_json() const {
    std::stringstream ss;

    ss << "{\n";
    ss << "  \"nodes\": [\n";

    // Add nodes
    bool first_node = true;
    for (const auto& [node, _] : adjacency_list_) {
        if (!first_node) ss << ",\n";
        ss << "    {\"id\": \"" << node << "\"}";
        first_node = false;
    }

    ss << "\n  ],\n";
    ss << "  \"edges\": [\n";

    // Add edges
    bool first_edge = true;
    for (const auto& [from, neighbors] : adjacency_list_) {
        for (const auto& to : neighbors) {
            if (!first_edge) ss << ",\n";
            ss << "    {\"source\": \"" << from << "\", \"target\": \"" << to << "\"";

            auto edge_key = std::make_pair(from, to);
            auto edge_it = edge_details_.find(edge_key);
            if (edge_it != edge_details_.end()) {
                if (edge_it->second.optional) {
                    ss << ", \"optional\": true";
                }
                if (!edge_it->second.version.empty()) {
                    ss << ", \"version\": \"" << edge_it->second.version << "\"";
                }
            }

            ss << "}";
            first_edge = false;
        }
    }

    ss << "\n  ]\n";
    ss << "}\n";

    return ss.str();
}

std::vector<std::vector<std::string>> plugin_dependency_resolver::get_strongly_connected_components() const {
    std::vector<std::vector<std::string>> sccs;
    std::unordered_map<std::string, int> indices;
    std::unordered_map<std::string, int> lowlinks;
    std::unordered_set<std::string> on_stack;
    std::vector<std::string> stack;
    int index = 0;

    for (const auto& [node, _] : adjacency_list_) {
        if (indices.find(node) == indices.end()) {
            tarjan_scc(node, index, stack, indices, lowlinks, on_stack, sccs);
        }
    }

    return sccs;
}

void plugin_dependency_resolver::clear() {
    adjacency_list_.clear();
    reverse_adjacency_list_.clear();
    edge_details_.clear();
}

size_t plugin_dependency_resolver::edge_count() const {
    size_t count = 0;
    for (const auto& [_, neighbors] : adjacency_list_) {
        count += neighbors.size();
    }
    return count;
}

bool plugin_dependency_resolver::has_plugin(const std::string& plugin_name) const {
    return adjacency_list_.find(plugin_name) != adjacency_list_.end();
}

bool plugin_dependency_resolver::dfs_detect_cycle(const std::string& node,
                                                 std::unordered_set<std::string>& visited,
                                                 std::unordered_set<std::string>& rec_stack,
                                                 std::vector<std::string>& path) const {
    visited.insert(node);
    rec_stack.insert(node);
    path.push_back(node);

    auto it = adjacency_list_.find(node);
    if (it != adjacency_list_.end()) {
        for (const auto& neighbor : it->second) {
            if (rec_stack.find(neighbor) != rec_stack.end()) {
                // Found cycle - trim path to show only the cycle
                auto cycle_start = std::find(path.begin(), path.end(), neighbor);
                path.erase(path.begin(), cycle_start);
                path.push_back(neighbor); // Add to show complete cycle
                return true;
            }

            if (visited.find(neighbor) == visited.end()) {
                if (dfs_detect_cycle(neighbor, visited, rec_stack, path)) {
                    return true;
                }
            }
        }
    }

    rec_stack.erase(node);
    path.pop_back();
    return false;
}

void plugin_dependency_resolver::dfs_topological_sort(const std::string& node,
                                                     std::unordered_set<std::string>& visited,
                                                     std::vector<std::string>& stack) const {
    visited.insert(node);

    auto it = adjacency_list_.find(node);
    if (it != adjacency_list_.end()) {
        for (const auto& neighbor : it->second) {
            if (visited.find(neighbor) == visited.end()) {
                dfs_topological_sort(neighbor, visited, stack);
            }
        }
    }

    stack.push_back(node);
}

void plugin_dependency_resolver::get_dependencies_recursive(const std::string& plugin_name,
                                                           std::unordered_set<std::string>& visited,
                                                           std::unordered_set<std::string>& result) const {
    visited.insert(plugin_name);

    auto it = adjacency_list_.find(plugin_name);
    if (it != adjacency_list_.end()) {
        for (const auto& dep : it->second) {
            result.insert(dep);
            if (visited.find(dep) == visited.end()) {
                get_dependencies_recursive(dep, visited, result);
            }
        }
    }
}

void plugin_dependency_resolver::tarjan_scc(const std::string& node,
                                           int& index,
                                           std::vector<std::string>& stack,
                                           std::unordered_map<std::string, int>& indices,
                                           std::unordered_map<std::string, int>& lowlinks,
                                           std::unordered_set<std::string>& on_stack,
                                           std::vector<std::vector<std::string>>& sccs) const {
    indices[node] = index;
    lowlinks[node] = index;
    index++;
    stack.push_back(node);
    on_stack.insert(node);

    auto it = adjacency_list_.find(node);
    if (it != adjacency_list_.end()) {
        for (const auto& neighbor : it->second) {
            if (indices.find(neighbor) == indices.end()) {
                tarjan_scc(neighbor, index, stack, indices, lowlinks, on_stack, sccs);
                lowlinks[node] = std::min(lowlinks[node], lowlinks[neighbor]);
            } else if (on_stack.find(neighbor) != on_stack.end()) {
                lowlinks[node] = std::min(lowlinks[node], indices[neighbor]);
            }
        }
    }

    if (lowlinks[node] == indices[node]) {
        std::vector<std::string> scc;
        std::string w;
        do {
            w = stack.back();
            stack.pop_back();
            on_stack.erase(w);
            scc.push_back(w);
        } while (w != node);

        if (scc.size() > 1) {
            sccs.push_back(scc);
        }
    }
}

} // namespace logger_module