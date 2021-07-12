
#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <fstream>
#include <string>
#include <cstring>

#include "encode.cpp"
#include "decode.cpp"

#define ENCODE_MODE 1
#define DECODE_MODE 0

int main() {
    bool mode = 0;

    std::string fileName;
    if (mode == 1) {
        fileName = "test.txt";
    }
    else {
        fileName = "test.huf";
    }
    
    std::ifstream ifs(fileName);
    std::string fileContent((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()) );

    char fileContentBuffer[fileContent.size()];
    strcpy(fileContentBuffer, fileContent.c_str());

    if (mode == ENCODE_MODE) {
        encode(fileContentBuffer, fileContent.size());
    }
    else if (mode == DECODE_MODE) {
        decode(fileContentBuffer, fileContent.size());
    }
}

/*
  -> 0-0-
----------------
! -> 0-0-1-
----------------
d -> 1-0-0-0-
----------------
e -> 1-0-0-1-
----------------
f -> 1-1-0-1-0-
----------------
h -> 1-1-0-1-1-
----------------
i -> 1-1-1-0-0-
----------------
l -> 0-1-0-
----------------
m -> 1-1-1-0-1-
----------------
n -> 1-1-1-1-0-
----------------
o -> 0-1-1-
----------------
r -> 1-0-1-0-
----------------
s -> 1-1-1-1-1-
----------------
w -> 1-0-1-1-
----------------
y -> 1-1-0-0-
----------------
*/