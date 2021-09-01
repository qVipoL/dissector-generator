#include "../include/Lexer/Lexer.h"
#include "../include/io.h"
#include "../include/std_include.h"

int main(int argc, char *argv[]) {
    string content;

    if (argc < 2) {
        cout << "No file-name was passed." << endl;
        return EXIT_FAILURE;
    }

    try {
        content = readFile(argv[1]);

        Lexer lexer = Lexer(content);

        for (int i = 0; i < 30; i++) {
            Token t = lexer.getNextToken();
            t.display();
        }
    } catch (runtime_error err) {
        cout << err.what();
    }
    // cout << content << endl;

    return EXIT_SUCCESS;
}