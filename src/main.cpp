#include "../include/Lexer/Lexer.h"
#include "../include/Parser/Parser.h"
#include "../include/util/io.h"
#include "../include/util/std_include.h"

int main(int argc, char *argv[]) {
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
        delete parser;
        delete lexer;
    } catch (runtime_error err) {
        cout << err.what();
    }

    return EXIT_SUCCESS;
}