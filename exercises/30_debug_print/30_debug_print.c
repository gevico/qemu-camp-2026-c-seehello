#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>


#ifdef DEBUG_LEVEL
#if DEBUG_LEVEL == 1
#define DEBUG_PRINT(fmt, ...) \
    printf("DEBUG: " fmt "\n", ##__VA_ARGS__)
#elif DEBUG_LEVEL == 2
#define DEBUG_PRINT(fmt, ...) \
    printf("DEBUG: func=%s, line=%d, " fmt "\n", __func__, __LINE__, ##__VA_ARGS__)
#elif DEBUG_LEVEL == 3
#define DEBUG_PRINT(fmt, ...) \
    do { \
        printf("DEBUG: func=%s, line=%d, " fmt "\n", __func__, __LINE__, ##__VA_ARGS__); \
        void *buffer[100]; \
        int nptrs = backtrace(buffer, 100); \
        backtrace_symbols_fd(buffer, nptrs, fileno(stderr)); \
    } while (0)
#endif
#else
#define DEBUG_PRINT(fmt, ...) do {} while (0)
#endif




















// MUST ENSURE DEBUG_PRINT("x=%d", x) is at line 48.
void test(void) {
    int x = 42;
    DEBUG_PRINT("x=%d", x);
}

int main(void) {
    test();
    return 0;
}
