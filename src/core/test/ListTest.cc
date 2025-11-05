#include "List.h"

#include <boost/test/unit_test.hpp>
#include <ostream>

namespace kfl {
    template<typename T>
    std::ostream& operator<<(std::ostream& os, const List<T>& list) {
        return os << list.head(); // << ' ' << list.tail();
    }
}

using namespace kfl;

BOOST_AUTO_TEST_SUITE( ListTest )

BOOST_AUTO_TEST_CASE( EmptyPredicateReturnsTrueForEmptyList )
{
    const List<int> empty{};

    BOOST_TEST(empty.empty() == true);
}

BOOST_AUTO_TEST_CASE (EmptyPredicateReturnsFalseForNonEmptyList )
{
    const List<int> empty{};
    const List<int> list = empty.cons(42);

    BOOST_TEST(list.empty() == false);
}

BOOST_AUTO_TEST_CASE( EmptyListEqualsEmptyList )
{
    const List<int> empty1{};
    const List<int> empty2{};

    BOOST_TEST(empty1 == empty2);
}

BOOST_AUTO_TEST_CASE( EmptyListDoesNotEqualNonEmptyList )
{
    const List<int> empty{};
    const List<int> list = empty.cons(42);

    BOOST_TEST(empty != list);
}

BOOST_AUTO_TEST_CASE( ListsWithSameValuesAreEqual )
{
    const List<int> empty{};
    const List<int> list1 = empty.cons(42).cons(41).cons(40);
    const List<int> list2 = empty.cons(42).cons(41).cons(40);

    BOOST_TEST(list1 == list2);
}

BOOST_AUTO_TEST_CASE( ListsWithDifferentValuesAreNotEqual )
{
    const List<int> empty{};
    const List<int> list1 = empty.cons(42).cons(41).cons(40);
    const List<int> list2 = empty.cons(42).cons(99).cons(40);

    BOOST_TEST(list1 != list2);
}

BOOST_AUTO_TEST_CASE( ListsWithDifferentLengthsAreNotEqual )
{
    const List<int> empty{};
    const List<int> list1 = empty.cons(42).cons(41).cons(40);
    const List<int> list2 = empty.cons(42).cons(41);

    BOOST_TEST(list1 != list2);
}

BOOST_AUTO_TEST_CASE( HeadOfEmptyListThrowsException )
{
    const List<int> empty{};

    BOOST_CHECK_THROW(empty.head(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE( HeadOfNonEmptyListGivesLastConsValue )
{
    constexpr auto value = 42;
    const List<int> empty{};

    const auto list = empty.cons(value);

    BOOST_CHECK_EQUAL(list.head(), value);
}

BOOST_AUTO_TEST_CASE( TailOfEmptyListThrowsException )
{
    const List<int> empty{};

    BOOST_CHECK_THROW(empty.tail(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE( TailOfSingleItemListGivesEmptyList )
{
    constexpr auto value = 42;
    const List<int> empty{};

    const auto list = empty.cons(value);

    BOOST_CHECK_EQUAL(list.tail(), empty);
}

BOOST_AUTO_TEST_SUITE_END()
