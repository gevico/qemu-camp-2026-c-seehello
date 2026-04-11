#include <stdio.h>

unsigned int gcd_asm(unsigned int a, unsigned int b) {
    unsigned int result;
    
    __asm__ volatile (
        "mov %1, %%eax\n\t"       // eax = a
        "mov %2, %%ebx\n\t"       // ebx = b
        ".L_loop:\n\t"
        "test %%ebx, %%ebx\n\t"   // while (b != 0)
        "je .L_done\n\t"
        "mov %%ebx, %%ecx\n\t"    // ecx = old b
        "xor %%edx, %%edx\n\t"    // edx:eax 被除数高位清零
        "div %%ebx\n\t"           // eax = a / b, edx = a % b
        "mov %%ecx, %%eax\n\t"    // a = old b
        "mov %%edx, %%ebx\n\t"    // b = a % b
        "jmp .L_loop\n\t"
        ".L_done:\n\t"
        "mov %%eax, %0"             // 结果在 eax
        : "=r" (result)           // 输出操作数
        : "r" (a), "r" (b)      // 输入操作数
        : "eax", "ebx", "ecx", "edx", "cc" // 被修改的寄存器
    );
    
    return result;
}

int main(int argc, char* argv[]) {
    printf("%d\n", gcd_asm(12, 8));
    printf("%d\n", gcd_asm(7, 5));
    return 0;
}