#pragma once
#include <initializer_list>
#include "Vector.h"

template<unsigned int M, unsigned int N>
struct Matrix
{
    double nums[M * N] = {0};


    double GetElement(unsigned int y, unsigned int x) const;
    bool SetElement(unsigned int y, unsigned int x, double val);

    Matrix<M, N> operator+(const Matrix<M, N>& other) const;
    Matrix<M, N> operator-(const Matrix<M, N>& other) const;
    template<unsigned int P>
    Matrix<M,P> operator*(const Matrix<N, P>& other) const;

    Matrix<M, N>& operator+=(const Matrix<M, N>& other);
    Matrix<M, N>& operator-=(const Matrix<M, N>& other);
    
    bool operator==(const Matrix<M,N>& other);

    template<unsigned int P, unsigned int O>
    bool operator==(const Matrix<P,O>& other);

    Matrix<N,M> Transpose();

    Vector<N> operator*(const Vector<N>& other) const;

    static Matrix<M,N> IdentityMatrix();
};

template<unsigned int M, unsigned int N>
double Matrix<M, N>::GetElement(unsigned int y, unsigned int x) const
{
    return nums[y * N + x];
}

template<unsigned int M, unsigned int N>
bool Matrix<M, N>::SetElement(unsigned int y, unsigned int x, double val)
{
    if(y >= M || x >= N)
    {
        return false;
    }

    nums[y * N + x] = val;
    return true;
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
template<unsigned int P>
Matrix<M,P> Matrix<M,N>::operator*(const Matrix<N, P>& other) const
{
    Matrix<M,P> res;
    for(unsigned int y = 0; y < P; y++)
    {
        for(unsigned int x = 0; x < M; x++)
        {
            res.SetElement(y, x, 0);
            
            for(unsigned int k = 0; k < N; k++)
            {
                res.SetElement(y, x, (res.GetElement(y, x) + GetElement(y,k) * other.GetElement(k,x))); 
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
    return *this;

}

template<unsigned int M, unsigned int N>
Matrix<M, N>& Matrix<M,N>::operator-=(const Matrix<M, N>& other)
{
    unsigned int numElements = M*N;
    for(unsigned int i = 0; i < numElements; i++)
    {
        nums[i] = nums[i] - other.nums[i];
    }
    return *this;
}

template<unsigned int M, unsigned int N>
bool Matrix<M,N>::operator==(const Matrix<M,N>& other)
{
    unsigned int size = M*N;
    for(unsigned int i = 0; i < size; i++)
    {
        if(nums[i] != other.nums[i])
        {
            return false;
        }
    }
    return true;
}

template<unsigned int M, unsigned int N>
template<unsigned int P, unsigned int O>
bool Matrix<M, N>::operator==(const Matrix<P,O>& other)
{
    if(P != M || O != N)
    {
        return false;
    }
    
    return *this == other;
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
    for(unsigned int y = 0; y < N; y++)
    {
        res[y] = 0;
        
        for(unsigned int x = 0; x < M; x++)
        {
            res[y] += GetElement(y,x) * other[x]; 
        }
    }
    return res;
}

template<unsigned int M, unsigned int N>
Matrix<M,N> Matrix<M,N>::IdentityMatrix()
{
    Matrix<M,N> res{0};

    for(int i = 0; i < M*N; i++)
    {
        if(i / M == i % N)
        {
            res.nums[i] = 1;
        }
    }
    return res;
}