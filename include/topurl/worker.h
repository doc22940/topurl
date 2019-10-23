/**
 * topurl
 *
 * Licensed under the MIT License <https://opensource.org/licenses/MIT>.
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2019 Dillon <https://dillonzq.com>.
 */

#pragma once

#include <condition_variable>
#include <memory>

#include "buffer.h"
#include "define.h"

BEGIN_NAMESPACE_TOPURL

class Worker {
public:
    Worker(int id, Buffer &buf) : id_(id), buffer_(buf), start_(0), end_(0) {}

    ~Worker() = default;

    int id() { return id_; }

    bool worked() { return buffer_.worked(); }

    bool get_task();

    virtual void work() = 0;

protected:
    int id_;
    Buffer &buffer_;
    int64_t start_;
    int64_t end_;
};

static std::mutex mu;

void work_thread(Worker *worker,
                 const std::shared_ptr<std::condition_variable>& from_reader,
                 const std::shared_ptr<std::condition_variable>& to_reader,
                 const std::shared_ptr<std::atomic_bool>& stop);

END_NAMESPACE_TOPURL
