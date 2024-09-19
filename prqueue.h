#pragma once

#include <iostream>  // For debugging
#include <sstream>   // For as_string

using namespace std;

template <typename T>
class prqueue {
   private:
    struct NODE {
        int priority;
        T value;
        NODE* parent;
        NODE* left;
        NODE* right;
        NODE* link;  // Link to duplicates -- Part 2 only
    };

    NODE* root;
    size_t sz;

    // Utility pointers for begin and next.
    NODE* curr;
    NODE* temp;  // Optional

    // TODO_STUDENT: add private helper function definitions here
    // Clones a given tree, used in copy constructor and assignment operator
    NODE* _clone(NODE* node, NODE* parent = nullptr) {
        if (!node) return nullptr; 

        // Create a new node with the same value and priority
        NODE* newNode = new NODE{node->priority, node->value, parent, nullptr, nullptr, nullptr};
        newNode->left = _clone(node->left, newNode);  // Recursively clone the left subtree
        newNode->right = _clone(node->right, newNode); // Recursively clone the right subtree

        // Clone the linked list for duplicates
        NODE* currentLink = node->link;
        NODE* newLinkParent = newNode;
        while (currentLink) {
            NODE* newLink = new NODE{currentLink->priority, currentLink->value, newLinkParent, nullptr, nullptr, nullptr};
            newLinkParent->link = newLink;
            currentLink = currentLink->link;
            newLinkParent = newLink;
        }

        return newNode; 
    }

    // Recursive helper function for inserting a new node
    void _insert(NODE*& node, NODE* parent, T value, int priority) {
        if (!node) {
            // Insert the new node if the current spot is empty
            node = new NODE{priority, value, parent, nullptr, nullptr, nullptr};
        } else if (priority < node->priority) {
            // If the new node's priority is less, insert it in the left subtree
            _insert(node->left, node, value, priority);
        } else if (priority > node->priority) {
            // If the new node's priority is greater, insert it in the right subtree
            _insert(node->right, node, value, priority);
        } else {
            // Handle the case of duplicate priorities by creating a linked list
            NODE* temp = node;
            while (temp->link) {
                temp = temp->link;
            }
            temp->link = new NODE{priority, value, parent, nullptr, nullptr, nullptr};
        }
    }

    // Performs an in-order traversal of the tree and builds a string representation
    void _inOrderTraversal(NODE* node, ostringstream& oss) const {
        if (!node) return; 

        _inOrderTraversal(node->left, oss); // Traverse left subtree

        // Process the current node and its duplicates
        for (NODE* curr = node; curr != nullptr; curr = curr->link) {
            oss << curr->priority << " value: " << curr->value << endl;
        }

        _inOrderTraversal(node->right, oss); // Traverse right subtree
    }

    // Recursively clears the memory used by the tree
    void _clear(NODE* node) {
        if (!node) return; 

        _clear(node->left);   // Recursively delete left subtree
        _clear(node->right);  // Recursively delete right subtree

        // Delete the nodes in the duplicates linked list
        NODE* linkNode = node->link;
        while (linkNode) {
            NODE* temp = linkNode;
            linkNode = linkNode->link;
            delete temp;
        }

        delete node; 
    }

    // Compares two trees for equality in structure, values, and priorities
    bool _areEqual(NODE* a, NODE* b) const {
        if (!a && !b) return true; // Both trees are empty
        if (!a || !b) return false; // One tree is empty, and the other is not

        // Check if current nodes are equal
        if (a->value != b->value || a->priority != b->priority)
            return false;

        // Recursively compare left, right, and linked children
        return _areEqual(a->left, b->left) && _areEqual(a->right, b->right) && _areEqual(a->link, b->link);
    }

   public:
    /// Creates an empty `prqueue`.
    ///
    /// Runs in O(1).
    prqueue() {
        // TODO_STUDENT
        root = nullptr;
        sz = 0;
        curr = nullptr;
        temp = nullptr;
    }

    /// Copy constructor.
    ///
    /// Copies the value-priority pairs from the provided `prqueue`.
    /// The internal tree structure must be copied exactly.
    ///
    /// Runs in O(N), where N is the number of values in `other`.
    prqueue(const prqueue& other) {
        // TODO_STUDENT
        root = nullptr;  
        sz = other.sz;   

        if (other.root != nullptr) {
            root = _clone(other.root);
        }

    }

    /// Assignment operator; `operator=`.
    ///
    /// Clears `this` tree, and copies the value-priority pairs from the
    /// provided `prqueue`. The internal tree structure must be copied exactly.
    ///
    /// Runs in O(N + O), where N is the number of values in `this`, and O is
    /// the number of values in `other`.
    prqueue& operator=(const prqueue& other) {
        // TODO_STUDENT
        if (this != &other) { // Handle self-assignment
            clear(); // Clear existing content
            root = _clone(other.root); // Deep copy
            sz = other.sz;
        }
        return *this;
    }

