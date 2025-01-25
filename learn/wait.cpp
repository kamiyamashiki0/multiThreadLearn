#include <thread>
#include <condition_variable>
#include <mutex>
#include <iostream>

std::condition_variable cv;
std::mutex mux;
int cargo = 0;

void consum(int n)
{
    for (int i = 0; i < n;i++)
    {
        std::unique_lock<std::mutex> lck(mux);
        cv.wait(lck, []()
                { return cargo != 0; });//第二个参数为false时才会阻塞，当被其他线程唤醒时，若第二个参数仍为false，则继续阻塞
        std::cout << cargo << std::endl;
        cargo = 0;
    }
}

int main()
{
    std::thread t1(consum, 10);
    for (int i = 0; i < 10;i++)
    {
        while(cargo!=0)
            std::this_thread::yield();
        std::unique_lock<std::mutex> lck(mux);
        cargo=i+1;
        cv.notify_one();
    }

    t1.join();
    return 0;
}
