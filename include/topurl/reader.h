/**
 * topurl
 *
 * Licensed under the MIT License <https://opensource.org/licenses/MIT>.
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2019 Dillon <https://dillonzq.com>.
 */

#pragma once

#include <atomic>
#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>
#include <vector>

#include "buffer.h"
#include "define.h"

BEGIN_NAMESPACE_TOPURL

class Reader {
public:
    Reader(Buffer &buffer,
           std::shared_ptr<std::condition_variable> to_workers,
           std::shared_ptr<std::condition_variable> from_worker,
           std::shared_ptr<std::atomic_bool> stop)
           : fd_(0), file_size_(0), pos_(0), next_read_(0), buffer_(buffer),
             to_workers_(std::move(to_workers)),
             from_worker_(std::move(from_worker)),
             stop_(std::move(stop)) {}

    ~Reader();

    void add_file(const std::string& file);

    int read_file();

    bool finish() { return pos_ > file_size_; }

    std::int64_t file_size() { return file_size_; }

private:
    int fd_;
    std::int64_t file_size_;
    std::int64_t pos_;
    std::int64_t next_read_;
    Buffer &buffer_;
    std::deque<std::string> files_;
    std::shared_ptr<std::condition_variable> to_workers_;
    std::shared_ptr<std::condition_variable> from_worker_;
    std::shared_ptr<std::atomic_bool> stop_;
    std::mutex mu_;

    void init_(const std::string &fp);
};

END_NAMESPACE_TOPURL
