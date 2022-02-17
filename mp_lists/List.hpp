/**
 * @file list.cpp
 * Doubly Linked List (MP 3).
 */

template <class T>
List<T>::List() { 
    head_ = nullptr;
    tail_ = nullptr;
    length_ = 0;
}

/**
 * Returns a ListIterator with a position at the beginning of
 * the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::begin() const {
  // @TODO: graded in MP3.1
  return List<T>::ListIterator(head_);
}

/**
 * Returns a ListIterator one past the end of the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::end() const {
  // @TODO: graded in MP3.1
  if (head_ == NULL) {
    return List<T>::ListIterator(tail_);
  }
  return List<T>::ListIterator(tail_->next);
}


/**
 * Destroys all dynamically allocated memory associated with the current
 * List class.
 */
template <typename T>
void List<T>::_destroy() {
  ListNode* toRem = head_;
  while (toRem != NULL) {
    ListNode* nextNode = toRem->next;
    delete toRem;
    length_--;
    toRem = nextNode;
  }
  head_ = NULL;
  tail_ = NULL;
  /// @todo Graded in MP3.1
}

/**
 * Inserts a new node at the front of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertFront(T const & ndata) {
  /// @todo Graded in MP3.1
  ListNode * newNode = new ListNode(ndata);
  newNode -> next = head_;
  newNode -> prev = NULL;
  
  if (head_ != NULL) {
    head_ -> prev = newNode;
  }
  if (tail_ == NULL) {
    tail_ = newNode;
  }
  head_ = newNode;
  length_++;
}

/**
 * Inserts a new node at the back of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertBack(const T & ndata) {
  /// @todo Graded in MP3.1
  ListNode* newNode = new ListNode(ndata);
  newNode->next = NULL;
  if (empty()) {
    newNode->prev = NULL;
    tail_ = newNode;
    head_ = newNode;
  } else {
    if (tail_ != NULL) {
      tail_->next = newNode;
    }
    tail_ = newNode;
    newNode->prev = tail_;
  }
  length_++;
}

/**
 * Helper function to split a sequence of linked memory at the node
 * splitPoint steps **after** start. In other words, it should disconnect
 * the sequence of linked memory after the given number of nodes, and
 * return a pointer to the starting node of the new sequence of linked
 * memory.
 *
 * This function **SHOULD NOT** create **ANY** new List or ListNode objects!
 *
 * This function is also called by the public split() function located in
 * List-given.hpp
 *
 * @param start The node to start from.
 * @param splitPoint The number of steps to walk before splitting.
 * @return The starting node of the sequence that was split off.
 */
template <typename T>
typename List<T>::ListNode * List<T>::split(ListNode * start, int splitPoint) {
  if (start ==  NULL || splitPoint == 0){
    return start;
  }
  ListNode * newTail = start;
  for (int i = 0; i < splitPoint-1 && newTail != NULL; i++) {
    newTail = newTail->next;
  }

  if (newTail != nullptr) {
    ListNode* splitHead = newTail->next;
    newTail->next = nullptr;
    splitHead->prev = nullptr;
    return splitHead;
  }
  
  return newTail;
}

/**
  * Modifies List using the rules for a TripleRotate.
  *
  * This function will to a wrapped rotation to the left on every three 
  * elements in the list starting for the first three elements. If the 
  * end of the list has a set of 1 or 2 elements, no rotation all be done 
  * on the last 1 or 2 elements.
  * 
  * You may NOT allocate ANY new ListNodes!
  */
template <typename T>
void List<T>::tripleRotate() {
  int adjLen = size() - (size() % 3);
  
  ListNode* curr = head_;
  for (int i = 0; i < adjLen; i += 3) {
    ListNode * first3 = &*curr;
    ListNode * second1 = first3->next;
    ListNode * third2 = second1->next;
    second1->prev = first3->prev;
    if (second1->prev != NULL) {
      second1->prev->next = second1;
    }
    first3->prev = third2;
    third2->prev = second1;
    //moves first item to third sets previous to second
    second1->next = third2;
    first3->next = third2->next;
    third2->next = first3;


    if (first3->next == NULL){
      tail_ = first3;
    } else {
      first3->next->prev = first3;
    }
    if (i == 0) {
      head_ = second1;
    }
    curr = first3->next;
  }
  // @todo Graded in MP3.1
}


/**
 * Reverses the current List.
 */
template <typename T>
void List<T>::reverse() {
  reverse(head_, tail_);
}

