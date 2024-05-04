#include "Dataset.hpp"

Dataset::Dataset()
{
}

Dataset::~Dataset()
{
}

bool Dataset::loadFromCSV(const char *fileName)
{
  return false;
}

void Dataset::printHead(int nRows, int nCols) const
{
}

void Dataset::printTail(int nRows, int nCols) const
{
}

void Dataset::getShape(int &nRows, int &nCols) const
{
}

void Dataset::columns() const
{
}

bool Dataset::drop(int axis, int index, std::string columns)
{
  return false;
}

Dataset Dataset::extract(int startRow, int endRow, int startCol, int endCol) const
{
  return Dataset();
}

Dataset::Dataset(const Dataset &other)
{
}

Dataset &Dataset::operator=(const Dataset &other)
{
  // TODO: insert return statement here
}

void train_test_split(Dataset &X, Dataset &y, double test_size, Dataset &X_train, Dataset &X_test, Dataset &y_train, Dataset &y_test)
{
}
