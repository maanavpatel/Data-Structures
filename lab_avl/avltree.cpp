/**
 * @file avltree.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */

template <class K, class V>
V AVLTree<K, V>::find(const K& key) const
{
    return find(root, key);
}

template <class K, class V>
V AVLTree<K, V>::find(Node* subtree, const K& key) const
{
    if (subtree == NULL)
        return V();
    else if (key == subtree->key)
        return subtree->value;
    else {
        if (key < subtree->key)
            return find(subtree->left, key);
        else
            return find(subtree->right, key);
    }
}
template <class K, class V>
void AVLTree<K, V>::setHeights(Node*& r) {
    //r is root of new subtree
  if (r->left == NULL && r->right == NULL) {
    r->height = 0;
  } else if (r->left  == NULL) {
    r->height = r->right->height + 1;
  } else if (r->right == NULL) {
    r->height = r->left->height  + 1;
  } else {
    r->height = std::max(r->right->height, r->left->height) + 1;
  }
}
template <class K, class V>
void AVLTree<K, V>::rotateLeft(Node*& t)
{
    functionCalls.push_back("rotateLeft"); // Stores the rotation name (don't remove this)
    if (t == NULL || t->right == NULL) {
        return;
    }
    Node* prev = t;
    Node* newRoot = t->right;
    Node* toMove = t->right->left;
    t = newRoot;
    prev->right = toMove;
    newRoot->left = prev;

    setHeights(prev);
    setHeights(newRoot);
}

template <class K, class V>
void AVLTree<K, V>::rotateLeftRight(Node*& t)
{
    functionCalls.push_back("rotateLeftRight"); // Stores the rotation name (don't remove this)
    // Implemented for you:
    rotateLeft(t->left);
    rotateRight(t);
}

template <class K, class V>
void AVLTree<K, V>::rotateRight(Node*& t)
{
    functionCalls.push_back("rotateRight"); // Stores the rotation name (don't remove this)
    
    if (t == NULL || t->left == NULL) return;
    Node* prev = t;
    Node* newRoot = t->left;
    Node* toMove = t->left->right;
    t = newRoot;
    prev->left = toMove;
    newRoot->right = prev;

    setHeights(prev);
    setHeights(newRoot);
}

template <class K, class V>
void AVLTree<K, V>::rotateRightLeft(Node*& t)
{
    functionCalls.push_back("rotateRightLeft"); // Stores the rotation name (don't remove this)
    // your code here
    rotateRight(t->right);
    rotateLeft(t);
}

template <class K, class V>
int AVLTree<K, V>::balance(Node*& r) { //helper method for rebalance
  if (r == NULL)
    return 0;
  int leftHeight  = heightOrNeg1(r->left);
  int rightHeight = heightOrNeg1(r->right);
  return leftHeight - rightHeight;
}

template <class K, class V>
void AVLTree<K, V>::rebalance(Node*& subtree)
{
    if (subtree == NULL) return;

    setHeights(subtree);
    int b = balance(subtree);

    if (abs(b) <= 1) return;

    if (b < 0) {                // tree is right heavy
        if (balance(subtree->right) > 0) {  // tree's right subtree is left heavy
            rotateRightLeft(subtree);
        } else {
            rotateLeft(subtree);
        }
    } else {                              // left heavy
        if (balance(subtree->left) < 0) {   // tree's left subtree is right heavy
            rotateLeftRight(subtree);
        } else {
            rotateRight(subtree);
        }
    }
}

template <class K, class V>
void AVLTree<K, V>::insert(const K & key, const V & value)
{
    insert(root, key, value);
    
}

template <class K, class V>
void AVLTree<K, V>::insert(Node*& subtree, const K& key, const V& value)
{
    if (subtree == NULL) {
        subtree = new Node(key, value);
    } else if (subtree->key == key) {
        subtree->value = value;
    } else if (key < subtree->key) {
        insert(subtree->left, key, value);
        rebalance(subtree);
    } else {
        insert(subtree->right, key, value);
        rebalance(subtree);
    }
}

template <class K, class V>
void AVLTree<K, V>::remove(const K& key)
{
    remove(root, key);
}

template <class K, class V>
void AVLTree<K, V>::remove(Node*& subtree, const K& key)
{
    if (subtree == NULL) return;

    if (key < subtree->key) {
        remove(subtree->left, key);
        rebalance(subtree);
    } else if (key > subtree->key) {
        remove(subtree->right, key);
        rebalance(subtree);
    } else {
        if (subtree->left == NULL && subtree->right == NULL) {
            /* no-child remove */
            delete subtree;
            subtree = NULL;
        } else if (subtree->left != NULL && subtree->right != NULL) {
            /* two-child remove */
            Node*& n = furthestRight(subtree->left);
            swap(n, subtree);
            delete n;
            n = NULL;
        } else {
            /* one-child remove */
            Node* child = NULL;
            if (subtree->left != NULL) {
                child = subtree->left;
            } else {
                child = subtree->right;
            }
            delete subtree;
            subtree = child;
        }
        rebalance(subtree);
    }
}

template <class K, class V>
typename AVLTree<K, V>::Node*& AVLTree<K, V>::furthestRight(Node*& r) {
    if (r->right == NULL) {
        return r;
    }
    return furthestRight(r->right);
}
