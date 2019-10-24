/**
 * topurl - reader.h
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

// Reader
//   Class for reading files
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

    // add_file
    //   add a file to file-list
    void add_file(const std::string& file);

    // read_file
    //   whether to successfully read a file
    bool read_file();

    // finish
    //   whether to finish reading
    bool finish() { return pos_ > file_size_; }

    // file_size
    //   return the reading file size
    std::int64_t file_size() { return file_size_; }

private:
    int fd_;                                               // current reading file fd
    std::int64_t file_size_;                               // current reading file size
    std::int64_t pos_;                                     // current reading position of the file
    std::int64_t next_read_;                               // current next read position of the file
    Buffer &buffer_;                                       // current reading buffer
    std::deque<std::string> files_;                        // file-list for reading
    std::shared_ptr<std::condition_variable> to_workers_;  // condition variable sending to workers
    std::shared_ptr<std::condition_variable> from_worker_; // condition variable from one worker
    std::shared_ptr<std::atomic_bool> stop_;               // bool variable whether reading stop
    std::mutex mu_;                                        // mutex for to_workers_

    // init_
    //   prepare the reader before reading
    void init_(const std::string &fp);
};

END_NAMESPACE_TOPURL