    /// Empties the `prqueue`, freeing all memory it controls.
    ///
    /// Runs in O(N), where N is the number of values.
    void clear() {
        // TODO_STUDENT
        _clear(root);
        root = nullptr;
        sz = 0;
    }

    /// Destructor, cleans up all memory associated with `prqueue`.
    ///
    /// Runs in O(N), where N is the number of values.
    ~prqueue() {
        // TODO_STUDENT
        clear();
        
    }

    /// Adds `value` to the `prqueue` with the given `priority`.
    ///
    /// Uses the priority to determine the location in the underlying tree.
    ///
    /// Runs in O(H + M), where H is the height of the tree, and M is
    /// the number of duplicate priorities.
    void enqueue(T value, int priority) {
        // TODO_STUDENT
        _insert(root, nullptr, value, priority);
        sz++;
          
    }


    /// Returns the value with the smallest priority in the `prqueue`, but does
    /// not modify the `prqueue`.
    ///
    /// If the `prqueue` is empty, returns the default value for `T`.
    ///
    /// Runs in O(H + M), where H is the height of the tree, and M is
    /// the number of duplicate priorities.
    T peek() const {
        // TODO_STUDENT
        if (!root) {
            return T{}; // Return default value for T if the queue is empty.
        }
        NODE* current = root;
        while (current->left) {
            current = current->left; // Traverse to the leftmost node.
        }
        return current->value;
    }

    /// Returns the value with the smallest priority in the `prqueue` and
    /// removes it from the `prqueue`.
    ///
    /// If the `prqueue` is empty, returns the default value for `T`.
    ///
    /// Runs in O(H + M), where H is the height of the tree, and M is
    /// the number of duplicate priorities.
    T dequeue() {
        // TODO_STUDENT
        if (!root) {
            return T{};  // If the queue is empty, return the default value of T.
        }

        NODE* nodeToRemove = root;
        NODE* parent = nullptr;

        // Find the leftmost node which has the smallest priority.
        while (nodeToRemove->left) {
            parent = nodeToRemove;
            nodeToRemove = nodeToRemove->left;
        }

        T returnValue = nodeToRemove->value;  // Value to return

        if (nodeToRemove->link) {
            // Handle duplicates
            NODE* linkedNode = nodeToRemove->link;
            linkedNode->left = nodeToRemove->left;
            linkedNode->right = nodeToRemove->right;
            if (linkedNode->left) linkedNode->left->parent = linkedNode;
            if (linkedNode->right) linkedNode->right->parent = linkedNode;
            
            if (parent) {
                if (parent->left == nodeToRemove) {
                    parent->left = linkedNode;
                } else {
                    parent->right = linkedNode;
                }
            } else {
                root = linkedNode;  // Update the root
            }
            linkedNode->parent = parent;
        } else {
            // Standard BST removal
            NODE* replacementNode = nullptr;
            if (nodeToRemove->left && nodeToRemove->right) {
                // Node with two children
                replacementNode = nodeToRemove->right;
                NODE* successorParent = nodeToRemove;
                while (replacementNode->left) {
                    successorParent = replacementNode;
                    replacementNode = replacementNode->left;
                }
                if (successorParent != nodeToRemove) {
                    successorParent->left = replacementNode->right;
                    if (replacementNode->right) replacementNode->right->parent = successorParent;
                    replacementNode->right = nodeToRemove->right;
                    nodeToRemove->right->parent = replacementNode;
                }
                replacementNode->left = nodeToRemove->left;
                nodeToRemove->left->parent = replacementNode;
            } else if (nodeToRemove->left || nodeToRemove->right) {
                // Node with one child
                replacementNode = nodeToRemove->left ? nodeToRemove->left : nodeToRemove->right;
                replacementNode->parent = parent;
            }

            if (parent) {
                if (parent->left == nodeToRemove) {
                    parent->left = replacementNode;
                } else {
                    parent->right = replacementNode;
                }
            } else {
                root = replacementNode;
                if (root) root->parent = nullptr;  // New root should have no parent
            }
        }

        delete nodeToRemove;
        sz--;
        return returnValue;

    }

    /// Returns the number of elements in the `prqueue`.
    ///
    /// Runs in O(1).
    size_t size() const {
        // TODO_STUDENT
        return sz;
    }

