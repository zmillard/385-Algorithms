/*******************************************************************************
 * Name        : rbtree.h
 * Author      : Zoe Millard
 * Version     : 1.0
 * Date        : 6/25/16
 * Description : Implementation of red-black tree.
 * Pledge      : I pledge my honor that I have abided by the Stevens honor System -ZM
 ******************************************************************************/
#ifndef RBTREE_H_
#define RBTREE_H_

#include "node.h"
#include "tree.h"
#include "treeprinter.h"
#include <iostream>
#include <cstdlib>
#include <exception>
#include <string>
#include <sstream>
#include <algorithm>
#include <utility>

using namespace std;
// Forward declaration;
template <typename K, typename V>

class RedBlackTree;

/**
 * tree_exception class
 * Demonstrates how you can write your own custom exceptions in C++.
 */
class tree_exception: public std::exception {
public:
    tree_exception(const std::string &message) : message_(message) { }

    ~tree_exception() throw() { }

    virtual const char* what() const throw() {
        return message_.c_str();
    }

private:
    std::string message_;
};

template <typename K, typename V>
class RedBlackTreeIterator {
public:
    /**
     * Constructor
     */
    RedBlackTreeIterator() : node_ptr(NULL), tree(NULL) { }

    /**
     * Equality operator. Compares node pointers.
     */
    bool operator==(const RedBlackTreeIterator &rhs) const {
        return node_ptr == rhs.node_ptr;
    }

    /**
     * Inequality operator. Compares node pointers.
     */
    bool operator!=(const RedBlackTreeIterator &rhs) const {
        return node_ptr != rhs.node_ptr;
    }

    /**
     * Dereference operator. Returns a reference to key-value pair pointed to
     * by node_ptr.
     */
    std::pair<K, V> operator*() const {
        return node_ptr->key_value();
    }

    std::pair<K, V>* operator->() const {
        return &node_ptr->key_value();
    }

    /**
     * Preincrement operator. Moves forward to next larger value.
     */
    RedBlackTreeIterator& operator++() {
        Node<K, V> *p;

        if (node_ptr == NULL) {
            // ++ from end(). Get the root of the tree.
            node_ptr = tree->root_;

            // Error, ++ requested for an empty tree.
            if (node_ptr == NULL)
                throw tree_exception(
                        "RedBlackTreeIterator operator++(): tree empty");

            // Move to the smallest value in the tree, which is the first node
            // in an inorder traversal.
            while (node_ptr->left() != NULL) {
                node_ptr = node_ptr->left();
            }
        } else {
            if (node_ptr->right() != NULL) {
                // Successor is the leftmost node of right subtree.
                node_ptr = node_ptr->right();

                while (node_ptr->left() != NULL) {
                    node_ptr = node_ptr->left();
                }
            } else {
                // Have already processed the left subtree, and
                // there is no right subtree. Move up the tree,
                // looking for a parent for which node_ptr is a left child,
                // stopping if the parent becomes NULL (or in this case,
                // root_parent_. A non-NULL parent is the successor. If parent
                // is NULL, the original node was the last node inorder, and
                // its successor is the end of the list.
                p = node_ptr->parent();
                while (p != NULL && node_ptr == p->right()) {
                    node_ptr = p;
                    p = p->parent();
                }

                // If we were previously at the rightmost node in
                // the tree, node_ptr = NULL, and the iterator specifies
                // the end of the list.
                node_ptr = p;
            }
        }

        return *this;
    }

    /**
     * Postincrement operator. Moves forward to next larger value.
     */
    RedBlackTreeIterator operator++(int) {
        RedBlackTreeIterator tmp(*this);
        operator++(); // prefix-increment this instance
        return tmp;   // return value before increment
    }

private:
    // node_ptr is the current location in the tree. We can move
    // freely about the tree using left, right, and parent.
    // tree is the address of the RedBlackTree object associated
    // with this iterator. It is used only to access the
    // root pointer, which is needed for ++ and --
    // when the iterator value is end().
    Node<K, V> *node_ptr;
    RedBlackTree<K, V> *tree;
    friend class RedBlackTree<K, V>;

    /**
     * Constructor used to construct an iterator return value from a tree
     * pointer.
     */
    RedBlackTreeIterator(RedBlackNode<K, V> *p, RedBlackTree<K, V> *t) :
        node_ptr(p), tree(t) { }
};

template<typename K, typename V>
class RedBlackTree : public Tree {
public:
    typedef RedBlackTreeIterator<K, V> iterator;

