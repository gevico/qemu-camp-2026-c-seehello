#include <stdint.h>
#include <stdio.h>

uint32_t swap_endian(uint32_t num) {
    uint32_t byte0 = (num & 0x000000FF) << 24; // 最低字节移到最高位
    uint32_t byte1 = (num & 0x0000FF00) << 8;  // 第二字节移到次高位
    uint32_t byte2 = (num & 0x00FF0000) >> 8;  // 第三字节移到次低位
    uint32_t byte3 = (num & 0xFF000000) >> 24; // 最高字节移到最低位
    return byte0 | byte1 | byte2 | byte3;      // 合并所有字节
}

int main(int argc, char* argv[]) {
    uint32_t num = 0x78563412;
    uint32_t swapped = swap_endian(num);
    printf("0x%08x -> 0x%08x\n", num, swapped);
    return 0;
}