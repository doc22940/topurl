/**
 * topurl - buffer.h
 *
 * Licensed under the MIT License <https://opensource.org/licenses/MIT>.
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2019 Dillon <https://dillonzq.com>.
 */

#pragma once

#include <atomic>
#include <deque>
#include <mutex>

#include "define.h"

BEGIN_NAMESPACE_TOPURL

// Buffer
//   Class for reading buffer
class Buffer {
public:
    explicit Buffer(const int64_t max_size) : max_size_(max_size), worked_(0) {
        buffer_ = (char *) malloc(max_size_ * sizeof(char));
    }

    ~Buffer() { free(buffer_); }

    // buffer
    //   return the buffer string
    char *buffer() { return buffer_; }

    // is_last
    //   whether the task to work is the last one
    bool is_last() {
        return worked_.fetch_add(1, std::memory_order_acq_rel) == WORKER_NUM - 1;
    };

    // read
    //   read some length of buffer and make task-list
    void read(const std::int64_t read_length) {
        std::int64_t intervel = read_length / WORKER_NUM, start, end;

        std::unique_lock<std::mutex> guard(mu_);
        worked_.store(0, std::memory_order_release);
        for (int i = 0; i < WORKER_NUM; i++) {
            start = intervel * i;
            end = (i == WORKER_NUM - 1) ? read_length : (i + 1) * intervel - 1;
            tasks_.emplace_back(std::make_pair(start, end));
        }
    }

    // pop_task
    //   whether to successfully pop a task from task-list
    bool pop_task(task_t &task) {
        task.first = 0;
        task.second = 0;

        std::unique_lock<std::mutex> guard(mu_);
        if (tasks_.empty()) {
            return false;
        }
        task.first = tasks_.front().first;
        task.second = tasks_.front().second;
        tasks_.pop_front();
        return true;
    }

private:
    char *buffer_;             // buffer string
    std::int64_t max_size_;    // buffer max size
    std::deque<task_t> tasks_; // task-list
    std::atomic_int worked_;   // worked task number
    std::mutex mu_;            // unique_lock mutex for tasks_
};

END_NAMESPACE_TOPURL
