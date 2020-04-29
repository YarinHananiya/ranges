/*******************************************************************************
 * generic range
 * Author: Yarin Hananiya
 * ****************************************************************************/
#ifndef RANGES_HPP
#define RANGES_HPP

#include <type_traits> // std::enable_if

namespace ranges {

// is_range
template<typename Range, typename Iter = void, typename Begin = void, typename End = void>
struct is_range : std::false_type {};
template<typename Range>
struct is_range<Range, 
                std::void_t<typename Range::const_iterator>,
                std::void_t<decltype(std::cbegin(std::declval<Range>()))>,
                std::void_t<decltype(std::cend(std::declval<Range>()))>> 
        : std::true_type {};
template<typename Range> 
constexpr bool is_range_v = is_range<Range>::value;

// range
template<typename RangeIterator,
         typename = std::void_t<typename std::iterator_traits<RangeIterator>::iterator_category>>
class range {
public:
	explicit range(RangeIterator begin_iter, RangeIterator end_iter)
      : m_begin_iter(begin_iter), m_end_iter(end_iter) {
    }

    range(const range&) = default;
    range(range&&) = default;
    range& operator=(const range&) = default;
    range& operator=(range&&) = default;
    ~range() = default;

    auto begin() -> RangeIterator {
        return m_begin_iter;
    }

    auto end() -> RangeIterator {
        return m_end_iter;
    }

private:
    RangeIterator m_begin_iter;
    RangeIterator m_end_iter;
};

} // namespace ranges

#endif