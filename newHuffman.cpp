
#include <iostream>
#include <vector>
#include <map>
#include <memory>

#define print(x) std::cout << x << std::endl
#define length(x) sizeof(x) / sizeof(*x)
#define debug false


struct Node {
    std::vector<int> asciiChars;
    int frequency;
    int side;

    std::shared_ptr<Node> left;// Node *left;
    std::shared_ptr<Node> right;

    void toString() {
        std::cout << "\nnode:\n" << "ascii-char: ";
        for (int asciiChar : asciiChars) {
            std::cout << asciiChar << " ";
        }
        std::cout << "| frequency: " << frequency << " | side: " << side << " | child-left: " << left << " | child-right: " << right << std::endl << "----------" << std::endl;

    }
};

Node createNodeFamily(Node child1, Node child2) {
    std::vector<int> asciiChars = child1.asciiChars;
    asciiChars.insert(asciiChars.end(), child2.asciiChars.begin(), child2.asciiChars.end());

    Node parent;
    parent.asciiChars = asciiChars;
    parent.frequency = child1.frequency + child2.frequency;

    parent.side = -1;
    
    parent.left = std::make_shared<Node>();
    parent.left->frequency = child1.frequency;
    parent.left->side = 0;
    parent.left->left = child1.left;
    parent.left->right = child1.right;

    parent.right = std::make_shared<Node>();
    parent.right->frequency = child2.frequency;
    parent.right->side = 1;
    parent.right->left = child2.left;
    parent.right->right = child2.right;

    return parent;
}


void sortNodesByFrequency(std::vector<Node>& nodes, uint32_t leftNodeIdx, uint32_t rightNodeIdx) {
    uint32_t pivotNodeIdx = rightNodeIdx;
    Node pivotNode = nodes[pivotNodeIdx];

    if (leftNodeIdx < rightNodeIdx) {
        uint32_t i = leftNodeIdx - 1;
        for (size_t j=leftNodeIdx; j<rightNodeIdx; j++) {
            if (nodes[j].frequency < pivotNode.frequency) {
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


Node buildHuffmanTree(std::vector<Node> nodes) {
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
            Node parent = createNodeFamily(leftChild, rightChild);
            nodes.erase(nodes.begin()+idx);
            nodes.at(idx) = parent;
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
    
    char fileContent[50] = "mississippi";
    std::map<int, int> frequencyTable = getCharacterFrequencies(fileContent, length(fileContent));

    if(debug) {
        std::cout << "initial nodes: " << std::endl;
    }

    // initialize nodes
    std::vector<Node> nodes;
    for (const auto &x : frequencyTable) {
        Node leaf;
        std::vector<int> asciiChar;
        asciiChar.push_back(x.first);
        leaf.asciiChars = asciiChar;

        leaf.frequency = x.second;
        leaf.side = -1;
        leaf.left = leaf.right = 0;

        if (debug) {
            leaf.toString();
        }

        nodes.push_back(leaf);
    }

    sortNodesByFrequency(nodes, 0, nodes.size() - 1);

    Node root = buildHuffmanTree(nodes);
    root.left->toString();
    root.right->toString();
    root.right->right->toString();
    root.right->left->toString();
    root.right->left->right->toString();
    root.right->left->left->toString();

    print("\n\nend");

    return 0;
}