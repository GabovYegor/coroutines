#include <iostream>
#include <coroutine>

struct Promise;

struct CoroReturnType {
    using promise_type = Promise;
};

struct Promise {
    auto get_return_object() noexcept { return CoroReturnType{}; }

    auto initial_suspend() noexcept { return std::suspend_never(); }

    auto final_suspend() noexcept { return std::suspend_never(); }

    void return_void() {}

    void unhandled_exception() {}
};

CoroReturnType coroutine() {
    co_return;
}

int main() {
    coroutine();
    return 0;
}