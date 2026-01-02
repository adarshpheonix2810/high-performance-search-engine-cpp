#include "Search.hpp"
using namespace std;
const float k1=1.2;
const float b=0.75;
const int MAX_QUERY_WORDS = 10;  // Maximum search terms in one query
const int MAX_WORDS_STORAGE = 100;  // Storage array size
const int MAX_WORD_LENGTH = 256;  // Maximum length per word

void search(char *token, TrieNode *trie, Mymap *map, int k)
{
    char queryWords[MAX_WORDS_STORAGE][MAX_WORD_LENGTH];
    double IDF[MAX_WORDS_STORAGE];
    
    token = strtok(NULL, " \t\n");
    if(token == NULL){
        cout << "Error: Please enter search terms" << endl;
        return;
    }
    
    Scorelist* scorelist = new Scorelist();
    int i;
    for(i=0; i<MAX_QUERY_WORDS; i++){
        if(token == NULL){
            break;
        }
        strcpy(queryWords[i], token);
        int wordlen = strlen(queryWords[i]);
        IDF[i]=log10(((double)map->get_size()-(double)trie->dfsearchword(queryWords[i],0,wordlen)+0.5)/((double)trie->dfsearchword(queryWords[i],0,wordlen)+0.5));
        trie->search(queryWords[i],0,scorelist);
        token = strtok(NULL, " \t\n");
    }
    double avgdl=0;
    for(int m=0; m<map->get_size(); m++){
        avgdl+=(double)map->getlength(m);
    }
    avgdl/=(double)map->get_size();
    double score=0;
    Scorelist* currentDoc=scorelist;
    //maxheap
    int resultCount = 0;
    Maxheap* heap=new Maxheap(k);
    while(currentDoc!=NULL){
        if(currentDoc->get_id() != -1){  // Skip empty placeholder node
            for(int l=0;l<i;l++){
                int wordlen = strlen(queryWords[l]);
                double tf = (double)trie->tfsearchword(currentDoc->get_id(),queryWords[l],0,wordlen);
                score+=IDF[l]*(tf*(k1+1.0))/(tf+k1*(1.0-b+b*((double)map->getlength(currentDoc->get_id())/(double)avgdl)));
            }
            heap->insert(score, currentDoc->get_id());
            score=0;
            resultCount++;
        }
        currentDoc=currentDoc->get_next();
    }
    if(resultCount>k){
        resultCount=k;
    }
    
    // Display top k results from heap
    int actualResults = heap->get_count();
    if(actualResults == 0){
        cout << "No documents found matching the query." << endl;
    } else {
        cout << "Top " << actualResults << " results:" << endl;
        cout << "----------------------------------------" << endl;
        for(int j = 0; j < actualResults; j++){
            int docId = heap->get_id();
            cout << "Document " << docId << endl;
            heap->remove();
        }
    }
    
    delete heap;
    delete scorelist;
}

void df(TrieNode *trie)
{
    char *token2 = strtok(NULL, " \t\n");
    if (token2 != NULL)
    {
        int wordlen = strlen(token2);
        int docCount = trie->dfsearchword(token2, 0, wordlen);

        // Display result with clear message
        if (docCount == 0)
        {
            cout << "Term '" << token2 << "' not found in any document" << endl;
        }
        else
        {
            cout << "Term '" << token2 << "' appears in " << docCount << " document(s)" << endl;
        }
    }
    else
    {
        cout << "Error: Missing word. Usage: /df <word>" << endl;
    }
}

int tf(char *token, TrieNode *trie)
{
    // Get document ID
    char *token2 = strtok(NULL, " \t\n");
    if (token2 == NULL)
    {
        cout << "Error: Missing document ID. Usage: /tf <doc_id> <word>" << endl;
        return -1;
    }

    // Validate document ID is a number
    int len = strlen(token2);
    for (int i = 0; i < len; i++)
    {
        if (!isdigit(token2[i]))
        {
            cout << "Error: Document ID must be a number" << endl;
            return -1;
        }
    }
    int id = atoi(token2);

    // Get the word to search
    token2 = strtok(NULL, " \t\n");
    if (token2 == NULL)
    {
        cout << "Error: Missing word. Usage: /tf <doc_id> <word>" << endl;
        return -1;
    }

    // Search for the word and get frequency
    int wordlen = strlen(token2);
    int frequency = trie->tfsearchword(id, token2, 0, wordlen);

    // Display result with clear message
    if (frequency == 0)
    {
        cout << "Term '" << token2 << "' not found in document " << id << endl;
    }
    else
    {
        cout << "Term '" << token2 << "' appears " << frequency << " time(s) in document " << id << endl;
    }

    return 0;
}