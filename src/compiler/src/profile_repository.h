#pragma once
#include <yaml-cpp/yaml.h>
#include <string>
<<<<<<< HEAD
#include <unordered_map>
=======
>>>>>>> upstream/compiler

using namespace std;

class ProfileRepository {
public:
<<<<<<< HEAD
    ProfileRepository(const string& basePath);

    YAML::Node getComponentProfile(const string& name);
    YAML::Node getNetworkProfile(const string& name);

private:
    string basePath;
    unordered_map<string, YAML::Node> compCache;
    unordered_map<string, YAML::Node> netCache;
=======
    explicit ProfileRepository(const string& root);
    YAML::Node getComponentProfile(const string& type);
    YAML::Node getNetworkProfile(const string& type);

private:
    string root;
>>>>>>> upstream/compiler
};
