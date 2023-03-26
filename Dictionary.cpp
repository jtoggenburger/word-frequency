//-----------------------------------------------
// Jack Toggenburger
// jtoggenb
// Dictionary.cpp
//-----------------------------------------------

#include "Dictionary.h"

#include <iostream>
#include <string>
#include <stdexcept>

#define BLACK 1
#define RED 0

int temp = 0;

// Private Constructors ---------------------------------------------------------------------------

Dictionary::Node::Node(keyType k, valType v) {
	key = k;
	val = v;
	
	parent = nullptr;
	left = nullptr;
	right = nullptr;

	color = RED;
}

// Helper Functions -------------------------------------------------------------------------------
void Dictionary::inOrderString(std::string& s, Node* R, Node* NIL) const {
	if (R != NIL) {
		inOrderString(s, R->left, NIL);	
		s += R->key;
		s += " : ";
		s += std::to_string(R->val);
		s += "\n";
		inOrderString(s, R->right, NIL);
	}	
}

void Dictionary::preOrderString(std::string &s, Node* R, Node * NIL) const {
	if (R != NIL) {
                s += R->key;
                s += "\n";
                preOrderString(s, R->left, NIL);
		preOrderString(s, R->right, NIL);
        }
}

/*
void Dictionary::preOrderCopy(Node* R, Node* NIL) {
	if (R != NIL) {
		preOrderCopy(R->left, NIL);
		preOrderCopy(R->right, NIL);	
        }
}
*/

void Dictionary::postOrderDelete(Node* R, Node* NIL) {
	if (R != NIL) {
                postOrderDelete(R->left, NIL);
                postOrderDelete(R->right, NIL);
                delete R;
        }
}


Dictionary::Node* Dictionary::search(Node* R, keyType k, Node* NIL) const {
	if (R == NIL || k == R->key) {
		return R;
	}	
	else if (k < R->key) {
		return search(R->left, k, NIL);
	}
	else {
		return search(R->right, k, NIL);
	}
}

Dictionary::Node* Dictionary::findMin(Node* R, Node* NIL) {
	if (R != NIL) {

		while (R->left != NIL) {
			R = R->left;
		}

		return R;
	}

	return NIL;
}

Dictionary::Node* Dictionary::findMax(Node* R, Node* NIL) {
	if (R != NIL) {

		while (R->right != NIL) {
			R = R->right;
		}

		return R;
	
	}

	return NIL;
}


Dictionary::Node* Dictionary::findNext(Node* N, Node* NIL) {
	//if (N == findMax(N, NIL) || N == NIL) {
	//	return NIL;
	//}
	
	// otherwise we are not rightmost or NIL
		
	if (N->right != NIL) {
		return findMin(N->right, NIL);
	}
	
	Node* y = N->parent;

	while (y != NIL && N == y->right) {
		N = y;
		y = y->parent;
	}

	return y;

}

Dictionary::Node* Dictionary::findPrev(Node* N, Node* NIL) {
	//if (N == findMin(N, NIL) || N == NIL) {
        //        return NIL;
        //}

	// otherwise we are not the leftmost or NIL
	
	if (N->left != NIL) {
		return findMax(N->left, NIL);
	}

	Node* y = N->parent;

	while (y != NIL && N == y->left) {
		N = y;
		y = y->parent;
	}

	return y;
}


// RBT Helper Functions ---------------------------------------------------------------------------

void Dictionary::LeftRotate(Node* N, Node* NIL) {

	// set y	
	Node* y = N->right;

	// turn y's left subtree into x's right subtree	
	N->right = y->left;
	if (y->left != NIL) {
		y->left->parent = N;
	}

	// link y's parent to x
	y->parent = N->parent;
	if (N->parent == NIL) {
		root = y;
	}

	else if (N == N->parent->left) {
		N->parent->left = y;
	}

	else {
		N->parent->right = y;		
	}

	// put x on y's right
	
	y->left = N;
	N->parent = y;
}

void Dictionary::RightRotate(Node* N, Node* NIL) {
	
	// set y
	Node* y = N->left;

	// turn y's right subtree into x's left subtree
	
	N->left = y->right;
	if (y->right != NIL) {
		y->right->parent = N;
	}

	// link y's parent to x
	y->parent = N->parent;
	if (N->parent == NIL) {
		root = y;
	}

	else if (N == N->parent->right) {
		N->parent->right = y;
	}

	else {
		N->parent->left = y;
	}

	// put x on y's right
	
	y->right = N;
	N->parent = y;
}

