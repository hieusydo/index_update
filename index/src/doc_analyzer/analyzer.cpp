#include "analyzer.h"

#include "Matcher/matcher.h"
#include "global_parameters.hpp"

using namespace std;

//Assumed this is called from the index when a new document arrives
MatcherInfo indexUpdate(string& url, string& newpage, string& timestamp, DocumentStore& docstore, TranslationTable& transtable) {
    //-fetch the previous version, and the did of the document, from a tuple store or database (TBD)
    DocumentTuple olddoc = docstore.getDocument(url);
    //Document does not exist yet
    if(olddoc.timestamp.empty()) {
        //Set equal to next docID. This will be officially assigned in the doc store once the document is inserted
        olddoc.docID = docstore.getNextDocID();
    }
    
    //-call makePosts(URL, did, currentpage, previouspage), which generates and returns the new postings that you are creating by your matching algorithm (that is, non-positional and position postings) and the additional translation statements to be appended.
    MatcherInfo info = makePosts(olddoc, newpage);

    //-now you can directly call Fengyuan's code to insert those posts(to be done later)

    //-and then append the translation commands to the right translation vector
    transtable.insert(info.translations, olddoc.docID);

    //-and store the currentpage instead of the previouspage in the tuple store.
    docstore.insertDocument(url, newpage, info.se.getNewSize(), info.maxfragID, timestamp);

    info.docID = olddoc.docID;

    return info;
}

MatcherInfo makePosts(DocumentTuple& olddoc, string& newpage) {
    //-check if there was a previous version, if not create postings with fragid = 0
    unsigned int fragID = olddoc.maxfragID;

    StringEncoder se(olddoc.doc, newpage);

    vector<std::shared_ptr<Block>> commonblocks;
    if(olddoc.doc.length() != 0) {
        //-else, run the graph based matching algorithm on the two versions
        commonblocks = getOptimalBlocks(se, MIN_BLOCK_SIZE, MAX_BLOCK_COUNT, 0.5);
    }

    //Get the translation and posting list
    vector<Translation> translist = getTranslations(se.getOldSize(), se.getNewSize(), commonblocks);
    auto postingslist = getPostings(commonblocks, olddoc.docID, fragID, se);

    //-generate postings and translation statements, and return them. (Question: how do we know the previous largest fragid for this document, so we know what to use as the next fragid? Maybe store with did in the tuple store?)
    MatcherInfo posts(postingslist.first, postingslist.second, translist, se, fragID);

    return posts;
}
