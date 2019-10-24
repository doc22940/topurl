/**
 * topurl - define.cpp
 *
 * Licensed under the MIT License <https://opensource.org/licenses/MIT>.
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2019 Dillon <https://dillonzq.com>.
 */

#include "topurl/define.h"

BEGIN_NAMESPACE_TOPURL

// init_arg
//   init args
void init_arg(int argc, char **argv) {
    const char *optstring = "f:T:t:b:w:vh";
    int c;
    option opts[] = {
            {"file", 1, nullptr, 'f'},
            {"top", 1, nullptr, 'T'},
            {"temp", 1, nullptr, 't'},
            {"block", 1, nullptr, 'b'},
            {"worker", 1, nullptr, 'w'},
            {"version", 0, nullptr, 'v'},
            {"help", 0, nullptr, 'h'}
    };

    while((c = getopt_long(argc, argv, optstring, opts, nullptr)) != -1) {
        switch(c) {
            case 'f':
                URL_FILE = optarg;
                break;
            case 'T':
                TOP_NUM = std::stoi(optarg);
                if (TOP_NUM > 1024 || TOP_NUM <= 0) FATAL("Invalid args");
                break;
            case 't':
                TEMPFILE_NUM = std::stoi(optarg);
                if (TEMPFILE_NUM > 4096 || TEMPFILE_NUM <= 0) FATAL("Invalid args");
                break;
            case 'b':
                BLOCK_SIZE = std::stoi(optarg);
                if (BLOCK_SIZE > 512 * 1024 * 1024 || BLOCK_SIZE <= 0) FATAL("Invalid args");
                break;
            case 'w':
                WORKER_NUM = std::stoi(optarg);
                if (WORKER_NUM > 128 || WORKER_NUM <= 0) FATAL("Invalid args");
                break;
            case 'v':
                printf("version is 0.0.1\n");
                break;
            case 'h':
                printf("this is help\n");
                break;
            case '?':
                printf("unknown option\n");
                break;
            default:
                printf("-----------\n");
        }
    }
}

// split
//   split the string by '\n' and return the split position
std::int64_t split(const char *ch) {
    std::int64_t pos_split = 0;
    while ((*(ch + pos_split) != '\n') && (*(ch + pos_split) != EOF)) {
        pos_split++;
    }
    return pos_split;
}

END_NAMESPACE_TOPURL
