#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

/**
 * @file plugin_dependency_resolver.h
 * @brief Plugin dependency resolution and graph management
 *
 * This file provides dependency resolution for plugins including:
 * - Circular dependency detection
 * - Loading order optimization
 * - Dependency conflict resolution
 * - Dependency graph visualization
 */

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <optional>
#include <sstream>
#include <algorithm>  // Added for graph algorithms
#include <stack>      // Added for graph traversal algorithms

namespace logger_module {

/**
 * @brief Plugin dependency resolver for managing plugin dependencies
 */
class plugin_dependency_resolver {
public:
    /**
     * @brief Dependency edge information
     */
    struct dependency_edge {
        std::string from;      ///< Source plugin
        std::string to;        ///< Target plugin
        std::string version;   ///< Required version
        bool optional;         ///< Whether dependency is optional

        dependency_edge(const std::string& f, const std::string& t,
                       const std::string& v = "", bool opt = false)
            : from(f), to(t), version(v), optional(opt) {}
    };

    /**
     * @brief Dependency conflict information
     */
    struct dependency_conflict {
        std::string plugin_a;
        std::string plugin_b;
        std::string conflicting_dependency;
        std::string version_a;
        std::string version_b;
        std::string reason;

        dependency_conflict(const std::string& a, const std::string& b,
                          const std::string& dep, const std::string& va,
                          const std::string& vb, const std::string& r)
            : plugin_a(a), plugin_b(b), conflicting_dependency(dep),
              version_a(va), version_b(vb), reason(r) {}
    };

    /**
     * @brief Resolution result
     */
    struct resolution_result {
        bool success;
        std::vector<std::string> loading_order;
        std::vector<dependency_conflict> conflicts;
        std::vector<std::string> circular_dependencies;
        std::string error_message;

        resolution_result() : success(true) {}
    };

    /**
     * @brief Constructor
     */
    plugin_dependency_resolver() = default;

    /**
     * @brief Destructor
     */
    ~plugin_dependency_resolver() = default;

    /**
     * @brief Add a plugin to the dependency graph
     * @param plugin_name Name of the plugin
     * @param dependencies List of dependencies
     */
    void add_plugin(const std::string& plugin_name,
                   const std::vector<std::string>& dependencies = {});

    /**
     * @brief Add a dependency edge
     * @param edge Dependency edge to add
     */
    void add_dependency(const dependency_edge& edge);

    /**
     * @brief Remove a plugin from the dependency graph
     * @param plugin_name Name of the plugin to remove
     */
    void remove_plugin(const std::string& plugin_name);

    /**
     * @brief Check if there are circular dependencies
     * @return Optional containing circular path if found
     */
    std::optional<std::vector<std::string>> detect_circular_dependencies() const;

    /**
     * @brief Get topological loading order
     * @param plugins Specific plugins to order (empty for all)
     * @return Resolution result with loading order
     */
    resolution_result resolve_loading_order(
        const std::vector<std::string>& plugins = {}) const;

    /**
     * @brief Get all dependencies of a plugin
     * @param plugin_name Name of the plugin
     * @param recursive Include transitive dependencies
     * @return List of dependency names
     */
    std::vector<std::string> get_dependencies(const std::string& plugin_name,
                                             bool recursive = false) const;

    /**
     * @brief Get all plugins that depend on the given plugin
     * @param plugin_name Name of the plugin
     * @param recursive Include transitive dependents
     * @return List of dependent plugin names
     */
    std::vector<std::string> get_dependents(const std::string& plugin_name,
                                           bool recursive = false) const;

    /**
     * @brief Check if plugin A depends on plugin B
     * @param plugin_a Source plugin
     * @param plugin_b Target plugin
     * @param transitive Check transitive dependencies
     * @return true if A depends on B
     */
    bool depends_on(const std::string& plugin_a, const std::string& plugin_b,
                   bool transitive = true) const;

    /**
     * @brief Find conflicting dependencies
     * @return List of dependency conflicts
     */
    std::vector<dependency_conflict> find_conflicts() const;

    /**
     * @brief Suggest resolution for conflicts
     * @param conflicts List of conflicts to resolve
     * @return Suggested resolution strategy
     */
    std::vector<std::string> suggest_conflict_resolution(
        const std::vector<dependency_conflict>& conflicts) const;

    /**
     * @brief Export dependency graph in DOT format
     * @return DOT graph representation
     */
    std::string export_as_dot() const;

    /**
     * @brief Export dependency graph as JSON
     * @return JSON representation
     */
    std::string export_as_json() const;

    /**
     * @brief Get strongly connected components
     * @return List of component groups
     */
    std::vector<std::vector<std::string>> get_strongly_connected_components() const;

    /**
     * @brief Clear all dependency information
     */
    void clear();

    /**
     * @brief Get number of plugins in the graph
     * @return Number of plugins
     */
    size_t plugin_count() const { return adjacency_list_.size(); }

    /**
     * @brief Get number of dependency edges
     * @return Number of edges
     */
    size_t edge_count() const;

    /**
     * @brief Check if a plugin exists in the graph
     * @param plugin_name Name of the plugin
     * @return true if plugin exists
     */
    bool has_plugin(const std::string& plugin_name) const;

private:
    /**
     * @brief Internal DFS for cycle detection
     * @param node Current node
     * @param visited Visited nodes
     * @param rec_stack Recursion stack
     * @param path Current path
     * @return true if cycle detected
     */
    bool dfs_detect_cycle(const std::string& node,
                         std::unordered_set<std::string>& visited,
                         std::unordered_set<std::string>& rec_stack,
                         std::vector<std::string>& path) const;

    /**
     * @brief Internal DFS for topological sort
     * @param node Current node
     * @param visited Visited nodes
     * @param stack Result stack
     */
    void dfs_topological_sort(const std::string& node,
                             std::unordered_set<std::string>& visited,
                             std::vector<std::string>& stack) const;

    /**
     * @brief Get all dependencies recursively
     * @param plugin_name Plugin name
     * @param visited Already visited plugins
     * @param result Result set
     */
    void get_dependencies_recursive(const std::string& plugin_name,
                                   std::unordered_set<std::string>& visited,
                                   std::unordered_set<std::string>& result) const;

    /**
     * @brief Tarjan's algorithm for SCCs
     * @param node Current node
     * @param index Current index
     * @param stack DFS stack
     * @param indices Node indices
     * @param lowlinks Node lowlinks
     * @param on_stack Nodes on stack
     * @param sccs Result SCCs
     */
    void tarjan_scc(const std::string& node,
                   int& index,
                   std::vector<std::string>& stack,
                   std::unordered_map<std::string, int>& indices,
                   std::unordered_map<std::string, int>& lowlinks,
                   std::unordered_set<std::string>& on_stack,
                   std::vector<std::vector<std::string>>& sccs) const;

    // Member variables
    std::unordered_map<std::string, std::unordered_set<std::string>> adjacency_list_;
    std::unordered_map<std::string, std::unordered_set<std::string>> reverse_adjacency_list_;
    std::unordered_map<std::pair<std::string, std::string>, dependency_edge,
                      std::hash<std::pair<std::string, std::string>>> edge_details_;
};

} // namespace logger_module