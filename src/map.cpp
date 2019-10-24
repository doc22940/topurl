/**
 * topurl - map.cpp
 *
 * Licensed under the MIT License <https://opensource.org/licenses/MIT>.
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2019 Dillon <https://dillonzq.com>.
 */

#include "topurl/map.h"

BEGIN_NAMESPACE_TOPURL

Map::Map() {
    // construct map-list
    maps_.resize(WORKER_NUM);
    for (int i = 0; i < WORKER_NUM; i++) {
        std::shared_ptr<std::mutex> p = std::make_shared<std::mutex>();
        mus_.emplace_back(p);
    }
}

// Map::insert
//   insert a URL into idx-map
void Map::insert(std::size_t idx, const std::string& url) {
    std::lock_guard<std::mutex> guard(*(mus_[idx]));
    if (maps_[idx].find(url) == maps_[idx].end()) {
        maps_[idx][url] = 1;
    } else {
        maps_[idx][url] += 1;
    }
}

// Map::reduce
//   use heap to reduce maps to top K URLs
void Map::reduce() {
    INFO("Reduce starts");
    for (auto & map : maps_) {
        for (auto & it : map) {
            if (heap_.size() < TOP_NUM) {
                heap_.emplace(std::make_pair(it.first, it.second));
            } else if (heap_.top().second < it.second) {
                heap_.pop();
                heap_.emplace(std::make_pair(it.first, it.second));
            }
        }
    }
    INFO("Reduce is finished");

    // clear map-list
    for (int i = 0; i < WORKER_NUM; ++i) {
        std::unordered_map<std::string, std::uint32_t> tmp_map;
        {
            std::lock_guard<std::mutex> guard(*(mus_[i]));
            std::swap(tmp_map, maps_[i]);
        }
    }
}

// Map::topk
//   return top K URL-list
std::vector<url_count_t> Map::topk() {
    std::vector<url_count_t> res;
    while(!heap_.empty()) {
        res.push_back(heap_.top());
        heap_.pop();
    }
    return res;
}

END_NAMESPACE_TOPURL
