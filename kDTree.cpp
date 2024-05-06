#include "kDTree.hpp"


// Helper functions
void merge(vector<vector<int>>& vec, int left, int mid, int right, int dimen) {
    int leftSize = mid - left + 1;
    int rightSize = right - mid;

    vector<vector<int>> newLeft, newRight;
    for (int i = 0; i < leftSize; i++) {
        newLeft.push_back(vec.at(left + i));
    }
    for (int j = 0; j < rightSize; j++) {
        newRight.push_back(vec.at(mid + 1 + j));
    }

    int i = 0, j = 0, k = left;
    while (i < leftSize && j < rightSize) {
        if (newLeft.at(i).at(dimen) <= newRight.at(j).at(dimen)) {
            vec.at(k) = newLeft.at(i);
            i++;
        }
        else {
            vec.at(k) = newRight.at(j);
            j++;
        }
        k++;
    }

    while (i < leftSize) {
        vec.at(k) = newLeft.at(i);
        i++;
        k++;
    }

    while (j < rightSize) {
        vec.at(k) = newRight.at(j);
        j++;
        k++;
    }
};

void mergeSort(vector<vector<int>>& vec, int left, int right, int dimen) {
    if (dimen == 2)
        dimen = 0;

    if (left >= right)
        return;

    int mid = (left + right) / 2;
    mergeSort(vec, left, mid, dimen);
    mergeSort(vec, mid + 1, right, dimen);
    merge(vec, left, mid, right, dimen);
};

// kDTreeNode
kDTreeNode::kDTreeNode(vector<int> data, kDTreeNode *left, kDTreeNode *right)
{
    this->data = data;
    this->left = left;
    this->right = right;
};

void kDTreeNode::printData() {
    cout << "(";
    int idx = 0;
    for (const auto &i : this->data) {
        if (idx == 0)
            cout << i;
        else
            cout << " " << i;
        idx++;
    }
    cout << ")" << endl;
};


// kDTree
kDTree::kDTree(int k) {
    this->k = k;
};


kDTree::~kDTree() {
    this->clearTree(this->root);
};


void kDTree::inorderTraversal() const {
    this->printInorder(this->root);
};


void kDTree::printInorder(struct kDTreeNode* node) const {
    if (node == nullptr)
        return;


    this->printInorder(node->left);
    node->printData();
    this->printInorder(node->right);
};


void kDTree::preorderTraversal() const {
    this->printPreorder(this->root);
};


void kDTree::printPreorder(struct kDTreeNode* node) const {
    if (node == nullptr)
        return;


    node->printData();
    this->printPreorder(node->left);
    this->printPreorder(node->right);
};


void kDTree::postorderTraversal() const {
    this->printPostorder(this->root);
};


void kDTree::printPostorder(struct kDTreeNode* node) const {
    if (node == nullptr)
        return;


    this->printPostorder(node->left);
    this->printPostorder(node->right);
    node->printData();
};


int kDTree::height() const {
    return this->maxHeight(this->root);
};


int kDTree::maxHeight(struct kDTreeNode* node) const {
    int leftHeight = this->maxHeight(this->root->left);
    int rightHeight = this->maxHeight(this->root->right);
    return max(leftHeight, rightHeight) + 1;
};


int kDTree::nodeCount() const {
    return this->getNodeCount(this->root);
};


int kDTree::getNodeCount(struct kDTreeNode* node) const {
    if (node == nullptr)
        return 0;

    return 1 + this->getNodeCount(node->left) + this->getNodeCount(node->right);
};


int kDTree::leafCount() const {
    return this->getLeafNodeCount(this->root);
};


int kDTree::getLeafNodeCount(struct kDTreeNode* node) const {
    if (node == nullptr)
        return 0;


    if (node->left == nullptr && node->right == nullptr)
        return 1;
    else
        return this->getLeafNodeCount(node->left) + this->getLeafNodeCount(node->right);
};


void kDTree::insert(const vector<int> &point) {
    if (point.size() != this->k)
        return;

    this->insertRecursive(this->root, point, 0);
};


struct kDTreeNode* kDTree::insertRecursive(struct kDTreeNode* node, const vector<int> &point, int depth) {
    if (node == nullptr)
        return new kDTreeNode(point);

    if (node->data.at(depth) < point.at(depth)) {
        node->left = this->insertRecursive(root->left, point, depth + 1);
    }
    else {
        node->right = this->insertRecursive(root->right, point, depth + 1);
    }

    return node;
};


void kDTree::buildTree(const vector<vector<int>> &pointList) {
    kDTreeNode* node = this->root;
    vector<vector<int>> points = pointList;
    int left = 0;
    int right = pointList.size() - 1;
    int depth = 0;
    this->root = this->buildTreeRecursive(this->root, points, 0, pointList.size() - 1, 0);
};


struct kDTreeNode* kDTree::buildTreeRecursive(struct kDTreeNode* node, vector<vector<int>> &pointList, int left, int right, int depth) {
    if (left > right)
        return nullptr;

    mergeSort(pointList, left, right, depth);
    int mid = (left + right) / 2;

    node = new kDTreeNode(pointList.at(mid));
    node->left = buildTreeRecursive(node->left, pointList, left, mid - 1, depth + 1);
    node->right = buildTreeRecursive(node->right, pointList, mid + 1, right, depth + 1);
    return node;
};


bool kDTree::search(const vector<int> &point) {
    kDTreeNode* node = this->searchRecursive(this->root, point, 0);
    return !node;
};


struct kDTreeNode* kDTree::searchRecursive(struct kDTreeNode* node, const vector<int> &point, int depth) {
    if (node == NULL || node->data.at(depth) == point.at(depth))
        return node;

    if (node->data.at(depth) > point.at(depth))
        return this->searchRecursive(node->left, point, depth + 1);
    else
        return this->searchRecursive(node->right, point, depth + 1);
};


void kDTree::clearTree(struct kDTreeNode* node) {
    if (node == nullptr)
        return;

    this->clearTree(node->left);
    this->clearTree(node->right);
    delete node;
};

// Checking
void kDTree::print2DUtil(kDTreeNode* node, int space)
{
    // Base case
    if (node == NULL)
        return;
 
    // Increase distance between levels
    space += 10;
 
    // Process right child first
    this->print2DUtil(node->right, space);
 
    // Print current node after space
    // count
    cout << endl;
    for (int i = 10; i < space; i++)
        cout << " ";
    node->printData();
 
    // Process left child
    print2DUtil(node->left, space);
}

void kDTree::print2D()
{
    // Pass initial space count as 0
    print2DUtil(this->root, 0);
}
