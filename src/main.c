#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "json_lib.h"

char *escapeJsonStr(const char *js) {
    int i; 
    int len = strlen(js);
    char *output = malloc(len * 2 + 1);
    for (i = 0; i < len; i++) {
        switch (js[i]) {
            case '\\': 
                output[i] = '\\';
                output[i + 1] = '\\';
                break;
            case '\"':
                output[i] = '\\';
                output[i + 1] = '\"';
                break;
            case '\n':
                output[i] = '\\';
                output[i + 1] = '\n';
                break;
            case '\b':
                output[i] = '\\';
                output[i + 1] = '\b';
                break;
            case '\f':
                output[i] = '\\';
                output[i + 1] = '\f';
                break;
            case '\r':
                output[i] = '\\';
                output[i + 1] = '\r';
                break;
            case '\t':
                output[i] = '\\';
                output[i + 1] = '\t';
                break;
            default:
                break;
        }
    }

    output[len] = '\0';
    return output;
}

int main() {
    

    const char *str = "hello \n buddy \t guy";
    printf("str: %s", str);

    str = escapeJsonStr(str);
    printf("str: %s", str);

    return 0;
}
