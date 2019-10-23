#pragma once

#include <vector>

#include "define.h"
#include "worker.h"

BEGIN_NAMESPACE_TOPURL

class Partitioner : public Worker {
public:
    Partitioner(int id, Buffer &buffer, std::vector<std::int64_t> &fds)
                : Worker(id, buffer), fds_(fds) {}

    void work() override;

private:
    std::vector<std::int64_t> &fds_;
};

END_NAMESPACE_TOPURL
