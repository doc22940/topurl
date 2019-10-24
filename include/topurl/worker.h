/**
 * topurl - worker.h
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

// Worker
//   Abstract class for worder thread
class Worker {
public:
    Worker(int id, Buffer &buf) : id_(id), buffer_(buf), start_(0), end_(0) {}

    ~Worker() = default;

    // id
    //   return the id of the worker
    int id() { return id_; }

    // is_last
    //   whether the worker thread is the last one
    bool is_last() { return buffer_.is_last(); }

    // get_task
    //   whether to successfully get a task from task-list and fill start_ and end_
    bool get_task();

    // work
    //   pure virtual function
    virtual void work() = 0;

protected:
    int id_;         // worker id
    Buffer &buffer_; // work buffer
    int64_t start_;  // start position of work buffer
    int64_t end_;    // end position of work buffer
};

// Global Mutex for worker thread synchronization
static std::mutex mu;

// work_thread
//   to construct the work thread
void work_thread(Worker *worker,
                 const std::shared_ptr<std::condition_variable>& from_reader,
                 const std::shared_ptr<std::condition_variable>& to_reader,
                 const std::shared_ptr<std::atomic_bool>& stop);

END_NAMESPACE_TOPURL
