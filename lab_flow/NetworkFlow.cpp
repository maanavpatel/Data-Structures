/**
 * @file NetworkFlow.cpp
 * CS 225: Data Structures
 */

#include <vector>
#include <algorithm>
#include <set>

#include "graph.h"
#include "edge.h"

#include "NetworkFlow.h"
int min(int a, int b) {
  if (a<b)
    return a;
  else return b;
}

NetworkFlow::NetworkFlow(Graph & startingGraph, Vertex source, Vertex sink) :
  g_(startingGraph), residual_(Graph(true,true)), flow_(Graph(true,true)), source_(source), sink_(sink) {
    
    vector<Vertex> g_verts = g_.getVertices();
    vector<Edge> g_edges = g_.getEdges();
    
    for (Vertex v : g_verts) {
      flow_.insertVertex(v);
      residual_.insertVertex(v);

    }

    for (Edge e : g_edges) {
      Vertex src = e.source;
      Vertex dest = e.dest;
      //build flow
      flow_.insertEdge(src, dest);
      flow_.setEdgeLabel(src, dest, e.getLabel());
      flow_.setEdgeWeight(src, dest, 0);

      //build residual
      //regular direction
      residual_.insertEdge(src, dest);
      residual_.setEdgeLabel(src, dest, e.getLabel());
      residual_.setEdgeWeight(src, dest, e.getWeight());

      //opposite
      residual_.insertEdge(dest, src);
      residual_.setEdgeLabel(dest, src, e.getLabel());
      residual_.setEdgeWeight(dest, src, 0);
    }
      

    // std::cout<< "size is : \t " << g_edges.size() << std::endl;
  // YOUR CODE HERE
}

  /**
   * findAugmentingPath - use DFS to find a path in the residual graph with leftover capacity.
   *  This version is the helper function.
   *
   * @param source  The starting (current) vertex
   * @param sink    The destination vertex
   * @param path    The vertices in the path
   * @param visited A set of vertices we have visited
   */

bool NetworkFlow::findAugmentingPath(Vertex source, Vertex sink, 
  std::vector<Vertex> &path, std::set<Vertex> &visited) {

  if (visited.count(source) != 0)
    return false;
  visited.insert(source);

  if (source == sink) {
    return true;
  }

  vector<Vertex> adjs = residual_.getAdjacent(source);
  for(auto it = adjs.begin(); it != adjs.end(); it++) {
    if (visited.count(*it) == 0 && residual_.getEdgeWeight(source,*it) > 0) {
      path.push_back(*it);
      if (findAugmentingPath(*it,sink,path,visited))
        return true;
      else {
        path.pop_back();
      }
    }
  }

  return false;
}

  /**
   * findAugmentingPath - use DFS to find a path in the residual graph with leftover capacity.
   *  This version is the main function.  It initializes a set to keep track of visited vertices.
   *
   * @param source The starting (current) vertex
   * @param sink   The destination vertex
   * @param path   The vertices in the path
   */

bool NetworkFlow::findAugmentingPath(Vertex source, Vertex sink, std::vector<Vertex> &path) {
   std::set<Vertex> visited;
   path.clear();
   path.push_back(source);
   return findAugmentingPath(source,sink,path,visited);
}

  /**
   * pathCapacity - Determine the capacity of a path in the residual graph.
   *
   * @param path   The vertices in the path
   */

int NetworkFlow::pathCapacity(const std::vector<Vertex> & path) const {
  int min_weight = INT_MAX;
  for (size_t i = 0; i < path.size() - 1; i++) {
    Vertex src = path[i];
    Vertex dest = path[i+1];
    min_weight = min(min_weight , residual_.getEdgeWeight(src,dest));
  }
  return min_weight;
}

  /**
   * calculuateFlow - Determine the capacity of a path in the residual graph.
   * Sets the member function `maxFlow_` to be the flow, and updates the
   * residual graph and flow graph according to the algorithm.
   *
   * @return The network flow graph.
   */

const Graph & NetworkFlow::calculateFlow() {
  // YOUR CODE HERE
  /*
    Keep looping until no more valid augmenting paths can be found.
    In the loop, get the capacity of the path found using pathCapacity()
    Using that capacity, you will update three paths:
        1 Add the capacity to the edges in the corresponding path in the flow graph. 
            Note that this path may go in the opposite direction of the edge in your graph. In that case, reverse the vertices and subtract the capcity from the edge in the flow graph
        2 Subtract the capacity from the forward edges in the residual graph.
        3 Add the capacity to the reverse edges in the residual graph.
  */
  vector<Vertex> p;
  // bool found_path = findAugmentingPath(source_, sink_, path);

  while (findAugmentingPath(source_, sink_, p)){
    
    int cap = pathCapacity(p);
    for (size_t i = 0; i < p.size() - 1; ++i) {
      Vertex& src = p[i];
      Vertex& dest = p[i+1];
      if (flow_.edgeExists(src, dest)) {
        int wt = flow_.getEdgeWeight(src, dest);
        flow_.setEdgeWeight(src, dest, wt + cap);
      } else {
        int wt = flow_.getEdgeWeight(dest, src);
        flow_.setEdgeWeight(dest, src, wt - cap);
      }
      int fwd = residual_.getEdgeWeight(src, dest);
      int rev = residual_.getEdgeWeight(dest, src);

      residual_.setEdgeWeight(src, dest, fwd - cap);
      residual_.setEdgeWeight(dest, src, rev + cap);
    }
  }

  int vertexOutflow = 0;
  for (auto v2 : flow_.getAdjacent(source_)) {
    vertexOutflow += flow_.getEdgeWeight(source_, v2);
  }
  maxFlow_ = vertexOutflow;
  return flow_;
}

int NetworkFlow::getMaxFlow() const {
  return maxFlow_;
}

const Graph & NetworkFlow::getGraph() const {
  return g_;
}

const Graph & NetworkFlow::getFlowGraph() const {
  return flow_;
}

const Graph & NetworkFlow::getResidualGraph() const {
  return residual_;
}

