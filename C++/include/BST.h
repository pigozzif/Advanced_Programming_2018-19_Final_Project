//: include/BST.h

#ifndef __BST_H__
#define __BST_H__

#include <functional>
#include <utility>
#include <memory>

template <class K, class V, class Comp = std::less>
class BST{

    public:
	using key_type = K;
	using value_type = V;
	//! The key is declared as const to prevent it from being changed once in the tree
	using pair_type = std::pair< const K, V>;

    private:

	class BST_node;
	class BST_iterator;
	class BST_const_iterator;

	using node_type = BST_node;

	std::unique_ptr<node_type> root;
	static node_type nullnode;

    public:

	using iterator = BST_iterator;
	using const_iterator = BST_const_iterator;

};

template<class K, class V, class Comp>
class BST<K,V,Comp>::BST_node {
	static Comp compare;
	std::unique_ptr<node_type> left_child, right_child;
	node_type* parent;
	std::unique_ptr<pair_type> data;
	public:
	BST_node() = default;
	BST_node(key_type key, value_type value, const node_type* father)
	 : left_child{nullptr}, right_child{nullptr}, parent{father}, data{key, value}
	{}
};

template<class K, class V, class Comp>
class BST<K,V,Comp>::BST_iterator {
	node_type* current;
};

#endif
