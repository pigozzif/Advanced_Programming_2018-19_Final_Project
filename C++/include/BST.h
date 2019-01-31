//: include/BST.h

#ifndef __BST_H__
#define __BST_H__


#include <functional>
#include <utility>
#include <memory>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <iterator>
#include <initializer_list>


#ifdef __BST_DEV__
/**
 * Namespace containing all the testing tools for BST and
 * related classes.
 */
namespace BST_testing {

    /**
     * The Tester class provides a suite of tests to be performed on the
     * BST and related classes to test their correctness
     */
    class Tester;
}
#endif


namespace {

    /**
     * BST_Node struct, represents a node in a BST.
     */
    template <class K, class V>
    struct BST_node;
    /**
     * BST_iterator class, made compliant with the STL. Allows in-order traversal of BSTs.
     */
    template <class K, class V>
    class BST_iterator;
    /**
     *BST_const_iterator class. Allows iteration through const BSTs.
     */
    template <class K, class V>
    class BST_const_iterator;
}

template <class K, class V, class Comp = std::less<K>>
class BST{

    public:
	//!Alias for the type of keys in the tree
	using key_type = K;
	//!Alias for the type of values associated to keys in the tree
	using value_type = V;
	//!Alias for the key-value pairs stored in the BST. The key is declared as const to prevent it from being changed once in the tree.
	using pair_type = std::pair<const K, V>;

	#ifdef __BST_DEV__
	friend BST_testing::Tester;
	#endif

    private:

	//!Alias for the node type
	using node_type = BST_node<K,V>;//This alias is left private since nodes are not intendend for user usage.

	//!Pointer to the root node of the BST
	std::unique_ptr<node_type> root;
	//!Function object defining the comparison criteria for key_type objects.
	Comp compare;

	/**
	 * Utility function to insert in the BST a full subtree. Elements of the subtree are
	 * inserted in the BST starting at the root and then recursively inserting the
	 * left and right subtrees.
	 * @param subtree to copy into the BST
	 */
	void insert( const node_type& subtree);
	/**
	 * Utility function to insert in the tree the median element, with respect to
	 * given boundaries, from a vector of pair_type.
	 * @param vect vector of pair_type elements to be inserted in the BST
	 * @param lo min index to consider in the given vector
	 * @param hi max index to consider in the given vector
	 */
	void insert_median(std::vector<pair_type>& vect , const size_t lo, const size_t hi);
        /**
         * Return a pointer to the node having the smallest key.
         */
        node_type* get_min() const noexcept;

    public:

	/**
	 * Create an empty BST. The root pointer is set to nullptr and the compare function is
	 * default initialized.
	 */
	BST () = default;
        /**
         * Create a BST from std::initializer_list, the compare function is default initialized, nodes
         * are added by repeatedly calling insert
         * @param args an std::initializer_list of std::pair<K,V>
         */
        BST(const std::initializer_list<std::pair<K,V>> args) : root{}, compare{} {
            for (const auto& x : args) insert(x);
        }
	/**
	 * Copy constructor, create a new BST having the same key-value pairs as other. This
	 * constructor also preserves the structure of the copied BST.
	 * @param other BST to be copied
	 */
	BST (const BST<K,V,Comp> &other)
	{
	    insert(*other.root);//recursively inserts all nodes in other starting at the root node
	}
        /**
         * Copy assignment, copy all the members from one tree to this
         * @param other BST to copy
         */
        BST& operator=(const BST<K,V,Comp> &other) {
            clear();    //free any memory
            auto temp{other};    //call copy constructor
            (*this) = std::move(temp);    //call move assignment
            return *this;
        }
	/**
	 * Move constructor, create a new BST by swapping members.
	 * @param other BST to move
	 */
	BST (BST<K,V,Comp> &&other) noexcept {

	    root.swap(other.root);
	}
        /**
         * Move assignment, move the members of other onto this.
         * @param other BST to move
         */
        BST& operator=(BST<K,V,Comp> &&other) noexcept {
            root = std::move(other.root);
            return *this;
        }
	/**
	 * Default destructor
	 */
	~BST() noexcept = default;