    /**
     * Constructor to create an empty red-black tree.
     */
    RedBlackTree() : root_(NULL), size_(0) { }

    /**
     * Constructor to create a red-black tree with the elements from the
     * vector.
     */
    RedBlackTree(std::vector< std::pair<K, V> > &elements) :
                                            root_(NULL), size_(0) {
        insert_elements(elements);
    }

    /**
     * Destructor.
     */
    ~RedBlackTree() {
        delete_tree(root_);
    }

    /**
     * Inserts elements from the vector into the red-black tree.
     * Duplicate elements are not inserted.
     */
    void insert_elements(std::vector< std::pair<K, V> > &elements) {
        for (size_t i = 0, len = elements.size(); i < len; ++i) {
            try {
                insert(elements[i].first, elements[i].second);
            } catch (const tree_exception &te) {
                std::cerr << "Warning: " << te.what() << std::endl;
            }
        }
    }

    /**
     * Inserts a key-value pair into the red black tree.
     * const iterator &it indicates where to start the search for the place to
     * insert the node. If it == end(), the search starts at the root.
     */
    void insert(const iterator &it, const std::pair<K, V> &key_value) {
        K key = key_value.first;
        Node<K, V> *x, *y;
        if (it != end()) {
            x = it.node_ptr;
            y = x->parent();
        } else {
            x = root_;
            y = NULL;
        }
        // TODO
        while(x != NULL){ //throws duplication error to prevent mem leak
        	y = x;
			if(key < x->key()){
				x = x->left();
			}else if(key == x->key()){
				cerr << "Warning: Attempt to insert duplicate key '" << key << "'." << endl;
				return;
			}else{
				x = x->right();
			}
        }
        //textbook pseudocode
        while( x != NULL){
        	y = x;
        	if(key < x->key()){
        		x = x->left();
        	}else{
        		x = x->right();
        	}
        }
        RedBlackNode<K, V> *z = new RedBlackNode<K, V>(key, key_value.second);
        z->set_parent(y);
        if(y == NULL){
        	root_ = z;
        }else if(key < y->key()){
        	y->set_left(z);
        }else{
        	y->set_right(z);
        }
        z->set_value(key_value.second);
		z->set_left(NULL);
		z->set_right(NULL);
		z->set_color(RED);
		size_++;
		insert_fixup(z);
    }

    /**
     * Inserts a key-value pair into the red-black tree.
     */
    void insert(const K &key, const V &value) {
        iterator e = end();
        insert(e, std::pair<K, V>(key, value));
    }

    /**
     * Returns an ASCII representation of the red-black tree.
     */
    std::string to_ascii_drawing() {
        BinaryTreePrinter<K, V> printer(root_);
        return printer.to_string();
    }

    /**
     * Returns the height of the red-black tree.
     */
    int height() const {
        return height(root_) - 1;
    }

    /**
     * Returns the number of nodes in the red-black tree.
     */
    size_t size() const {
        return size_;
    }

    /**
     * Returns the leaf count of the red-black tree.
     */
    size_t leaf_count() const {
        return leaf_count(root_);
    }

    /**
     * Returns the internal node count of the red-black tree.
     */
    size_t internal_node_count() const {
        return internal_node_count(root_);
    }

    /**
     * Returns the diameter of the red-black tree.
     */
    size_t diameter() const {
        return diameter(root_);
    }

    /**
     * Returns the max width of the red-black tree, i.e. the largest number of
     * nodes on any level.
     */
    size_t max_width() const {
        size_t max_width = 0;
        for (int i = 0, h = height(root_) + 1; i < h; ++i) {
            size_t w = width(root_, i);
            if (w > max_width) {
                max_width = w;
            }
        }
        return max_width;
    }

    /**
     * Returns the successful search cost, i.e. the average number of nodes
     * visited to find a key that is present.
     */
    double successful_search_cost() const {
        return size_ == 0 ? 0 : 1 + (double)sum_levels() / size_;
    }

    /**
     * Returns the unsuccessful search cost, i.e. the average number of nodes
     * visited to find a key that is not present.
     */
    double unsuccessful_search_cost() const {
        return (double)sum_null_levels() / null_count();
    }

    /**
     * Searches for item. If found, returns an iterator pointing
     * at it in the tree; otherwise, returns end().
     */
    iterator find(const K &key) {
        Node<K, V> *x = root_;
        while (x != NULL) {
            K current_key = x->key();
            if (key == current_key) {
                break; // Found!
            } else if (key < current_key) {
                x = x->left();
            } else {
                x = x->right();
            }
        }
        return iterator(static_cast< RedBlackNode<K, V>* >(x), this);
    }

