#include <iostream>
#include <chrono>

inline void fun(int x)
{
    while (x--);
}
int main()
{
    int k = 100000;
    auto start = std::chrono::high_resolution_clock::now();

        for (int j = 0; j < 10000; j++)
        {
            fun(k);

        }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    std::cout << "duration:" << duration << std::endl;
    return 0;
}
