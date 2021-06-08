
#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>

#include "Node.h"

#define print(x) std::cout << x << std::endl
#define length(x) sizeof(x) / sizeof(*x)
#define debug false


//struct Node {
//    std::vector<int> asciiChars;
//    int frequency;
//    int side;
//
//    std::shared_ptr<Node> parent;
//    std::shared_ptr<Node> left;
//    std::shared_ptr<Node> right;
//
//    void toString() {
//        std::cout << "\nnode:\n" << "ascii-char: ";
//        for (int asciiChar : asciiChars) {
//            std::cout << asciiChar << " ";
//        }
//        std::cout << "| frequency: " << frequency << " | side: " << side << " | child-left: " << left << " | child-right: " << right << std::endl << "----------" << std::endl;
//    }
//
//    bool isLeaf() {
//        if (left == 0 && right == 0) {
//            return true;
//        }
//        return false;
//    }
//
//    void setLeftChild(Node &childNode) {
//        left = std::make_shared<Node>();
//        left->frequency = childNode.frequency;
//        left->side = 1;
//        left->left = childNode.left;
//        left->right = childNode.right;
//    }
//
//    void setRightChild(Node &childNode) {
//        right = std::make_shared<Node>();
//        right->frequency = childNode.frequency;
//        right->side = 0;
//        right->left = childNode.left;
//        right->right = childNode.right;
//    }
//
//    void setParent(Node &parentNode) {
//        parent = std::make_shared<Node>();
//        parent->frequency = parentNode.frequency;
//        parent->side = -1;
//        parent->left = parentNode.left;
//        parent->right = parentNode.right;
//    }
//};



void traversal(std::shared_ptr<Node>& node, bool direction, std::vector<bool> currentCode, std::map<int, std::vector<bool>> &codeTable) {

    //for (auto &x : codeTable) {
    //    std::cout << x.first << " -> ";
    //    for (bool b : x.second) {
    //        std::cout << b << "-";
    //    }
    //    std::cout << "\n----------------\n";
    //}

    if (node->finishedLeft && node->finishedRight) {
        if (node->parent == 0) {
            return;
        }
        traversal(node->parent, 1, currentCode, codeTable);
    }

    if (direction == 1) {
        currentCode.push_back(1);

        if (!(node->left->isLeaf())) {
            traversal(node->left, 1, currentCode, codeTable);
        }
        else {
            codeTable[node->left->asciiChars[0]] = currentCode;
            node->finishedLeft = true;
            currentCode.pop_back();
            traversal(node, 0, currentCode, codeTable);
        }
    }
    else if (direction == 0) {
        currentCode.push_back(0);

        if (!(node->right->isLeaf())) {
            traversal(node->right, 1, currentCode, codeTable);
        }
        else {
            codeTable[node->right->asciiChars[0]] = currentCode;
            node->finishedRight = true;
            currentCode.pop_back();
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

    child1.setParent(parent);
    child2.setParent(parent);
    
    parent.setLeftChild(child1);
    parent.setRightChild(child2);




    //parent.left = std::make_shared<Node>();
    //parent.left->frequency = child1.frequency;
    //parent.left->side = 0;
    //parent.left->left = child1.left;
    //parent.left->right = child1.right;
//
    //parent.right = std::make_shared<Node>();
    //parent.right->frequency = child2.frequency;
    //parent.right->side = 1;
    //parent.right->left = child2.left;
    //parent.right->right = child2.right;

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
    while (nodes.size() > 1) {

        if (debug) {
            for (Node n : nodes) {
                std::cout << n.frequency << " ";
            }
            std::cout << "\nnnnnnnn\n";
        }

        if (idx == nodes.size() - 1 && nodes.size() > 1) {
            nodes.insert(nodes.begin(), nodes[idx]);
            nodes.erase(nodes.end());
            idx = 0;
            continue;
        }
        else {
            Node leftChild = nodes[idx];
            Node rightChild = nodes[idx+1];

            if (leftChild.frequency > rightChild.frequency && leftChild.frequency > nodes[idx+2].frequency) { // maybe needed? dunno...: idx + 2 < nodes.size() && 
                idx++;
                continue;
            }
            else {
                Node parent = createNodeFamily(leftChild, rightChild);
                nodes.erase(nodes.begin()+idx);
                nodes.at(idx) = parent;
            }
            
        }
        
        if (debug) {
            for (Node n : nodes) {
                std::cout << n.frequency << " ";
            }
            std::cout << "\nnnnnnnn\n";
        }
        
    }

    return nodes[0];
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
    
    char fileContent[200] = "milch macht munter";
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
        leaf.parent = leaf.left = leaf.right = 0;

        if (debug) {
            leaf.toString();
        }

        nodes.push_back(leaf);
    }

    sortNodesByFrequency(nodes, 0, nodes.size() - 1);

    Node root = buildHuffmanTree(nodes);
    //root.left->toString();
    //root.left->left->toString();
    //root.left->right->toString();
    //root.right->toString();
    //root.right->left->toString();
    //root.right->right->toString();

    std::shared_ptr<Node> node = std::make_shared<Node>();
    node->frequency = root.frequency;
    node->side = -1;
    node->parent = 0;
    node->left = root.left;
    node->right = root.right;

    std::vector<bool> currentCode;
    traversal(node, 1, currentCode, huffmanCodes);

    print("\n\nend\n\n");

    for (auto &x : huffmanCodes) {
        std::cout << x.first << " -> ";
        for (bool b : x.second) {
            std::cout << b << "-";
        }
        std::cout << "\n----------------\n";
    }

    return 0;
}