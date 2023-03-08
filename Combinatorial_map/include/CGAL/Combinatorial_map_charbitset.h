//
// Created by constantin on 02/03/23.
//

#ifndef CGAL_COMBINATORIAL_MAP_CONCURRENT_CHARBITSET_H
#define CGAL_COMBINATORIAL_MAP_CONCURRENT_CHARBITSET_H 1

#pragma once

#include <array>
#include <atomic>
#include <mutex>
#include <memory>
#include <cassert>
#include <cstddef>
#include <limits>

template <size_t N>
class CharBitset {
public:
    CharBitset();

    std::mutex mtx;

    CharBitset(const CharBitset&);
    CharBitset& operator=(const CharBitset& rhs);

    void reset();
    bool reset(size_t idx);
    void set();
    bool set(size_t idx, bool value =true);
    bool flip(size_t idx);

    bool test(size_t idx) const;

    bool operator[](size_t idx) const;

    constexpr size_t size() const { return N; }
private:
    std::array<char,N> data_;
};

// value-initialize to zero
template <size_t N>
inline CharBitset<N>::CharBitset() : data_() {
    for (auto & val : data_) {
        val = false;
    }
}

template <size_t N>
inline CharBitset<N>::CharBitset(const CharBitset& other)
{
    for (int i = 0; i < N; i++)
    {
        data_[i] = other.data_[i];
    }
}

template <size_t N>
CharBitset<N>& CharBitset<N>::operator=(const CharBitset& rhs)
{
    std::unique_lock<std::mutex> lck(mtx);
    for (int i = 0; i < N; i++) {
        data_[i]=rhs.data_[i];
    }
    return *this;
}



template <size_t N>
inline void CharBitset<N>::reset()
{
    std::unique_lock<std::mutex> lck(mtx);
    for (auto &_val :data_) {
        _val = false;
    }
}

template <size_t N>
inline bool CharBitset<N>::reset(size_t idx)
{
    //std::unique_lock<std::mutex> lck(mtx);
    assert(idx < N);
    data_[idx]= false;
    return false;
}


template <size_t N>
inline void CharBitset<N>::set() {
    std::unique_lock<std::mutex> lck(mtx);
    for (auto &_val :data_) {
        _val = true;
    }
}
template <size_t N>
inline bool CharBitset<N>::set(size_t idx,bool value) {
    //std::unique_lock<std::mutex> lck(mtx);
    assert(idx < N);
    data_[idx]=value;
    return value;
}

template <size_t N>
inline bool CharBitset<N>::flip(size_t idx)
{
    //std::unique_lock<std::mutex> lck(mtx);
    assert(idx < N);
    data_[idx] = !data_[idx];
    return data_[idx];
}

template <size_t N>
inline bool CharBitset<N>::test(size_t idx) const
{
    assert(idx < N);
    return data_[idx];
}


template <size_t N>
inline bool CharBitset<N>::operator[](size_t idx) const
{
    return data_[idx];
}


#endif //CGAL_COMBINATORIAL_MAP_CONCURRENT_CHARBITSET_H
