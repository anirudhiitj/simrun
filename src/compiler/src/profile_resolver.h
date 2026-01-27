#pragma once
<<<<<<< HEAD

#include "ir.h"
#include "profile_repository.h"
#include <yaml-cpp/yaml.h>

using namespace std;

class ProfileResolver {
public:
    ProfileResolver(ProfileRepository& repo);

=======
#include "ir.h"
#include "profile_repository.h"

class ProfileResolver {
public:
    explicit ProfileResolver(ProfileRepository& repo);
>>>>>>> upstream/compiler
    void resolve(IR& ir);

private:
    ProfileRepository& repo;
<<<<<<< HEAD

    // Converts a YAML scalar into IR Value safely
    Value parseYamlValue(const YAML::Node& node);
=======
    Value yamlToValue(const YAML::Node& node);
>>>>>>> upstream/compiler
};
