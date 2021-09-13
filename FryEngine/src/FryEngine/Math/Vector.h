#pragma once
#include <math.h>

template<unsigned int S>
struct Vector
{
    double nums[S] = {0};

    Vector<S>& operator=(std::initializer_list<double> const & vals);
    Vector<S>& operator=(const Vector<S>& other);

    double operator[](unsigned int i) const;
    double & operator[](unsigned int i);

    Vector<S> operator+(const Vector<S>& other) const;
    Vector<S> operator-(const Vector<S>& other) const;
    Vector<S> operator*(double num) const;
    Vector<S> operator/(double num) const;

    Vector<S>& operator+=(const Vector<S>& other);
    Vector<S>& operator-=(const Vector<S>& other);
    Vector<S>& operator*=(double num);
    Vector<S>& operator/=(double num);

    bool operator==(const Vector<S>& other) const;

    Vector<S> Cross(const Vector<S>& other);
    double Dot(const Vector<S>& other) const;
    double Length() const;
};

template<unsigned int S>
Vector<S>& Vector<S>::operator=(std::initializer_list<double> const & vals)
{
    for(unsigned int i = 0; i < vals.size(); i++)
    {
        nums[i] = *(vals.begin() + i);
    }
    return *this;
}

template<unsigned int S>
Vector<S>& Vector<S>::operator=(const Vector<S>& other)
{
    for(unsigned int i = 0; i < S; i++)
    {
        nums[i] = other.nums[i];
    }
    return *this;
}

template<unsigned int S>
double Vector<S>::operator[](unsigned int i) const
{
    return nums[i];
}

template<unsigned int S>
double & Vector<S>::operator[](unsigned int i)
{
    return nums[i];
}

template<unsigned int S>
Vector<S> Vector<S>::operator+(const Vector<S>& other) const
{
    Vector<S> vec;
    for(unsigned int i = 0; i < S; i++)
    {
        vec.nums[i] = nums[i] + other.nums[i];
    }
    return vec;
}

template<unsigned int S>
Vector<S> Vector<S>::operator-(const Vector<S>& other) const
{
    Vector<S> vec;
    for(unsigned int i = 0; i < S; i++)
    {
        vec.nums[i] = nums[i] - other.nums[i];
    }
    return vec;
}

template<unsigned int S>
Vector<S> Vector<S>::operator*(double num) const
{
    Vector<S> vec;
    for(unsigned int i = 0; i < S; i++)
    {
        vec.nums[i] = nums[i] * num;
    }
    return vec;
}

template<unsigned int S>
Vector<S> Vector<S>::operator/(double num) const
{
    Vector<S> vec;
    for(unsigned int i = 0; i < S; i++)
    {
        vec.nums[i] = nums[i] / num;
    }
    return vec;
}

template<unsigned int S>
Vector<S>& Vector<S>::operator+=(const Vector<S>& other)
{
    for(unsigned int i = 0; i < S; i++)
    {
        nums[i] += other.nums[i];
    }
    return this;
}

template<unsigned int S>
Vector<S>& Vector<S>::operator-=(const Vector<S>& other)
{
    for(unsigned int i = 0; i < S; i++)
    {
        nums[i] -= other.nums[i];
    }
    return this;
}

template<unsigned int S>
Vector<S>& Vector<S>::operator*=(double num)
{
    for(unsigned int i = 0; i < S; i++)
    {
        nums[i] *= num;
    }
    return *this;
}

template<unsigned int S>
Vector<S>& Vector<S>::operator/=(double num)
{
    for(unsigned int i = 0; i < S; i++)
    {
        nums[i] /= num;
    }
    return *this;
}

template< unsigned int S>
bool Vector<S>::operator==(const Vector<S>& other) const
{
    for(int i = 0; i < S; i++)
    {
        if(nums[i] != other.nums[i])
        {
            return false;
        }
    }
    return true;
}

template<unsigned int S>
Vector<S> Vector<S>::Cross(const Vector<S>& other)
{
    Vector<3> res;
    for(int i = 0; i < 3; i++)
    {
        res.nums[i] = nums[(i + 1) % 3] * other.nums[(i + 2) % 3] - nums[(i + 2) % 3] * other.nums[(i + 1) % 3];
    }
    return res;
}

template<unsigned int S>
double Vector<S>::Dot(const Vector<S>& other) const
{
    double res = 0;
    for(int i = 0; i < S; i++)
    {
        res += nums[i] * other.nums[i];
    }
    return res;
}

template<unsigned int S>
double Vector<S>::Length() const
{
    return sqrt(Dot(*this));
}