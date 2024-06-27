#include "kDTree.hpp"

// kDTreeNode
kDTreeNode::kDTreeNode(vector<int> data, kDTreeNode *left, kDTreeNode *right)
{
    this->data = data;
    this->left = left;
    this->right = right;
};

void kDTreeNode::printData()
{
    cout << "(";
    int idx = 0;
    for (const auto &i : this->data)
    {
        if (idx == 0)
            cout << i;
        else
            cout << ", " << i;
        idx++;
    }
    cout << ") ";
};

// kDTree
kDTree::kDTree(int k)
{
    this->k = k;
    this->root = nullptr;
};

kDTree::kDTree(const kDTree &other)
{
    this->operator=(other);
};

const kDTree &kDTree::operator=(const kDTree &other)
{
    if (this != &other)
    {
        this->copyTree(this->root, other.root);
    }
    return *this;
}

void kDTree::copyTree(kDTreeNode *&dest, const kDTreeNode *src)
{
    if (src == nullptr)
    {
        dest = nullptr;
    }
    else
    {
        this->root = new kDTreeNode(src->data);
        this->copyTree(dest->left, src->left);
        this->copyTree(dest->right, src->right);
    }
}

kDTree::~kDTree()
{
    this->clearTree(this->root);
};

void kDTree::inorderTraversal() const
{
    this->printInorder(this->root);
};

void kDTree::printInorder(struct kDTreeNode *node) const
{
    if (node == nullptr)
        return;

    this->printInorder(node->left);
    node->printData();
    this->printInorder(node->right);
};

void kDTree::preorderTraversal() const
{
    this->printPreorder(this->root);
};

void kDTree::printPreorder(struct kDTreeNode *node) const
{
    if (node == nullptr)
        return;

    node->printData();
    this->printPreorder(node->left);
    this->printPreorder(node->right);
};

void kDTree::postorderTraversal() const
{
    this->printPostorder(this->root);
};

void kDTree::printPostorder(struct kDTreeNode *node) const
{
    if (node == nullptr)
        return;

    this->printPostorder(node->left);
    this->printPostorder(node->right);
    node->printData();
};

int kDTree::height() const
{
    return this->maxHeight(this->root);
};

int kDTree::maxHeight(struct kDTreeNode *node) const
{
    int leftHeight = this->maxHeight(this->root->left);
    int rightHeight = this->maxHeight(this->root->right);
    return max(leftHeight, rightHeight) + 1;
};

int kDTree::nodeCount() const
{
    return this->getNodeCount(this->root);
};

int kDTree::getNodeCount(struct kDTreeNode *node) const
{
    if (node == nullptr)
        return 0;

    return 1 + this->getNodeCount(node->left) + this->getNodeCount(node->right);
};

int kDTree::leafCount() const
{
    return this->getLeafNodeCount(this->root);
};

int kDTree::getLeafNodeCount(struct kDTreeNode *node) const
{
    if (node == nullptr)
        return 0;

    if (node->left == nullptr && node->right == nullptr)
        return 1;
    else
        return this->getLeafNodeCount(node->left) + this->getLeafNodeCount(node->right);
};

void kDTree::insert(const vector<int> &point)
{
    if (point.size() != this->k)
        return;

    this->root = this->insertRecursive(this->root, point, 0);
};

struct kDTreeNode *kDTree::insertRecursive(struct kDTreeNode *node, const vector<int> &point, int depth)
{
    if (depth == this->k)
        depth = 0;

    if (node == nullptr)
        return new kDTreeNode(point);

    if (point.at(depth) < node->data.at(depth))
    {
        node->left = this->insertRecursive(node->left, point, depth + 1);
    }
    else
    {
        node->right = this->insertRecursive(node->right, point, depth + 1);
    }

    return node;
};

void kDTree::remove(const vector<int> &point)
{
    this->removeRecursive(this->root, point, 0);
};

struct kDTreeNode *kDTree::removeRecursive(struct kDTreeNode *node, const vector<int> &point, int depth)
{
    if (depth == this->k)
        depth = 0;

    if (node == nullptr)
        return node;

    if (point.at(depth) < node->data.at(depth))
    {
        node->left = this->removeRecursive(node->left, point, depth + 1);
    }
    else if (point.at(depth) > node->data.at(depth))
    {
        node->right = this->removeRecursive(node->right, point, depth + 1);
    }

    if (node->left == nullptr && node->right == nullptr)
    {
        delete node;
        return nullptr;
    }
    else if (node->right != nullptr)
    {
        struct kDTreeNode *temp = this->findMinValueNode(node->right, depth + 1, depth);
        node->data = temp->data;
        node->right = this->removeRecursive(node->right, temp->data, depth + 1);
        return node;
    }
    else
    {
        struct kDTreeNode *temp = this->findMinValueNode(node->left, depth + 1, depth);
        node->data = temp->data;
        node->right = node->left;
        node->left = nullptr;
        node->right = this->removeRecursive(node->right, temp->data, depth + 1);
        return node;
    }
};

