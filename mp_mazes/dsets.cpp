#include "dsets.h"

void DisjointSets::addelements(int num) {
  for (int i = 0; i < num; i++){
    dset_.push_back(-1);
  }
}

int DisjointSets::find(int elem) {
  if (dset_[elem] < 0) {
    return elem;
  } else {
    dset_[elem] = find(dset_[elem]);
    return dset_[elem];
  }
}

void DisjointSets::setunion(int a, int b) {
  int x = find(a);
  int y = find(b);

  int newSize = dset_[x] + dset_[y];

  if (dset_[x] >= dset_[y]) {
    dset_[x] = y;
    dset_[y] = newSize;
  } else {
    dset_[y] = x;
    dset_[x] = newSize;
  }
}

int DisjointSets::size(int elem) {
  return -1 * dset_[find(elem)];
}