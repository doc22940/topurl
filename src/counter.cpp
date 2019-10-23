/**
 * topurl
 *
 * Licensed under the MIT License <https://opensource.org/licenses/MIT>.
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2019 Dillon <https://dillonzq.com>.
 */

#include "topurl/counter.h"

BEGIN_NAMESPACE_TOPURL

void Counter::work() {
    INFO("Counter%d starts working", id_);
    if (start_ == end_) {
        INFO("Counter%d has no job to do, exit", id_);
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
        std::string url(buffer_.buffer() + pos, pos_split);
        if (pos_split > MAX_URL_LEN) {
            ERROR("Get a invaild URL, maybe input file is not compliant. >> %s "
                  "<< Counter%d, position %ld length %ld", url.c_str(), id_, long(pos), long(pos_split));
        } else {
            std::size_t hash_id = (str_hash(url) >> std::size_t(2)) % WORKER_NUM;
            map_->insert_url(hash_id, url);
        }
        pos += pos_split + 1;
        if ((*(buffer_.buffer() + pos - 1)) == EOF) {
            break;
        }
    }
    INFO("Counter%d finishs", id_);
}

END_NAMESPACE_TOPURL