	//!Alias for iterators
	using iterator = BST_iterator<K,V>;
	//!Alias for const iterators
	using const_iterator = BST_const_iterator<K,V>;
        /**
         * Returns an iterator to the node having a key equal to the input key, end()
         * if it is not found. Moves down the tree exploiting the ordering of the keys.
         * @param key the sought-after key
         */
        iterator find(const key_type key) const noexcept;
        /**
         * non-const begin and end functions. Allow the BST to support range for-loops.
         * begin returns an iterator to the node having the smallest key
         */
        iterator begin() noexcept {return iterator{get_min()};}
        /**
         * end returns an iterator to nullptr
         */
        iterator end() noexcept {return iterator{nullptr};}
        /**
         * const begin and end functions. begin returns a const_iterator to the node with smallest key
         */
        const_iterator begin() const noexcept {return const_iterator{get_min()};}
        /**
         * returns a const_iterator to nullptr
         */
        const_iterator end() const noexcept {return const_iterator{nullptr};}
        /**
         * cbegin and cend behave like const begin and const end, but can be useful to force an algorithm
         * of the STL to not modify input iterators. cbegin returns a const_iterator to the node with the smallest key
         */
        const_iterator cbegin() const noexcept {return const_iterator{get_min()};}
        /**
         * cend returns a const_iterator to nullptr
         */
        const_iterator cend() const noexcept {return const_iterator{nullptr};}
	/**
	 * Insert a key-value pair in the BST composed by the given key and value.
	 * @param key the key in the pair
	 * @param value the value in the pair
	 */
	void insert(const key_type& key, const value_type& value);
	/**
	 * Insert a key-value pair in the BST.
	 * @param pair the key-value pair to insert
	 */
	void insert(const pair_type& pair){

	    insert(pair.first, pair.second);
	}
	/**
	 * Balance the current BST.
	 */
	void balance();
	/**
	 * Remove all key-value pairs from the BST.
	 */
	void clear() noexcept {

	    root.reset(nullptr);
	}
	/**
         * Overload of the operator[], in const and non-const version
         */
	value_type& operator[] (const key_type&);
	const value_type& operator[] (const key_type&) const;

};

/*
 * Node struct
 */
namespace{
    template<class K, class V>
    struct BST_node {

	    using pair_type=typename BST<K,V>::pair_type;
	    using key_type=typename BST<K,V>::key_type;
	    using value_type=typename BST<K,V>::value_type;
	    using node_type=BST_node<K,V>;

	    //! Pointers to left and right child of the node
	    std::unique_ptr<node_type> left_child, right_child;
	    //! Pointer to the parent of this node
	    node_type* parent;
	    //! Key-value pair stored in the node
	    pair_type data;

	    /**
	     * Default constructor for BST_node
	     */
	    BST_node() = delete;    //delete default constructor
	    /**
	     * Create a new node having the given data stored in it and the given parent
	     * @param key key of the key-value pair to store in the node
	     * @param value value of the key-value pair to store in the node
	     * @param father pointer to the parent of the node
	     */
	    BST_node(const key_type key, const value_type value, node_type* father)
	     : left_child{nullptr}, right_child{nullptr}, parent{father}, data{key, value}
	    {}
	    /**
	     * Default destructor for nodes
	     */
	    ~BST_node() noexcept = default;
    };
}

/*
 * Iterator class, made compliant with the STL. In particular, the iterator allows to traverse the tree
 * in-order, that is from the smallest to the greatest key.
 */
namespace {
template<class K, class V>
class BST_iterator : public std::iterator<std::forward_iterator_tag, std::pair<const K,V>>{

        using pair_type = typename BST<K,V>::pair_type;
        using node_type=BST_node<K,V>;
        //! a pointer to the node the iterator is currently over
        node_type* current;

    public:
        BST_iterator(node_type* n) : current{n} {}
        /**
         * dereferencing operator, returns an std::pair<const K, V> of the current node
         */
        pair_type& operator*() const {return current->data;}
        /**
         * pre-increment operator. Implements algorithm for retrieving
         * the successor of a node.
         */
        BST_iterator& operator++() {
            if (current->right_child != nullptr) {    //if current node has right child, go down right and then as much to the left as possible
                current = current->right_child.get();
                while (current->left_child.get()) {
                    current = current->left_child.get();
                }
                return *this;
            }
            else {     //otherwise, go up until you reach the root or you find a node that is not the right child of its parent
                node_type* p = current->parent;
                while (p != nullptr && current == p->right_child.get()) {
                    current = p;
                    p = p->parent;
                }
                current = p;
                return *this;
	    }
        }
        /**
         * Comparison operators
         */
        bool operator==(const BST_iterator& other) {return current == other.current;} //tests whether two iterators share the same current node
        bool operator!=(const BST_iterator& other) {return !(*this==other);}
};
}

/*
 *const_iterator class. Populated with the functions of the iterator class, with the
 *exception of the dereferencing operator that is const, as appropriate
 */
namespace {
template<class K, class V>
class BST_const_iterator : public BST_iterator<K,V> {
    using node_type=BST_node<K,V>;
    using base = ::BST_iterator<K,V>;
    using pair_type = typename BST<K,V>::pair_type;
     public:
        using base::BST_iterator;
	const pair_type& operator*() const {return base::operator*();}
	using base::operator++;
	using base::operator==;
	using base::operator!=;
};
}


#ifdef __BST_DEV__
namespace BST_testing{

    class Tester{

        std::vector<std::pair<int,std::string>> init_test() const;
        using bst_type = BST<int, std::string>;
	public:

	    Tester() = default;

