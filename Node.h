#pragma once


class Node {

    public:
        std::vector<int> asciiChars;
        int frequency;
        int side;
        bool finishedLeft = false;
        bool finishedRight = false;

        std::shared_ptr<Node> parent;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;

        void toString() {
            std::cout << "\nnode:\n" << "ascii-char: ";
            for (int asciiChar : asciiChars) {
                std::cout << asciiChar << " ";
            }
            std::cout << "| frequency: " << frequency << " | side: " << side << " | child-left: " << left << " | child-right: " << right << std::endl << "----------" << std::endl;
        }

        bool isLeaf() {
            if (left == 0 && right == 0) {
                return true;
            }
            return false;
        }

        void setLeftChild(Node &childNode) {
            left = std::make_shared<Node>();
            left->asciiChars = childNode.asciiChars;
            left->frequency = childNode.frequency;
            left->side = 1;
            left->left = childNode.left;
            left->right = childNode.right;
        }

        void setRightChild(Node &childNode) {
            right = std::make_shared<Node>();
            right->asciiChars = childNode.asciiChars;
            right->frequency = childNode.frequency;
            right->side = 0;
            right->left = childNode.left;
            right->right = childNode.right;
        }

        void setParent(Node &parentNode) {
            parent = std::make_shared<Node>();
            parent->frequency = parentNode.frequency;
            parent->side = -1;
            parent->left = parentNode.left;
            parent->right = parentNode.right;
        }
};