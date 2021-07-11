#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <fstream>
#include <string>
#include <bitset>

#include "Node.h"

#define infoPrint(x) std::cout << "\n[INFO] " << x << std::endl
#define debugPrint(x) std::cout << "\n[DEBUG] " << x << std::endl
#define errorPrint(x) std::cout << "\n[ERROR] " << x << std::endl

#define len(x) sizeof(x) / sizeof(*x)
#define debug false

typedef std::pair<int, std::vector<bool>> codePair;


void traversal(std::shared_ptr<Node> &node, bool direction, std::vector<bool> currentCode, std::map<int, std::vector<bool>>& codeTable) {
    if (debug) {
        debugPrint("------------------------------------\n");
        for (auto &x : codeTable) {
            std::cout << x.first << " -> ";
            for (bool b : x.second) {
                std::cout << b << "-";
            }
            std::cout << "\n----------------\n";
        }
    }

    // if the left and right child-node of the current node has been traversed, go to the parent-node
    if (node->finishedLeft && node->finishedRight) {
        currentCode.pop_back();

        // if the current node is left, mark the parent-node with "finished left" and continue with the parent-node
        if (node->side == 1) {
            node->parent->finishedLeft = true;

            traversal(node->parent, 0, currentCode, codeTable);
        }

        // if the current node is right, mark the parent-node with "finished right" and continue with the parent-node
        else if (node->side == 0) {
            node->parent->finishedRight = true;

            // if the parent-node is the root node (side = -1) the traversal is done
            if (node->parent->side == -1) {
                return;
            }

            traversal(node->parent, 1, currentCode, codeTable);
        }
    }

    // if the direction is "left" (= 1), add "1" to the current huffman code and continue with left child-node if possible
    else if (direction == 1) {
        currentCode.push_back(1);

        // if the next left node isn't a leaf node continue with it
        if (!node->left->isLeaf()) {
            traversal(node->left, 1, currentCode, codeTable);
        }
        // if not, save the current huffman code to the dictionary where key = the char of the leaf-node
        else {
            codeTable[(node->left->asciiChars)[0]] = currentCode;
            currentCode.pop_back();
            node->finishedLeft = true;

            // continue with the node on the right (= 0) side
            traversal(node, 0, currentCode, codeTable);
        }
    }
    // if the direction is "right" (= 0), add "0" to the current huffman code and continue with left child-node if possible
    else if (direction == 0) {
        currentCode.push_back(0);

        // if the next right node isn't a leaf node continue with it
        if (!node->right->isLeaf()) {
            traversal(node->right, 1, currentCode, codeTable);
        }
        // if not, save the current huffman code to the dictionary where key = the char of the leaf-node
        else {
            codeTable[(node->right->asciiChars)[0]] = currentCode;
            currentCode.pop_back();
            node->finishedRight = true;

            // continue with the node on the left (= 1) side
            traversal(node, 1, currentCode, codeTable);
        }
    }
}



Node createNodeFamily(Node &child1, Node &child2) {
    // set the parent-nodes assci-chars to the combination of the chars of both children
    std::vector<int> asciiChars = child1.asciiChars;
    asciiChars.insert(asciiChars.end(), child2.asciiChars.begin(), child2.asciiChars.end());

    // create the parent-node, side = -1 (current root)
    Node parent;
    parent.asciiChars = asciiChars;
    parent.frequency = child1.frequency + child2.frequency;
    parent.side = -1;

    // set the child-node pointers of the parent-node the child-nodes
    parent.setLeftChild(child1);
    parent.setRightChild(child2);

    // set the parent-node pointer of the children to the just created parent-node
    parent.left->setParent(parent);
    parent.right->setParent(parent);

    return parent;
}


// sort the leaf-nodes by frequency (lower -> higher) using quick-sort
void sortNodesByFrequency(std::vector<Node>& nodes, int leftNodeIdx, int rightNodeIdx) {
    if (nodes.size() <= 1) {
        return;
    }

    int pivotNodeIdx = rightNodeIdx;
    Node pivotNode = nodes[pivotNodeIdx];

    if (leftNodeIdx < rightNodeIdx) {
        int i = leftNodeIdx - 1;

        for (size_t j=leftNodeIdx; j<rightNodeIdx; j++) {
            if (nodes[j].frequency <= pivotNode.frequency) {
                i++;
                std::swap(nodes[j], nodes[i]);
            }
        }
        std::swap(nodes[i+1], nodes[pivotNodeIdx]);
        pivotNodeIdx = i + 1;

        sortNodesByFrequency(nodes, leftNodeIdx, pivotNodeIdx - 1);
        sortNodesByFrequency(nodes, pivotNodeIdx + 1, rightNodeIdx);
    }
    else {
        return;
    }
}


