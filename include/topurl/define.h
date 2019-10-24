/**
 * topurl - define.h
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

// BEGIN_NAMESPACE_TOPURL
// END_NAMESPACE_TOPURL
// USE_NAMESPACE_TOPURL
//   macros for namespace topurl
#define BEGIN_NAMESPACE_TOPURL namespace topurl {
#define END_NAMESPACE_TOPURL }
#define USE_NAMESPACE_TOPURL using namespace topurl;

BEGIN_NAMESPACE_TOPURL

// INFO
// ERROR
// FATAL
//   macros for logging
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

#define MAX_URL_LEN 65536                          // max URL length
static std::string URL_FILE = "url_file";           // URL file path
static int TOP_NUM = 100;                           // top K
static int TEMPFILE_NUM = 512;                      // temp files number
static std::int64_t BLOCK_SIZE = 256 * 1024 * 1024; // block size
static int WORKER_NUM = 4;                          // workers number

// init_arg
//   init args
void init_arg(int argc, char **argv);

// split
//   split the string by '\n' and return the split position
std::int64_t split(const char *ch);

END_NAMESPACE_TOPURL
