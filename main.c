#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

# define BFACTOR 261
# define BMIN2 259 // used for insertions
# define BFACTORLOG2 9

// B+ tree
    // Node structure
struct Node {
    size_t parent; // 0 if root, pointer to parent otherwise
    size_t leaf; // 0 if internal, -1 if root and leaf, pointer to next leaf if non-root leaf
    unsigned int nkeys:BFACTORLOG2;
    size_t keys[BFACTOR];
    size_t values[BFACTOR];
};

    // Search
        // Helper: Find correct leaf
struct Node* leafSearch(size_t k, struct Node *node) {
    if (node->leaf != 0) {return node;}
    for (unsigned int i = 0; i < node->nkeys; i++) {
        if (k < node->keys[i]) {
            return leafSearch(k, (struct Node *)node->values[i]);
        }
    }
    return leafSearch(k, (struct Node *)node->values[node->nkeys - 1]);
};

    // Search
        // Actual Search
size_t treeSearch(size_t k, struct Node *root) {
    struct Node *leaf = leafSearch(k, root);
    for(unsigned int i = 0; i < leaf->nkeys; i++) {
        if (leaf->keys[i] == k) {
            return leaf->values[i];
        }
    }
    return 0;
};

    // Insertion
        // Helper:
void treeInsert(size_t k, size_t v, struct Node *root) {
    struct Node *leaf = leafSearch(k, root);
    nodeInsert(k, v, leaf);
};
        // Add value into node
void nodeInsert(size_t k, size_t v, struct Node *node) {
    if (likely(node->nkeys < BMIN2)) {
        unsigned int i = 0;
        while(node->keys[i] <= k && i < node->nkeys) i++;
        for (unsigned int j = node->nkeys; j > i; j--) {
            node->keys[j] = node->keys[j - 1];
            node->values[j] = node->values[j - 1];
        }
        node->keys[i] = k;
        node->values[i] = v;
        node->nkeys++;
    } else {
        unsigned int newN = BFACTOR / 2;
        unsigned int oldN = BFACTOR - newN;
        struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
        newNode->parent = node->parent;
        newNode->leaf = node->leaf;
        node->leaf = (size_t)newNode;
        newNode->nkeys = newN;
        node->nkeys = oldN;
        memcpy(newNode->keys, node->keys + (oldN * sizeof(size_t)), newN * sizeof(size_t));
        memcpy(newNode->values, node->values + (oldN * sizeof(size_t)), newN * sizeof(size_t));
    }
};
