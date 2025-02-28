#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int rows;
	int cols;
	int **data;
} matrix_t;

void resize_matrices_array(matrix_t ***matrices, int *num_matrices)
{
	//Increment no. of matrices in the array
	*num_matrices = *num_matrices + 1;

	//Allocate memory for updated array
	if (!(*matrices))
		*matrices = malloc(*num_matrices * sizeof(matrix_t *));
	else
		*matrices = realloc(*matrices, *num_matrices * sizeof(matrix_t *));

	if (!(*matrices)) {
		printf("Error reallocating matrices array\n");
		exit(EXIT_FAILURE);
	}
}

matrix_t *create_matrix(int rows, int cols)
{
	matrix_t *matrix = malloc(sizeof(matrix_t));
	// Set the number of rows and columns
	matrix->rows = rows;
	matrix->cols = cols;
	// Allocate memory for the data array
	matrix->data = malloc(rows * sizeof(int *));
	for (int i = 0; i < rows; i++)
		matrix->data[i] = malloc(cols * sizeof(int));
	return matrix;
}

void load_matrix(matrix_t ***matrices, int *num_matrices)
{
	// Read the number of rows and columns from user input
	int rows, cols;
	scanf("%d %d", &rows, &cols);

	// Resize the matrices array
	resize_matrices_array(matrices, num_matrices);

	// Create a new matrix and set its dimensions
	matrix_t *new_matrix = create_matrix(rows, cols);

	// Read the matrix values from user input
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			scanf("%d", &new_matrix->data[i][j]);

	// Add the new matrix to the array
	(*matrices)[*num_matrices - 1] = new_matrix;
}

void free_matrix(matrix_t *matrix)
{
	// Free the memory for the data array
	for (int i = 0; i < matrix->rows; i++)
		free(matrix->data[i]);
	free(matrix->data);

	free(matrix);
}

matrix_t *transpose_matrix(matrix_t *matrix)
{
	// Swap rows and columns
	int temp = matrix->rows;
	matrix->rows = matrix->cols;
	matrix->cols = temp;

	// Allocate memory for the transposed data array
	int **transposed_data = malloc(matrix->rows * sizeof(int *));
	for (int i = 0; i < matrix->rows; i++)
		transposed_data[i] = malloc(matrix->cols * sizeof(int));

	// Calculate the transpose of the matrix
	for (int i = 0; i < matrix->rows; i++)
		for (int j = 0; j < matrix->cols; j++)
			transposed_data[i][j] = matrix->data[j][i];

	// Free the memory for the original data array
	for (int i = 0; i < matrix->cols; i++)
		free(matrix->data[i]);
	free(matrix->data);

	// Update the matrix to use the transposed data array
	matrix->data = transposed_data;

	return matrix;
}

void transpose_matrix_command(matrix_t ***matrices, int num_matrices,
							  int index)

{
	if (index < 0 || index >= num_matrices) {
		printf("No matrix with the given index\n");
		return;
	}

	matrix_t *new_matrix = transpose_matrix((*matrices)[index]);

	if (!new_matrix) {
		printf("Could not calculate the transpose of the matrix\n");
		return;
	}

	(*matrices)[index] = new_matrix;
}

matrix_t *multiply(matrix_t *matrix1, matrix_t *matrix2)
{
	// Check if the matrices can be multiplied
	if (matrix1->cols != matrix2->rows) {
		printf("Cannot perform matrix multiplication\n");
		return NULL;
	}

	// Create a new matrix to hold the result
	matrix_t *result = create_matrix(matrix1->rows, matrix2->cols);

	// Calculate the result
	for (int i = 0; i < result->rows; i++) {
		for (int j = 0; j < result->cols; j++) {
			int sum = 0;
			for (int k = 0; k < matrix1->cols; k++) {
				sum += matrix1->data[i][k] * matrix2->data[k][j];
				sum = sum % 10007;
			}
			sum = sum % 10007;
			if (sum < 0)
				sum += 10007;
			result->data[i][j] = sum;
		}
	}

	return result;
}

matrix_t *m_cmd(matrix_t **matrices, int num_matrices,
				int index1, int index2)

{
	if (index1 < num_matrices && index2 < num_matrices) {
		if (matrices[index1]->cols == matrices[index2]->rows) {
			matrix_t *result = multiply(matrices[index1], matrices[index2]);
			return result;
		}
		printf("Cannot perform matrix multiplication\n");
	} else {
		printf("No matrix with the given index\n");
	}
	return NULL;
}

void add_result(matrix_t ***matrices, int *num_matrices,
				matrix_t *result)
{
	// Resize the matrices array
	resize_matrices_array(matrices, num_matrices);

	// Add the new matrix to the array
	(*matrices)[*num_matrices - 1] = result;
}

