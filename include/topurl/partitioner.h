/**
 * topurl - partitioner.h
 *
 * Licensed under the MIT License <https://opensource.org/licenses/MIT>.
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2019 Dillon <https://dillonzq.com>.
 */

#pragma once

#include <vector>

#include "define.h"
#include "worker.h"

BEGIN_NAMESPACE_TOPURL

// Partitioner
//   Inheritance class from Worker for partitioning URL file
class Partitioner : public Worker {
public:
    Partitioner(int id, Buffer &buffer, std::vector<std::int64_t> &fds)
                : Worker(id, buffer), fds_(fds) {}

    // work
    //   override Worker::work
    void work() override;

private:
    std::vector<std::int64_t> &fds_;
};

END_NAMESPACE_TOPURL