void Dictionary::RB_InsertFixUp(Node* N, Node* NIL) {

	while (N->parent->color == RED) {

		if (N->parent == N->parent->parent->left) {	//THIS IS GIVING A SEGFAULT (N->parent->parent->left is not declared
			Node* y = N->parent->parent->right;
	
			if (y->color == RED) {
				N->parent->color = BLACK;
				y->color = BLACK;
				N->parent->parent->color = RED;
				N = N->parent->parent;
			}
			
			else {
				if (N == N->parent->right) {
					N = N->parent;
					LeftRotate(N, NIL);
				}

				N->parent->color = BLACK;
				N->parent->parent->color = RED;
				RightRotate(N->parent->parent, NIL);
			}
		}

		else {
			Node* y = N->parent->parent->left;

			if (y->color == RED) {
				N->parent->color = BLACK;
				y->color = BLACK;
				N->parent->parent->color = RED;
				N = N->parent->parent;
			}

			else {
				if (N == N->parent->left) {
					N = N->parent;
					RightRotate(N, NIL);
				}

				N->parent->color = BLACK;
				N->parent->parent->color = RED;
				LeftRotate(N->parent->parent, NIL);
			}
		}
	}
	root->color = BLACK;
	
}

void Dictionary::RB_Transplant(Node* u, Node* v, Node* NIL) {
	if (u->parent == NIL) {
		root = v;
	}

	else if (u == u->parent->left) {
		u->parent->left = v;
	}

	else {
		u->parent->right = v;
	}

	v->parent = u->parent;

}

void Dictionary::RB_DeleteFixUp(Node* N, Node* NIL) {
	while (N != root && N->color == BLACK) {
		
		if (N == N->parent->left) {
			Node* w = N->parent->right;
			
			if (w->color == RED) {
				w->color = BLACK;
				N->parent->color = RED;
				LeftRotate(N->parent, NIL);
				w = N->parent->right;
			}

			if (w->left->color == BLACK && w->right->color == BLACK) {
				w->color = RED;
				N = N->parent;
			}

			else {
				if (w->right->color == BLACK) {
					w->left->color = BLACK;
					w->color = RED;
					RightRotate(w, NIL);
					w = N->parent->right;
				}
				
				w->color = N->parent->color;
				N->parent->color = BLACK;
				w->right->color = BLACK;
				LeftRotate(N->parent, NIL);
				N = root;
			}		
		}

		else {
			Node* w = N->parent->left;
			
			if (w->color == RED) {
				w->color = BLACK;
				N->parent->color = RED;
				RightRotate(N->parent, NIL);
				w = N->parent->left;
			}

			if (w->right->color == BLACK && w->left->color == BLACK) {
				w->color = RED;
				N = N->parent;
			}	
			
			else {
				if (w->left->color == BLACK) {
					w->right->color = BLACK;
					w->color = RED;
					LeftRotate(w, NIL);
					w = N->parent->left;
				}

				w->color = N->parent->color;
				N->parent->color = BLACK;
				w->left->color = BLACK;
				RightRotate(N->parent, NIL);
				N = root;

			}
		}
	}

	N->color = BLACK;
}

void Dictionary::RB_Delete(Node* N, Node* NIL) {
	Node* y = N;
	Node* x;
      //Node* z = N;

	int y_original_color = y->color;

	if (N->left == NIL) {
		x = N->right;	
		RB_Transplant(N, N->right, NIL);
		num_pairs--;
		delete (N);
	}

	else if (N->right == NIL) {
		x = N->left;
		RB_Transplant(N, N->left, NIL);
		num_pairs--;
		delete(N);
	}

	else {
		y = findMin(N->right, NIL);
		y_original_color = y->color;
		x = y->right;
		
		if (y->parent == N) {
			x->parent = y;
		}

		else {
			RB_Transplant(y, y->right, NIL);
			y->right = N->right;
			y->right->parent = y;
		}
		
		RB_Transplant(N, y, NIL);
		y->left = N->left;
		y->left->parent = y;
		y->color = N->color;
		num_pairs--;
                delete(N);
	}

	if (y_original_color == BLACK) {
		RB_DeleteFixUp(x, NIL);
	}

} 


// Public Contructors -----------------------------------------------------------------------------

Dictionary::Dictionary() {
	nil = new Node("", -1);
	nil->color = BLACK;
	root = nil;
	current = nil;

	num_pairs = 0;
}

Dictionary::Dictionary(const Dictionary& D) {
        nil = new Node("", -1);
	nil->color = BLACK;
        root = nil;
        current = nil;
	
        num_pairs = 0;
	//preOrderCopy(root, nil);	//figure this out
}

Dictionary::~Dictionary() {
	clear();
	delete nil;
}



// Access functions--------------------------------------------------------------------------------

// returns the size of this Dictionary
int Dictionary::size() const {
	return num_pairs;
}

