/*******************************************************************************
 * tranform range, use as: range | transform(function)
 * Author: Yarin Hananiya
 * ****************************************************************************/
#ifndef TRANFORM_RANGE_HPP
#define TRANFORM_RANGE_HPP

#include <type_traits> // std::enable_if

#include "ranges.hpp" // ranges::range, ranges::is_range

namespace my_view {

template<typename Func>
class transform_adaptor {
public:
    transform_adaptor(Func func) : m_func(func) {}
    transform_adaptor(const transform_adaptor&) = default;
    transform_adaptor(transform_adaptor&&) = default;
    transform_adaptor& operator=(const transform_adaptor&) = default;
    transform_adaptor& operator=(transform_adaptor&&) = default;
    ~transform_adaptor() = default;

    constexpr auto get_function() -> Func {
        return m_func;
    }

private:
    Func m_func;
};

template<typename UnderlyingIter, typename Func>
class transform_iterator {
public:
    using iterator_category = typename std::iterator_traits<UnderlyingIter>::iterator_category;
    using value_type = std::invoke_result_t<Func, typename std::iterator_traits<UnderlyingIter>::value_type>;
    using difference_type = typename std::iterator_traits<UnderlyingIter>::difference_type;
    using pointer = value_type*;
    using reference = value_type&;

    explicit transform_iterator(UnderlyingIter iter, Func func)
      : m_iter(iter), m_func(func) {
    }

    transform_iterator(const transform_iterator&) = default;
    transform_iterator(transform_iterator&&) = default;
    transform_iterator& operator=(const transform_iterator&) = default;
    transform_iterator& operator=(transform_iterator&&) = default;
    ~transform_iterator() = default;

    auto operator++() -> transform_iterator& { 
        ++m_iter;
        return *this;
    }

    auto operator++(int) -> const transform_iterator {
        auto oldVal = *this;
        ++(*this);
        return oldVal;
    }

    auto operator*() -> value_type {
        return m_func(*m_iter);
    }

    friend bool operator==(const transform_iterator& lo, const transform_iterator& ro) {
        return (lo.m_iter == ro.m_iter);
    }

    friend bool operator!=(const transform_iterator& lo, const transform_iterator& ro) {
        return !(lo == ro);
    }

private:
    UnderlyingIter m_iter;
    Func m_func;
};

template<typename Func>
auto transform(Func func) -> transform_adaptor<Func> {
    return transform_adaptor<Func>(func);
}

template<typename Range, 
         typename Func, 
         typename = std::enable_if_t<ranges::is_range_v<Range>>,
         typename = std::void_t<typename Range::value_type>,
         typename = std::enable_if_t<std::is_invocable_v<Func, typename Range::value_type>>>
auto operator|(const Range& range, transform_adaptor<Func> adaptor) 
    -> ranges::range<transform_iterator<typename Range::const_iterator, Func>> {
    return ranges::range(transform_iterator<typename Range::const_iterator, Func>(std::cbegin(range), adaptor.get_function()), 
                         transform_iterator<typename Range::const_iterator, Func>(std::cend(range), adaptor.get_function()));
}

} // namespace my_view

#endif