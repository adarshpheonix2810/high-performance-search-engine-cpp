#include "Document_store.hpp"
using namespace std;
int read_sizes(int *linecounter,int *maxlength, char *file_name){
    FILE *file=fopen(file_name, "r");
    if(file==NULL){
        cout<<"Cannot open file: "<<file_name<<endl;
        return -1;
    }
    
    // Check if file is empty
    int c = fgetc(file);
    if(c == EOF){
        cout<<"File is empty: "<<file_name<<endl;
        fclose(file);
        return -1;
    }
    ungetc(c, file);
    
    char *line=NULL;
    size_t buffersize=0;
    int current_length;
    while((current_length=getline(&line, &buffersize, file))!=-1){
        // Remove newline character from count if present
        if(current_length > 0 && line[current_length-1] == '\n'){
            current_length--;
        }
        if(current_length>*maxlength)
            *maxlength=current_length;
        (*linecounter)++;
        free(line);
        line=NULL;
        buffersize=0;
    }
    fclose(file);
    free(line);
    return 1;
}
int read_input(Mymap* mymap, char* file_name){
    FILE *file = fopen(file_name, "r");
    char *line = NULL;
    size_t buffersize = 0;
    int current_length;
    for(int i=0;i<mymap->get_size();i++){
        getline(&line, &buffersize, file);
        if (mymap->insert(line, i) == -1) {
            cout << "Error inserting line " << endl;
            free(line);
            fclose(file);
            return -1;
        }
        free(line);
        line = NULL;
        buffersize = 0;
    }
    free(line);
    fclose(file);
    return 1;
}