#include <iostream>

/**
 * $ g++ test.cpp -Wall && ./a.exe
 */
int main()
{
    int response[7] = {0, 0, 0, 0x1, 0x1, 0, 0};

    uint32_t high = response[3];
    uint32_t low = response[4];
    std::cout << (high << 8) + low << std::endl;
}