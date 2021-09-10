#pragma once
#include <initializer_list>
#include "Vector.h"

template<unsigned int M, unsigned int N>
struct Matrix
{
    double* nums[M * N] = {0};

    Matrix(std::initializer_list<double>& vals);
    double GetElement(unsigned int y, unsigned int x) const;
    bool SetElement(unsigned int y, unsigned int x, double val);

    Matrix<M, N> operator+(const Matrix<M, N>& other) const;
    Matrix<M, N> operator-(const Matrix<M, N>& other) const;
    template<unsigned int O>
    Matrix<M,O> operator*(const Matrix<N, O>& other) const;

    Matrix<M, N>& operator+=(const Matrix<M, N>& other);
    Matrix<M, N>& operator-=(const Matrix<M, N>& other);

    Matrix<N,M> Transpose();

    Vector<N> operator*(const Vector<N>& other) const;
};

template<unsigned int M, unsigned int N>
Matrix<M,N>::Matrix(std::initializer_list<double>& vals) : nums(vals)
{

}

template<unsigned int M, unsigned int N>
double Matrix<M, N>::GetElement(unsigned int y, unsigned int x) const
{
    return nums[y * N + x];
}

template<unsigned int M, unsigned int N>
bool Matrix<M, N>::SetElement(unsigned int y, unsigned int x, double val)
{
    nums[y * N + x] = val;
}

template<unsigned int M, unsigned int N>
Matrix<M, N> Matrix<M,N>::operator+(const Matrix<M, N>& other) const
{    
    Matrix<M, N> res;
    unsigned int numElements = M*N;
    for(unsigned int i = 0; i < numElements; i++)
    {
        res.nums[i] = nums[i] + other.nums[i];
    }
    return res;

}

template<unsigned int M, unsigned int N>
Matrix<M, N> Matrix<M,N>::operator-(const Matrix<M, N>& other) const
{
    Matrix<M, N> res;
    unsigned int numElements = M*N;
    for(unsigned int i = 0; i < numElements; i++)
    {
        res.nums[i] = nums[i] - other.nums[i];
    }
    return res;
}

template<unsigned int M, unsigned int N>
template<unsigned int O>
Matrix<M,O> Matrix<M,N>::operator*(const Matrix<N, O>& other) const
{
    Matrix<M,O> res;
    for(unsigned int y = 0; y < O; i++)
    {
        for(unsigned int x = 0; x < M; x++)
        {
            res.SetElement(y, x, 0);
            
            for(unsigned int k = 0; k < N; k++)
            {
                res.SetElement(y, x, (res.GetElement(j, i) + GetElement(y,k) * other.GetElement(k,x))); 
            }
        }
    }
    return res;
}

template<unsigned int M, unsigned int N>
Matrix<M, N>& Matrix<M,N>::operator+=(const Matrix<M, N>& other)
{    
    unsigned int numElements = M*N;
    for(unsigned int i = 0; i < numElements; i++)
    {
        nums[i] = nums[i] + other.nums[i];
    }
    return this;

}

template<unsigned int M, unsigned int N>
Matrix<M, N>& Matrix<M,N>::operator-=(const Matrix<M, N>& other)
{
    unsigned int numElements = M*N;
    for(unsigned int i = 0; i < numElements; i++)
    {
        nums[i] = nums[i] - other.nums[i];
    }
    return this;
}


template<unsigned int M, unsigned int N>
Matrix<N,M> Matrix<M,N>::Transpose()
{
    Matrix<N, M> res;

    for(unsigned int y = 0; y < M; y++)
    {
        for(unsigned int x = 0; x < N; x++)
        {
            res.nums[x * M + y] = nums[y * N + x];
        }
    }
    return res;
}

template<unsigned int M, unsigned int N>
Vector<N> Matrix<M,N>::operator*(const Vector<N>& other) const
{
    Vector<N> res;
    for(unsigned int row = 0; row < M; row++)
    {
        for(unsigned int col = 0; col < N; col++)
        {
            res[row] += nums[row * N + col] * nums[col]; 
        }
    }
    return res;
}