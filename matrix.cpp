#include <iostream>
#include <vector>

template<typename T>
class Matrix {
public:

    // Default constructor
    Matrix();

    // Constructor
    Matrix(int m, int n, std::vector<T> data): rows_{m}, columns_{n} {
        if (m*n != (int) data.size()) {
            throw "Dimensions do not match data!";
        }
        data_ = data;
    }

    // Copy constructor (makes a copy of the argument)
    Matrix(const Matrix<T>& other): rows_{other.rows_}, columns_{other.columns_} {
        data_ = other.data_;
    }

    // Move constructor (moves the information of the argument to the created object, the
    // argument is destroyed)
    Matrix(Matrix<T>&& other): rows_{other.rows_}, columns_{other.columns_}, data_{other.data_} {
        other.data_.~vector();
    }

    // Copy assignment operator (overloads = when we have an object on the other side of =)
    Matrix<T>& operator=(const Matrix<T>& other) {
        if (this != &other) {
            rows_ = other.rows_;
            columns_ = other.columns_;
            data_ = other.data_;
        }

        return *this;
    }

    // Move assignment operator (overloads = when we have a reference on the other side =)
    Matrix<T>& operator=(Matrix<T>&& other) {
        if (this != &other) {
            rows_ = other.rows_;
            columns_ = other.columns_;
            data_ = &other.data_;
            other.data_.~vector();
        }

        return *this;
    }

    // Destructor
    ~Matrix() {
        data_.~vector();
    }

    // Returns a reference to the entry at (i, j), so we can change it.
    T& operator()(int i, int j) {
        return data_.at(columns_*i + j);
    }

    Matrix<T> operator+(const Matrix<T>& other) {
        if (columns_ != other.columns_ || rows_ != other.rows_) {
            throw "Addition failure: dimensions do not match";
        }

        // (Why doesn't 'Matrix<T> sum = this' work as an application of the copy constructor?)
        Matrix<T> sum(rows_, columns_, data_);
        for (int i = 0; i < rows_*columns_; i++) {
            sum.data_[i] += other.data_[i];
        }

        return sum;
    }

    // Design question: how can I change operator() to allow other to be constant?
    Matrix<T> operator*(Matrix<T>& other) {
        if (columns_ != other.rows_) {
            throw "Multiplication error: dimensions do not match";
        }

        std::vector<T> product;

        for (int i = 0; i < rows_; i++) {
            for (int j = 0; j < other.columns_; j++) {
                T sum = 0;
                for (int k = 0; k < columns_; k++) {
                    sum += other(k, j) * this->operator()(i, k);
                }
                product.push_back(sum);
            }
        }

        return Matrix(rows_, columns_, product);
    }

private:
    int rows_;
    int columns_;
    std::vector<T> data_;

    template<typename U>
    friend std::ostream& operator<<(std::ostream& outputStream, Matrix<U>& matrix);
};

template<typename U>
std::ostream& operator<<(std::ostream& outputStream, Matrix<U>& matrix) {

    for (int i = 0; i < matrix.rows_; i++) {
        outputStream << '{';
        for (int j = 0; j < matrix.columns_ - 1; j++) {
            outputStream << matrix(i, j) << ", ";
        }
        outputStream << matrix(i, matrix.columns_ - 1) << "}\n";
    }

    return outputStream;
}

int main() {
    std::vector<int> data{1, 2, 3, 4};
    Matrix<int> mat(2, 2, data);

    Matrix<int> sum = mat + mat;
    Matrix<int> product = mat * mat;

    std::cout << mat << "+\n" << mat << "=\n" << sum;
    std::cout << "------------------" << std::endl;
    std::cout << mat << "*\n" << mat << "=\n" << product;

    mat(0, 0) = 0;
    std::cout << "Changed mat:\n" << mat;
}