/**
 * Helper function to reverse a sequence of linked memory inside a List,
 * starting at startPoint and ending at endPoint. You are responsible for
 * updating startPoint and endPoint to point to the new starting and ending
 * points of the rearranged sequence of linked memory in question.
 *
 * @param startPoint A pointer reference to the first node in the sequence
 *  to be reversed.
 * @param endPoint A pointer reference to the last node in the sequence to
 *  be reversed.
 */
template <typename T>
void List<T>::reverse(ListNode *& startPoint, ListNode *& endPoint) {
   /// @todo Graded in MP3.1
    //just have to reverse what each node is pointing to.
    //0->1->2->3->4->5
    //0<-1<-2<-3<-4<-5

    if(startPoint == endPoint){
      return;
    }

    ListNode * endNext = endPoint->next;
    ListNode* curr = startPoint;
    ListNode * curr2 = startPoint;

    while (curr != endNext){
        curr2 = curr->prev;
        curr->prev = curr->next;
        curr->next = curr2;
        curr = curr->prev;
    }

    curr2 = endPoint->prev;

    if (startPoint->next == NULL){
        endPoint ->prev = NULL;
    }

    else{
      endPoint ->prev = startPoint->next;
      startPoint->next->next = endPoint;
    }

    if (curr2 == NULL){
      startPoint ->next = NULL;

    }

    else{
      startPoint->next = curr2;
      curr2->prev = startPoint;
    }

    curr = startPoint;
    startPoint = endPoint;
    endPoint = curr;

}

/**
 * Reverses blocks of size n in the current List. You should use your
 * reverse( ListNode * &, ListNode * & ) helper function in this method!
 *
 * @param n The size of the blocks in the List to be reversed.
 */
template <typename T>
void List<T>::reverseNth(int n) {
  /// @todo Graded in MP3.2
}


/**
 * Merges the given sorted list into the current sorted list.
 *
 * @param otherList List to be merged into the current list.
 */
template <typename T>
void List<T>::mergeWith(List<T> & otherList) {
    // set up the current list
    head_ = merge(head_, otherList.head_);
    tail_ = head_;

    // make sure there is a node in the new list
    if (tail_ != NULL) {
        while (tail_->next != NULL)
            tail_ = tail_->next;
    }
    length_ = length_ + otherList.length_;

    // empty out the parameter list
    otherList.head_ = NULL;
    otherList.tail_ = NULL;
    otherList.length_ = 0;
}

/**
 * Helper function to merge two **sorted** and **independent** sequences of
 * linked memory. The result should be a single sequence that is itself
 * sorted.
 *
 * This function **SHOULD NOT** create **ANY** new List objects.
 *
 * @param first The starting node of the first sequence.
 * @param second The starting node of the second sequence.
 * @return The starting node of the resulting, sorted sequence.
 */
template <typename T>
typename List<T>::ListNode * List<T>::merge(ListNode * first, ListNode* second) {
  if(first == NULL || second == NULL){
    return NULL;
  }
	if(first == second){
    	return second;
  }
	ListNode * firstN = first;
	ListNode * secondN = second;
	ListNode * headN = first;
  ListNode * r = headN;

	if(first->data < second->data){
		firstN = first->next;
	} else {
		headN = second;
		secondN = second->next;
	}

	r = headN;

	while(firstN != NULL && secondN != NULL){
		
    if(firstN->data < secondN->data || secondN == NULL){
			r->next = firstN;
			firstN->prev = r;
			r = firstN;
			firstN = firstN->next;
		}	else {
			r->next = secondN;
			secondN->prev = r;
			r = secondN;
			secondN = secondN->next;
		}

	}

	if (firstN == NULL){
		r->next = secondN;
		secondN->prev = r;
	} else { 
		r->next = firstN;
		firstN->prev = r;
	}
  r = headN;
	return r;
}

/**
 * Sorts a chain of linked memory given a start node and a size.
 * This is the recursive helper for the Mergesort algorithm (i.e., this is
 * the divide-and-conquer step).
 *
 * Called by the public sort function in List-given.hpp
 *
 * @param start Starting point of the chain.
 * @param chainLength Size of the chain to be sorted.
 * @return A pointer to the beginning of the now sorted chain.
 */
template <typename T>
typename List<T>::ListNode* List<T>::mergesort(ListNode * start, int chainLength) {
  
    if(chainLength == 1){
      start->next = NULL;
      start->prev = NULL;
      return start;
    } 

    int adjLen = chainLength /2;
    ListNode* mid = start;
    for(int i =0; i < adjLen; i++){
      mid = mid->next;
    }

    mid = split(start, adjLen);
    start = mergesort(start, adjLen);
    mid = mergesort(mid, chainLength- adjLen);

    ListNode* newStart = merge(start, mid);
    return newStart;

}
