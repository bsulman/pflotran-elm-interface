#include "Block.h"

Block::Block(int n) {
  size = n;
  A = new double*[size];
  for (int i=0; i<size; i++)
    A[i] = new double[size];
}

void Block::zero() {
  for (int i=0; i<size; i++)
    for (int j=0; j<size; j++)
      A[i][j] = 0.;
}

void Block::setDiagonal(double d) {
  for (int i=0; i<size; i++)
    A[i][i] = d;
}

int Block::getSize() {
  return size;
}

double **Block::getValues() {
  return A;
}

double Block::getRowAbsMax(int irow) {
  double max = 0.;
  for (int i=0; i<size; i++) {
    double value = fabs(A[irow][i]);
    if (value > max) max = value;
  }
  return max;
}

void Block::scaleRow(int irow, double scale) {
  for (int i=0; i<size; i++)
    A[irow][i] *= scale;
}

void Block::scaleColumn(int icol, double scale) {
  for (int i=0; i<size; i++)
    A[i][icol] *= scale;
}

void Block::scale(double scale) {
  for (int i=0; i<size; i++)
    for (int j=0; j<size; j++)
      A[i][j] *= scale;
}

void Block::setValue(int i, int j, double value) {
  A[i][j] = value;
}

void Block::setValues(double **values) {
  for (int i=0; i<size; i++)
    for (int j=0; j<size; j++)
      A[i][j] = values[i][j];
}

void Block::setValues(int ioffset, int joffset, Block *b) {
  int n_ = b->getSize();
  double **A_ = b->getValues();
  for (int i=0; i<size; i++)
    for (int j=0; j<size; j++)
      A[i+ioffset][j+joffset] = A_[i][j];
}

void Block::addValue(int i, int j, double value) {
  A[i][j] += value;
}

void Block::addValues(double **values) {
  for (int i=0; i<size; i++)
    for (int j=0; j<size; j++)
      A[i][j] += values[i][j];
}

void Block::addValues(int ioffset, int joffset, Block *b) {
  int n_ = b->getSize();
  double **A_ = b->getValues();
  for (int i=0; i<size; i++)
    for (int j=0; j<size; j++)
      A[i+ioffset][j+joffset] += A_[i][j];
}

void Block::print() {
  for (int i=0; i<size; i++)
    for (int j=0; j<size; j++)
      cout << i << " " << j << " : " << A[i][j] << endl;
}

Block::~Block() {
  if (A) {
    for (int i=1; i<size; i++)
      delete [] A[i];
    delete [] A;
  }
}
