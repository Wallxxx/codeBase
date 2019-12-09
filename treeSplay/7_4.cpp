#include <iostream>

using namespace std;

template<class T>
struct node 
{
	T element;
	int height;
	int nodesCount;
	node *left;
	node *right;
	node() = default;
	node(const T& element) : element(element), height(1), nodesCount(1), left(nullptr), right(nullptr) {};
};

template<class T>
class tree 
{
public:
	tree() : root(nullptr) {};
	~tree();
	void Add(const T &element);
	void Delete(const T &element);
	void deleteAll(node<T> *x);
	int getheight(const node<T> *x) const { return x != nullptr ? x->height : 0; }
	int balance(const node<T> *x) const { return getheight(x->right) - getheight(x->left); }
	void fixHeight(node<T> *x);
	void fixnodesCount(node<T> *x);
	T statistic(const int &kstatistic) const;
private:
	node<T> *root;
	node<T> *rotate_left(node<T> *x);
	node<T> *rotate_right(node<T> *x);
	node<T> *checkBalance(node<T> *x);
	node<T> *Insert(node<T> *x, const T &element);
	node<T> *findAndRemove(node<T> *x, node<T> *&prenode, const bool isLeft);
	node<T> *Remove(node<T> *x, const T &element);
};

template<class T>
tree<T>::~tree() 
{
	deleteAll(root);
}

template<class T>
void tree<T>::deleteAll(node<T> *x)
{
	while (!x)
	{
		deleteAll(x->left);
		deleteAll(x->right);
		delete x;
	}
}

template<class T>
void tree<T>::fixHeight(node<T> *x) 
{
	int heightLeft = getheight(x->left);
	int heightRight = getheight(x->right);
	x->height = (heightLeft > heightRight ? heightLeft : heightRight) + 1;
}

template<class T>
void tree<T>::fixnodesCount(node<T> *x)
{
	x->nodesCount = 1;
	if (x->left != nullptr) x->nodesCount += x->left->nodesCount;
	if (x->right != nullptr) x->nodesCount += x->right->nodesCount;
}

template<class T>
node<T> *tree<T>::rotate_left(node<T> *x)
{
	node<T> *node = x->right;
	x->right = node->left;
	node->left = x;
	fixHeight(x);
	fixnodesCount(x);
	fixHeight(node);
	fixnodesCount(node);
	return node;
}

template<class T>
node<T> *tree<T>::rotate_right(node<T> *x) 
{
	node<T> *node = x->left;
	x->left = node->right;
	node->right = x;
	fixHeight(x);
	fixnodesCount(x);
	fixHeight(node);
	fixnodesCount(node);
	return node;
}

template<class T>
node<T> *tree<T>::checkBalance(node<T> *x) 
{
	fixHeight(x);
	fixnodesCount(x);
	if (balance(x) == 2) 
	{
		if (balance(x->right) < 0)
			x->right = rotate_right(x->right);
		return rotate_left(x);
	}
	if (balance(x) == -2) 
	{
		if (balance(x->left) > 0)
			x->left = rotate_left(x->left);
		return rotate_right(x);
	}
	return x;
}

template<class T>
node<T> *tree<T>::Insert(node<T> *x, const T &element)
{
	if (x == nullptr)
		return new node<T>(element);
	if (element < x->element)
		x->left = Insert(x->left, element);
	else
		x->right = Insert(x->right, element);
	return checkBalance(x);
}

template<class T>
void tree<T>::Add(const T &element)
{
	root = Insert(root, element);
}

template<class T>
node<T> *tree<T>::findAndRemove(node<T> *x, node<T> *&prenode, const bool isLeft) {
	if (isLeft) 
	{
		if (x->right == nullptr) 
		{
			prenode = x;
			return x->left;
		}
		x->right = findAndRemove(x->right, prenode, isLeft);
		return checkBalance(x);
	}
	else 
	{
		if (x->left == nullptr) 
		{
			prenode = x;
			return x->right;
		}
		x->left = findAndRemove(x->left, prenode, isLeft);
		return checkBalance(x);
	}
}

template<class T>
node<T> *tree<T>::Remove(node<T> *x, const T &element) {
	if (x == nullptr) return nullptr;
	if (element < x->element) x->left = Remove(x->left, element);
	else if (element > x->element) x->right = Remove(x->right, element);
	else 
	{
		node<T> *nodeLeft = x->left;
		node<T> *nodeRight = x->right;
		delete(x);
		if (nodeRight == nullptr) return nodeLeft;
		if (nodeLeft == nullptr) return nodeRight;
		node<T> *replacingnode = nullptr;
		bool goToLeftTree = false;
		if (nodeLeft->height > nodeRight->height) 
		{
			goToLeftTree = true;
			node<T> *leftTree = findAndRemove(nodeLeft, replacingnode, goToLeftTree);
			replacingnode->left = leftTree;
			replacingnode->right = nodeRight;
		}
		else 
		{
			node<T> *rightTree = findAndRemove(nodeRight, replacingnode, goToLeftTree);
			replacingnode->right = rightTree;
			replacingnode->left = nodeLeft;
		}
		return checkBalance(replacingnode);
	}
	return checkBalance(x);

}

template<class T>
void tree<T>::Delete(const T &element)
{
	root = Remove(root, element);
}

template<class T>
T tree<T>::statistic(const int &kstatistic) const 
{
	node<T> *currentnode = root;
	int position = kstatistic;
	while (currentnode != nullptr) 
	{
		int countLeft = (currentnode->left == nullptr ? 0 : currentnode->left->nodesCount);
		if (countLeft == position) return currentnode->element;
		if (countLeft > position) 
		{
			currentnode = currentnode->left;
		}
		else 
		{
			position -= countLeft;
			position--;
			currentnode = currentnode->right;
		}
	}
	return -1;
}

int main() 
{
	size_t n = 0;
	cin >> n;
	int *buffer = new int[n];
	tree<int> mytree;
	int a = 0, statistics = 0;
	for (size_t i = 0; i < n; i++) {
		cin >> a >> statistics;
		if (a < 0) mytree.Delete(abs(a));
		else mytree.Add(a);
		buffer[i] = mytree.statistic(statistics);
	}
	for (size_t i = 0; i < n; i++) cout << buffer[i] << endl;
	delete[] buffer;
	return 0;
}