void show_matrix_dimensions(matrix_t **matrices, int num_matrices, int index)
{
	if (index < 0 || index >= num_matrices) {
		printf("No matrix with the given index\n");
		return;
	}
	printf("%d %d\n", matrices[index]->rows, matrices[index]->cols);
}

void show_matrix_contents(matrix_t **matrices, int num_matrices, int index)
{
	if (index < 0 || index >= num_matrices) {
		printf("No matrix with the given index\n");
		return;
	}
	for (int i = 0; i < matrices[index]->rows; i++) {
		for (int j = 0; j < matrices[index]->cols; j++)
			printf("%d ", matrices[index]->data[i][j]);
		printf("\n");
	}
}

void sort_matrices(matrix_t **matrices, int num_matrices)
{
	for (int i = 0; i < num_matrices - 1; i++) {
		for (int j = i + 1; j < num_matrices; j++) {
			//calculate sum of elements in matrix i
			int sum_i = 0;
			for (int row = 0; row < matrices[i]->rows; row++)
				for (int col = 0; col < matrices[i]->cols; col++)
					sum_i += matrices[i]->data[row][col];

			//prevent overflow
			if (sum_i % 10007 < 0)
				sum_i = sum_i % 10007 + 10007;
			else
				sum_i = sum_i % 10007;

			//do the same for matrix j
			int sum_j = 0;
			for (int row = 0; row < matrices[j]->rows; row++)
				for (int col = 0; col < matrices[j]->cols; col++)
					sum_j += matrices[j]->data[row][col];

			if (sum_j % 10007 < 0)
				sum_j = sum_j % 10007 + 10007;
			else
				sum_j = sum_j % 10007;

			//sort matrices by their sum
			if (sum_i > sum_j) {
				matrix_t *temp_matrix = matrices[i];
				matrices[i] = matrices[j];
				matrices[j] = temp_matrix;
			}
		}
	}
}

void crop_matrix(matrix_t ***matrices, int *num_matrices, int index)
{
	// Check if the given index is valid
	if (index < 0 || index >= *num_matrices) {
		printf("No matrix with the given index\n");
		return;
	}

	int rows, cols;
	scanf("%d", &rows);

	// Validate the number of rows for cropping
	if (rows <= 0 || rows > (*matrices)[index]->rows) {
		printf("Error: Invalid number of rows for cropping\n");
		return;
	}

	int *rows_to_crop = malloc(rows * sizeof(int));

	for (int i = 0; i < rows; i++)
		scanf("%d", &rows_to_crop[i]);

	scanf("%d", &cols);

	// Validate the number of columns for cropping
	if (cols <= 0 || cols > (*matrices)[index]->cols) {
		printf("Error: Invalid number of columns for cropping\n");
		free(rows_to_crop);
		return;
	}

	int *cols_to_crop = malloc(cols * sizeof(int));

	for (int i = 0; i < cols; i++)
	    scanf("%d", &cols_to_crop[i]);

	// Create a new matrix to hold the result
	matrix_t *result = create_matrix(rows, cols);

	// Calculate the result
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			result->data[i][j] =
				(*matrices)[index]->data[rows_to_crop[i]][cols_to_crop[j]];
		}
	}

    // Free the memory for the original data array
	for (int i = 0; i < (*matrices)[index]->rows; i++)
		free((*matrices)[index]->data[i]);

	free((*matrices)[index]->data);

	// Update the matrix to use the result data array
	(*matrices)[index]->rows = result->rows;
	(*matrices)[index]->cols = result->cols;

	(*matrices)[index]->data = malloc((*matrices)[index]->rows * sizeof(int *));
	for (int i = 0; i < (*matrices)[index]->rows; i++)
		(*matrices)[index]->data[i] =
			malloc((*matrices)[index]->cols * sizeof(int));

	// Copy the result matrix to the original matrix
	for (int i = 0; i < (*matrices)[index]->rows; i++)
		for (int j = 0; j < (*matrices)[index]->cols; j++)
			(*matrices)[index]->data[i][j] = result->data[i][j];

	// Free temporary arrays and result matrix
	free(rows_to_crop);
	free(cols_to_crop);
	free_matrix(result);
}

