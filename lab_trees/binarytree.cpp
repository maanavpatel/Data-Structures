/**
 * @file binarytree.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */
#include "TreeTraversals/InorderTraversal.h"
#include <iostream>

/**
 * @return The height of the binary tree. Recall that the height of a binary
 *  tree is just the length of the longest path from the root to a leaf, and
 *  that the height of an empty tree is -1.
 */
template <typename T>
int BinaryTree<T>::height() const
{
    // Call recursive helper function on root
    return height(root);
}

/**
 * Private helper function for the public height function.
 * @param subRoot
 * @return The height of the subtree
 */
template <typename T>
int BinaryTree<T>::height(const Node* subRoot) const
{
    // Base case
    if (subRoot == NULL)
        return -1;

    // Recursive definition
    return 1 + std::max(height(subRoot->left), height(subRoot->right));
}

/**
 * Prints out the values of the nodes of a binary tree in order.
 * That is, everything to the left of a node will be printed out before that
 * node itself, and everything to the right of a node will be printed out after
 * that node.
 */
template <typename T>
void BinaryTree<T>::printLeftToRight() const
{
    // Call recursive helper function on the root
    printLeftToRight(root);

    // Finish the line
    std::cout << std::endl;
}

/**
 * Private helper function for the public printLeftToRight function.
 * @param subRoot
 */
template <typename T>
void BinaryTree<T>::printLeftToRight(const Node* subRoot) const
{
    // Base case - null node
    if (subRoot == NULL)
        return;

    // Print left subtree
    printLeftToRight(subRoot->left);

    // Print this node
    std::cout << subRoot->elem << ' ';

    // Print right subtree
    printLeftToRight(subRoot->right);
}

/**
 * Flips the tree over a vertical axis, modifying the tree itself
 *  (not creating a flipped copy).
 */
    template <typename T>
void BinaryTree<T>::mirror()
{
    mirror(root);
    //your code here
}

template <typename T>
void BinaryTree<T>::mirror(BinaryTree<T>::Node* subroot) {
    if (subroot == NULL) {
        return;
    }
    Node* temp = &*(subroot->right);
    subroot->right = subroot->left;
    subroot->left = temp;
    mirror(subroot->left);
    mirror(subroot->right);
}


/**
 * isOrdered() function iterative version
 * @return True if an in-order traversal of the tree would produce a
 *  nondecreasing list output values, and false otherwise. This is also the
 *  criterion for a binary tree to be a binary search tree.
 */
template <typename T>
bool BinaryTree<T>::isOrderedIterative() const
{
    InorderTraversal<int> iot(this->getRoot());
    Node* prev = NULL;
    int count = 0;
	for (auto it = iot.begin(); it != iot.end(); ++it) {
        if (count == 0) {
            count++;
            prev = *it;
            continue;
        }
		if ((*it)->elem < prev->elem) {
            return false;
        }
        count++;
        prev = *it;
	}
	return true;
}

/**
 * isOrdered() function recursive version
 * @return True if an in-order traversal of the tree would produce a
 *  nondecreasing list output values, and false otherwise. This is also the
 *  criterion for a binary tree to be a binary search tree.
 */
template <typename T>
bool BinaryTree<T>::isOrderedRecursive() const
{
    Node *prev = NULL;
    return isOrderedRecursive(root);
}

template <typename T>
bool BinaryTree<T>::isOrderedRecursive(Node* subRoot) const
{
	bool inOrder = true;
	if(subRoot == NULL)
		return true;
	if(subRoot->left != NULL){
		if(subRoot->elem < maxL(subRoot->left))
			inOrder = false;
	}
	if(subRoot->right != NULL){
		if(subRoot->elem > minR(subRoot->right))
			inOrder = false;
	}	
	return (inOrder && isOrderedRecursive(subRoot->left) && isOrderedRecursive(subRoot->right));
}

template <typename T>
T BinaryTree<T>::maxL(Node* subRoot) const
{
	if(subRoot->left == NULL&&subRoot->right == NULL) return(subRoot->elem);		
	else if (subRoot->right == NULL) {
		return(std::max(subRoot->elem, maxL(subRoot->left)));
    } else if (subRoot->left == NULL) {
        return(std::max(subRoot->elem, maxL(subRoot->right)));
    } else {
		return(std::max(subRoot->elem, std::max(maxL(subRoot->left), maxL(subRoot->right))));
    }
}

template <typename T>
T BinaryTree<T>::minR(Node* subRoot) const
{
	if(subRoot->left == NULL&&subRoot->right == NULL) {
		return(subRoot->elem);		
    } else if (subRoot->right == NULL) {
		return(std::min(subRoot->elem, minR(subRoot->left)));	
    } else if (subRoot->left == NULL) {
		return(std::min(subRoot->elem, minR(subRoot->right)));
    } else {
		return(std::min(subRoot->elem,  std::min(minR(subRoot->left), minR(subRoot->right))));
    }
}

