/**
 * topurl
 *
 * Licensed under the MIT License <https://opensource.org/licenses/MIT>.
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2019 Dillon <https://dillonzq.com>.
 */

#pragma once

#include <cstdint>
#include <cstdlib>
#include <getopt.h>
#include <string>
#include <unistd.h>
#include <utility>

#define BEGIN_NAMESPACE_TOPURL namespace topurl {
#define END_NAMESPACE_TOPURL }
#define USE_NAMESPACE_TOPURL using namespace topurl;

BEGIN_NAMESPACE_TOPURL

#define MAX_URL_LEN 65536

static std::string URL_FILE = "url_file";
static int TOP_NUM = 100;
static int TEMPFILE_NUM = 512;
static std::int64_t BLOCK_SIZE = 256 * 1024 * 1024;
static int WORKER_NUM = 4;

void init_arg(int argc, char **argv);

#define INFO(format, ...)                   \
do {                                        \
    fprintf(stdout, "INFO: ");              \
    fprintf(stdout, format, ##__VA_ARGS__); \
    fprintf(stdout, "\n");                  \
} while(0)
#define ERROR(format, ...)                  \
do {                                        \
    fprintf(stderr, "ERROR: ");             \
    fprintf(stderr, format, ##__VA_ARGS__); \
    fprintf(stderr, "\n");                  \
} while(0)
#define FATAL(format, ...)                  \
do {                                        \
    fprintf(stderr, "FATAL: ");             \
    fprintf(stderr, format, ##__VA_ARGS__); \
    fprintf(stderr, "\n");                  \
    if (EXIT_FAILURE) exit(EXIT_FAILURE);  \
    else exit(1);                           \
} while(0)

typedef std::pair<std::int64_t, std::int64_t> task_t;
typedef std::pair<std::string, std::uint32_t> url_count_t;

std::int64_t split(const char *ch);

END_NAMESPACE_TOPURL
