/**
 * topurl
 *
 * Licensed under the MIT License <https://opensource.org/licenses/MIT>.
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2019 Dillon <https://dillonzq.com>.
 */

#include "topurl/worker.h"

BEGIN_NAMESPACE_TOPURL

bool Worker::get_task() {
    std::pair<int64_t, int64_t> task;
    bool get = buffer_.pop_task(task);
    if (get) {
        start_ = task.first;
        end_ = task.second;
    }
    return get;
}

void work_thread(Worker *worker,
                 const std::shared_ptr<std::condition_variable>& from_reader,
                 const std::shared_ptr<std::condition_variable>& to_reader,
                 const std::shared_ptr<std::atomic_bool>& stop) {
    bool the_last = false;
    while (!stop->load(std::memory_order_acquire)) {
        {
            std::unique_lock<std::mutex> garud(mu);
            if (the_last) {
                to_reader->notify_one();
                the_last = false;
            }
            from_reader->wait(garud);
        }
        if (stop->load(std::memory_order_acquire)) {
            break;
        }
        while (worker->get_task()) {
            worker->work();
            the_last = worker->worked();
        }
    }
}

END_NAMESPACE_TOPURL
