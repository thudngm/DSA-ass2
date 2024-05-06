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

void kDTree::mergeSort(const vector<vector<int>> &pointList, int dimension)
{
  if (pointList.size() <= 1) {
    return;
  }

  int mid = pointList.size() / 2;
  vector<vector<int>> leftSide = leftSide<pointList.begin(), pointList.begin() + mid>;
  vector<vector<int>> rightSide = rightSide<pointList.begin() + mid + 1, pointList.end()>;

  mergeSort(leftSide, dimension);
  mergeSort(rightSide, dimension);

  merge(pointList, leftSide, rightSide, dimension);
}

void kDTree::merge(vector<vector<int>> &result, vector<int> &leftSide, vector<int> &rightSide, int dimension)
{
  int leftIndex = 0, rightIndex = 0, resultIndex = 0;

  while(leftIndex < leftSide.size() && rightIndex < rightSize.size()){
    if (leftSide[leftIndex][dimension] < rightSide[rightIndex][dimension]){
      result[resultIndex++] = leftSide[leftIndex++];
    }
    else{
      result[resultIndex++] = rightSide[rightIndex++];
    }
  }

  while(leftIndex < leftSide.size()){
    result[resultIndex++] = leftSide[leftIndex++];
  }

  while(rightIndex < rightSide.size()){
    result[resultIndex++] = rightSide[rightIndex++];
  }
}

kDTreeNode *kDTree::findPivot(const vector<vector<int>> &pointList)
{
  int size = pointList.size();
  if (size <= 0) return nullptr;
  int mid;
  if (size % 2 == 0){
    mid = size / 2 - 1;
  }
  else {
    mid = size / 2;
  }
  return new kDTreeNode(pointList[mid]);
}

int leafCountRec(kDTreeNode *root)
{
  if (!root)
    return 0;
  if (!root->left && !root->right)
    return 1;
  return leafCountRec(root->left) + leafCountRec(root->right);
}

kDTreeNode *kDTree::insertRec(kDTreeNode *root, const vector<int> &point, int depth)
{
  if(!this->root){
    return new kDTreeNode(point);
  }

  int axis = depth % point.size();
  if(root->data[axis] > point[axis]){
    root->left = insertRec(root->left, point, depth + 1);
  }
  else{
    root->right = insertRec(root->right, point, depth + 1);
  }
  return root;
}

void kDTree::insert(const vector<int> &point)
{
  if(point.size() != this->k){
    return;
  }

  root = insertRec(root, point, 0);
}

void kDTree::remove(const vector<int> &point)
{
}

kDTreeNode *kDTree::removeNode(kDTreeNode *node, const vector<int> &point, int depth)
{
  
}

bool kDTree::search(const vector<int> &point)
{
  return false;
}

void kDTree::buildTree(const vector<vector<int>> &pointList)
{
  if (pointList.empty()){
    root = nullptr;
    return;
  }
  root = buildTreeRec(pointList, 0);
}

void kDTree::buildTreeRec(const vector<vector<int>> &pointList, int depth)
{
  int k = pointList[0].size();
  int axis = depth % k;

  mergeSort(pointList, axis);
  int midIndex = findPivot(pointList);

  insert(pointList[midIndex]);
  
  vector<vector<int>> leftList = leftList<pointList.begin(), pointList.begin() + midIndex - 1>;
  vector<vector<int>> rightList = rightList<pointList.begin() + minIndex + 1, pointList.end()>;

  depth++;
  int nextAxios = depth % k;
  buildTreeRec(leftList, depth);
  buildTreeRec(rightList, depth);
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