    /**
     * Returns an iterator pointing to the first item in order.
     */
    iterator begin() {
        RedBlackNode<K, V> *curr = root_;

        // if the tree is not empty, the first node
        // in order is the farthest node left from root
        if (curr != NULL) {
            while (curr->left() != NULL) {
                curr = static_cast< RedBlackNode<K, V>* >(curr->left());
            }
        }

        // build return value using private constructor
        return iterator(curr, this);
    }

    /**
     * Returns an iterator pointing just past the end of the tree data.
     */
    iterator end() {
        return iterator(NULL, this);
    }

private:
    RedBlackNode<K, V> *root_;
    size_t size_;
    friend class RedBlackTreeIterator<K, V>;

    /**
     * Deletes all nodes from the red-black tree.
     */
    void delete_tree(Node<K, V> *n) {
    	if(n != NULL){
			delete_tree(n->left());
			delete_tree(n->right());
			delete n;
		}
    	delete n;
    }

    /**
     * Fixup method described on p. 316 of CLRS.
     */
    void insert_fixup(RedBlackNode<K, V> *z) {
        // TODO
    	RedBlackNode<K, V> *y;
    	while((z->parent() != NULL) && (z->parent()->color() == RED)){
    		if(z->parent() == z->parent()->parent()->left()){
    			y = z->parent()->parent()->right();
    			if((y != NULL) && (y->color() == RED)){
    				z->parent()->set_color(BLACK); //use ->set_color(BLACK) method
    				y->set_color(BLACK);
    				z->parent()->parent()->set_color(RED);
    				z = z->parent()->parent();
    			}else{
    				if(z == z->parent()->right()){
						z = z->parent();
						left_rotate(z);
    				}
					z->parent()->set_color(BLACK);
					z->parent()->parent()->set_color(RED);
					right_rotate(z->parent()->parent());
    			}
    		}else{
    			y = z->parent()->parent()->left();
				if((y != NULL) && (y->color() == RED)){
					z->parent()->set_color(BLACK);
					y->set_color(BLACK);
					z->parent()->parent()->set_color(RED);
					z = z->parent()->parent();
				}else{
					if(z == z->parent()->left()){
						z = z->parent();
						right_rotate(z);
					}
					z->parent()->set_color(BLACK);
					z->parent()->parent()->set_color(RED);
					left_rotate(z->parent()->parent());
				}
    		}
    	}
        // Last line below
        root_->set_color(BLACK);
    }

    /**
     * Left-rotate method described on p. 313 of CLRS.
     * Implementation of textbook pseudocode
     */
    void left_rotate(Node<K, V> *x) {
    	// TO DO
    	Node<K, V> *y;
        y = x->right();
        x->set_right(y->left());
        y->set_left(x->right());
        if(y->left() != NULL){
        	y->left()->set_parent(x);
        }
        y->set_parent(x->parent());
        x->set_parent(y->parent());
        if(x->parent() == NULL){
        	root_ = static_cast<RedBlackNode<K, V>*>(y);
        }else if(x== x->parent()->left()){
        	x->parent()->set_left(y);
        }else{
        	x->parent()->set_right(y);
        }
        y->set_left(x);
        x->set_parent(y);
    }

    /**
     * Right-rotate method described on p. 313 of CLRS.
     * Implementation of textbook pseudocode
     */
    void right_rotate(Node<K, V> *x) {
        // TODO
    	//LEFT ROTATE: Just switched left and right
    	Node<K, V> *y;
		y = x->left();
		x->set_left(y->right());
		y->set_right(x->left());
		if(y->right() != NULL){
			y->right()->set_parent(x);
		}
		y->set_parent(x->parent());
		x->set_parent(y->parent());
		if(x->parent() == NULL){
			root_ = static_cast<RedBlackNode<K, V>*>(y);
		}else if(x== x->parent()->right()){
			x->parent()->set_right(y);
		}else{
			x->parent()->set_left(y);
		}
		y->set_right(x);
		x->set_parent(y);
    }

    /**
     * Returns the height of the red-black tree starting at node.
     * A null node starts at height -1.
     * Recursively finds the height
     */
    int height(Node<K, V> *node) const {
        // TODO
    	//static_cast<RedBlackNode<K, V>*>(node);
    	if(node == NULL){
    		return 0;
    	}else{
    		int height_left = height(node->left());
    		int height_right = height(node->right());
    		if(height_left > height_right){
    			return height_left + 1;
    		}else{
    			return height_right + 1;
    		}
    	}
    }

