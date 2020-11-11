#pragma once

#include <memory>

template<typename T>
struct DragContext {
    bool hasData = false;
    T data;
};
