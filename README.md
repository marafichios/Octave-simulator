# Octave-simulator

## Homework no 2 - PCLP

This code is mainly based on the submission I have done last year, for the same
homework, but improved, as the one last year did not pass any tests (my homework
last year was posted only on vmchecker). I found it challenging to work with
dynamic memory, pointers, but I think I have done a far better job this year.
In my implementation I used a structure (matrix_t) to represent the matrices
and their contents within a vector of pointer to these matrices (vector is
called matrices), so I can easily manipulate each command. This code reads
commands from the user so it can perform specific operations with matrices, as
it follows:

###Commands###

1. L - load command reads the number of rows and columns from user input, it
resizes the matrices array, adding one matrix at a time and reallocating it
when needed. After that it creates a new matrix by dinamically allocating only
the necessary memory for it by accessing the matrix_t structure. We then read
the matrix contents from the user input and store them in the matrix. After
that we add the matrix to the matrices vector.

2. D - show_matrix_dimensions function checks if the index is in bounds and if
it is, it prints the number of rows and columns of the matrix at the given
index, by accessing the matrix_t structure.

3. P - show_matrix_contents function checks if the index is in bounds and if it
is, it prints the contents of the matrix at the given index, by accessing the
matrix_t structure.

4. C - crop_matrix function validates the index and the numbers of rows and
columns also to check if they are in bounds, and if yes, it reads and allocates
the rows and columns indexes so that it can find the elements to keep in the
cropped matrix. A new matrix is allocated with the help of create_matrix, 
and the wanted elements are added to it. It then frees the memory of the
original data array so it can lose the old data to replace it with the one
from the result. It also copies the resulted mstrix to the original one, so
that the change is made in-place, and do not forget to also free the temporary
variables used.

5. M - the m_cmd function checks the indexes, and if correct, it returns a
result of the 2 matrixes multiplication that is given to the result matrix in
the main, that is later added at the end of the matrices array, increasing the
size of it. What happens in the multiply function that is called in the m_cmd
is that there is created a new matrix to hold the product, product that is
done by using 3 nested loops: the outer 2 iterate through the rows and cols of
the result matrix, and the inner one is used to calculate the product. Later on
the overflow is prevented by the modulo operation, as instructed.

6. O - sort_matrices function iterates through all the matrices in the array
with the help of the first loop, the second loop compares the current matrix
at index i with all other matrices withe greater indexes, and the thrird and
fourth loop are used to go through each elememnt of the matrix so it can be
added to the sum variable. After doing so for all matrices, the sum is compared
with the sum of the next matrix, and if it is greater, the matrices are
swapped. The swap is done by using a temporary variable, the matrices being
sorted depending on the sum of their elements.

5. T - transpose_matrix_command first checks the index, and if in bounds,
there is created a new matrix that is transposed in place, and afterwards
the original matrix at the given index is updated with the new one. The
transpose_matrix func swaps rows with cols and allocates as much memory as
needed for the new matrix, it iterates through the original matrix and assigns
the transposed values to the new data array so it could free at the end the
memory allocated for the original array, as the change is done in place. At
the end, it updates the all the data.

6. R - the index validation is done in the main and in the matrix_expo
function also, in the required order, it creates a matrix result for the final
result and one called temp, that is used for auxiliary work. Firstly, the
result is set to the identity matrix, in case the power is 0. The algorithm
used is based on a loop that checks whether the power is even or odd. In case
it is odd, it multiplies the result matrix with the original one and updates
the result, in the other case it multiplies the original matrix by itself and
updates the original one. The auxiliary matrix is then freed, the fucntion
returning a pointer to the final result matrix. In the main, the fuction is
called and then the original matrix at the given index is updated with the
result of the exponentiation.

7. F - the free_matrix_and_shift function checks the index, and if everything
is ok, the memory of the matrix at the specified index is freed with the help
of the free_matrix function, and the matrices in the array are shifted to the
left with one position, as there cannot be any empty places in the array.

