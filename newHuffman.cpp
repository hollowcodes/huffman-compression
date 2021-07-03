
#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>

#include "Node.h"

#define print(x) std::cout << x << std::endl
#define length(x) sizeof(x) / sizeof(*x)
#define debug false


void traversal(std::shared_ptr<Node> &node, bool direction, std::vector<bool> currentCode, std::map<int, std::vector<bool>>& codeTable) {

    if (debug) {
        std::cout << "\nm-m-m-m-m-m-m-m-m-m-m-m-m-m-m-m-m-m-m-m-m-\n" << std::endl;
        for (auto &x : codeTable) {
            std::cout << x.first << " -> ";
            for (bool b : x.second) {
                std::cout << b << "-";
            }
            std::cout << "\n----------------\n";
        }
    }


    if (node->finishedLeft && node->finishedRight) {
        currentCode.pop_back();

        //node->toString();
        //std::cout << node->parent->finishedLeft << " " << node->parent->finishedRight << std::endl;

        if (node->side == 1) {
            node->parent->finishedLeft = true;

            traversal(node->parent, 0, currentCode, codeTable);
        }
        else if (node->side == 0) {
            node->parent->finishedRight = true;

            if (node->parent->side == -1) {
                print("lul");
                return;
            }

            traversal(node->parent, 1, currentCode, codeTable);
        }
    }

    else if (direction == 1) {
        currentCode.push_back(1);

        if (!node->left->isLeaf()) {
            traversal(node->left, 1, currentCode, codeTable);
        }
        else {
            codeTable[(node->left->asciiChars)[0]] = currentCode;
            currentCode.pop_back();
            node->finishedLeft = true;
            traversal(node, 0, currentCode, codeTable);
        }
    }
    else if (direction == 0) {
        currentCode.push_back(0);

        if (!node->right->isLeaf()) {
            traversal(node->right, 1, currentCode, codeTable);
        }
        else {
            codeTable[(node->right->asciiChars)[0]] = currentCode;
            currentCode.pop_back();
            node->finishedRight = true;
            traversal(node, 1, currentCode, codeTable);
        }
    }
}



Node createNodeFamily(Node &child1, Node &child2) {
    std::vector<int> asciiChars = child1.asciiChars;
    asciiChars.insert(asciiChars.end(), child2.asciiChars.begin(), child2.asciiChars.end());

    Node parent;
    parent.asciiChars = asciiChars;
    parent.frequency = child1.frequency + child2.frequency;

    parent.side = -1;

    parent.setLeftChild(child1);
    parent.setRightChild(child2);

    parent.left->setParent(parent);
    parent.right->setParent(parent);

    return parent;
}


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


Node buildHuffmanTree(std::vector<Node> &nodes) {
    int idx = 0;
    while (true) {

        Node leftChild = nodes[0];
        Node rightChild = nodes[1];

        Node parent = createNodeFamily(leftChild, rightChild);
        nodes.erase(nodes.begin());
        nodes.erase(nodes.begin());

        if (nodes.size() == 0) {
            return parent;
        }

        for (size_t i=0; i<nodes.size(); i++) {
            if (nodes[i].frequency > parent.frequency) {
                if (true) {
                    nodes.insert(nodes.begin() + i, parent);
                    break;
                }
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


int main() {
    
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

        // leaf.toString();

        nodes.push_back(leaf);
    }


    sortNodesByFrequency(nodes, 0, nodes.size() - 1);
    print("\nsorted\n");
    //for (Node n : nodes) {
    //    std::cout << n.frequency << " ";
    //}
    //print("\n");

    //std::vector<Node>::iterator minIter1 = std::max_element( nodes.begin(), nodes.end(), []( const Node &a, const Node &b )
    //                         {
    //                             return a.frequency > b.frequency;
    //                         } );
//
    //std::cout << "min element at: " << std::distance(std::begin(nodes), minIter1);

    Node root = buildHuffmanTree(nodes);
    print("\nbuilt tree\n");

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

//
    //root.right->left->toString();
    //root.right->right->toString();
////
    std::shared_ptr<Node> node = std::make_shared<Node>();
    node->asciiChars = root.asciiChars;
    node->frequency = root.frequency;
    node->side = -1;
    node->parent = NULL;
    node->left = root.left;
    node->right = root.right;
////
    std::vector<bool> currentCode;
    traversal(node, 1, currentCode, huffmanCodes);
////
    print("\n\nend\n\n");
////
    for (auto &x : huffmanCodes) {
        std::cout << static_cast<char>(x.first) << " -> ";
        for (bool b : x.second) {
            std::cout << b << "-";
        }
        std::cout << "\n----------------\n";
    }

    return 0;
}