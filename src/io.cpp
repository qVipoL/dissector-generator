#include "../include/util/io.h"

string readFile(string filename) {
    ifstream input_file(filename);
    ostringstream stringStream;

    if (!input_file.is_open()) {
        stringStream << "Could not open file: " << filename << endl;
        throw runtime_error(stringStream.str());
    }

    return string((istreambuf_iterator<char>(input_file)), istreambuf_iterator<char>());
}

void writeFile(string filename, string content) {
    ofstream out_file(filename);
    ostringstream stringStream;

    if (!out_file.is_open()) {
        stringStream << "Could not open file: " << filename << endl;
        throw runtime_error(stringStream.str());
    }

    out_file << content;
}