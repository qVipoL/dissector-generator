#include "../include/io.h"

string readFile(string filename) {
    ifstream input_file(filename);
    ostringstream stringStream;

    if (!input_file.is_open()) {
        stringStream << "Could not open file: " << filename << endl;
        throw runtime_error(stringStream.str());
    }

    return string((istreambuf_iterator<char>(input_file)), istreambuf_iterator<char>());
}