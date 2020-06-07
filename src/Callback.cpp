#include "../include/Callback.h"

Callback::Callback(CallbackFun fun, EventType type, bool isActive)
    : type(type), isActive(isActive), fun(fun) {}


bool Callback::operator==(const CallbackId& id) {
    return this->id == id;
}
