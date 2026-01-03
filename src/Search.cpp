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
        double df = (double)trie->dfsearchword(queryWords[i],0,wordlen);
        double N = (double)map->get_size();
        // IDF formula: log((N - df + 0.5) / (df + 0.5))
        // Add safety check to prevent log of negative or zero
        if(df == 0){
            IDF[i] = log((N + 1.0) / 1.0);  // Word not found, maximum IDF
        } else {
            IDF[i] = log((N - df + 0.5) / (df + 0.5));
        }
        trie->search(queryWords[i],0,scorelist);
        token = strtok(NULL, " \t\n");
    }
    
    // Check if any words were parsed
    if(i == 0){
        cout << "Error: Please enter valid search terms" << endl;
        delete scorelist;
        return;
    }
    double avgdl=0;
    for(int m=0; m<map->get_size(); m++){
        avgdl+=(double)map->getlength(m);
    }
    if(map->get_size() > 0){
        avgdl/=(double)map->get_size();
    }
    if(avgdl == 0){
        avgdl = 1.0;  // Prevent division by zero
    }
    
    double score=0;
    Scorelist* currentDoc=scorelist;
    //maxheap
    int resultCount = 0;
    Maxheap* heap=new Maxheap(k);

    while(currentDoc!=NULL){
        if(currentDoc->get_id() != -1){  // Skip empty placeholder node
            score = 0;
            for(int l=0;l<i;l++){
                int wordlen = strlen(queryWords[l]);
                double tf = (double)trie->tfsearchword(currentDoc->get_id(),queryWords[l],0,wordlen);
                if(tf > 0){  // Only calculate if term exists in document
                    double doclen = (double)map->getlength(currentDoc->get_id());
                    double bm25_tf = (tf * (k1 + 1.0)) / (tf + k1 * (1.0 - b + b * (doclen / avgdl)));
                    score += IDF[l] * bm25_tf;
                }
            }
            heap->insert(score, currentDoc->get_id());
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
        for(int j = 0; j < actualResults; j++){
            if(heap->get_count() == 0){
                break;  // No more results in heap
            }
            
            int docId = heap->get_id();
            if(docId == -1 || docId >= map->get_size()){
                heap->remove();
                continue;  // Skip invalid document
            }
            
            double docScore = heap->get_score();
            heap->remove();
            
            // Get document content
            const char *fullDoc = map->getDocument(docId);
            if(fullDoc == NULL){
                continue;  // Skip if document not found
            }
            
            // Print header: [docId] Document Title score=X.XXXXXX
            cout << "[" << docId << "] ";
            
            // Extract first line as title
            char *line = (char*)malloc(map->get_buffersize()*sizeof(char));
            if(line == NULL){
                cout << " score=" << docScore << endl;
                cout << fullDoc << endl;
                if(j < actualResults - 1){
                    cout << "---" << endl;
                }
                continue;
            }
            
            strcpy(line, fullDoc);
            char *firstLine = strtok(line, "\n");
            if(firstLine != NULL){
                cout << firstLine;
            }
            cout << " score=" << docScore << endl;
            free(line);
            
            // Print the full document content
            cout << fullDoc << endl;
            
            // Print separator
            if(j < actualResults - 1){
                cout << "---" << endl;
            }
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
    
    // Validate document ID is non-negative
    if(id < 0){
        cout << "Error: Document ID must be non-negative" << endl;
        return -1;
    }

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