#include <iostream>
#include <string>

class Matrix {
public:
    int rows;
    int columns;
    int* contents;

    // Constructor
    // data is the contents of the matrix in a list
    // like you read a book
    Matrix(int m, int n, int* data) {
        rows = m;
        columns = n;
        
        // We copy data to a dynamically allocated piece of memory
        // to make the program more robust.
        contents = new int[m*n];
        
        for (int i = 0; i < m*n; i++) {
            contents[i] = data[i];    
        }
    }       
    
    // Returns the element in row i, column j
    int getElement(int i, int j) {
        return contents[columns*i + j];
    }

    // Gets the i'th row of this matrix
    int* getRow(int i) {
        // Dynamic because we need the row outside this helper function
        int* row = new int[columns];
        
        for (int j = 0; j < columns; j++) {
            row[j] = getElement(i, j);
        }
        
        return row;
    }
    
    // Gets a the j'th column of matrix other
    int* getColumn(int j) {
        // Dynamic because we need the column outside this helper function
        int* column = new int[rows];
        
        for (int i = 0; i < rows; i++) {
            column[i] = getElement(i, j);
        }
        
        return column;
    }
    
    Matrix addition(Matrix other) {
        int* sum = new int[rows*columns];
        
        for (int i = 0; i < rows*columns; i++) {
            sum[i] = contents[i] + other.contents[i];    
        }
        
        Matrix sumMatrix(rows, columns, sum);
        
        delete sum;
        
        return sumMatrix;
    }
    
    Matrix product(Matrix other) {
        int* product = new int[rows*other.columns];
        
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < other.columns; j++) {
                int* row = getRow(i);
                int* column = other.getColumn(j);
                int value = innerProduct(row, column, columns); // Our row and the other guy's column contain as many elements as we have columns
                                
                delete row;
                delete column;
                product[other.columns*i + j] = value;
            }
        }
        
        Matrix productMatrix(rows, other.columns, product);
        
        delete product;
        
        return productMatrix;
    }
    
    void print() {
                
        for (int i = 0; i < rows; i++) {
            std::cout << '{';
            for (int j = 0; j < columns; j++) {
                std::cout << getElement(i, j) << ", ";
            }
            std::cout << "}\n";
        }
        
    }
    
private:    
    // Computes the inner product between
    // vec1, vec2, both vectors of length.
    int innerProduct(int* vec1, int* vec2, int length) {
        int sum = 0;
        for (int i = 0; i < length; i++) {
            sum += vec1[i]*vec2[i];
        }        
        return sum;
    }
    
};

int main() {
    int data1[6] = {0, 1, 
                    1, 0,
                    2, 2};
    Matrix mat1(3, 2, data1);
    
    int data2[8] = {1, 2, 3, 4,
                    5, 6, 7, 8};
    Matrix mat2(2, 4, data2);
    
    Matrix product = mat1.product(mat2);
    
    std::cout << "Matrix 1 looks like\n";
    mat1.print();
    std::cout << "Matrix 2 looks like\n";
    mat2.print();
    std::cout << "The product looks like \n";
    product.print();
}
