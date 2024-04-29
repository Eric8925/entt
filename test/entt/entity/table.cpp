#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>
#include <gtest/gtest.h>
#include <entt/core/iterator.hpp>
#include <entt/entity/table.hpp>
#include "../../common/linter.hpp"

TEST(Table, Constructors) {
    entt::table<int, char> table;

    ASSERT_NO_THROW([[maybe_unused]] auto alloc = table.get_allocator());

    table = entt::table<int, char>{std::allocator<void>{}};

    ASSERT_NO_THROW([[maybe_unused]] auto alloc = table.get_allocator());
}

TEST(Table, Move) {
    entt::table<int, char> table;

    table.emplace(3, 'c');

    static_assert(std::is_move_constructible_v<decltype(table)>, "Move constructible type required");
    static_assert(std::is_move_assignable_v<decltype(table)>, "Move assignable type required");

    entt::table<int, char> other{std::move(table)};

    test::is_initialized(table);

    ASSERT_TRUE(table.empty());
    ASSERT_FALSE(other.empty());

    ASSERT_EQ(other[0u], std::make_tuple(3, 'c'));

    entt::table<int, char> extended{std::move(other), std::allocator<void>{}};

    test::is_initialized(other);

    ASSERT_TRUE(other.empty());
    ASSERT_FALSE(extended.empty());

    ASSERT_EQ(extended[0u], std::make_tuple(3, 'c'));

    table = std::move(extended);
    test::is_initialized(extended);

    ASSERT_FALSE(table.empty());
    ASSERT_TRUE(other.empty());
    ASSERT_TRUE(extended.empty());

    ASSERT_EQ(table[0u], std::make_tuple(3, 'c'));

    other = entt::table<int, char>{};
    other.emplace(1, 'a');
    other = std::move(table);
    test::is_initialized(table);

    ASSERT_TRUE(table.empty());
    ASSERT_FALSE(other.empty());

    ASSERT_EQ(other[0u], std::make_tuple(3, 'c'));
}
