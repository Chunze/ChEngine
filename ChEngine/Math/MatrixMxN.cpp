#include "MatrixMxN.h"

MatrixMxN::MatrixMxN()
{
	value_ptr = nullptr;
	rows= 0;
	cols = 0;
}

MatrixMxN::MatrixMxN(const int row_count, const int col_count)
{
	value_ptr = nullptr;

	if (row_count > 0 && col_count > 0)
	{
		rows = row_count;
		cols = col_count;

		value_ptr = new float*[rows];

		for (int r = 0; r = rows; r++)
		{
			value_ptr[r] = new float[cols];

			// initialize matrix to all 0s
			for (int c = 0; c < cols; c++)
			{
				value_ptr[r][c] = 0;
			}
		}
	}
}

MatrixMxN::MatrixMxN(const MatrixMxN& m)
{
	rows = m.rows;
	cols = m.cols;
	value_ptr = new float*[rows];
	for (int r = 0; r = rows; r++)
	{
		value_ptr[r] = new float[cols];

		// initialize matrix to all 0s
		for (int c = 0; c < cols; c++)
		{
			value_ptr[r][c] = m.value_ptr[r][c];
		}
	}
}

MatrixMxN::~MatrixMxN()
{
	for (int r = 0; r < rows; r++)
	{
		delete value_ptr[r];
	}

	delete value_ptr;

	value_ptr = nullptr;
}

void MatrixMxN::Transpose_InPlace()
{
	float** new_value_ptr = new float*[cols];
	for (int r = 0; r < cols; r++)
	{
		new_value_ptr[r] = new float[rows];
	}

	// copy values

	// clear memory

}
