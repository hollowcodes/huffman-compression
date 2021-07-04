#pragma once


class Node {

    public:
        // initialize the nodes attributes
        std::vector<int> asciiChars;
        int frequency;
        int side;
        bool finishedLeft = false;
        bool finishedRight = false;

        // create smart pointers for the linked node-/tree-structure
        std::shared_ptr<Node> parent;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;

        // print out the information of the nodes
        void toString() {
            std::cout << "\nnode: " << "ascii-char: ";
            for (int asciiChar : asciiChars) {
                std::cout << static_cast<char>(asciiChar) << " ";
            }
            std::cout << "| frequency: " << frequency << " | side: " << side << " | parent: " << parent << " | child-left: " << left << " | child-right: " << right << std::endl << "----------" << std::endl;
        }

        // check if this node is a leaf-node (= no children)
        bool isLeaf() {
            if (left == NULL && right == NULL) {
                return true;
            }
            return false;
        }

        // set the left child-node of this node
        void setLeftChild(Node &childNode) {
            left = std::make_shared<Node>();
            left->asciiChars = childNode.asciiChars;
            left->frequency = childNode.frequency;
            left->side = 1;
            left->left = childNode.left;
            left->right = childNode.right;
            left->parent = childNode.parent;
            
            // if the left child-node has children, set the pointer to its parent-node this node
            if (left->left != 0) {
                left->left->parent = left;
                left->right->parent = left;
            }
        }

        // set the right child-node of this node
        void setRightChild(Node &childNode) {
            right = std::make_shared<Node>();
            right->asciiChars = childNode.asciiChars;
            right->frequency = childNode.frequency;
            right->side = 0;
            right->left = childNode.left;
            right->right = childNode.right;
            right->parent = childNode.parent;

            // if the right child-node has children, set the pointer to its parent-node this node
            if (right->left != 0) {
                right->left->parent = right;
                right->right->parent = right;
            }
        }

        // set the parent-node of this node
        void setParent(Node &parentNode) {
            parent = std::make_shared<Node>();
            parent->asciiChars = parentNode.asciiChars;
            parent->frequency = parentNode.frequency;
            parent->side = parentNode.side;
            parent->left = parentNode.left;
            parent->right = parentNode.right;
            parent->parent = NULL;
        }
};