	    //!Perform all available tests on BSTs.
	    void test() const;
	    //!Test the default constructor of BST.
	    bool bst_default_ctor() const noexcept;
	    //!Test the insertion of elements in a BST.
	    bool bst_insert() const;
	    //!Test the copy constructor of BST.
	    bool bst_copy_ctor() const;
	    //!Test that the copy constructor of BST performs deep copies.
	    bool bst_deep_copy() const;
	    //!Test the move constructor of BST.
	    bool bst_move_ctor() const;
	    //!Test copy and move assignments of BST.
            bool test_move_copy_assignment() const;
	    //!Test the balance function of BST.
	    bool bst_balance() const;
	    //!Test BST iterators.
	    bool test_iterator() const;
	    //!Test the find function of BST.
	    bool test_find() const;
	    //!Test the clear function of BST.
            bool test_clear() const;
    };
}
#endif

/*
 * get_min function
 */
template<class K, class V, class Comp>
typename BST<K,V,Comp>::node_type* BST<K,V,Comp>::get_min() const noexcept {
    if (root == nullptr) return nullptr; //if the tree is empty, return nullptr
    node_type* current{root.get()};
    while (current->left_child.get()) {   //do down to the left as much as possible
        current = current->left_child.get();
    }
    return current;
}

/*
 * find function
 */
template<class K, class V, class Comp>
typename BST<K,V,Comp>::iterator BST<K,V,Comp>::find(const key_type key) const noexcept {
    node_type* current{root.get()};
    while (current) {
        key_type curr_key = current->data.first;
        if (!compare(curr_key, key) && !compare(key, curr_key)) {   //if current node has sought-after key, return an iterator to it
            return iterator{current};
        }
        else if (compare(key, curr_key)) {    //if greater, proceed in the left subtree
            current = current->left_child.get();
        }
        else {    //if smaller, proceed in the right subtree
            current = current->right_child.get();
        }
    }
    return end();    //if not found, return end
}

/*
 * insert function (key_type, value_type version)
 */
template<class K, class V, class Comp>
void BST<K,V,Comp>::insert(const key_type& key, const value_type& value){

    if (root == nullptr){ //check if the BST is empty
	root.reset(new node_type{key, value, nullptr});
	return;
    }

    node_type *previous_node{root.get()}; //initialize previous node to root
    node_type *current_node{root.get()}; //initilize also the current node ptr to root
    while (current_node) {

	key_type current_key{current_node->data.first};
        if (!compare(current_key, key) && !compare(key, current_key)) { //if the key is already in the tree update the value
	    current_node->data.second = value;
	    break;
        }
        else if (compare(key, current_key)) { // if the new key is smaller go to left subtree
	    previous_node = current_node;
            current_node = current_node->left_child.get();
        }
        else {    //if new key is bigger go to in the right subtree
	    previous_node = current_node;
            current_node = current_node->right_child.get();
        }
    }
    auto& child = (compare(key, previous_node->data.first)) ? previous_node->left_child : previous_node->right_child;
    child.reset(new node_type{key, value, previous_node});
}

/*
 * insert function (node_type version)
 */
template<class K, class V, class Comp>
void BST<K,V,Comp>::insert( const node_type& subtree){

    insert(subtree.data); //copy data in target to the new tree
    if (subtree.left_child)
	insert(*subtree.left_child); //copy left subtree
    if (subtree.right_child)
	insert(*subtree.right_child); //copy right subtree
}

/*
 * insert_median function
 */
template <class K, class V, class Comp>
void BST<K,V,Comp>::insert_median(std::vector<pair_type>& vect, const size_t lo, const size_t hi){

    if (hi-lo == 1){
    
	insert(vect[lo]);
	insert(vect[hi]);
	return;
    }
    if (hi == lo){
    
	insert(vect[lo]);
	return;
    }
    
    size_t mid = lo + ((hi - lo) >> 1);
    insert(vect[mid]);
    insert_median (vect, lo, mid - 1);
    insert_median (vect,mid + 1, hi);
}

/*
 * Balance function
 */
template<class K, class V, class Comp>
void BST<K,V,Comp>::balance(){

    std::vector<pair_type> pairs;
    for (const auto& x : *this)
	pairs.push_back(x);
    clear();
    insert_median(pairs, 0, pairs.size() - 1);
}

/**
 * Overload of operator[] for BSTs, non-const version
 */
template<class K, class V, class Comp>
typename BST<K,V,Comp>::value_type& BST<K,V,Comp>::operator[](const key_type& arg_key) {

    iterator iter = find(arg_key);
    if (iter != end())
	return (*iter).second;
    insert(pair_type{arg_key, value_type{}});
    return (*find(arg_key)).second;
}

/**
 * Overload of operator[] for BSTs, const version
 */
template<class K, class V, class Comp>
const typename BST<K,V,Comp>::value_type& BST<K,V,Comp>::operator[](const key_type& arg_key) const {
    iterator iter = find(arg_key);
    if (iter != end()) {
        return (*iter).second;
    }
    throw std::out_of_range{"const operator[] trying to access key not present in given BST"};
}

/**
 * Overload of the operator<< for BSTs, allows to print
 * the key: value pairs of the tree in-order.
 */
template<class K, class V, class Comp>
std::ostream& operator<<(std::ostream& os, const BST<K,V,Comp>& tree) {
    for (const auto& x : tree) {
        os << x.first << ": " << x.second << std::endl;    //iterate in order and print the key: value pairs
    }
    return os;
}


#endif
