#include<iostream>
#include<string>

using namespace std;

template<class Type, int degree>
struct bTreeNode {
	int count;// count number of elements of the Node
	Type list[degree - 1];
	bTreeNode *children[degree] = { 0 };
};

template<class Type,int degree>
class bTree {
protected:
	bTreeNode<Type, degree> *root;
	int actualDe = 0;
public:
	bTree() {};
	bTree(int de) {
		actualDe = de;
	}
	void searchNode(bTreeNode<Type, degree> *current, Type item, bool &found, int &location){
		location = 0;
		while (location<current->count&&item>current->list[location])
			location++;
		if (location < current->count && item == current->list[location])
			found = true;
		else
			found = false;
	}
	bool search(Type& searchItem) {
		bool found = false;
		int location;
		bTreeNode<Type, degree> *current;
		current = root;
		while (current != 0; && !found) {
			searchNode(current, searchItem, found, location);
			if (!found)
				current = current->children[location];
		}
		return found;
	}
	void insert(Type& key) {
		bool isTaller = false;
		Type median;
		bTreeNode<Type, degree> *rightChild;
		insertBTree(root, key, median, rightChild, isTaller);
		if (isTaller) {// if the root needed to be split
			bTreeNode<Type, degree> *tempRoot;
			//newRoot will hold median value
			tempRoot = new bTreeNode<Type, degree>;
			tempRoot->count = 1;
			tempRoot->list[0] = median;
			tempRoot->children[0] = root;
			tempRoot->children[1] = rightChild;
			root = tempRoot;
		}
	}
	void insertAt(bTreeNode<Type, degree> *current, Type item, int index) {
		for (int i = 0; i < current->count - index; i++) {
			current->list[current->count - i] = current->list[current->count - i - 1];
		}
		current->list[index] = item;
		current->count = current->count + 1;
	}

	bool insertBTree(bTreeNode<Type, degree> *current, Type&item, Type&median,
		bTreeNode<Type, degree> *&rightChild, bool& isTaller) {//&
		if (current == nullptr) {
			//B tree is empty or search ends at an empty subtree
			median = item;
			rightChild = 0;
			isTaller = true;// will be able to add to an existing node, or split to new node
		}
		else {
			bool found;
			int index = 0;
			searchNode(current, item, found, index);
			if (found)
				cout << "item is already exist" << endl;
			else {
				
				insertBTree(current->children[index], item, median, rightChild, isTaller);
				if (isTaller) {
					if (current->count!=actualDe-1) {
						insertNode(current, item,rightChild,index);
						isTaller = false;
					}
					else {
						bTreeNode<Type, degree> *rightNode;
						splitNode(current, item, rightChild, index, rightNode, median);
						rightChild = rightNode;//corrected
						item = median;//corrected
						//insertBTree(cur)
					}
				}
			}
		}
		return isTaller;
	}

	void insertNode(bTreeNode<Type, degree> *current, Type& insertItem,
		bTreeNode<Type, degree> * &rightChild, int insertPosition) {//&
		int index;
		for (index = current->count;index > insertPosition;index--) {
			current->list[index] = current->list[index - 1];
			current->children[index + 1] = current->children[index];
		}
		current->list[index] = insertItem;
		current->children[index + 1] = rightChild;//rightChild a pointer to the right subtree of the item to be inserted
		current->count++;

	}

	void splitNode(bTreeNode<Type, degree>*current, Type &insertItem,
		bTreeNode<Type, degree> *rightChild, int insertPostion,
		bTreeNode<Type, degree> * &rightNode, Type &median) {//&
		rightNode = new bTreeNode<Type, degree>;
		int mid = (actualDe - 1) / 2; 
		if (insertPostion <= mid)// new item goes in the first half of the node
		{
			int index = 0;
			int i = mid;
			while (i < actualDe - 1) {
				rightNode->list[index] = current->list[i];
				rightNode->children[index + 1] = current->children[i + 1];
				index++;
				i++;
			}
			current->count = mid;
			insertNode(current, insertItem,rightChild, insertPostion);
			(current->count)--;
			median = current->list[current->count];
			rightNode->count = index;
			rightNode->children[0] = current->children[current->count + 1];
		}
		else {
			int i = mid + 1;
			int index = 0;
			while (i < actualDe - 1) {
				rightNode->list[index] = current->list[i];
				rightNode->children[index + 1] = current->children[i + 1];
				index++;
				i++;
			}
			current->count = mid;//left
			rightNode->count = index;//right
			median = current->list[mid];
			insertNode(rightNode, insertItem,rightChild, insertPostion - mid - 1);
			//set right subtree of the mid to be the left subtree of the new right node
			rightNode->children[0] = current->children[current->count + 1];
		}
	}
	void inOrder() {
		recInorder(root);
	}
	void recInorder(bTreeNode<Type, degree> *current) {
		if (current != 0) {
			recInorder(current->children[0]);
			for (int i = 0; i < current->count;i++) {
				cout << current->list[i] << " ";
				recInorder(current->children[i + 1]);
			}
		}
	}
	int getLevel() {
		bTreeNode<Type, degree> *cu = root;
		return level(cu);
	}
	
	void printLevel(int L) {
		bTreeNode<Type, degree> *current = root;

			print(current, 1, L);
		}

	private:
		int level(bTreeNode<Type, degree> *cu) {
			if (cu == 0) return 0;
			else {
				return 1 + level(cu->children[0]);
			}
		}
	void print(bTreeNode<Type, degree> *cu, int currentLevel, int L) {
		if (cu == 0) {
			return;
		}
		else if (currentLevel!= L) {
			for (int i = 0; i <= cu->count;i++) {
				print(cu->children[i], currentLevel + 1, L);
			}
		}
		else {
			for (int i = 0; i < cu->count;i++)
				cout << cu->list[i] << " ";
		}
	}
};

int main() {

	const int degree = 1000;
	int de = 3;

	int a[21] = { 2,6,8,45,21,63,85,55,14,16,9,3,4,7,2,55,11,13,654,214,9};

	bTree<int,degree> tree(de);

	for (int i = 0; i < 21;i++) {
		tree.insert(a[i]);
	}
	tree.inOrder();
	cout << endl;
	cout << tree.getLevel();
	tree.printLevel(20);

	system("PAUSE");
	return 0;
}