#include <thread>
#include <iostream>
#include <condition_variable>

void fun1()
{
    while (1)
        std::cout << "11111" << std::endl;
}

void fun2()
{
    while (1)
        std::cout << "2222" << std::endl;
}

int main()
{
    std::thread t1(fun1);
    std::thread t2(fun2);

    t1.detach();
    t2.detach();//主线程不会等待子线程结束，如果主线程结束，程序也会结束

    std::cout << "主线程" << std::endl;

    return 0;
}
