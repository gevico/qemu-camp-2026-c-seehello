#include <stdio.h>
#include <limits.h>

#define CHECK_OVERFLOW(carry) \
    carry ? "Overflow" : "Not Overflow"

int check_add_overflow_asm(unsigned int a, unsigned int b) {
    unsigned char carry;
    __asm__ volatile(
        "mov %1, %%eax\n\t"
        "mov %2, %%ebx\n\t"
        "add %%ebx, %%eax\n\t"
        "setc %0"               // 将进位标志存入carry
        : "=r" (carry)           // 输出操作数
        : "r" (a), "r" (b)      // 输入操作数
        : "eax", "ebx", "cc"    // 被修改的寄存
    );
    return carry;
}

int check_sub_overflow_asm(unsigned int a, unsigned int b) {
    unsigned char carry;
    __asm__ volatile(
        "mov %1, %%eax\n\t"
        "mov %2, %%ebx\n\t"
        "sub %%ebx, %%eax\n\t"
        "setc %0"               // 将进位标志存入carry
        : "=r" (carry)           // 输出操作数
        : "r" (a), "r" (b)      // 输入操作数
        : "eax", "ebx", "cc"    // 被修改的寄存器
    );
    return carry;
}

int check_mul_overflow_asm(unsigned int a, unsigned int b) {
    unsigned int high_bits;
    unsigned char overflow;
    __asm__ volatile(
        "mov %2, %%eax\n\t"
        "mov %3, %%ebx\n\t"
        "mul %%ebx\n\t"          // edx:eax = eax * ebx
        "mov %%edx, %0\n\t"     // 将高位存入high_bits
        "setc %1"               // 将进位标志存入overflow
        : "=r" (high_bits), "=r" (overflow) // 输出操作数
        : "r" (a), "r" (b)      // 输入操作数
        : "eax", "ebx", "edx", "cc"    // 被修改的寄存器
    );
    return overflow || (high_bits != 0);
}

int check_div_overflow_asm(unsigned int a, unsigned int b) {
    unsigned char is_div_zero;
    __asm__ volatile(
        "mov $0, %0\n\t"          // 默认不是除零
        "mov %1, %%eax\n\t"
        "mov %2, %%ebx\n\t"
        "cmp $0, %%ebx\n\t"      // 检查除数是否为0
        "je .L_div_zero\n\t"
        "xor %%edx, %%edx\n\t"   // 清空edx，准备除法
        "div %%ebx\n\t"           // eax / ebx
        "jmp .L_done\n\t"
        ".L_div_zero:\n\t"
        "mov $1, %0\n\t"          // 设置is_div_zero为1
        ".L_done:\n\t"
        : "=r" (is_div_zero)     // 输出操作数
        : "r" (a), "r" (b)      // 输入操作数
        : "eax", "ebx", "edx", "cc"    // 被修改的寄存器
    );
    return is_div_zero;
}

int main() {
    printf("(UINT_MAX + 1)Add: %s\n", CHECK_OVERFLOW(check_add_overflow_asm(UINT_MAX, 1)));   // 1
    printf("(1, 0)Add: %s\n", CHECK_OVERFLOW(check_add_overflow_asm(1, 0)));  
    printf("(0, 1)Sub: %s\n", CHECK_OVERFLOW(check_sub_overflow_asm(0, 1)));          // 1
    printf("(2, 1)Sub: %s\n", CHECK_OVERFLOW(check_sub_overflow_asm(2, 1)));
    printf("(UINT_MAX, 2)Mul: %s\n", CHECK_OVERFLOW(check_mul_overflow_asm(UINT_MAX, 2)));   // 1
    printf("(1, 2)Mul: %s\n", CHECK_OVERFLOW(check_mul_overflow_asm(1, 2)));
    printf("(10, 0)Div: %s\n", CHECK_OVERFLOW(check_div_overflow_asm(10, 0)));                         // 1
    printf("(2, 1)Div: %s\n", CHECK_OVERFLOW(check_div_overflow_asm(2, 1)));
    return 0;
}