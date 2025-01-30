#include "myThreadPool.cpp"
#include <iostream>
#include <chrono>
// int i;
// std::mutex mux;
// void fun()
// {
//     std::lock_guard<std::mutex> lck(mux); // i是共享资源，需要加锁
//     std::cout << i << std::endl;
//     i++;
// }
void fun(int x)
{

    while(x--);
}

int main()
{
    int k = 100000;
    auto start = std::chrono::high_resolution_clock::now();
    {  
        myThreadPool<void> pool(8);
        for (int j = 0; j < 10000; j++)
        {
            pool.addTask(fun,k);
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    std::cout << "duration:" << duration << std::endl;
    return 0;
}
