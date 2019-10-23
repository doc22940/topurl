/**
 * topurl
 *
 * Licensed under the MIT License <https://opensource.org/licenses/MIT>.
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2019 Dillon <https://dillonzq.com>.
 */

#include <atomic>
#include <fcntl.h>
#include <iostream>
#include <memory.h>
#include <mutex>
#include <thread>

#include "topurl/buffer.h"
#include "topurl/counter.h"
#include "topurl/map.h"
#include "topurl/reader.h"
#include "topurl/partitioner.h"

USE_NAMESPACE_TOPURL

int main(int argc, char **argv) {
    init_arg(argc, argv);

    Buffer buffer(BLOCK_SIZE + MAX_URL_LEN);
    auto to_works = std::make_shared<std::condition_variable>();
    auto from_work = std::make_shared<std::condition_variable>();
    auto stop = std::make_shared<std::atomic_bool>();
    auto map = std::make_shared<Map>();

    Reader reader(buffer, to_works, from_work, stop);

    reader.add_file(URL_FILE);

    std::vector<int64_t> fds;
    std::vector<std::string> temp_files;
    for (int i = 0; i < TEMPFILE_NUM; i++) {
        std::string temp_file = "temp/" + URL_FILE + "_" + std::to_string(i);
        temp_files.emplace_back(temp_file);

        int fd = open(temp_file.c_str(), O_RDWR | O_CREAT, 0777);
        if (fd == -1) {
            FATAL("Create temp file %d failed", i);
        }
        fds.emplace_back(fd);
    }

    std::vector<Worker *> workers;
    std::vector<std::thread> partition_threads;
    partition_threads.reserve(WORKER_NUM);
    for (int i = 0; i < WORKER_NUM; i++) {
        Worker *worker = new Partitioner(i, buffer, fds);
        INFO("Construct partitioner%d", i);
        workers.emplace_back(worker);
        partition_threads.emplace_back(std::move(std::thread(work_thread, workers[i], to_works, from_work, stop)));
    }

    reader.read_file();

    for (int i = 0; i < WORKER_NUM; i++) {
        partition_threads[i].join();
    }
    for (int i = 0; i < TEMPFILE_NUM; i++) {
        if (close(fds[i]) == -1) {
            FATAL("FATAL: Close temp file failed");
        }
    }

    for (int i = 0; i < TEMPFILE_NUM; i++) {
        reader.add_file(temp_files[i]);
    }
    temp_files.clear();

    std::vector<std::thread> count_threads;
    count_threads.reserve(WORKER_NUM);
    for (int i = 0; i < WORKER_NUM; i++) {
        Worker *worker = new Counter(i, buffer, map);
        INFO("Construct counter%d", i);
        free(workers[i]);
        workers[i] = worker;
        count_threads.emplace_back(std::move(std::thread(work_thread, workers[i], to_works, from_work, stop)));
    }

    while (reader.read_file() == 1) {
        map->reduce();
    }

    for (int i = 0; i < WORKER_NUM; i++) {
        count_threads[i].join();
        free(workers[i]);
    }

    std::vector<url_count_t> topk = map->topk();
    for (auto &it : topk) {
        std::cout << it.first << " " << it.second << std::endl;
    }

    return 0;
}
