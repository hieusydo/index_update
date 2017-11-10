#include <iostream>
#include <fstream>

#include "dirent.h"
#include "index.hpp"

using namespace std;

//http://forum.codecall.net/topic/60157-read-all-files-in-a-folder/
vector<string> openInDir(string path = ".") {
    DIR*    dir;
    dirent* pdir;
    vector<string> files;

    dir = opendir(path.c_str());

    while (pdir = readdir(dir)) {
        files.push_back(pdir->d_name);
    }
    
    return files;
}

int main(int argc, char **argv) {
    Index index;
    vector<string> filelist = openInDir("./dataset-format");

    for(string& i : filelist) {
        if(i == "." || i == "..")
            continue;
        string filename;
        //Split on |, everything before is filename
        for(auto iter = i.begin(); iter != i.end(); iter++) {
            if(*iter == '|') {
                filename = string(i.begin(), iter);
                break;
            }
        }

        ifstream inputfile(i);
        //https://www.reddit.com/r/learnprogramming/comments/3qotqr/how_can_i_read_an_entire_text_file_into_a_string/cwh8m4d/
        string filecontents{ istreambuf_iterator<char>(inputfile), istreambuf_iterator<char>() };

        cout << "Inserting " << filename << endl;
        index.insert_document(filename, filecontents);
    }

    return 0;
}
