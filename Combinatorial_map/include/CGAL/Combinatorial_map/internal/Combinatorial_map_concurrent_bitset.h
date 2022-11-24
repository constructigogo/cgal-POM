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
    //static constexpr const int element_count = (int)std::ceil(size / sizeof(unsigned long int));

    AtomicBitset() :
        m_element_count(std::ceil((double)size / (sizeof(unsigned long int) * 8))),
        mdata(std::vector<std::atomic<unsigned long int>>(m_element_count)) {};

    void operator=(const AtomicBitset<size>& other)
    {
        mdata = std::vector<std::atomic<unsigned long int>>(m_element_count);
        for (int i = 0; i < m_element_count; i++)
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

        if (value == 0)
            reset(index);
        else
            set(index);
    }

    void reset()
    {
        for (std::atomic<unsigned long int>& element : mdata)
            element = 0;
    }

    void reset(std::size_t index)
    {
        std::size_t quot;
        std::size_t rem;
        divMod(index, quot, rem);

        mdata[quot] &= (0UL << index);
    }

    void flip(std::size_t index)
    {
        std::size_t quot;
        std::size_t rem;
        divMod(index, quot, rem);

        mdata[quot] ^= (1UL << index);
    }

    bool operator[] (std::size_t index)
    {
        return get(index);
    }

private:
    int m_element_count;
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
