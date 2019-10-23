/**
 * topurl
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

auto compare = [](const url_count_t& left, const url_count_t& right) {
    return left.second > right.second;
};

class Map {
public:
    Map();

    ~Map() = default;

    void reduce();

    void insert_url(std::size_t idx, const std::string& url);

    std::vector<url_count_t> topk();

private:
    // multiple hashtables to support concurrency.
    // <std::string, int32_t> -> <url,  emerged times>
    std::priority_queue<url_count_t, std::vector<url_count_t>, decltype(compare)> heap_{compare};
    std::vector<std::unordered_map<std::string, std::uint32_t>> maps_;
    // using mutexes to protect hashtables.
    std::vector<std::shared_ptr<std::mutex>> mus_;
};

END_NAMESPACE_TOPURL
