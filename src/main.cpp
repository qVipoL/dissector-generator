#include "../include/Lexer/Lexer.h"
#include "../include/util/io.h"
#include "../include/util/std_include.h"

int main(int argc, char *argv[]) {
    string content;
    Lexer *lexer;
    Token *token;

    if (argc < 2) {
        cout << "No file-name was passed." << endl;
        return EXIT_FAILURE;
    }

    try {
        content = readFile(argv[1]);
        lexer = new Lexer(content);

        for (int i = 0; i < 15; i++) {
            token = lexer->getNextToken();
            token->display();
            delete token;
        }

        delete lexer;
    } catch (runtime_error err) {
        cout << err.what();
    }

    return EXIT_SUCCESS;
}