    /**
     * Returns the count of leaves in the red-black tree starting at node.
     * For this method, a leaf is a non-null node that has no children.
     * recursively counts the leaves
     */
    size_t leaf_count(Node<K, V> *node) const {
        // TODO
    	int leaf = 0;
		if(node == NULL){
			return 0;
		}
		if(node->left() == NULL && node->right()==NULL){
			leaf++;
		}else{
			leaf = leaf_count(node->left())+leaf_count(node->right());
		}
		return leaf;
    }
    /**
     * Returns the count of internal nodes in the red-black tree starting at
     * node.
     * An internal node has at least one child.
     * Uses recursion to count the nodes
     */
    size_t internal_node_count(Node<K, V> *node) const {
        // TODO
    	int node_c = 0;
    	if(node == NULL){
			return 0;
    	}else if(node->left() == NULL && node->right()==NULL){
    		node_c = internal_node_count(node->left())+internal_node_count(node->right());
		}else{
			node_c = 1 + internal_node_count(node->left())+internal_node_count(node->right());
		}
    	return node_c;
    }

    /**
     * Returns the diameter of the red-black tree rooted at node.
     * Diameter is defined as the number of nodes on the longest path between
     * two (non-null) leaves in the tree. The path does not necessarily have to
     * pass through the root.
     */
    int diameter(Node<K, V> *node) const {
        // TODO
    	int total_height, total_d;
    	if (node == NULL){
			return 0;
		}
    	//finds hight and diameter of each side
    	int left_height = height(node->left());
    	int right_height = height(node->right());
    	int left_diameter = diameter(node->left());
    	int right_diameter = diameter(node->right());

    	total_height = left_height +  right_height;
    	//finds max
    	if(left_diameter > right_diameter){
    		total_d = left_diameter;
    	}else{
    		total_d = right_diameter;
    	}
    	//finds max
    	if(total_height > total_d){
    		return total_height;
    	}else{
    		return total_d;
    	}
    	return 1;
    }

    /**
     * Returns the width of the red-black tree at the designated level.
     * Width is defined as the number of nodes residing at a level.
     */
    size_t width(Node<K, V> *node, size_t level) const {
        // TODO
    	size_t wide = 0;
		if(node == NULL){
			return 0;
		}else if(level == 0){
			++wide;
		}else{
    	    wide += width(node->left(), level-1) + width(node->right(), level-1);
		}
		return wide;
    }

    size_t null_count() const {
        return null_count(root_);
    }

    /**
     * Returns the count of null nodes in the red-black tree starting at node.
     * Uses recursion to find vals for each side
     */
    size_t null_count(Node<K, V> *node) const {
        // TODO
    	int count = 0;
		if (node == NULL)
			return 1;
		if (node->left() != NULL)
			count += null_count(node->left());
		else
			count++;
		if (node->right() != NULL)
			count += null_count(node->right());
		else
			count++;
		return count;
    }

    size_t sum_levels() const {
        return sum_levels(root_, 0);
    }

    /**
     * Returns the sum of the levels of each non-null node in the red-black
     * tree starting at node.
     * For example, the tree
     *   5 <- level 0
     *  / \
     * 2   8 <- level 1
     *      \
     *       10 <- level 2
     * has sum 0 + 2(1) + 2 = 4.
     */
    size_t sum_levels(Node<K, V> *node, size_t level) const {
        // TODO
    	size_t level_c = 0; //level_c counts levels
    	if (node != NULL){
			level_c = level + sum_levels(node->left(), level + 1) + sum_levels(node->right(), level + 1); //counts levels w/ recursion
    	}
		return level_c;
    }

    size_t sum_null_levels() const {
        return sum_null_levels(root_, 0);
    }

    /**
     * Returns the sum of the levels of each null node in the red-black tree
     * starting at node.
     * For example, the tree
     *    5 <- level 0
     *   / \
     *  2   8 <- level 1
     * / \ / \
     * * * * 10 <- level 2
     *       / \
     *       * * <- level 3
     * has sum 3(2) + 2(3) = 12.
     */
    size_t sum_null_levels(Node<K, V> *node, size_t level) const {
        // TODO
    	size_t level_c = level;
    	if(node != NULL){
    		level_c =  sum_null_levels(node->left(), level + 1) + sum_null_levels(node->right(), level + 1); //counts levels w/recursion
    	}
		return level_c;
    }
};

#endif /* RBTREE_H_ */
