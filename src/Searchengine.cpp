#include "Searchengine.hpp"

using namespace std;
// read document/books/searchengine.md for more information
int main(int argc, char** argv) {
    if (argc != 5 ||
        strcmp(argv[1], "-d") != 0 ||
        strcmp(argv[3], "-k") != 0) {
        cout << "Wrong arguments. Usage: -d <file> -k <number>" << endl;
        return -1;
    }

    cout << "Please wait..." << endl;
    int linecounter = 0;
    int maxlength = -1;
    int k;
    try {
        k = stoi(argv[4]); 
    } catch (...) {
        cout << "Invalid value for -k (must be an integer)" << endl;
        return -1;
    }
    read_file(&linecounter, &maxlength,argv[2]);

    /*
    FILE *file=fopen(argv[2], "r");
    if(file==NULL){
        cout<<"Cannot open file: "<<argv[2]<<endl;
        return -1;
    }
    fclose(file);
    */
    
}