struct kDTreeNode *kDTree::findMinValueNode(struct kDTreeNode *node, int depth, int dimen)
{
    if (depth == this->k)
        depth = 0;

    if (node == nullptr || (node->left == nullptr && node->right == nullptr))
        return node;

    if (depth == dimen)
    {
        if (node->left == nullptr)
            return node;
        else
            return this->findMinValueNode(node->left, depth + 1, dimen);
    }
    else
    {
        struct kDTreeNode *minLeftNode = this->findMinValueNode(node->left, depth + 1, dimen);
        struct kDTreeNode *minRightNode = this->findMinValueNode(node->right, depth + 1, dimen);
        return this->minNode(node, this->minNode(minLeftNode, minRightNode, dimen), dimen);
    }
};

struct kDTreeNode *kDTree::minNode(struct kDTreeNode *node_a, struct kDTreeNode *node_b, int dimen)
{
    if (node_a != nullptr && node_b != nullptr)
    {
        if (node_a->data.at(dimen) <= node_b->data.at(dimen))
            return node_a;
        else
            return node_b;
    }
    else if (node_a == nullptr)
        return node_b;
    else
        return node_a;
};

void kDTree::buildTree(const vector<vector<int>> &pointList)
{
    kDTreeNode *node = this->root;
    vector<vector<int>> points = pointList;
    int left = 0;
    int right = pointList.size() - 1;
    int depth = 0;
    this->root = this->buildTreeRecursive(this->root, points, 0, pointList.size() - 1, 0);
};

struct kDTreeNode *kDTree::buildTreeRecursive(struct kDTreeNode *node, vector<vector<int>> &pointList, int left, int right, int depth)
{
    if (left > right)
        return nullptr;

    // this->mergeSort(pointList, left, right, depth);
    // int mid = (left + right) / 2;

    // node = new kDTreeNode(pointList.at(mid));
    // node->left = buildTreeRecursive(node->left, pointList, left, mid - 1, depth + 1);
    // node->right = buildTreeRecursive(node->right, pointList, mid + 1, right, depth + 1);
    // return node;

    int dim = depth % k;
    int mid = (left + right) / 2;

    // Sort the points along the current dimension
    this->mergeSort(pointList, left, right, dim);

    node = new kDTreeNode(pointList[mid]);
    node->left = buildTreeRecursive(node->left, pointList, left, mid - 1, depth + 1);
    node->right = buildTreeRecursive(node->right, pointList, mid + 1, right, depth + 1);
    return node;
};

bool kDTree::search(const vector<int> &point)
{
    kDTreeNode *node = this->searchRecursive(this->root, point, 0);
    return node != nullptr;
};

struct kDTreeNode *kDTree::searchRecursive(struct kDTreeNode *node, const vector<int> &point, int depth)
{
    // if (node == nullptr || node->data.at(depth) == point.at(depth))
    //     return node;

    // if (point.at(depth) < node->data.at(depth))
    //     return this->searchRecursive(node->left, point, depth + 1);
    // else
    //     return this->searchRecursive(node->right, point, depth + 1);
    if (node == nullptr)
        return nullptr;

    if (node->data == point)
        return node;

    int cd = depth % k;
    if (point[cd] < node->data[cd])
        return this->searchRecursive(node->left, point, depth + 1);
    else
        return this->searchRecursive(node->right, point, depth + 1);
};

void kDTree::clearTree(struct kDTreeNode *node)
{
    if (node == nullptr)
        return;

    this->clearTree(node->left);
    this->clearTree(node->right);
    delete node;
};

void kDTree::nearestNeighbour(const vector<int> &target, kDTreeNode *&best)
{
    this->nearestNeighbourRecursive(this->root, target, best, 0);
};

void kDTree::nearestNeighbourRecursive(struct kDTreeNode *node, const vector<int> &target, kDTreeNode *&best, int depth)
{
    if (depth == this->k)
        depth = 0;

    if (node == nullptr)
        return;

    if (
        (node->left == nullptr && node->right == nullptr) ||
        (node->data.at(depth) > target.at(depth) && node->left == nullptr) ||
        (node->data.at(depth) < target.at(depth) && node->right == nullptr))
    {
        if (best == nullptr)
        {
            best = node;
            return;
        }
        else
        {
            int currentDistance = 0, newDistance = 0;
            for (int i = 0; i < target.size(); i++)
            {
                currentDistance += pow(best->data.at(i) - target.at(i), 2);
                newDistance += pow(node->data.at(i) - target.at(i), 2);
            }
            if (sqrt(newDistance) < sqrt(currentDistance))
                best = node;
            return;
        }
    }

    if (node->data.at(depth) > target.at(depth))
        this->nearestNeighbourRecursive(node->left, target, best, depth + 1);
    else
        this->nearestNeighbourRecursive(node->right, target, best, depth + 1);

    int R = 0, r = 0;
    for (int i = 0; i < target.size(); i++)
    {
        R += pow(best->data.at(i) - target.at(i), 2);
    }
    R = sqrt(R);
    r = abs(node->data.at(depth) - target.at(depth));
    if (R >= r)
    {
        // Find in opposite branch of node
        if (node->data.at(depth) > target.at(depth))
            this->nearestNeighbourRecursive(node->right, target, best, depth + 1);
        else
            this->nearestNeighbourRecursive(node->left, target, best, depth + 1);
    }
};

