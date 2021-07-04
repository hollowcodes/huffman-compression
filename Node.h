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
                std::cout << static_cast<char>(asciiChar) << " ";
            }
            std::cout << "| frequency: " << frequency << " | side: " << side << " | parent: " << parent << " | child-left: " << left << " | child-right: " << right << std::endl << "----------" << std::endl;
        }

        bool isLeaf() {
            if (left == NULL && right == NULL) {
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
            left->parent = childNode.parent;

            std::cout << left->left << std::endl;
            
            if (left->left != 0) {
                left->left->parent = left;
                left->right->parent = left;
            }
            //    left->left->parent = left;
            //    left->right->parent = left;
            //}
            //catch (const std::exception&) { ; }
        }

        void setRightChild(Node &childNode) {
            right = std::make_shared<Node>();
            right->asciiChars = childNode.asciiChars;
            right->frequency = childNode.frequency;
            right->side = 0;
            right->left = childNode.left;
            right->right = childNode.right;
            right->parent = childNode.parent;

            if (right->left != 0) {
                right->left->parent = right;
                right->right->parent = right;
            }
            //try {
            //    right->left->parent = right;
            //    right->right->parent = right;
            //}
            //catch (const std::exception&) { ; }

        }

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