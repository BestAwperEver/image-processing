#ifndef AVLTREE_H
#define AVLTREE_H

/*
 *	ПЕРЕД РАБОТОЙ В НОРМАЛЬНОМ РЕЖИМЕ УДАЛИТЬ СТРОКИ ПО ТЕГУ
 *	"для домашки по малининой"
 */

template <class T> struct AVLNode {
	enum balance {
		d2left = -2,
		dleft,
		bal,
		dright,
		d2right
	};
	balance status;
	AVLNode *left, *right;
	T data;
	AVLNode(const T& Data = T()): status(bal), data(Data), left(0), right(0) {}
};
template <class T> class AVLTree {
	AVLNode<T> *m_root;
	int m_size, m_height;
	int (*cmp)(const T&, const T&);
public:
	AVLTree(int Compare_Function(const T&, const T&));
	~AVLTree();

	int size() const {return m_size;}
	int height() const {return m_height;}
	void print() const;
	void traverse(void some_doing(const T&)) const;	
	// для домашки по малининой
	void add_xy(bool x_is_the_first_el);

	AVLTree& add_item(const T&/*, int cmp(const T&,const T&)*/);

	AVLTree& operator << (const T&);
private:
	void print_subtree(AVLNode<T>*, int n) const;
	int height_subtree(AVLNode<T>*) const;
	void traverse_subtree(void some_doing(const T&), AVLNode<T>* subtree) const;
	// для домашки по малининой
	void add_xy(AVLNode<T>* subtree, bool& x_is_the_first_el);

	void SingleRightRotation(AVLNode<T>*&);
	void SingleLeftRotation(AVLNode<T>*&);
	void add_item_subtree(const T&, /*int cmp(const T&,const T&),*/ AVLNode<T>*&);
	void delete_subtree(AVLNode<T>*);
	void set_status(AVLNode<T>*);
};

template <class T> AVLTree<T>::AVLTree(int c(const T&, const T&)): cmp(c), m_root(0), m_size(0), m_height(0) { }

template <class T> int AVLTree<T>::height_subtree(AVLNode<T>* subtree) const {
	if (!subtree) return 0;
	else return 1 + (height_subtree(subtree->left)>height_subtree(subtree->right)?
		height_subtree(subtree->left):height_subtree(subtree->right));
}
template <class T> void AVLTree<T>::SingleRightRotation(AVLNode<T>*& subtree) {
	AVLNode<T>* LR = subtree->left->right, *L = subtree->left;
	subtree->left->right = subtree;
	subtree->left = LR;
	subtree = L;
}
template <class T> void AVLTree<T>::SingleLeftRotation(AVLNode<T>*& subtree) {
	AVLNode<T>* RL = subtree->right->left, *R = subtree->right;
	subtree->right->left = subtree;
	subtree->right = RL;
	subtree = R;
}
template <class T> void AVLTree<T>::set_status(AVLNode<T>* subtree) {
	subtree->status = (AVLNode<T>::balance)(height_subtree(subtree->right)-height_subtree(subtree->left));
}
template <class T> AVLTree<T>& AVLTree<T>::add_item(const T& data/*, int cmp(const T&,const T&)*/) {
	if (!m_root) {
		m_root = new AVLNode<T>(data);
		m_size = 1;
		m_height = 1;
		return *this;
	}
	add_item_subtree(data,/*cmp,*/m_root);
	m_size++;
	m_height = height_subtree(m_root);
	return *this;
}
template <class T> void AVLTree<T>::add_item_subtree(const T& data/*, int cmp(const T&,const T&)*/, AVLNode<T>*& subtree) {
	if (cmp(data,subtree->data)<0) {
		if (!subtree->left) 
			subtree->left = new AVLNode<T>(data);
		else 
			add_item_subtree(data,/*cmp,*/subtree->left);
	}
	else {
		if (!subtree->right) 
			subtree->right = new AVLNode<T>(data);
		else 
			add_item_subtree(data/*,cmp*/,subtree->right);
	}

	set_status(subtree);

	if (subtree->status==AVLNode<T>::d2left)
		if (subtree->left->status==AVLNode<T>::dleft) {
//			SingleRightRotation(subtree->left);
			SingleRightRotation(subtree);
			set_status(subtree->right);
			set_status(subtree);
		}
		else {
			SingleLeftRotation(subtree->left);
			SingleRightRotation(subtree);
			set_status(subtree->right);
			set_status(subtree->left);
			set_status(subtree);
		}
	else if (subtree->status==AVLNode<T>::d2right)
		if (subtree->right->status==AVLNode<T>::dright) {
//			SingleLeftRotation(subtree->right);
			SingleLeftRotation(subtree);
			set_status(subtree->left);
			set_status(subtree);
		}
		else {
			SingleRightRotation(subtree->right);
			SingleLeftRotation(subtree);
			set_status(subtree->right);
			set_status(subtree->left);
			set_status(subtree);
		}
}
template <class T> AVLTree<T>::~AVLTree() {
	if (!m_size) return;
	delete_subtree(m_root);
}
template <class T> void AVLTree<T>::delete_subtree(AVLNode<T>* subtree) {
	if (subtree->left) delete_subtree(subtree->left);
	if (subtree->right) delete_subtree(subtree->right);
	delete subtree;
}

template <class T> void AVLTree<T>::print() const {
	if (!m_root) return;
	print_subtree(m_root,0);
}
template <class T> void AVLTree<T>::traverse(void some_doing(const T&)) const {
	if (!m_root) return;
	traverse_subtree(some_doing,m_root);
}

template <class T> void AVLTree<T>::print_subtree(AVLNode<T>* subtree, int n) const {
	if (subtree->right)	print_subtree(subtree->right, n+1);
	for (int i=0; i<n; i++) 
		cout<<'\t';
	// для домашки по малининой
	if (subtree->data == -1) cout << 'x' << endl;
	else if (subtree->data == -2) cout << 'y' << endl;
	else cout<<subtree->data<<endl;

	if (subtree->left) print_subtree(subtree->left, n+1);
}
template <class T> void AVLTree<T>::traverse_subtree(void some_doing(const T&), AVLNode<T>* subtree) const {
	if (subtree->left) traverse_subtree(some_doing, subtree->left);
	some_doing(subtree->data);
	if (subtree->right) traverse_subtree(some_doing, subtree->right);
}
// для домашки по малининой
template <class T> void AVLTree<T>::add_xy(bool x_is_the_first_el) {
	if (!m_root) return;
	add_xy(m_root, x_is_the_first_el);
}
// для домашки по малининой
template <class T> void AVLTree<T>::add_xy(AVLNode<T>* subtree, bool& x_is_the_first_el) {
	if (subtree->left) add_xy(subtree->left, x_is_the_first_el);
	else {
		subtree->left = new AVLNode<T>(x_is_the_first_el ? -1 : -2);
		x_is_the_first_el = !x_is_the_first_el;
	}
	if (subtree->right) add_xy(subtree->right, x_is_the_first_el);
	else {
		subtree->right = new AVLNode<T>(x_is_the_first_el ? -1 : -2);
		x_is_the_first_el = !x_is_the_first_el;
	}
}
template <class T> AVLTree<T>& AVLTree<T>::operator<<(const T& data) {
	return add_item(data);
}

#endif