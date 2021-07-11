#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <bitset>

#define infoPrint(x) std::cout << "\n[INFO] " << x << std::endl
#define debugPrint(x) std::cout << "\n[DEBUG] " << x << std::endl
#define errorPrint(x) std::cout << "\n[ERROR] " << x << std::endl



std::string recoverContent(std::string& binaryEncodedContent, std::map<char, int>& codeLengthMap) {
    std::string decodedContent = "";

    return decodedContent;
}


void decode(char* fileContent, unsigned int contentSize) {
    std::map<char, int> codeLengthMap;

    std::string encodedSequenceBinary = "";
    bool finishedHead = false;
    for (size_t i=0; i<contentSize; i+0) {
        if (!finishedHead) {
            if (fileContent[i] == '#' && fileContent[i+1] == '#') {
                finishedHead = true;
            }
            codeLengthMap[fileContent[i]] = fileContent[i+1];
            i += 2;
        }
        else {
            encodedSequenceBinary += std::bitset<8>(fileContent[i]).to_string();
            i++;
        }
    }

    //std::string encodedContent(fileContent);

    debugPrint(encodedSequenceBinary);
}


/*
  -> 1-0-1-0-
----------------
! -> 0-0-
----------------
d -> 1-0-1-1-
----------------
e -> 1-1-0-0-
----------------
h -> 1-1-0-1-
----------------
l -> 0-1-
----------------
o -> 1-0-0-
----------------
r -> 1-1-1-0-
----------------
w -> 1-1-1-1-
----------------
*/