// build the huffman tree from the initial leaf-node list
Node buildHuffmanTree(std::vector<Node> &nodes) {
    int idx = 0;
    while (true) {

        Node leftChild = nodes[0];
        Node rightChild = nodes[1];

        // pair two (smallest) nodes to build the parent-node
        Node parent = createNodeFamily(leftChild, rightChild);

        // delete the nodes that were used
        nodes.erase(nodes.begin());
        nodes.erase(nodes.begin());

        // if all leaf-nodes are used up (= the node list is empty), the tree building is done
        if (nodes.size() == 0) {
            return parent;
        }


        // insert the created parent-node back into the node list so that the frequency remains in order
        // TODO replace with binary search
        for (size_t i=0; i<nodes.size(); i++) {
            if (nodes[i].frequency > parent.frequency) {
                nodes.insert(nodes.begin() + i, parent);
                break;
            }
            if (i == nodes.size() - 1) {
                nodes.insert(nodes.end(), parent);
                break;
            }
        }
    }
}


// count the frequency of each character and save them to a dictionary
std::map<int, int> getCharacterFrequencies(char *fileContent, unsigned int contentSize) {
    std::map<int, int> frequencyTable;
    for (size_t i=0; i<contentSize; i++) {
        int currentAsciiChar = int(fileContent[i]);

        if (frequencyTable.count(currentAsciiChar) == 0) {
            frequencyTable.insert(std::pair<int, int>(currentAsciiChar, 1));
        }
        else {
            frequencyTable[currentAsciiChar]++;
        }
    }

    frequencyTable.erase(0);

    return frequencyTable;
}


// splits the bit (/bool) stream into packs of 8 (= 1 byte), pads with zeros and stores them as chars
std::string bitStreamToString(std::vector<bool>& bitStream) {
    std::string charSequence = "";
    std::bitset<8> currentByte;

    unsigned int c = 0;
    unsigned int idx = 0;
    for (int i=0; i<bitStream.size(); i++) {
        bool currentBit = bitStream[i];

        if (currentBit) {
            currentByte |= 1 << 7-c;
        }

        c++;

        if (c == 8 || i == bitStream.size() - 1) {
            charSequence += static_cast<unsigned char>(currentByte.to_ulong());
            currentByte = 0x0;
            c = 0;
            idx += 1;
        }
    }

    return charSequence;
}


// sort huffman codes according to the code length
std::vector<codePair> sortCodesByLength(std::map<int, std::vector<bool>>& huffmanCodeTable) {
    std::vector<codePair> sortedHuffmanCodes;
    std::copy(huffmanCodeTable.begin(), huffmanCodeTable.end(), std::back_inserter<std::vector<codePair>>(sortedHuffmanCodes));

    std::sort(sortedHuffmanCodes.begin(), sortedHuffmanCodes.end(),
        [](const codePair &l, const codePair &r)
        {
            if (l.second.size() != r.second.size()) {
                return l.second.size() < r.second.size();
            }

            return l.first < r.first;
        }
    );

    return sortedHuffmanCodes;
}


// create a header for the encoded file
std::string createHeader(std::map<int, std::vector<bool>>& huffmanCodeTable) {
    /*
        format (ordered by code-length):
        <code-length><character><code-length><character>...##
    */

    // map didn't perserve the order, therefore sort again according to the code length
    std::vector<codePair> sortedHuffmanCodes = sortCodesByLength(huffmanCodeTable);

    std::string header = "";
    for (codePair p : sortedHuffmanCodes) {
        header += std::to_string(p.second.size()) + static_cast<char>(p.first);
    }

    return header + "##";
}


// convert the generated huffman codes to canonical huffman codes
std::map<int, std::vector<bool>> makeCanonical(std::vector<codePair>& huffmanCodes) {
    /* 
        algorithm:
        1. sort the huffman codes according to length
        2. create the first canonical code by taking as many zeros as the length of the smalles code
        3. to create the next code, add 1 to the last code and padd it with zeros so that it matches the original huffman code length
    */

    std::map<int, std::vector<bool>> newHuffmanCodes;

    unsigned int previousCodeLength = huffmanCodes[0].second.size();
    newHuffmanCodes[huffmanCodes[0].first] = std::vector<bool>(2, 0);

    // sadly the bitLength can't be dynamical (let's hope there is no code with lengh over 64)
    const unsigned int bitLength = 64;
    std::bitset<bitLength> previousCanonicalCode;

    for (size_t i=1; i<huffmanCodes.size(); i++) {
        unsigned int currentCodeLength = huffmanCodes[i].second.size();
        
        // add 1 to last code and shift/padd it in order to keep its length
        std::bitset<64> currentCanonicalCode(previousCanonicalCode.to_ulong() + 1);
        currentCanonicalCode <<= (currentCodeLength - previousCodeLength);

        // remove padding zeros
        std::string currentCanonicalCodeString = currentCanonicalCode.to_string().erase(0, bitLength - currentCodeLength);

        std::vector<bool> currentCanonicalCodeVector;
        for (auto b : currentCanonicalCodeString) {
            currentCanonicalCodeVector.push_back(b == '1');
        }

        newHuffmanCodes[huffmanCodes[i].first] = currentCanonicalCodeVector;

        previousCodeLength = currentCodeLength;
        previousCanonicalCode = currentCanonicalCode;
    }

    return newHuffmanCodes;
}