    /// Resets internal state for an iterative inorder traversal.
    ///
    /// See `next` for usage details.
    ///
    /// O(H), where H is the maximum height of the tree.
    void begin() {
        // TODO_STUDENT
        curr = root; // Starting traversal from the root

        while (curr) {
            // If no left child, ready to process right subtree
            if (!curr->left) {
                temp = curr->right;
                break;
            }

            // Locating the in-order predecessor
            NODE* pNode = curr->left;
            while (pNode->right && pNode->right != curr) {
                pNode = pNode->right;
            }

            // Establishing thread for reverse traversal
            if (!pNode->right) {
                pNode->right = curr;
                curr = curr->left;
            } else {
                pNode->right = nullptr;
                temp = curr->right;
                break;
            }
        }
    
    }

    /// Uses the internal state to return the next in-order value and priority
    /// by reference, and advances the internal state. Returns true if the
    /// reference parameters were set, and false otherwise.
    ///
    /// Example usage:
    ///
    /// ```c++
    /// pq.begin();
    /// T value;
    /// int priority;
    /// while (pq.next(value, priority)) {
    ///   cout << priority << " value: " << value << endl;
    /// }
    /// ```
    ///
    /// Runs in worst-case O(H + M) or O(H), depending on implementation, where
    /// H is the height of the tree, and M is the number of duplicate
    /// priorities.
    bool next(T& value, int& priority) {
        // TODO_STUDENT
        // When both current and temp are null, traversal is complete
        if (!curr && !temp) {
            return false;
        }

        // Process the current node if it's available
        if (curr) {
            value = curr->value;
            priority = curr->priority;

            // Handle any duplicate nodes
            if (curr->link) {
                curr = curr->link;
            } else {
                curr = nullptr; // Move to higher priority
            }
            return true;
        } else {
            // Move to the saved node in temp
            curr = temp;

            while (curr) {
                // If no left child, ready to process right subtree
                if (!curr->left) {
                    temp = curr->right;
                    break;
                }

                // Searching for the in-order predecessor again
                NODE* pNode = curr->left;
                while (pNode->right && pNode->right != curr) {
                    pNode = pNode->right;
                }

                // Reversing the temporary right pointer
                if (!pNode->right) {
                    pNode->right = curr;
                    curr = curr->left;
                } else {
                    pNode->right = nullptr;
                    temp = curr->right;
                    break;
                }
            }
            return next(value, priority); // Recursive call for the next node
        }
    }

    

    /// Converts the `prqueue` to a string representation, with the values
    /// in-order by priority.
    ///
    /// Example:
    ///
    /// ```c++
    /// prqueue<string> names;
    /// names.enqueue("Gwen", 3);
    /// names.enqueue("Jen", 2);
    /// names.enqueue("Ben", 1);
    /// names.enqueue("Sven", 2);
    /// ```
    ///
    /// Calling `names.as_string()` would return the following multi-line
    /// string:
    ///
    /// ```text
    /// 1 value: Ben
    /// 2 value: Jen
    /// 2 value: Sven
    /// 3 value: Gwen
    /// ```
    ///
    /// Runs in O(N), where N is the number of values.
    string as_string() const {
        // TODO_STUDENT
        ostringstream oss;
        _inOrderTraversal(root, oss);
        return oss.str();
    }

    /// Checks if the contents of `this` and `other` are equivalent.
    ///
    /// Two `prqueues` are equivalent if they have the same priorities and
    /// values, as well as the same internal tree structure.
    ///
    /// These two `prqueue`s would be considered equivalent, because
    /// they have the same internal tree structure:
    ///
    /// ```c++
    /// prqueue<string> a;
    /// a.enqueue("2", 2);
    /// a.enqueue("1", 1);
    /// a.enqueue("3", 3);
    /// ```
    ///
    /// and
    ///
    /// ```c++
    /// prqueue<string> b;
    /// a.enqueue("2", 2);
    /// a.enqueue("3", 3);
    /// a.enqueue("1", 1);
    /// ```
    ///
    /// While this `prqueue`, despite containing the same priority-value pairs,
    /// would not be considered equivalent, because the internal tree structure
    /// is different.
    ///
    /// ```c++
    /// prqueue<string> c;
    /// a.enqueue("1", 1);
    /// a.enqueue("2", 2);
    /// a.enqueue("3", 3);
    /// ```
    ///
    /// Runs in O(N) time, where N is the maximum number of nodes in
    /// either `prqueue`.
    ///
    bool operator==(const prqueue& other) const {
        // TODO_STUDENT
        return _areEqual(root, other.root);
    }

    /// Returns a pointer to the root node of the BST.
    ///
    /// Used for testing the internal structure of the BST. Do not edit or
    /// change.
    ///
    /// Runs in O(1).
    void* getRoot() {
        return root;
    }
};
