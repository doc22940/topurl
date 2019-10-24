/**
 * topurl - worker.cpp
 *
 * Licensed under the MIT License <https://opensource.org/licenses/MIT>.
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2019 Dillon <https://dillonzq.com>.
 */

#include "topurl/worker.h"

BEGIN_NAMESPACE_TOPURL

// Worker::get_task
//   whether to successfully get a task from task-list and fill start_ and end_
bool Worker::get_task() {
    std::pair<int64_t, int64_t> task;
    bool get = buffer_.pop_task(task);
    if (get) {
        start_ = task.first;
        end_ = task.second;
    }
    return get;
}

// work_thread
//   to construct the work thread
void work_thread(Worker *worker,
                 const std::shared_ptr<std::condition_variable>& from_reader,
                 const std::shared_ptr<std::condition_variable>& to_reader,
                 const std::shared_ptr<std::atomic_bool>& stop) {
    // used to notify the reader to flush the buffer
    bool is_last = false;
    // until notified by the reader when the reading is finished
    while (!stop->load(std::memory_order_acquire)) {
        {
            std::unique_lock<std::mutex> garud(mu);
            if (is_last) {
                to_reader->notify_one();
                is_last = false;
            }
            // waiting for the reader to work
            from_reader->wait(garud);
        }
        if (stop->load(std::memory_order_acquire)) {
            break;
        }
        // get task from task-list to work
        while (worker->get_task()) {
            worker->work();
            is_last = worker->is_last();
        }
    }
}

END_NAMESPACE_TOPURL
