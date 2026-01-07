// compiler.cpp
#include <iostream>
#include <sstream>
#include "parser/parser.h"
#include "validator/validator.h"

using namespace std;

int main() {
    // Read full JSON from stdin (Electron-friendly)
    stringstream buffer;
    buffer << cin.rdbuf();
    string json_input = buffer.str();

    try {
        ArchitectureAST ast = parse_json_to_ast(json_input);
        auto errors = validate_ast(ast);

        if (!errors.empty()) {
            cout << "{ \"status\": \"error\", \"errors\": [";
            for (size_t i = 0; i < errors.size(); i++) {
                cout << "{ \"message\": \"" << errors[i].message << "\" }";
                if (i + 1 < errors.size()) cout << ",";
            }
            cout << "] }";
            return 1;
        }

        cout << "{ \"status\": \"ok\" }";
        return 0;

    } catch (const exception& e) {
        cout << "{ \"status\": \"fatal\", \"message\": \"" << e.what() << "\" }";
        return 1;
    }
}
