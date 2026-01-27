#include "profile_resolver.h"
<<<<<<< HEAD
#include <stdexcept>

using namespace std;
=======
>>>>>>> upstream/compiler

ProfileResolver::ProfileResolver(ProfileRepository& repo)
    : repo(repo) {}

<<<<<<< HEAD
/*
 * Safely convert YAML scalar → IR Value
 * Order matters: int → double → bool → string
 */
Value ProfileResolver::parseYamlValue(const YAML::Node& node) {

    if (!node.IsScalar()) {
        throw runtime_error("Only scalar YAML values are supported");
    }

    // Try int
    try {
        return node.as<int>();
    } catch (...) {}

    // Try double
    try {
        return node.as<double>();
    } catch (...) {}

    // Try bool
    try {
        return node.as<bool>();
    } catch (...) {}

    // Fallback: string
    return node.as<string>();
=======
Value ProfileResolver::yamlToValue(const YAML::Node& n) {
    try { return n.as<int>(); } catch (...) {}
    try { return n.as<double>(); } catch (...) {}
    try { return n.as<bool>(); } catch (...) {}
    return n.as<string>();
>>>>>>> upstream/compiler
}

void ProfileResolver::resolve(IR& ir) {

<<<<<<< HEAD
    // ---------- COMPONENT PROFILES ----------
    for (auto& comp : ir.components) {

        YAML::Node profile = repo.getComponentProfile(comp.implementation);

        // Apply defaults from YAML
        if (profile["defaults"]) {
            for (auto it : profile["defaults"]) {
                string key = it.first.as<string>();
                comp.resolved_params[key] = parseYamlValue(it.second);
            }
        }

        // Override with user-provided parameters
        for (auto& [key, value] : comp.user_params) {
            comp.resolved_params[key] = value;
        }
    }

    // ---------- NETWORK PROFILES ----------
    for (auto& link : ir.links) {

        YAML::Node profile = repo.getNetworkProfile(link.type);

        if (profile["defaults"]) {
            for (auto it : profile["defaults"]) {
                string key = it.first.as<string>();
                link.resolved_params[key] = parseYamlValue(it.second);
=======
    for (auto& c : ir.context.components) {
        YAML::Node profile = repo.getComponentProfile(c.type);
        if (profile["defaults"]) {
            for (auto it : profile["defaults"]) {
                string k = it.first.as<string>();
                if (!c.config.count(k)) {
                    c.config[k] = yamlToValue(it.second);
                }
            }
        }
    }

    for (auto& l : ir.context.links) {
        YAML::Node profile = repo.getNetworkProfile("ethernet");
        if (profile["defaults"]) {
            for (auto it : profile["defaults"]) {
                string k = it.first.as<string>();
                if (!l.config.count(k)) {
                    l.config[k] = yamlToValue(it.second);
                }
>>>>>>> upstream/compiler
            }
        }
    }
}
