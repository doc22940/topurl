/**
 * topurl - map.h
 *
 * Licensed under the MIT License <https://opensource.org/licenses/MIT>.
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2019 Dillon <https://dillonzq.com>.
 */

#pragma once

#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>
#include <queue>

#include "define.h"

BEGIN_NAMESPACE_TOPURL

// compare
//   lambda for construct heap_
auto compare = [](const url_count_t& left, const url_count_t& right) {
    return left.second > right.second;
};

// Map
//   class for counting and get top K URLs
class Map {
public:
    Map();

    ~Map() = default;

    // insert
    //   insert a URL into idx-map
    void insert(std::size_t idx, const std::string& url);

    // reduce
    //   use heap to reduce maps to top K URLs
    void reduce();

    // topk
    //   return top K URL-list
    std::vector<url_count_t> topk();

private:
    // heap to get top K URLs
    std::priority_queue<url_count_t, std::vector<url_count_t>, decltype(compare)> heap_{compare};
    // maps for different worker counting
    std::vector<std::unordered_map<std::string, std::uint32_t>> maps_;
    // mutexes to protect maps_
    std::vector<std::shared_ptr<std::mutex>> mus_;
};

END_NAMESPACE_TOPURL
