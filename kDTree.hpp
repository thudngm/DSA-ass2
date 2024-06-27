#include "main.hpp"
#include "Dataset.hpp"

struct kDTreeNode
{
    vector<int> data;
    kDTreeNode *left;
    kDTreeNode *right;
    kDTreeNode(vector<int> data, kDTreeNode *left = nullptr, kDTreeNode *right = nullptr);
    void printData();

    friend ostream &operator<<(ostream &os, const kDTreeNode &node)
    {
        os << "(";
        for (int i = 0; i < node.data.size(); i++)
        {
            os << node.data[i];
            if (i != node.data.size() - 1)
            {
                os << ", ";
            }
        }
        os << ")";
        return os;
    }
};

class kDTree
{
private:
    int k;
    kDTreeNode *root;

public:
    kDTree(int k = 2);
    ~kDTree();

    const kDTree &operator=(const kDTree &other);
    kDTree(const kDTree &other);

    void inorderTraversal() const;
    void preorderTraversal() const;
    void postorderTraversal() const;
    int height() const;
    int nodeCount() const;
    int leafCount() const;

    void insert(const vector<int> &point);
    void remove(const vector<int> &point);
    bool search(const vector<int> &point);
    void buildTree(const vector<vector<int>> &pointList);
    void nearestNeighbour(const vector<int> &target, kDTreeNode *&best);
    void kNearestNeighbour(const vector<int> &target, int k, vector<kDTreeNode *> &bestList);

    void printInorder(struct kDTreeNode *node) const;
    void printPreorder(struct kDTreeNode *node) const;
    void printPostorder(struct kDTreeNode *node) const;
    int maxHeight(struct kDTreeNode *node) const;
    int getNodeCount(struct kDTreeNode *node) const;
    int getLeafNodeCount(struct kDTreeNode *node) const;

    void copyTree(kDTreeNode *&dest, const kDTreeNode *src);
    struct kDTreeNode *insertRecursive(struct kDTreeNode *node, const vector<int> &point, int depth);
    struct kDTreeNode *removeRecursive(struct kDTreeNode *node, const vector<int> &point, int depth);
    struct kDTreeNode *searchRecursive(struct kDTreeNode *node, const vector<int> &point, int depth);
    struct kDTreeNode *buildTreeRecursive(struct kDTreeNode *node, vector<vector<int>> &pointList, int left, int right, int depth);
    void nearestNeighbourRecursive(struct kDTreeNode *node, const vector<int> &target, kDTreeNode *&best, int depth);
    struct kDTreeNode *findMinValueNode(struct kDTreeNode *node, int depth, int dimen);
    struct kDTreeNode *minNode(struct kDTreeNode *node_a, struct kDTreeNode *node_b, int dimen);
    void clearTree(struct kDTreeNode *node);

    void merge(vector<vector<int>> &vec, int left, int mid, int right, int dimen);
    void mergeSort(vector<vector<int>> &vec, int left, int right, int dimen);

    void print2D();
    void print2DUtil(kDTreeNode *node, int space);
};

class kNN
{
private:
    int k;
    Dataset X_train;
    Dataset y_train;
    int numClasses;

public:
    kNN(int k = 5);
    void fit(Dataset &X_train, Dataset &y_train);
    Dataset predict(Dataset &X_test);
    double score(const Dataset &y_test, const Dataset &y_pred);

    // void convertDatasetToTree(Dataset &dataset, kDTree &train_Tree);
    // void convertTreeToDataset(Dataset &dataset, kDTree &train_Tree);
};

// Please add more or modify as needed
