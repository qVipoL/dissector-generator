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

    if (argc < 2) {
        cout << "No file-name was passed." << endl;
        return EXIT_FAILURE;
    }

    try {
        content = readFile(argv[1]);
        lexer = new Lexer(content);
        parser = new Parser(lexer);

        tree = parser->parse();

        tree->display(1);
        delete tree;
    } catch (runtime_error err) {
        cout << err.what();
        error_code = EXIT_FAILURE;
    }

    delete parser;
    delete lexer;
    return error_code;
}