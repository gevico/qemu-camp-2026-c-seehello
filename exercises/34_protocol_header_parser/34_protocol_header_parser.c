#include <stdint.h>
#include <stdio.h>

/*
 * 14 紧凑网络协议头解析器（位域）
 * 要求：
 *  - 协议头格式：2 字节版本（4 位主版本 + 4 位次版本）、2 字节长度（16 位无符号）、1 字节标志（3 位保留 + 5
 * 位功能标志）。
 *  - 输入示例：\x00\x03\x00\x20\x00  →  version:0.3, length:32, flags:0x00
 *  - 考察点：位域定义、结构体对齐（#pragma pack(1)）、字节流解析
 */

#pragma pack(push, 1)

typedef struct {
    uint8_t ver_minor : 4;
    uint8_t ver_major : 4;
} version_bits_t;

typedef struct {
    uint8_t flags : 5;
    uint8_t reserved : 3;
} flag_bits_t;

#pragma pack(pop)

int main(void) {
    const uint8_t stream[5] = {0x00, 0x03, 0x00, 0x20, 0x00};

    uint8_t version_low = stream[1];
    version_bits_t version = {
        .ver_minor = (uint8_t)(version_low & 0x0Fu),
        .ver_major = (uint8_t)((version_low >> 4) & 0x0Fu),
    };

    uint16_t length = (uint16_t)(((uint16_t)stream[2] << 8) | (uint16_t)stream[3]);

    flag_bits_t flag = {
        .flags = (uint8_t)(stream[4] & 0x1Fu),
        .reserved = (uint8_t)((stream[4] >> 5) & 0x07u),
    };

    printf("version:%u.%u, length:%u, flags:0x%02X\n", version.ver_major, version.ver_minor, length, flag.flags);

    return 0;
}
