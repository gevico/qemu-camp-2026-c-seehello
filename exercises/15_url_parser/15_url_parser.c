#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/**
 * URL参数解析器
 * 输入：包含http/https超链接的字符串
 * 输出：解析出所有的key-value键值对，每行显示一个
 */

int parse_url(const char* url) {
    int err = 0;
    int state = 0;

    if(url == NULL)
        return -1;

    while(*url != '\0')
    {
        if(*url == '\?' && state == 0)
        {
            state = 1;
            url++;
            continue;
        }

        if(state == 1)
        {
            printf("key = ");
            state = 2;
        }

        if(state == 2)
        {
            if(*url == '=')
            {
                url++;
                printf(", value = ");
                state = 3;
                continue;
            }

            printf("%c", *url);
        }

        if(state == 3)
        {
            if(*url == '&' || *url == '"' || *url == '\0')
            {
                state = 1;
                printf("\n");
                url++;
                continue;
            }
            printf("%c", *url);
        }

        url++;
    }

    return err;
}

int main() {
    const char* test_url = "https://cn.bing.com/search?name=John&age=30&city=New+York";

    printf("Parsing URL: %s\n", test_url);
    printf("Parameters:\n");

    parse_url(test_url);

    return 0;
}