// returns true if there exists a pair such that key==k, and returns false otherwise
bool Dictionary::contains(keyType k) const {
	if (search(root, k, nil) == nil || num_pairs == 0) {
		return false;
	} else {
		return true;
	}
}

// returns a reference to the value corresponding to key k
valType& Dictionary::getValue(keyType k) const {
	if (search(root, k, nil) == nil) {
		throw std::invalid_argument("Dictionary: getValue: key is not in binary search tree");
	}

	return search(root, k, nil)->val;
}

// returns true if the current interator is defined, false otherwise
bool Dictionary::hasCurrent() const {
	if (current != nil) {
		return true;
	} else {
		return false;
	}
}

// returns the current key
// pre: hasCurrent()
keyType Dictionary::currentKey() const {
	if (!hasCurrent()) {
		throw std::logic_error("Dictionary: currentKey: current is not defined");
		//return "";
	}

	return current->key;
}

// returns reference to the current value
// pre: hasCurrent()
valType& Dictionary::currentVal() const {
	if (!hasCurrent()) {
                throw std::logic_error("Dictionary: currentVal: current is not defined");
                //return temp;
        }
	
	return current->val;
}

// Manipulation procedures ------------------------------------------------------------------------

// resets this Dictionary to the empty state, containing no pairs
void Dictionary::clear() {
	postOrderDelete(root, nil);
	current = nil;	
	num_pairs = 0;	
}


// CHANGE TO WORK WITH BST
// if a pair with key==k exists, overwrites the corresponding value with v, otherwise inserts the new pair (k, v)
void Dictionary::setValue(keyType k, valType v) {

	// Extra Nodes
	Node* y = nil;
	Node* x = root;

	// new node we might be inserting
	Node* z = new Node(k, v);
	z->left = nil;
        z->right = nil;
        z->color = RED;
	num_pairs++;	

	while (x != nil) { 
		y = x;

		if (z->key == x->key) {
			y->val = v;
                        num_pairs--;
                        delete (z);
			return;
		}

		else if (z->key < x->key) {	//try to turn this into the equals statment first
			x = x->left;	
		}
	
		else {
			x = x->right;
		}
	}

	z->parent = y;
	
	if (y == nil) {
		root = z;
	}	

	else if (z->key < y->key) {
		y->left = z;
	}

	else {
		y->right = z;
	}

	RB_InsertFixUp(z, nil); 
}

// change to work with BST
// deletes the pair for which key==k. If that pair is current, then current becomes undefined
void Dictionary::remove(keyType k) {
	Node* z = search(root, k, nil);
	if (z == nil) {
		return;
	}

	//num_pairs--;

	if (current == z) {
		current = nil;
	}

	// begin deletion
	
	RB_Delete(z, nil);
}

// places current iterator on smallest value
void Dictionary::begin() {
	if (num_pairs > 0) {
		current = findMin(root, nil);
	}
	
	else {
		current = nil;
	}
}

// places current iterator on largest value
void Dictionary::end() {
	if (num_pairs > 0) {
		current = findMax(root, nil);
	} else {
		current = nil;
	}
}

// if the current iterator is not at the last pair, advances current to the nect pair, if last pair, undefined
void Dictionary::next() {
	
	if (!hasCurrent()) {
		return;
	}

	if (current == findMax(root, nil)) {
		current = nil;
		return;
	}

	current = findNext(current, nil);
}

// if current is not first pair, move current to previous pair. If current is at first, make undefined
void Dictionary::prev() {
	
	if (!hasCurrent()) {
		return;
	}

	if (current == findMin(root, nil)) {
		current = nil;
		return;
	}

	current = findPrev(current, nil);
}

// Other functions --------------------------------------------------------------------------------

// outputs string using an inorder walk
std::string Dictionary::to_string() const {
	std::string s = "";
	inOrderString(s, root, nil);
	return s;
}

// outputs string using a pre-order walk
std::string Dictionary::pre_string() const {
	std::string s = "";
	preOrderString(s, root, nil);
	return s;
}

// returns true if and only id this dictionary contains the same (key, value) pairs as Dictionary D
bool Dictionary::equals(const Dictionary& D) const {
	std::string A = to_string();
	std::string B = D.to_string();

	if (A == B) {
		return true;
	}

	else {
		return false;
	}

}

// Overloaded Operators ---------------------------------------------------------------------------

std::ostream& operator<<( std::ostream& stream, Dictionary& D) {
	return stream << D.Dictionary::to_string();
}

bool operator==( const Dictionary& A, const Dictionary& B ) {
	return A.Dictionary::equals(B);
} 

Dictionary& Dictionary::operator=( const Dictionary& D) {
	if (this != &D) {
		
		Dictionary t = D;
	}
	
	return *this;	
}
