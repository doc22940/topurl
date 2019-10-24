/**
 * topurl - counter.h
 *
 * Licensed under the MIT License <https://opensource.org/licenses/MIT>.
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2019 Dillon <https://dillonzq.com>.
 */

#pragma once

#include "define.h"
#include "map.h"
#include "worker.h"

BEGIN_NAMESPACE_TOPURL

// Counter
//   Inheritance class from Worker for counting URL and recording in map
class Counter : public Worker {
public:
    Counter(int id, Buffer &buffer, std::shared_ptr<Map> map)
            : Worker(id, buffer), map_(std::move(map)) {}

    // work
    //   override Worker::work
    void work() override;

private:
    std::shared_ptr<Map> map_;
};

END_NAMESPACE_TOPURL