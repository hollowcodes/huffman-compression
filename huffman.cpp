
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


#define len(x) sizeof(x) / sizeof(*x)

int main() {
    std::string fileName = "test.txt";
    
    bool mode = 1;
    if (mode == ENCODE_MODE) {
        std::ifstream ifs(fileName);
        std::string fileContent((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()) );

        char fileContentBuffer[fileContent.size()];
        strcpy(fileContentBuffer, fileContent.c_str());
	
        encode(fileContentBuffer, len(fileContentBuffer));

    }
    else if (mode == DECODE_MODE) {
        // TODO decode
    }
}


/*int main() {
    
    char fileContent[200] = "yoyoyo what is up peoplowwwwls huh pewpew"; // mississippi milchzjtm aggghhhhhhhhhmmmmmmmrrrrtttt
    std::map<int, int> frequencyTable = getCharacterFrequencies(fileContent, length(fileContent));

    if(debug) {
        std::cout << "initial nodes: " << std::endl;
    }

    // initialize nodes
    std::map<int, std::vector<bool>> huffmanCodes;
    std::vector<Node> nodes;

    for (const auto &entry : frequencyTable) {
        std::vector<bool> codeRepresentationList;
        huffmanCodes.insert(std::pair<int, std::vector<bool>>(entry.first, codeRepresentationList));

        Node leaf;
        std::vector<int> asciiChar;
        asciiChar.push_back(entry.first);
        leaf.asciiChars = asciiChar;

        leaf.frequency = entry.second;
        leaf.side = -1;
        leaf.parent = leaf.left = leaf.right = NULL;

        nodes.push_back(leaf);
    }

    sortNodesByFrequency(nodes, 0, nodes.size() - 1);
    print("\nsorted\n");

    Node root = buildHuffmanTree(nodes);
    print("\nbuilt tree\n");

    std::shared_ptr<Node> node = std::make_shared<Node>();
    node->asciiChars = root.asciiChars;
    node->frequency = root.frequency;
    node->side = -1;
    node->parent = NULL;
    node->left = root.left;
    node->right = root.right;

    std::vector<bool> currentCode;
    traversal(node, 1, currentCode, huffmanCodes);

    print("\n\nend\n\n");

    return 0;
}*/



    
    //root.toString();
    //root.left->toString();
    //root.left->left->toString();
    //root.left->left->parent->toString();

    //root.left->right->toString();
    //root.left->right->left->toString();
    //root.left->right->right->toString();
    //print("\n");
    //root.right->toString();
    //root.right->left->toString();
    //root.right->right->toString();
    //root.right->right->left->toString();
    //root.right->right->right->toString();

    //root.right->left->toString();
    //root.right->right->toString();
