#include <type_traits> // std::enable_if

namespace ranges {

// is_range
template<typename Range, typename Iter = void, typename Begin = void, typename End = void>
struct is_range : std::false_type {};
template<typename Range>
struct is_range<Range, 
                std::void_t<typename Range::const_iterator>,
                std::void_t<decltype(std::declval<Range>().begin())>,
                std::void_t<decltype(std::declval<Range>().end())>> 
        : std::true_type {};
template<typename Range> 
constexpr bool is_range_v = is_range<Range>::value;

// range
template<typename RangeIterator,
         typename = std::void_t<typename RangeIterator::iterator_category>>
class range {
public:
	using const_iterator = const RangeIterator;
	using value_type = typename RangeIterator::value_type;

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

}