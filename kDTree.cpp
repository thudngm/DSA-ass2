#include "kDTree.hpp"

/* TODO: You can implement methods, functions that support your data structures here.
 * */

kDTree::kDTree(int k)
{
  this->k = k;
  this->root = NULL;
}

kDTree::~kDTree()
{
  clear(this->root);
}

int kDTree::getkD() const
{
  return this->k;
}

void clear(kDTreeNode *root)
{
  if (root)
  {
    clear(root->left);
    clear(root->right);
    delete root;
  }
}

const kDTree &kDTree::operator=(const kDTree &other)
{
  if (this != &other)
  {
    clear(this->root);
    this->root = copyTree(other.root);
  }
  return *this;
}

kDTreeNode *copyTree(kDTreeNode *node)
{
  if (!node)
    return nullptr;

  return new kDTreeNode(node->data, copyTree(node->left), copyTree(node->right));
}

kDTree::kDTree(const kDTree &other)
{
  this->root = copyTree(other.root);
  this->k = other.k;
}

void kDTree::inorderTraversal() const
{
  inorderTraversalRec(root);
}

void inorderTraversalRec(kDTreeNode *root)
{
  if (!root)
    return;
  inorderTraversalRec(root->left);
  cout << "(";
  for (int i = 0; i < root->data.size(); i++)
  {
    cout << root->data[i];
    if (i < root->data.size() - 1)
    {
      cout << ", ";
    }
  }
  cout << ")";
  if (root->right || (root->left && !root->left->data.empty()))
  {
    cout << " ";
  }
  inorderTraversalRec(root->right);
}

void kDTree::preorderTraversal() const
{
  preorderTraversalRec(root);
}

void preorderTraversalRec(kDTreeNode *root)
{
  if (!root)
    return;
  cout << "(";
  for (int i = 0; i < root->data.size(); i++)
  {
    cout << root->data[i];
    if (i < root->data.size() - 1)
    {
      cout << ", ";
    }
  }
  cout << ")";
  if (root->right || root->left)
  {
    cout << " ";
  }
  preorderTraversalRec(root->left);
  preorderTraversalRec(root->right);
}

void kDTree::postorderTraversal() const
{
  postorderTraversalRec(root);
}

void postorderTraversalRec(kDTreeNode *root)
{
  if (!root)
    return;
  postorderTraversalRec(root->left);
  postorderTraversalRec(root->right);
  cout << "(";
  for (int i = 0; i < root->data.size(); i++)
  {
    cout << root->data[i];
    if (i < root->data.size() - 1)
    {
      cout << ", ";
    }
  }
  cout << ")";
  if (root->right || root->left)
  {
    cout << " ";
  }
}

int kDTree::height() const
{
  return heightRec(root);
}

int heightRec(kDTreeNode *root)
{
  if (!root)
    return 0;
  int leftHeight = heightRec(root->left);
  int rightHeight = heightRec(root->right);

  return max(leftHeight, rightHeight) + 1;
}

int kDTree::nodeCount() const
{
  return nodeCountRec(root);
}

int nodeCountRec(kDTreeNode *root)
{
  if (!root)
    return 0;
  return 1 + nodeCountRec(root->left) + nodeCountRec(root->right);
}

int kDTree::leafCount() const
{
  return leafCountRec(root);
}

int leafCountRec(kDTreeNode *root)
{
  if (!root)
    return 0;
  if (!root->left && !root->right)
    return 1;
  return leafCountRec(root->left) + leafCountRec(root->right);
}

void kDTree::insert(const vector<int> &point)
{
  if (point.size() != this->k)
  {
    return;
  }

  root = insertNode(root, point, 0);
}

kDTreeNode *kDTree::insertNode(kDTreeNode *node, const vector<int> &point, int depth)
{
  if (!node)
    return new kDTreeNode(point);

  int cd = depth % this->k;
  if (point[cd] < (node->data[cd]))
    node->left = insertNode(node->left, point, depth + 1);
  else
    node->right = insertNode(node->right, point, depth + 1);

  return node;
}

void kDTree::remove(const vector<int> &point)
{
  if (point.size() != this->k)
  {
    return;
  }
  root = removeNode(root, point, 0);
}

bool arePointsSame(const vector<int> &point1, const vector<int> &point2)
{
  if (point1.size() != point2.size())
  {
    return false;
  }

  for (int i = 0; i < point1.size(); i++)
  {
    if (point1[i] != point2[i])
    {
      return false;
    }
  }
  return true;
}

void copyPoint(vector<int> &point1, const vector<int> &point2)
{
  for (int i = 0; i < point1.size(); i++)
  {
    point1[i] = point2[i];
  }
}

kDTreeNode *minNode(kDTreeNode *x, kDTreeNode *y, kDTreeNode *z, int d)
{
  kDTreeNode *res = x;
  if (y != nullptr && y->data[d] < res->data[d])
  {
    res = y;
  }
  if (z != nullptr && z->data[d] < res->data[d])
  {
    res = z;
  }
  return res;
}

kDTreeNode *kDTree::findMinRec(kDTreeNode *root, int d, int depth)
{
  if (!root)
    return root;

  int cd = depth % this->k;

  if (cd == d)
  {
    if (root->left == nullptr)
    {
      return root;
    }
    return findMinRec(root->left, d, depth + 1);
  }

  return minNode(root, findMinRec(root->left, d, depth + 1), 
  findMinRec(root->right, d, depth + 1));
}

kDTreeNode* findMin(kDTreeNode *root, int d)
{
  return findMinRec(root, d, 0);
}

kDTreeNode *kDTree::removeNode(kDTreeNode *node, const vector<int> &point, int depth)
{
  if (!node)
    return nullptr;

  int cd = depth % this->k;

  // if the point to be delete is present at root
  if (arePointsSame(node->data, point))
  {
    // if right child is not null
    if (node->right != nullptr)
    {
      kDTreeNode *min = findMin(root->right, cd);
      copyPoint(node->data, min->data);
      node->right = removeNode(node->right, min->data, depth + 1);
    }
    // if left child is not null
    else if (node->left != nullptr)
    {
      kDTreeNode *min = findMin(node->left, cd);
      copyPoint(node->data, min->data);
      node->right = removeNode(node->left, min->data, depth + 1);
    }
    else
    {
      delete node;
      return NULL;
    }
    return node;
  }

  if (point[cd] < node->data[cd])
  {
    node->left = removeNode(node->left, point, depth + 1);
  }
  else
  {
    node->right = removeNode(node->right, point, depth + 1);
  }

  return node;
}

bool kDTree::search(const vector<int> &point)
{
  return false;
}

void kDTree::buildTree(const vector<vector<int>> &pointList)
{
}

void kDTree::nearestNeighbour(const vector<int> &target, kDTreeNode *best)
{
}

void kDTree::kNearestNeighbour(const vector<int> &target, int k, vector<kDTreeNode *> &bestList)
{
}

//-------------------------------

//-------------------------------

kNN::kNN(int k)
{
  this->k = k;
}

void kNN::fit(Dataset &X_train, Dataset &y_train)
{
  this->X_train = &X_train;
  this->y_train = &y_train;
}

Dataset kNN::predict(Dataset &X_test)
{
  return Dataset();
}

double kNN::score(const Dataset &y_test, const Dataset &y_pred)
{
  return 0.0;
}
