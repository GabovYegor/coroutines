#include <iostream>

#include "simplest_coroutine.h"
#include "co_return_value_coroutine.h"

int main() {
    simplest_coroutine::coroutine();
    auto coro = co_return_value_coroutine::coroutine();
    std::cout << coro.get_value();
    return 0;
}