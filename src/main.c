#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "json_lib.h"
#include <ctype.h>

JsonObject *initJsonObject() {
    JsonObject *jo = malloc(sizeof(JsonObject));
    return jo;
}

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
                output[j++] = '"';
                break;
            case '\n':
                output[j++] = '\\';
                output[j++] = 'n';
                break;
            case '\b':
                output[j++] = '\\';
                output[j++] = 'b';
                break;
            case '\f':
                output[j++] = '\\';
                output[j++] = 'f';
                break;
            case '\r':
                output[j++] = '\\';
                output[j++] = 'r';
                break;
            case '\t':
                output[j++] = '\\';
                output[j++] = 't';
                break;
            default:
                output[j++] = js[i];
                break;
        }
    }

    output[j] = '\0';
    return output;
}

static const char *skipWhitespace(const char *js, int *idx) {
    while (js[*idx] == ' ') {
        (*idx)++;
    }
    return js + (*idx);
}

static char *strndup(const char *s, size_t n) {
    size_t len = 0;
    while (len < n && s[len] != '\0') {
        len++;
    }
    char *copy = malloc(len + 1); 
    if (!copy) return NULL;
    size_t i;
    for (i = 0; i < len; i++) {
        copy[i] = s[i];
    }
    copy[len] = '\0';
    return copy;
}

static int *skipChar(int *idx) {
    *idx = *idx + 1;
    return idx;
}

static char *parseNum(const char *js, int *idx) {
    int start = *idx;
    skipWhitespace(js, idx);
    if (!(isdigit(js[*idx]) || js[*idx] == '-' || js[*idx] == '.')) { printf("first char should be num or - or . \n"); exit(1); } 
    while (isdigit(js[*idx]) || js[*idx] == '-' || js[*idx] == '.') { skipChar(idx); }
    int length = *idx - start;
    char *numStr = strndup(js + start, length);
    return numStr;
}

static char *parseStr (const char *js, int *idx) {
    skipWhitespace(js, idx);
    if (js[*idx] != '"') { printf("PARSESTR: First non-whitespace char should be double quote. acutal: %c \n", js[*idx]); exit(1); }
    skipChar(idx);
    int start = *idx;
    while (js[*idx] != '"') { skipChar(idx); }
    int length = *idx - start;
    char *str = strndup(js + start, length);
    skipChar(idx);
    return str;
}

JsonPair *jsonStrToPair(const char *js, int *idx) {
    skipWhitespace(js, idx);
    if (js[*idx] != '"') { printf("JSONSTRTOPAIR: First non-whitespace char should be double quote. actual: %c \n", js[*idx]); exit(1); }

    char *key = parseStr(js, idx);
    skipWhitespace(js, idx);
    if (js[*idx] != ':') { printf("JSONSTRTOPAIR: Second non-whitespace char should be colon. Actual: %c \n", js[*idx]); exit(1); }
    skipChar(idx);
    skipWhitespace(js, idx);
    char *value;
    if (isdigit(js[*idx])) { value = parseNum(js, idx); }
    else { value = parseStr(js, idx); }

    JsonPair *jp = malloc(sizeof(JsonPair));
    if (jp == NULL) { printf("JSONSTRTOPAIR: jp is null \n"); exit(1); }
    jp->key = key;
    jp->value = value;
    enum DataType type = TYPE_NULL;
    
    if (isdigit(value[0])) { type = TYPE_NUMBER; }
    else if (strcmp(value, "true") == 0 || strcmp(value, "false") == 0) { type = TYPE_BOOL; }
    else if (strcmp(value, "null") == 0 || strcmp(value, "none") == 0) { type = TYPE_NULL; }
    else { type = TYPE_STRING; }

    jp->type = type;
    return jp;
}

JsonObject *jsonDecode(const char *js) {
    int idx = 0;
    skipWhitespace(js, &idx);
    if (js[idx] != '{') { printf("JSONDECODE: json obj should start with opening bracket. actual: %c \n", js[idx]); exit(1); }
    skipChar(&idx);
    JsonObject *jo = initJsonObject();
    jo->size = 0;
    jo->pairArray = NULL;

    while (js[idx] != '}') {
        JsonPair *jp = jsonStrToPair(js, &idx); 
        jo->pairArray = realloc(jo->pairArray, sizeof(JsonPair *) * (jo->size + 1));
        jo->pairArray[jo->size] = jp;
        jo->size++;

        skipWhitespace(js, &idx);
        if (js[idx] != ',') { printf("JSONDECODE: each pair should be separated by comma. actual: %c \n", js[idx]); exit(1); }
        skipChar(&idx);
    }
    skipChar(&idx);
    return jo;
}

char *jsonEncode(JsonObject *jo) {
    if (jo == NULL || jo->size == 0) { printf("JSONENCODE: jo is null or size == 0 \n"); exit(1); }
    int bufferSize = 256;
    char *buffer = malloc(bufferSize);
    if (buffer == NULL) { printf("JSONENCODE: buffer is null \n"); exit(1); }
    strcpy(buffer, "{");

    int i;
    for (i = 0; i < jo->size; i++) {
        JsonPair *pair = jo->pairArray[i];
        char *escKey = escapeJsonStr(pair->key);
        char *escValue = escapeJsonStr(pair->value);

        strcat(buffer, "\"");
        strcat(buffer, escKey);
        strcat(buffer, "\": ");

        if (pair->type == TYPE_STRING) {
            strcat(buffer, "\"");
            strcat(buffer, escValue);
            strcat(buffer, "\"");
        } else { strcat(buffer, escValue); }

        free(escKey);
        free(escValue);

        strcat(buffer, ",");
    }
    strcat(buffer, "}");
    return buffer;
}

void freeJsonObj(JsonObject *jo) {
    if (jo == NULL) { return; }

    int i;
    for (i = 0; i < jo->size; i++) {
        free(jo->pairArray[i]->key);
        free(jo->pairArray[i]->value);
        free(jo->pairArray[i]);
    }
    free(jo->pairArray);
    free(jo);
}

void freeJsonArray(JsonArray *ja) {
    free(ja->elements);
    free(ja);
}

int main() {
    JsonObject obj;
    obj.size = 0;
    obj.pairArray = malloc(2 * sizeof(JsonPair *));

    JsonPair *pair1 = malloc(sizeof(JsonPair));
    pair1->key = strndup("name", 4);
    pair1->value = strndup("John Doe", 8);
    pair1->type = TYPE_STRING;

    JsonPair *pair2 = malloc(sizeof(JsonPair));
    pair2->key = strndup("age", 3);
    pair2->value = strndup("30", 2);
    pair2->type = TYPE_NUMBER;

    obj.pairArray[obj.size++] = pair1;
    obj.pairArray[obj.size++] = pair2;

    char *jsonStr = jsonEncode(&obj);
    printf("Serialized JSON: %s\n", jsonStr);

    JsonObject *decodedObj = jsonDecode(jsonStr);
    printf("Deserialized JSON:\n");
    int i;
    for (i = 0; i < decodedObj->size; i++) {
        printf("Key: %s, Value: %s, Type: %d\n",
               decodedObj->pairArray[i]->key,
               decodedObj->pairArray[i]->value,
               decodedObj->pairArray[i]->type);
    }
    freeJsonObj(decodedObj);

    return 0;
}
