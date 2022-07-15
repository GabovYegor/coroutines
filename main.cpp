#include <iostream>

#include "simplest_coroutine.h"
#include "co_return_value_coroutine.h"
#include "resumable_coroutine.h"
#include "generator.h"
#include "generator_range.h"

int main() {
    // 1.
    {
        simplest_coroutine::coroutine();
    }

    // 2.
    {
        auto coro = co_return_value_coroutine::coroutine();
        std::cout << coro.get_value() << std::endl;
    }

    // 3.
    {
        auto coro = resumable_coroutine::coroutine();
        std::cout << "Start" << std::endl;
        coro.resume();
        std::cout << "Middle" << std::endl;
        coro.resume();
        std::cout << "Finish" << std::endl;
    }

    // 4.
    {
        auto generator = generator::coroutine();
        generator.move_next();
        std::cout << generator.current_value() << std::endl;
        generator.move_next();
        std::cout << generator.current_value() << std::endl;
        generator.move_next();
        std::cout << generator.current_value() << std::endl;
    }

    // 5.
    for(const auto i : generator_range::sequence(0, 10, 2))
    {
        std::cout << i << std::endl;
    }
    return 0;
}