matrix_t *multiply_square(matrix_t *matrix1, matrix_t *matrix2, matrix_t *temp)
{
	// Check if the matrices can be multiplied
	if (matrix1->cols != matrix2->rows) {
		printf("Cannot perform matrix multiplication\n");
		return NULL;
	}

	// Initialize result matrix to 0
	for (int i = 0; i < temp->rows; i++)
		for (int j = 0; j < temp->cols; j++)
			temp->data[i][j] = 0;

	// Calculate the result with modulo operation
	for (int i = 0; i < temp->rows; i++) {
		for (int j = 0; j < temp->cols; j++) {
			int sum = 0;
			for (int k = 0; k < matrix1->cols; k++) {
				sum += (matrix1->data[i][k] * matrix2->data[k][j]) % 10007;
				sum = (sum % 10007 + 10007) % 10007;
			}
			temp->data[i][j] = sum % 10007;
		}
	}

	return temp;
}

matrix_t *matrix_expo(matrix_t *matrix, int power)
{
    // Check if the power is a positive integer between (-10)^9 and 10^9
	if (power < 0) {
		printf("Power should be positive\n");
		return NULL;
	}

	// Check if the matrix is square
	if (matrix->rows != matrix->cols) {
		printf("Cannot perform matrix multiplication\n");
		return NULL;
	}

	// Create a new matrix to hold the exponentiation result
	matrix_t *result = create_matrix(matrix->rows, matrix->cols);

	// Initialize the result matrix to the identity matrix
	for (int i = 0; i < result->rows; i++) {
		for (int j = 0; j < result->cols; j++) {
			if (i == j)
				result->data[i][j] = 1;
			else
				result->data[i][j] = 0;
		}
	}

	matrix_t *temp = create_matrix(matrix->rows, matrix->cols);

	while (power) {
		if (power % 2 == 1) {
			// Multiply result by matrix and store the result in temp
			multiply_square(result, matrix, temp);

			// Copy the values from temp to result
			for (int i = 0; i < result->rows; i++)
				for (int j = 0; j < result->cols; j++)
					result->data[i][j] = temp->data[i][j];
		}

		// Multiply matrix by itself and store the result in temp
		multiply_square(matrix, matrix, temp);

		// Copy the values from temp to matrix
		for (int i = 0; i < matrix->rows; i++)
			for (int j = 0; j < matrix->cols; j++)
				matrix->data[i][j] = temp->data[i][j];

		power /= 2;
	}

	free_matrix(temp);

	return result;
}

void free_matrix_and_shift(matrix_t ***matrices, int *num_matrices, int index)
{
	if (index < 0 || index >= *num_matrices) {
		printf("No matrix with the given index\n");
		return;
	}

	free_matrix((*matrices)[index]);

	for (int i = index; i < *num_matrices - 1; i++)
		(*matrices)[i] = (*matrices)[i + 1];

	(*num_matrices)--;
}

int main(void)
{
	matrix_t **matrices = NULL;  // array of matrices // size of matrices array
	char cmd;
	int index, i1, i2, index_d, index_p, index_f, index_c, po;
	int num_matrices = 0;
	while (1) {
		scanf(" %c", &cmd);  // Note the space before %c to skip whitespace
		if (cmd == 'Q')
			break;
		if (cmd >= 'A' && cmd <= 'Z') {
			switch (cmd) {
			case 'Q':
				break;
			case 'L':
				load_matrix(&matrices, &num_matrices);
				break;
			case 'D':
				scanf("%d", &index_d);
				show_matrix_dimensions(matrices, num_matrices, index_d);
				break;
			case 'P':
				scanf("%d", &index_p);
				show_matrix_contents(matrices, num_matrices, index_p);
				break;
			case 'C':
				scanf("%d", &index_c);
				crop_matrix(&matrices, &num_matrices, index_c);
				break;
			case 'M':
				scanf("%d%d", &i1, &i2);
				matrix_t *result = m_cmd(matrices, num_matrices, i1, i2);
				if (result)
					add_result(&matrices, &num_matrices, result);
				break;
			case 'O':
				sort_matrices(matrices, num_matrices);
				break;
			case 'T':
				scanf("%d", &index);
				transpose_matrix_command(&matrices, num_matrices, index);
				break;
			case 'R':
				scanf("%d%d", &index_f, &po);
				if (index_f < 0 || index_f >= num_matrices) {
					printf("No matrix with the given index\n");
				} else {
					matrix_t *new_matrix = matrix_expo(matrices[index_f], po);
					if (new_matrix)	{
						free_matrix(matrices[index_f]);
						matrices[index_f] = new_matrix;
					}
				}
				break;
			case 'F':
				scanf("%d", &index_f);
				free_matrix_and_shift(&matrices, &num_matrices, index_f);
				break;
			case 'S':
				break;
			default:
				printf("Unrecognized command\n");
				break;
			}
		}
	}
	for (int i = 0; i < num_matrices; i++)
		free_matrix(matrices[i]);

	free(matrices);
	return 0;
}

