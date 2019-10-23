/**
 * topurl
 *
 * Licensed under the MIT License <https://opensource.org/licenses/MIT>.
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2019 Dillon <https://dillonzq.com>.
 */

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include "topurl/reader.h"

BEGIN_NAMESPACE_TOPURL

Reader::~Reader() {
    if (close(fd_) == -1) {
        FATAL("Close file failed");
    }
}

void Reader::init_(const std::string &fp) {
    if (fd_ > 0) {
        if (close(fd_) == -1) {
            FATAL("Close file failed");
        }
    }
    pos_ = 0;
    struct stat statbuf{};
    if (stat(fp.c_str(), &statbuf) != 0) {
        FATAL("Get file size failed");
    }
    file_size_ = statbuf.st_size;
    next_read_ = (BLOCK_SIZE < file_size_) ? BLOCK_SIZE + MAX_URL_LEN
                                           : file_size_ + MAX_URL_LEN;
    fd_ = open(fp.c_str(), O_RDONLY);
    if (fd_ == -1) {
        FATAL("Open file failed");
    }
}

void Reader::add_file(const std::string& file) {
    if (files_.empty()) {
        stop_->store(false, std::memory_order_release);
    }
    files_.emplace_back(file);
}

int Reader::read_file() {
    if (files_.empty()) {
        return -1;
    }
    std::string &file = files_.front();
    init_(file);
    INFO("Read %s", file.c_str());
    files_.pop_front();
    if (file_size_ == 0) {
        if (files_.empty()) {
            stop_->store(true, std::memory_order_release);
            to_workers_->notify_all();
        }
        return 1;
    }
    while (pos_ < file_size_) {
        INFO("Read length %ld", long(next_read_));
        if (read(fd_, buffer_.buffer(), next_read_) == -1) {
            FATAL("Read file failed >> position: %ld, length: %ld <<", long(pos_), long(next_read_));
        }
        pos_ += next_read_ + 1;
        buffer_.has_read(next_read_ - MAX_URL_LEN);
        next_read_ = (BLOCK_SIZE < file_size_ - pos_ + 1)
                     ? BLOCK_SIZE
                     : file_size_ - pos_ + 1;
        next_read_ += MAX_URL_LEN;
        to_workers_->notify_all();
        {
            std::unique_lock<std::mutex> guard(mu_);
            from_worker_->wait(guard);
        }
    }
    if (files_.empty()) {
        stop_->store(true, std::memory_order_release);
        to_workers_->notify_all();
    }
    return 1;
}

END_NAMESPACE_TOPURL
