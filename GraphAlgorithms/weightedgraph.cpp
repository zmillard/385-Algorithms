/*******************************************************************************
 * Name        : graph.h
 * Author      : Brian S. Borowski
 * Version     : 1.0
 * Date        : September 27, 2014
 * Description : Graph class with non-negative weights assigned to edges.
 ******************************************************************************/
#include "weightedgraph.h"
#include <iomanip>

using namespace std;

WeightedGraph::WeightedGraph() : Graph() { } //this is calling super

WeightedGraph::~WeightedGraph() { }

void WeightedGraph::add_undirected_edge(
        const vertex_id src, const vertex_id dst, const uint_t weight) {
    add_directed_edge(src, dst, weight);
    add_directed_edge(dst, src, weight);
}

void WeightedGraph::add_directed_edge(
        const vertex_id src, const vertex_id dst, const uint_t weight) {
    for (int i = 0, len = src - adjacency_list_.size() + 1; i < len; ++i) {
        adjacency_list_.push_back(list<vertex_weight_pair>());
    }
    list<vertex_weight_pair> * const adjacent_vertices = &adjacency_list_[src];
    size_t len = adjacent_vertices->size();
    if (len == 0) {
        // Add one to the vertex count, since this source node has never been
        // seen before.
        ++num_vertices_;
    }

    size_t i = 0;
    list<vertex_weight_pair>::iterator it = adjacent_vertices->begin();
    while (it != adjacent_vertices->end()) {
        if (it->first == dst) {
            it->second = weight;
            return;
        } else if (it->first > dst) {
            break;
        }
        ++it;
        ++i;
    }

    adjacent_vertices->insert(it, make_pair(dst, weight));
    // Add one to the vertex count for the destination.
    ++num_vertices_;
    ++num_edges_;
}

uint_t WeightedGraph::get_weight(
        const vertex_id src, const vertex_id dst) const {
    const list<vertex_weight_pair> * const adjacent_vertices =
            &adjacency_list_[src];
    list<vertex_weight_pair>::const_iterator it = adjacency_list_[src].begin();
    while (it != adjacent_vertices->end()) {
        if (it->first == dst) {
            return it->second;
        }
        ++it;
    }
    return 0;
}

string WeightedGraph::to_string() const {
    std::stringstream ss;
    size_t len = adjacency_list_.size(), col_width = Graph::num_digits(len);
    for (size_t i = 1; i < len; ++i) {
        if (i != 1) {
            ss << endl;
        }
        ss << setw(col_width) << i << " -> [";
        list<vertex_weight_pair> const *adjacent_vertices = &adjacency_list_[i];
        for (list<vertex_weight_pair>::const_iterator it =
                adjacent_vertices->begin(); it != adjacent_vertices->end();
                ++it) {
            if (it != adjacent_vertices->begin()) {
                ss << ", ";
            }
            ss << it->first << ":" << it->second;
        }
        ss << "]";
    }
    return ss.str();
}
