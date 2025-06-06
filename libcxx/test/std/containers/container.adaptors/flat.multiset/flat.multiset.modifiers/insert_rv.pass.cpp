//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// class flat_multiset

// iterator insert(value_type&& v);

#include <flat_set>
#include <cassert>
#include <deque>

#include "MinSequenceContainer.h"
#include "MoveOnly.h"
#include "min_allocator.h"
#include "test_macros.h"
#include "../helpers.h"

template <class KeyContainer>
void test_one() {
  using Key = typename KeyContainer::value_type;
  using M   = std::flat_multiset<Key, TransparentComparator, KeyContainer>;
  using R   = typename M::iterator;
  using V   = typename M::value_type;

  M m;
  std::same_as<R> decltype(auto) r = m.insert(V(2));
  assert(r == m.begin());
  assert(m.size() == 1);
  assert(*r == V(2));

  r = m.insert(V(1));
  assert(r == m.begin());
  assert(m.size() == 2);
  assert(*r == V(1));

  r = m.insert(V(3));
  assert(r == std::ranges::prev(m.end()));
  assert(m.size() == 3);
  assert(*r == V(3));

  r = m.insert(V(3));
  assert(r == std::ranges::prev(m.end()));
  assert(m.size() == 4);
  assert(*r == V(3));

  r = m.insert(V(2));
  assert(r == m.begin() + 2);
  assert(m.size() == 5);
  assert(*r == V(2));

  r = m.insert(V(1));
  assert(r == m.begin() + 1);
  assert(m.size() == 6);
  assert(*r == V(1));
}

void test() {
  test_one<std::vector<int>>();
  test_one<std::vector<MoveOnly>>();
  test_one<std::deque<int>>();
  test_one<std::deque<MoveOnly>>();
  test_one<MinSequenceContainer<int>>();
  test_one<MinSequenceContainer<MoveOnly>>();
  test_one<std::vector<int, min_allocator<int>>>();
  test_one<std::vector<MoveOnly, min_allocator<MoveOnly>>>();
}

void test_exception() {
  auto insert_func = [](auto& m, auto key_arg) {
    using FlatSet    = std::decay_t<decltype(m)>;
    using value_type = typename FlatSet::value_type;
    value_type p(key_arg);
    m.insert(std::move(p));
  };
  test_emplace_exception_guarantee(insert_func);
}

int main(int, char**) {
  test();
  test_exception();

  return 0;
}
