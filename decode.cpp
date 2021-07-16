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


std::map<std::string, int> recreateCodes(std::vector<std::pair<int, int>>& codeLengths) {
    std::map<std::string, int> canonicalCodes;

    unsigned int previousCodeLength = codeLengths[0].second;
    canonicalCodes[std::string(previousCodeLength, '0')] = codeLengths[0].first;

    const unsigned int bitLength = 32;
    std::bitset<bitLength> previousCanonicalCode;
    for (size_t i=1; i<codeLengths.size(); i++) {
        unsigned int currentCodeLength = codeLengths[i].second;
        
        // add 1 to last code and shift/padd it in order to keep its length
        std::bitset<bitLength> currentCanonicalCode(previousCanonicalCode.to_ulong() + 1ULL);
        currentCanonicalCode <<= (currentCodeLength - previousCodeLength);

        // remove padding zeros
        std::string currentCanonicalCodeString = currentCanonicalCode.to_string().erase(0, bitLength - currentCodeLength);
        canonicalCodes[currentCanonicalCodeString] = codeLengths[i].first;

        previousCodeLength = currentCodeLength;
        previousCanonicalCode = currentCanonicalCode;
    }

    return canonicalCodes;
}


std::string recoverContent(std::string& binaryStringEncodedContent, std::map<std::string, int>& canonicalCodes, unsigned int& zeroPadAmount) {
    binaryStringEncodedContent.erase((binaryStringEncodedContent.size() - zeroPadAmount), (binaryStringEncodedContent.size() - 1));

    std::string decodedContent = "";
    std::string currentCode = "";
    for (char &b : binaryStringEncodedContent) {
        currentCode += b;
        if (canonicalCodes.find(currentCode) != canonicalCodes.end()) {
            decodedContent += canonicalCodes[currentCode];
            currentCode = "";
        }
    }

    return decodedContent;
}


void decode(char* fileContent, unsigned int contentSize) {
    // codeLengthMap: pair<int (ascii value of char), int <length of the code of char)>

    char headDelimiterAsciiVal = 127;

    unsigned int zeroPadAmount = headDelimiterAsciiVal - 1 - (int) fileContent[0];

    std::vector<std::pair<int, int>> codeLengths;
    std::string encodedBinarySequence = "";
    bool finishedHead = false;

    for (size_t i=1; i<contentSize; i+=0) {
        // finishedHead == true: fileContent[i] : code-length (as char -> ascii table) , fileContent[i+1]: according character
        if (!finishedHead) {
            // if there is 35 x # in the text Im screwd...
            if ((int) fileContent[i] == headDelimiterAsciiVal) {
                finishedHead = true;
                i++;
            }
            else {
                //std::cout << "length: " << headDelimiterAsciiVal - 1 - (int) fileContent[i] << " , char: " << fileContent[i+1] << std::endl;
                codeLengths.push_back(std::pair<int, int>(fileContent[i+1], (headDelimiterAsciiVal - 1 - (int) fileContent[i])));
                i += 2;
            }
        }
        else {
            //std::cout << std::bitset<8>((int) fileContent[i]).to_string() << " = " << static_cast<unsigned char>( std::bitset<8>((int) fileContent[i]).to_ulong() ) << std::endl;
            //if (std::bitset<8>((int) fileContent[i]).to_string() == "00000000") { debugPrint("yo");}
            encodedBinarySequence += std::bitset<8>((int) fileContent[i]).to_string();
            i++;
        }
    }
    infoPrint("recieved header information");

    //for (std::pair<int, int> x : codeLengths) {
    //    std::cout << static_cast<char>(x.first) << " -> " << (int) x.second << std::endl;
    //}

    std::map<std::string, int> canonicalCodes = recreateCodes(codeLengths);
    infoPrint("canonical codes recreated");
    //for (auto&x : canonicalCodes) {
    //    std::cout << static_cast<char>(x.second) << " -> " << x.first << std::endl;
    //}

    std::string decodedContent = recoverContent(encodedBinarySequence, canonicalCodes, zeroPadAmount);
    infoPrint("compressed content recovered");
    //debugPrint(decodedContent);

    std::ofstream ofs("new_test.txt");
    ofs.write(decodedContent.c_str(), decodedContent.size());
    ofs.close();
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