// encode a set of characters to huffman their representations
void encode(char* fileContent, unsigned int contentSize) {
    // get letter frequency dictionary (keys: assci-value of char, value: frequency)

    std::map<int, int> frequencyTable = getCharacterFrequencies(fileContent, contentSize);

    // initialize huffman code dictionary (keys: assci-value of char, value: list of 0-1 huffman representation)
    std::map<int, std::vector<bool>> huffmanCodes;

    // initialize list containing all the chars as leaf-nodes
    std::vector<Node> nodes;

    // fill leaf-nodes list
    for (const auto &entry : frequencyTable) {
        std::vector<bool> codeRepresentationList;
        huffmanCodes.insert(std::pair<int, std::vector<bool>>(entry.first, codeRepresentationList));

        // create leaf node from current char
        Node leaf;
        std::vector<int> asciiChar;
        asciiChar.push_back(entry.first);
        leaf.asciiChars = asciiChar;
        leaf.frequency = entry.second;
        leaf.side = -1;
        leaf.parent = leaf.left = leaf.right = NULL;

        nodes.push_back(leaf);
    }

    // sort leaf-node list by frequency (lower -> higher)
    std::sort(nodes.begin(), nodes.end(),
        [](const Node &l, const Node &r)
        {
            if (l.frequency != r.frequency) {
                return l.frequency < r.frequency;
            }
            return l.frequency < r.frequency;
        });
        
    infoPrint("sorted characters");

    // build huffman tree
    Node root = buildHuffmanTree(nodes);
    infoPrint("built tree");

    // get root node
    std::shared_ptr<Node> node = std::make_shared<Node>();
    node->asciiChars = root.asciiChars;
    node->frequency = root.frequency;
    node->side = -1;
    node->parent = NULL;
    node->left = root.left;
    node->right = root.right;

    // traverse huffman tree to gather the huffman representations
    std::vector<bool> currentCode;
    traversal(node, 1, currentCode, huffmanCodes);
    infoPrint("gathered huffman codes");

    if (debug) {
        debugPrint("generated huffman codes: ");
        for (auto &x : huffmanCodes) {
            std::cout << static_cast<char>(x.first) << " -> ";
            for (bool b : x.second) {
                std::cout << b << "-";
            }
            std::cout << "\n----------------\n";
        }
    }
    
    // sort huffman codes according to the code lengths
    std::vector<codePair> sortedHuffmanCodes = sortCodesByLength(huffmanCodes);

    if (debug) {
        for (int i=0; i<sortedHuffmanCodes.size(); i++) {
            std::cout << sortedHuffmanCodes[i].first << " : ";
            for (int j=0; j<sortedHuffmanCodes[i].second.size(); j++) {
                std::cout << sortedHuffmanCodes[i].second[j] << " ";
            }
        std::cout << "\n";
        }
    }

    // create canonical huffman codes
    std::map<int, std::vector<bool>> canonicalHuffmanCodes = makeCanonical(sortedHuffmanCodes);

    if (debug) {
        std::cout << "\n\n";
        for (auto &x : canonicalHuffmanCodes) {
            std::cout << static_cast<char>(x.first) << " -> ";
            for (bool b : x.second) {
                std::cout << b << "-";
            }
            std::cout << "\n----------------\n";
        }
    }

    // encode the entire file content to the canonical code as one bit sequence
    std::vector<bool> bitStream;
    for (int i=0; i<contentSize; i++) {
        std::vector<bool> canonicalCode = canonicalHuffmanCodes[fileContent[i]];
        bitStream.insert(bitStream.end(), canonicalCode.begin(), canonicalCode.end());
    }

    // split the bit stream into 8 bit chunks, convert them to chars and create the final encoded sequence
    std::string encodedContent = bitStreamToString(bitStream);
    infoPrint("created canonical huffman codes");

    // create and add the header with the information for the decoder
    std::string header = createHeader(canonicalHuffmanCodes);
    encodedContent = header + encodedContent;

    // save the encoded file content to a .huf file
    std::ofstream ofs("test.huf");
    ofs << encodedContent;
    ofs.close();
    infoPrint("saved encoded file");
    
}



