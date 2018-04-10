#ifndef MATRIX_NxM_H
#define MATRIX_NxM_H


class MatrixMxN
{
public:
	MatrixMxN();
	MatrixMxN(const int row_count, const int col_count);
	MatrixMxN(const MatrixMxN& m);

	~MatrixMxN();

	float** value_ptr;
	int rows;
	int cols;

	// make this matrix into its transpose
	void Transpose_InPlace();


};

#endif