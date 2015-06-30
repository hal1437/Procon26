
#include<stdexcept>

namespace Anct{

template <typename _Tp, size_t _Size>
struct Array{
    typedef _Tp         value_type;
    typedef value_type& reference;
    typedef value_type* iterator;

    value_type elems[_Size];
    constexpr reference operator[](size_t l){ //accessor
        return elems[l];
    }
    constexpr value_type const& operator[](size_t l)const{ //accessor (allow reading)
        return elems[l];
    }
    constexpr size_t const size(){
        return _Size;
    }

    constexpr iterator begin() const { return iterator(elems); }
    constexpr iterator end() const { return iterator(elems + _Size); }

    constexpr reference at(size_t _n) const;


};

template <class _Tp, size_t _Size>
constexpr typename Array<_Tp, _Size>::reference
Array<_Tp, _Size>::at(size_t _n) const
{
    if (_n >= _Size)throw "out of range array::at";
    return elems[_n];
}

}
