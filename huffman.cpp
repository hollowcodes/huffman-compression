
#include <iostream>
#include <vector>
#include <map>
#include <memory>

#define print(x) std::cout << x << std::endl
#define length(x) sizeof(x) / sizeof(*x)
#define debug true


struct Node {
    std::vector<int> asciiChars;
    int frequency;
    int side;
    Node *left;
    Node *right;

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
    
    parent.left = new Node;
    parent.left->frequency = child1.frequency;
    parent.left->side = 0;
    parent.left->left = child1.left;
    parent.left->right = child1.right;

    parent.right = new Node;
    parent.right->frequency = child2.frequency;
    parent.right->side = 1;
    parent.right->left = child2.left;
    parent.right->right = child2.right;

    return parent;
}


std::vector<Node> combineNodePairs(std::vector<Node> nodes) {
    std::vector<Node> nodePairs;

    std::vector<Node> nodesCpy = nodes;
    for (size_t i=0; i<nodes.size(); i++) {

        if (nodes[i].frequency != -1) {

            bool paired = false;
            for (size_t j=0; j<nodesCpy.size(); j++) {

                if (i != j && nodes[i].frequency == nodesCpy[j].frequency) {
                    paired = true;

                    Node parent = createNodeFamily(nodes[i], nodesCpy[j]);
                    nodePairs.push_back(parent);

                    Node placeholderNode1;
                    placeholderNode1.frequency = -1;
                    nodes.at(j) = nodes.at(i) = placeholderNode1;
                    nodesCpy = nodes;
                    
                    break;
                }
            }
            if (!paired) {
                nodePairs.push_back(nodes[i]);
            }
        }
    }

    if(debug) {
        for (Node n : nodePairs) {
            n.toString();
        }
        print("\n\nnext stage:\n\n");
    }

    //print("bgin");
    //Node parent = nodePairs[0];
    //parent.toString();
    //parent.left->toString();
    //print("end");


    return nodePairs;
}



std::vector<Node> combineLeftOverNodes(std::vector<Node> nodes) {
    std::vector<Node> nodePairs;

    std::vector<Node> nodesCpy = nodes;
    for (size_t i=0; i<nodes.size(); i++) {

        if (nodes[i].frequency != -1) {

            bool paired = false;
            for (size_t j=0; j<nodesCpy.size(); j++) {

                if (i != j && nodesCpy[j].frequency != -1) {
                    paired = true;

                    Node parent = createNodeFamily(nodes[i], nodesCpy[j]);
                    nodePairs.push_back(parent);

                    Node placeholderNode1;
                    placeholderNode1.frequency = -1;
                    nodes.at(j) = nodes.at(i) = placeholderNode1;
                    nodesCpy = nodes;

                    break;
                }
            }
            if (!paired) {
                nodePairs.push_back(nodes[i]);
            }
        }
    }

    if(debug) {
        for (Node n : nodePairs) {
            n.toString();
        }
        print("\n\nnext stage:\n\n");
    }

    return nodePairs;
}



std::map<int, int> getCharacterFrequencies(char *textPtr, unsigned int totalChars) {
    std::map<int, int> frequencyTable;

    for (size_t i=0; i<totalChars; i++) {
        int currentAsciiChar = int(textPtr[i]);

        if (frequencyTable.count(currentAsciiChar) == 0) {
            frequencyTable.insert(std::pair<int, int>(currentAsciiChar, 1));
        }
        else {
            frequencyTable[currentAsciiChar]++;
        }
    }

    // for (const auto &x : frequencyTable) {
    //     std::cout << x.first << ": " << x.second << "\n";
    // }
    // print("\n");

    frequencyTable.erase(0);

    return frequencyTable;
}


int main() {
    
    char text[153] = "hey mo namo tbone la arachna discotheka moneka la bibliotheka estas muchas grandos peromonteca";
    std::map<int, int> frequencyTable = getCharacterFrequencies(text, length(text));

    if(debug) {
        std::cout << "initial nodes: " << std::endl;
    }

    // initialize nodes
    std::vector<Node> initNodes;
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

        initNodes.push_back(leaf);
    }

    if(debug) {
        std::cout << "\ntree begin:" << std::endl;
    }

    std::vector<Node> currentNodes = initNodes;
    int preNodeAmount = currentNodes.size();
    while (true) {
        currentNodes = combineNodePairs(initNodes);
        if (preNodeAmount == currentNodes.size()) {
            break;
        }
        preNodeAmount = currentNodes.size();
    }

    print("\n--\nLOLOLOLOLOLOLOLOLL\n--\n");

    while (currentNodes.size() > 1) {
        currentNodes = combineLeftOverNodes(currentNodes);
    }

    Node root = currentNodes[0];
    root.toString();
    root.left->toString();
    //root.right->toString();
    root.left->left->toString();
    root.left->right->toString();



    //auto *currentNode = &tree;
    //int c = 0;
    //
    //while (c < 5) {
    //    try {
    //        print(c);
    //        currentNode->toString();
    //        auto leftNode = currentNode->left;
    //        currentNode = leftNode;
    //        //print();
    //        c++;
    //    }
    //    catch (const std::exception& e) {
    //        print("end");
    //        break;
    //    }
    //}
    

    
    //Node childA1;
    //std::vector<int> asciiChar1;
    //asciiChar1.push_back(32);
    //childA1.asciiChars = asciiChar1;
    //childA1.frequency = 2;
    //childA1.side = 0;
    //childA1.left = 0;
    //childA1.right = 0;
//
    //Node childA2;
    //std::vector<int> asciiChar2;
    //asciiChar2.push_back(64);
    //childA2.asciiChars = asciiChar2;
    //childA2.frequency = 1;
    //childA2.side = 0;
    //childA2.left = 0;
    //childA2.right = 0;
//
    //std::vector<Node> children;
    //children.push_back(childA1);
    //children.push_back(childA2);
    //std::vector<Node> p = f(children);
//
    //Node parent = p[0];
    //parent.toString();
    //parent.left->toString();
    //parent.right->toString();

        /* NODE TEST (WORKS) 
Node parentA = createNodeFamily(childA1, childA2);

    Node childB1;
    std::vector<int> asciiChar3;
    asciiChar3.push_back(128);
    childB1.asciiChars = asciiChar3;
    childB1.frequency = 3;
    childB1.side = 0;
    childB1.left = 0;
    childB1.right = 0;

    Node childB2;
    std::vector<int> asciiChar4;
    asciiChar4.push_back(256);
    childB2.asciiChars = asciiChar4;
    childB2.frequency = 1;
    childB2.side = 0;
    childB2.left = 0;
    childB2.right = 0;

    Node parentB = createNodeFamily(childB1, childB2);

    Node root = createNodeFamily(parentA, parentB);


    root.toString();
    root.left->toString();
    root.left->left->toString();
    root.left->right->toString();

    root.right->toString();
    root.right->left->toString();
    root.right->right->toString();

    */

    print("\n\nend");
    
    
    return 0;
}