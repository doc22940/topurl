/**
 * topurl
 *
 * Licensed under the MIT License <https://opensource.org/licenses/MIT>.
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2019 Dillon <https://dillonzq.com>.
 */

#include <functional>

#include "topurl/partitioner.h"

BEGIN_NAMESPACE_TOPURL

void Partitioner::work() {
    INFO("Partitioner%d starts", id_);
    if (start_ == end_) {
        INFO("Partitioner%d has no job to do, exit", id_);
        return;
    }

    std::int64_t pos = start_, pos_split;
    if ((start_ == 0) || (*(buffer_.buffer() + start_ - 1) == '\n')) {
    } else {
        pos_split = split(buffer_.buffer() + pos);
        pos += pos_split + 1;
    }
    std::hash<std::string> str_hash;
    while (pos < end_) {
        pos_split = split(buffer_.buffer() + pos);
        std::string url(buffer_.buffer() + pos, pos_split + 1);
        if (pos_split > MAX_URL_LEN) {
            ERROR("Get a invaild url, maybe input file is not compliant. >> %s "
                  "<< partitioner%d position %ld length %ld", url.c_str(), id_, long(pos), long(pos_split));
        } else {
            std::size_t hash_id = str_hash(url) % TEMPFILE_NUM;
            if (write(fds_[hash_id], url.c_str(), url.size()) != url.size()) {
                ERROR("Partical write or write failed, url >> %s << file_id %d",
                       url.c_str(), int(hash_id));
            }
        }
        pos += pos_split + 1;
        if (*(buffer_.buffer() + pos - 1) == EOF) {
            break;
        }
    }
    INFO("Partitioner%d is finished", id_);
}

END_NAMESPACE_TOPURL
