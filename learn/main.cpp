#include <iostream>
#include <chrono>

int main()
{
    auto start = std::chrono::high_resolution_clock::now();

        for (int j = 0; j < 1000; j++)
        {
            std::cout << j << std::endl;
        }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    std::cout << "duration:" << duration << std::endl;
    return 0;
}
