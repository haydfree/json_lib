#ifndef JSON_LIB
#define JSON_LIB

typedef enum DataType { 
    TYPE_STRING, 
    TYPE_NUMBER, 
    TYPE_BOOL, 
    TYPE_NULL, 
    TYPE_OBJECT,
    TYPE_ARRAY 
} DataType;

typedef struct JsonPair {
    char *key;
    char *value;
    enum DataType type;
} JsonPair;

typedef struct JsonObject {
    JsonPair *pairArray;
    int size;
} JsonObject;

typedef struct JsonArray {
    void **elements;
    int size;
} JsonArray;

/* JSON serialization */
extern char *jsonEncode(JsonObject *jo);
extern char *jsonPairToStr(JsonPair *jp);
extern char *jsonArrayToStr(JsonArray *ja);

/* JSON deserialization */
extern JsonObject *jsonDecode(const char *js);
extern JsonPair *jsonStrToPair(const char *js, int idx);
extern JsonArray *jsonStrToArray(const char *js, int idx);

/* helper functions */
extern char *escapeJsonStr(const char *js);
extern char *jsonObjGet(JsonObject *jo, const char *key);
extern void jsonObjSet(JsonObject *jo, const char *key, const char *value);
extern void freeJsonObj(JsonObject *jo);
extern void freeJsonPair(JsonPair *jp);
extern void freeJsonArray(JsonArray *ja);

#endif
