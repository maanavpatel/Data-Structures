/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>
#include<cmath>

using namespace std;

template <int Dim>
bool KDTree<Dim>::smallerDimVal(const Point<Dim>& first,
                                const Point<Dim>& second, int curDim) const
{
    /**
     * @todo Implement this function!
     */
    if(first[curDim]>second[curDim])
      return false;
    else if(first[curDim]==second[curDim])
      return (first<second);
    else
      return true;
}

template<int Dim>
double KDTree<Dim>::distance(const Point<Dim>& target,const Point<Dim>& p) const {
  double distance = 0;
  for(double i = 0; i < Dim; i++) {
    distance += (target[i] - p[i]) * (target[i] - p[i]);
  }
  return distance;
}

template <int Dim>
bool KDTree<Dim>::shouldReplace(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential) const
{
    /**
     * @todo Implement this function!
     */
  double currDist = distance(target, currentBest);
  double pDist = distance(target, potential);

  if(currDist == pDist) return potential < currentBest;
  return pDist < currDist;
}

template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
{
    /**
     * @todo Implement this function!
     */
  // treepoints = new vector<Point<Dim>>();
  size = newPoints.size();
  if (size == 0) {
    root = NULL;
  } else {
    for (const Point<Dim>& p : newPoints){
      treepoints.push_back(p);
    }
    root = buildTree(0, size-1, 0);
  }
}

template <int Dim>
KDTree<Dim>::KDTree(const KDTree<Dim>& other) {
  /**
   * @todo Implement this function!
   */
  copy(other);
}

template <int Dim>
void KDTree<Dim>::copy(const KDTree<Dim>& other){
  this = new KDTree(other.treepoints);
}

template <int Dim>
void KDTree<Dim>::quickSelect(int start, int end, int pivIdx, int currentDim) {
  while(start != end){
    Point<Dim> piv = treepoints[pivIdx];

    treepoints[pivIdx] = treepoints[end];
    treepoints[end] = piv ;

    int storeIndex = start;

    for(int i=start;i<end;i++){
      if(smallerDimVal(treepoints[i], piv, currentDim) || treepoints[i] == piv){
        Point<Dim> temp=treepoints[storeIndex];
        
        treepoints[storeIndex]=treepoints[i];
        treepoints[i]=temp;
        
        storeIndex++;
      }
    }

    Point<Dim> tmp =treepoints[storeIndex];

    treepoints[storeIndex] = treepoints[end];
    treepoints[end] = tmp;

    if(storeIndex == pivIdx) {
      return;
    } else if (pivIdx < storeIndex)
      end = storeIndex-1;
    else {
      start = storeIndex+1;
    }
  }
}

template <int Dim>
typename KDTree<Dim>::KDTreeNode * KDTree<Dim>::buildTree(int start, int end,int currentDim){
  if(start > end) {
    return NULL;
  } else if (start == end) {
    return new KDTreeNode(treepoints[start]);
  } else {
    int piv = (start+end)/2;
    int nDim = (currentDim+1) % Dim;

    quickSelect(start, end, piv, currentDim);

    KDTreeNode *  curr = new KDTreeNode(treepoints[piv]);
    curr->left = buildTree(start, (piv-1), nDim);
    curr->right = buildTree((piv+1), end, nDim);

    return curr;
  }
}


template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree<Dim>& rhs) {
  /**
   * @todo Implement this function!
   */
  if(*this!=rhs){
     destroy();
     copy(rhs);
   }
  return *this;
}

template <int Dim>
KDTree<Dim>::~KDTree() {
  /**
   * @todo Implement this function!
   */
    destroy();
}

template <int Dim>
void KDTree<Dim>::destroy(){
  treepoints.clear();
  size = 0;
  destroyNodes(root);
}

template <int Dim>
void KDTree<Dim>::destroyNodes(typename KDTree<Dim>::KDTreeNode* subRoot){
  if (subRoot == NULL) {
    return;
  }
  destroyNodes(subRoot->left);
  destroyNodes(subRoot->right);
  delete subRoot;
  subRoot = NULL;
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
    /**
     * @todo Implement this function!
     */

    return findNearestNeighbor(query,root,0);
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query,typename KDTree<Dim>::KDTreeNode* currentNode, int currentDim) const{
  if(currentNode->left == NULL && currentNode->right == NULL) {
    return currentNode->point;
  }

  int nDim = (currentDim+1) % Dim;

  Point<Dim> currBest = currentNode->point;
  Point<Dim> pBest = currBest;

  bool dir = smallerDimVal(query,currBest,currentDim);

  if (dir && currentNode->left!=NULL) {
    pBest = findNearestNeighbor(query,currentNode->left,nDim);
  } else if (!dir && currentNode->right!=NULL) {
    pBest = findNearestNeighbor(query,currentNode->right,nDim);
  }

  if(shouldReplace(query,currBest,pBest)) {
    currBest = pBest;
  }

  int bestDistance = distance(query,currBest);
  int dimDistance = (query[currentDim] - currentNode->point[currentDim])
                    * (query[currentDim] - currentNode->point[currentDim]);
  if(dimDistance <= bestDistance) {
    if(!dir && currentNode->left != NULL) {

      pBest = findNearestNeighbor(query,currentNode->left, nDim);
      
      if(shouldReplace(query,currBest,pBest)) {
        currBest = pBest;
      }
    } else if(dir && currentNode->right!=NULL) {
      pBest = findNearestNeighbor(query,currentNode->right, nDim);
      if(shouldReplace(query, currBest, pBest)) {
        currBest = pBest;
      }
    }
  }
  return currBest;
 }

