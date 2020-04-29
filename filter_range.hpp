/*******************************************************************************
 * filter range, use as: range | filter(function)
 * Author: Yarin Hananiya
 * ****************************************************************************/
#ifndef FILTER_RANGE_HPP
#define FILTER_RANGE_HPP

#include <type_traits> // std::enable_if
#include <algorithm> // std::find_if

#include "ranges.hpp" // ranges::range, ranges::is_range

namespace my_view {

template<typename Func>
class filter_adaptor {
public:
    filter_adaptor(Func func) : m_func(func) {}
    filter_adaptor(const filter_adaptor&) = default;
    filter_adaptor(filter_adaptor&&) = default;
    filter_adaptor& operator=(const filter_adaptor&) = default;
    filter_adaptor& operator=(filter_adaptor&&) = default;
    ~filter_adaptor() = default;

    constexpr auto get_function() -> Func {
        return m_func;
    }

private:
    Func m_func;
};

template<typename UnderlyingIter, typename Func>
class filter_iterator {
public:
    using iterator_category = typename std::iterator_traits<UnderlyingIter>::iterator_category;
    using value_type = typename std::iterator_traits<UnderlyingIter>::value_type;
    using difference_type = typename std::iterator_traits<UnderlyingIter>::difference_type;
    using pointer = value_type*;
    using reference = value_type&;

    explicit filter_iterator(UnderlyingIter end, UnderlyingIter iter, Func func)
      : m_end(end), m_iter(get_next_valid_iter(iter)), m_func(func) {
      }

    filter_iterator(const filter_iterator&) = default;
    filter_iterator(filter_iterator&&) = default;
    filter_iterator& operator=(const filter_iterator&) = default;
    filter_iterator& operator=(filter_iterator&&) = default;
    ~filter_iterator() = default;

    auto operator++() -> filter_iterator& { 
        m_iter = get_next_valid_iter(++m_iter);
        return *this;
    }

    auto operator++(int) -> const filter_iterator {
        auto oldVal = *this;
        ++(*this);
        return oldVal;
    }

    auto operator*() -> value_type {
        return *m_iter;
    }

    friend bool operator==(const filter_iterator& lo, const filter_iterator& ro) {
        return (lo.m_iter == ro.m_iter);
    }

    friend bool operator!=(const filter_iterator& lo, const filter_iterator& ro) {
        return !(lo == ro);
    }

private:
    auto get_next_valid_iter(UnderlyingIter iter) -> UnderlyingIter {
        return std::find_if(iter, m_end, m_func);
    }

    UnderlyingIter m_end;
    UnderlyingIter m_iter;
    Func m_func;
};

template<typename Func>
auto filter(Func func) -> filter_adaptor<Func> {
    return filter_adaptor<Func>(func);
}

template<typename Range, 
         typename Func, 
         typename = std::enable_if_t<ranges::is_range_v<Range>>,
         typename = std::void_t<typename Range::value_type>,
         typename = std::enable_if_t<std::is_invocable_v<Func, typename Range::value_type>>>
auto operator|(const Range& range, filter_adaptor<Func> adaptor)
    -> ranges::range<filter_iterator<typename Range::const_iterator, Func>> {
    return ranges::range(filter_iterator<typename Range::const_iterator, Func>(range.cend(), range.cbegin(), adaptor.get_function()), 
                         filter_iterator<typename Range::const_iterator, Func>(range.cend(), range.cend(), adaptor.get_function()));
}

} // namespace my_view

#endif