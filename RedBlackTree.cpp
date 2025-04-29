#include "RedBlackTree.h"
#include <stdexcept>




static RBTNode* CreateNode(int data, int color, RBTNode* parent) {
    // Allocate a new node and initialize all fields
    RBTNode* node = new RBTNode;
    node->data = data;
    node->color = color;
    node->parent = parent;
    node->left = nullptr;
    node->right = nullptr;
    node->IsNullNode = false; // Real node, not a fake "null" node
    return node;
}


RedBlackTree::RedBlackTree() {
    root = nullptr;   
    numItems = 0;      
}


RedBlackTree::RedBlackTree(int newData) {
    root = CreateNode(newData, COLOR_BLACK, nullptr); 
    numItems = 1;
}

RedBlackTree::RedBlackTree(const RedBlackTree &rbt) {
    root = CopyOf(rbt.root);  
    numItems = rbt.numItems;
}


RBTNode* RedBlackTree::CopyOf(const RBTNode *node) {
    if (node == nullptr) return nullptr; // Base case

    RBTNode* newNode = new RBTNode(*node); // Copy current node
    newNode->left = CopyOf(node->left);    // Recursively copy left child
    if (newNode->left) newNode->left->parent = newNode;
    newNode->right = CopyOf(node->right);  // Recursively copy right child
    if (newNode->right) newNode->right->parent = newNode;

    return newNode;
}


void RedBlackTree::Insert(int newData) {
    if (Contains(newData)) {
        throw invalid_argument("Duplicate value inserted.");
    }

    RBTNode* newNode = CreateNode(newData, COLOR_RED, nullptr); 
    BasicInsert(newNode);  
    InsertFixUp(newNode);   
    numItems++;
}


void RedBlackTree::BasicInsert(RBTNode *node) {
    if (root == nullptr) {
        root = node; // If tree is empty, new node is root
        return;
    }

    RBTNode* temp = root;
    while (true) {
        if (node->data < temp->data) {
            // Go left
            if (temp->left == nullptr) {
                temp->left = node;
                node->parent = temp;
                return;
            } else {
                temp = temp->left;
            }
        } else {
            // Go right
            if (temp->right == nullptr) {
                temp->right = node;
                node->parent = temp;
                return;
            } else {
                temp = temp->right;
            }
        }
    }
}


void RedBlackTree::InsertFixUp(RBTNode *node) {
    while (node != root && node->parent->color == COLOR_RED) {
        RBTNode* grandparent = node->parent->parent;

        if (node->parent == grandparent->left) {
            // Parent is left child
            RBTNode* uncle = grandparent->right;

            if (uncle != nullptr && uncle->color == COLOR_RED) {
                // Case 1: Uncle is red (recolor)
                node->parent->color = COLOR_BLACK;
                uncle->color = COLOR_BLACK;
                grandparent->color = COLOR_RED;
                node = grandparent; // Move up the tree
            } else {
                if (node == node->parent->right) {
                    // Case 2: node is right child -> need to left rotate
                    node = node->parent;
                    LeftRotate(node);
                }
                // Case 3: node is left child -> right rotate
                node->parent->color = COLOR_BLACK;
                grandparent->color = COLOR_RED;
                RightRotate(grandparent);
            }
        } else {
            // Parent is right child
            RBTNode* uncle = grandparent->left;

            if (uncle != nullptr && uncle->color == COLOR_RED) {
                // Case 1 mirror: Uncle is red
                node->parent->color = COLOR_BLACK;
                uncle->color = COLOR_BLACK;
                grandparent->color = COLOR_RED;
                node = grandparent;
            } else {
                if (node == node->parent->left) {
                    // Case 2 mirror: node is left child -> right rotate
                    node = node->parent;
                    RightRotate(node);
                }
                // Case 3 mirror: node is right child -> left rotate
                node->parent->color = COLOR_BLACK;
                grandparent->color = COLOR_RED;
                LeftRotate(grandparent);
            }
        }
    }

    root->color = COLOR_BLACK; // Root must always be black
}


// Left Rotation
void RedBlackTree::LeftRotate(RBTNode *node) {
    RBTNode* y = node->right;    // y becomes new parent
    node->right = y->left;       // move y's left subtree to node's right

    if (y->left != nullptr) y->left->parent = node;

    y->parent = node->parent;    // link y to node's parent
    if (node->parent == nullptr) {
        root = y;                // node was root
    } else if (node == node->parent->left) {
        node->parent->left = y;
    } else {
        node->parent->right = y;
    }

    y->left = node;             // put node as left child of y
    node->parent = y;
}

// Right Rotation
void RedBlackTree::RightRotate(RBTNode *node) {
    RBTNode* y = node->left;     // y becomes new parent
    node->left = y->right;       // move y's right subtree to node's left

    if (y->right != nullptr) y->right->parent = node;

    y->parent = node->parent;    // link y to node's parent
    if (node->parent == nullptr) {
        root = y;                // node was root
    } else if (node == node->parent->right) {
        node->parent->right = y;
    } else {
        node->parent->left = y;
    }

    y->right = node;            // put node as right child of y
    node->parent = y;
}


bool RedBlackTree::Contains(int data) const {
    return (Get(data) != nullptr);  // Use helper to search
}

// Getting the node with the specific value
RBTNode* RedBlackTree::Get(int data) const {
    RBTNode* temp = root;
    while (temp != nullptr) {
        if (data < temp->data) {
            temp = temp->left;
        } else if (data > temp->data) {
            temp = temp->right;
        } else {
            return temp;  // Found
        }
    }
    return nullptr;  // Not found
}


int RedBlackTree::GetMin() const {
    if (!root) throw runtime_error("Tree is empty.");
    RBTNode* temp = root;
    while (temp->left) temp = temp->left;  // Go left as much as possible
    return temp->data;
}


int RedBlackTree::GetMax() const {
    if (!root) throw runtime_error("Tree is empty.");
    RBTNode* temp = root;
    while (temp->right) temp = temp->right; // Go right as much as possible
    return temp->data;
}


string RedBlackTree::ToInfixString(const RBTNode *n) {
    if (!n) return "";
    string result = "";
    result += ToInfixString(n->left);
    result += GetNodeString(n);
    result += ToInfixString(n->right);
    return result;
}

string RedBlackTree::ToPrefixString(const RBTNode *n) {
    if (!n) return "";
    string result = "";
    result += GetNodeString(n);
    result += ToPrefixString(n->left);
    result += ToPrefixString(n->right);
    return result;
}


string RedBlackTree::ToPostfixString(const RBTNode *n) {
    if (!n) return "";
    string result = "";
    result += ToPostfixString(n->left);
    result += ToPostfixString(n->right);
    result += GetNodeString(n);
    return result;
}


string RedBlackTree::GetNodeString(const RBTNode *n) {
    if (!n) return "";
    return " " + GetColorString(n) + to_string(n->data) + " ";
}


string RedBlackTree::GetColorString(const RBTNode *n) {
    if (n->color == COLOR_RED) return "R";
    if (n->color == COLOR_BLACK) return "B";
    return "D"; 
}
