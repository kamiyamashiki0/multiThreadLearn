#include "myThreadPool.cpp"
#include <iostream>
#include <chrono>
int i;
std::mutex mux;
void fun()
{
    std::lock_guard<std::mutex> lck(mux); // i是共享资源，需要加锁
    std::cout << i << std::endl;
    i++;
}

int main()
{
    auto start = std::chrono::high_resolution_clock::now();
    {
        myThreadPool<void> pool(16);
        for (int j = 0; j < 1000; j++)
        {
            pool.addTask(fun);
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    std::cout << "duration:" << duration << std::endl;
    return 0;
}