void kDTree::kNearestNeighbour(const vector<int> &target, int k, vector<kDTreeNode *> &bestList)
{
    for (int i = 0; i < k; i++)
    {
        kDTreeNode *best = nullptr;
        this->nearestNeighbour(target, best);
        bestList.push_back(best);
        this->remove(best->data);
    }
};

void kDTree::merge(vector<vector<int>> &vec, int left, int mid, int right, int dimen)
{
    int leftSize = mid - left + 1;
    int rightSize = right - mid;

    vector<vector<int>> newLeft, newRight;
    for (int i = 0; i < leftSize; i++)
    {
        newLeft.push_back(vec.at(left + i));
    }
    for (int j = 0; j < rightSize; j++)
    {
        newRight.push_back(vec.at(mid + 1 + j));
    }

    int i = 0, j = 0, k = left;
    while (i < leftSize && j < rightSize)
    {
        if (newLeft.at(i).at(dimen) <= newRight.at(j).at(dimen))
        {
            vec.at(k) = newLeft.at(i);
            i++;
        }
        else
        {
            vec.at(k) = newRight.at(j);
            j++;
        }
        k++;
    }

    while (i < leftSize)
    {
        vec.at(k) = newLeft.at(i);
        i++;
        k++;
    }

    while (j < rightSize)
    {
        vec.at(k) = newRight.at(j);
        j++;
        k++;
    }
};

void kDTree::mergeSort(vector<vector<int>> &vec, int left, int right, int dimen)
{
    if (dimen == this->k)
        dimen = 0;

    if (left >= right)
        return;

    int mid = (left + right) / 2;
    mergeSort(vec, left, mid, dimen);
    mergeSort(vec, mid + 1, right, dimen);
    merge(vec, left, mid, right, dimen);
};

// Checking
void kDTree::print2DUtil(kDTreeNode *node, int space)
{
    if (node == NULL)
        return;

    space += 10;

    this->print2DUtil(node->right, space);

    cout << endl;
    for (int i = 10; i < space; i++)
        cout << " ";
    node->printData();

    print2DUtil(node->left, space);
}

void kDTree::print2D()
{
    print2DUtil(this->root, 0);
}

kNN::kNN(int k)
{
    this->k = k;
}

// void kNN::fit(Dataset &X_train, Dataset &y_train)
// {

// }

// void kNN::convertDatasetToTree(Dataset &dataset, kDTree &train_Tree){
//     vector<vector<int>> train_vector;

//     list<list<int>> train_data = dataset.data;

//     for (const auto& innerList : train_data) {
//         train_vector.emplace_back(innerList.begin(), innerList.end());
//     }

//     train_Tree.buildTree(train_vector);
// }

// void kNN::convertTreeToDataset(Dataset &dataset, kDTree &train_Tree)
// {

// }

// Dataset kNN::predict(Dataset &X_test)
// {
//     list<list<int>> X_test_data = X_test.data;
//     int nRow_X_test = X_test_data.size();
//     auto it_test = X_test_data.begin();

//     list<list<int>> X_train_data = this->X_train.data;
//     list<list<int>> y_train_data = this->y_train.data;
//     int nRow_X_train = X_train_data.size();

// }

// double kNN::score(const Dataset &y_test, const Dataset &y_pred)
// {
//     list<list<int>> y_test_data = y_test.data;
//     list<list<int>> y_pred_data = y_pred.data;
//     int correctPredict = 0;
//     int nRow = y_test_data.size();
//     auto it_test = y_test_data.begin();
//     auto it_pred = y_pred_data.begin();
//     while(it_test != y_test_data.end() && it_pred != y_pred_data.end()){
//         if(it_test->front() == it_pred->front()){
//             correctPredict++;
//         }
//         it_test++;
//         it_pred++;
//     }

//     double score = (double)correctPredict / (double)nRow;
//     return score;
// }
void kNN::fit(Dataset &X_train, Dataset &y_train)
{
    return;
}

Dataset kNN::predict(Dataset &X_test)
{
    return X_test;
}

double kNN::score(const Dataset &y_test, const Dataset &y_pred)
{
    return 0;
}