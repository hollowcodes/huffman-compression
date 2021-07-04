
#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>

#include "Node.h"

#define infoPrint(x) std::cout << "\n[INFO] " << x << std::endl
#define debugPrint(x) std::cout << "\n[DEBUG] " << x << std::endl
#define errorPrint(x) std::cout << "\n[ERROR] " << x << std::endl

#define length(x) sizeof(x) / sizeof(*x)
#define debug true


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

void encode() {
    char fileContent[200] = "mississippiuwu"; // mississippi milchzjtm aggghhhhhhhhhmmmmmmmrrrrtttt yoyoyo what is up peoplowwwwls huh pewpew

    // get letter frequency dictionary (keys: assci-value of char, value: frequency)
    std::map<int, int> frequencyTable = getCharacterFrequencies(fileContent, length(fileContent));
    
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
    sortNodesByFrequency(nodes, 0, nodes.size() - 1);
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

}



int main() {
    encode();
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