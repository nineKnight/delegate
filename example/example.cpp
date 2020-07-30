#include <iostream>

#include "nineknight/delegate.hpp"

using namespace nineknight;

void add(delegate<void(uint32_t, std::string)> &onEvent)
{
    std::string lambda{"hello world"};
    auto func = std::bind([](std::string lambda, uint32_t,
                             std::string) { std::cout << lambda << std::endl; },
                          lambda, std::placeholders::_1, std::placeholders::_2);
    onEvent += func;
}

void del(delegate<void(uint32_t, std::string)> &onEvent)
{
    std::string lambda{"hello world"};
    auto func = std::bind([](std::string lambda, uint32_t,
                             std::string) { std::cout << lambda << std::endl; },
                          lambda, std::placeholders::_1, std::placeholders::_2);
    onEvent -= func;
}

int main()
{
    delegate<void(uint32_t, std::string)> onEvent;

    std::string lambda{"hello world"};

    auto func1 = [&](uint32_t, std::string) {
        std::cout << lambda << std::endl;
    };
    onEvent += func1;
    // onEvent -= func1;   // this can remove func1
    onEvent -= [&](uint32_t, std::string) {
        std::cout << lambda << std::endl;
    };    // this can't remove func1

    auto func2 =
        std::bind([](std::string lambda, uint32_t,
                     std::string) { std::cout << lambda << std::endl; },
                  lambda, std::placeholders::_1, std::placeholders::_2);
    onEvent += func2;
    // onEvent -= func2;   // this can remove func2
    onEvent -= std::bind([](std::string lambda, uint32_t,
                            std::string) { std::cout << lambda << std::endl; },
                         lambda, std::placeholders::_1,
                         std::placeholders::_2);    // this can't remove func2

    add(onEvent);
    del(onEvent);    // this can't remove the function that was added in `add`

    add(onEvent);    // this won't add the function in `add`, duplicated
    del(onEvent);

    // call delegate
    onEvent(12345, "hello");

    return 0;
}