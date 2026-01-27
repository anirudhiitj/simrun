#pragma once
#include <string>
<<<<<<< HEAD
#include <map>
#include <variant>
#include <vector>

using namespace std;

using Value = variant<int, double, string, bool>;

struct ComponentIR {
    string id;
    string category;
    string implementation;

    map<string, Value> user_params;
    map<string, Value> resolved_params;
};

struct NetworkLinkIR {
    string from;
    string to;
    string type;

    map<string, Value> resolved_params;
};

struct IR {
    vector<ComponentIR> components;
    vector<NetworkLinkIR> links;
=======
#include <vector>
#include <unordered_map>
#include <variant>

using namespace std;

using Value = variant<int, double, bool, string>;

struct Component {
    int id;
    string type;
    unordered_map<string, Value> config;
};

struct Link {
    int id;
    int from;
    int to;
    unordered_map<string, Value> config;
};

struct Context {
    vector<Component> components;
    vector<Link> links;
};

struct IR {
    unordered_map<string, Value> header;
    Context context;
>>>>>>> upstream/compiler
};
