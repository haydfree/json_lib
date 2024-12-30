#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "json_lib.h"

char *escapeJsonStr(const char *js) {
    int i, j = 0; 
    int len = strlen(js);
    char *output = malloc(len * 2 + 1);
    for (i = 0; i < len; i++) {
        switch (js[i]) {
            case '\\': 
                output[j++] = '\\';
                output[j++] = '\\';
                break;
            case '\"':
                output[j++] = '\\';
                output[j++] = '\"';
                break;
            case '\n':
                output[j++] = '\\';
                output[j++] = '\n';
                break;
            case '\b':
                output[j++] = '\\';
                output[j++] = '\b';
                break;
            case '\f':
                output[j++] = '\\';
                output[j++] = '\f';
                break;
            case '\r':
                output[j++] = '\\';
                output[j++] = '\r';
                break;
            case '\t':
                output[j++] = '\\';
                output[j++] = '\t';
                break;
            default:
                output[j++] = js[i];
                break;
        }
    }

    output[j] = '\0';
    return output;
}

int main() {

    const char *str = "hello \n buddy \t guy";
    str = escapeJsonStr(str);

    int i;
    for (i = 0; i < (int) strlen(str); i++) {
        printf("char: %c\n", str[i]);
    }

    return 0;
}
