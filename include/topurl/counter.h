/**
 * topurl
 *
 * Licensed under the MIT License <https://opensource.org/licenses/MIT>.
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2019 Dillon <https://dillonzq.com>.
 */

#pragma once

#include "define.h"
#include "map.h"
#include "worker.h"

BEGIN_NAMESPACE_TOPURL

class Counter : public Worker {
public:
    Counter(int id, Buffer &buffer, std::shared_ptr<Map> map)
            : Worker(id, buffer), map_(std::move(map)) {}

    void work() override;

private:
    std::shared_ptr<Map> map_;
};

END_NAMESPACE_TOPURL