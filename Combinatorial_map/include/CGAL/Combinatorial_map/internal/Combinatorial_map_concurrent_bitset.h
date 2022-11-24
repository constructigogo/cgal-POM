#ifndef CGAL_COMBINATORIAL_MAP_CONCURRENT_BITSET_H
#define CGAL_COMBINATORIAL_MAP_CONCURRENT_BITSET_H 1

#include <atomic>
#include <cmath>
#include <cstdint>
#include <vector>
#include <bitset>

template <std::size_t size>
class AtomicBitset {
public:
    enum info { element_count = (int) std::ceil(size / sizeof(unsigned long int))};

    AtomicBitset() :
        mdata(std::vector<std::atomic<unsigned long int>>(info::element_count)) {};

    void operator=(const AtomicBitset<size>& other)
    {
        mdata = std::vector<std::atomic<unsigned long int>>(info::element_count);
        for (int i = 0; i < info::element_count; i++)
            mdata.at(i).store(other.mdata.at(i).load());
    }

    bool get(std::size_t index) const
    {
        std::size_t quot;
        std::size_t rem;
        divMod(index, quot, rem);

        return mdata[quot] & (1 << rem);
    }

    void set(std::size_t index)
    {
        std::size_t quot;
        std::size_t rem;
        divMod(index, quot, rem);

        mdata[quot] |= (1 << rem);
    }

    void set(std::size_t index, bool value)
    {
        std::size_t quot;
        std::size_t rem;
        divMod(index, quot, rem);

        mdata[quot] ^= (-(unsigned long)value ^ mdata[quot]) & (1UL << rem);
    }

    void reset()
    {
        for(std::atomic<unsigned long int>& element : mdata)
            element = 0;
    }

    void flip(std::size_t index)
    {
        std::size_t quot;
        std::size_t rem;
        divMod(index, quot, rem);

        mdata[quot] = !mdata[quot];
    }

    bool operator[] (std::size_t index)
    {
        return get(index);
    }

private:
    std::vector<std::atomic<unsigned long int>> mdata;

    static void
    divMod(std::size_t index, std::size_t& quot, std::size_t& rem)
    {
        quot = std::floor(index / size);
        rem = index - (quot * size);
    }

    static std::size_t
    computeVectorSize(std::size_t elementCount)
    {
        std::size_t quot;
        std::size_t rem;

        divMod(elementCount, quot, rem);
        if (rem != 0)
            ++quot;

        return (quot * size);
    }
};

#endif // CGAL_COMBINATORIAL_MAP_CONCURRENT_BITSET_H //
