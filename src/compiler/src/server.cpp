#include <crow.h>
<<<<<<< HEAD
#include <string>
#include "compiler_driver.h"

using namespace std;

=======
#include <fstream>
#include <sstream>
#include "ir_parser.h"
#include "compiler_driver.h"

>>>>>>> upstream/compiler
int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/compile").methods("POST"_method)
<<<<<<< HEAD
    ([](const crow::request& req) {

        bool ok = false;
        string output = compileIR(req.body, ok);

        if (!ok) {
            return crow::response(400, output);
        }
=======
([](const crow::request& req) {
    try {
        IR ir = parseIR(req.body);
        string path = compileIR(ir);

        ifstream in(path);
        stringstream ss;
        ss << in.rdbuf();
>>>>>>> upstream/compiler

        crow::response res;
        res.code = 200;
        res.set_header("Content-Type", "application/json");
<<<<<<< HEAD
        res.body = output;
        return res;
    });

    app.port(8080).multithreaded().run();
=======
        res.body = ss.str();
        return res;

    } catch (const runtime_error& e) {
        // 🔵 VALIDATION / USER ERROR
        return crow::response(
            400,
            string("Validation error: ") + e.what()
        );

    } catch (const exception& e) {
        // 🔴 INTERNAL ERROR
        return crow::response(
            500,
            string("Internal compiler error: ") + e.what()
        );
    }
});


    app.port(8080).run();
>>>>>>> upstream/compiler
}
