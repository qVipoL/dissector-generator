#include "../include/Generator/Generator.h"
#include "../include/Lexer/Lexer.h"
#include "../include/Parser/Parser.h"
#include "../include/util/io.h"
#include "../include/util/std_include.h"

int main(int argc, char *argv[]) {
    int error_code = EXIT_SUCCESS;
    string content;
    Lexer *lexer;
    Parser *parser;
    AST *tree;
    Generator *generator;

    if (argc < 2) {
        cout << "No file-name was passed." << endl;
        return EXIT_FAILURE;
    }

    try {
        content = readFile(argv[1]);
        lexer = new Lexer(content);
        parser = new Parser(lexer);

        tree = parser->parse();
        // tree->display(1);

        generator = new Generator(tree);

        if (argv[2]) {
            writeFile(argv[2], generator->generateLua());
        } else {
            cout << generator->generateLua() << endl;
        }
    } catch (runtime_error err) {
        cout << err.what() << endl;
        error_code = EXIT_FAILURE;
    }

cleanup:
    if (generator != NULL)
        delete generator;
    if (parser != NULL)
        delete parser;
    if (lexer != NULL)
        delete lexer;
    if (tree != NULL)
        delete tree;

    return error_code;
}