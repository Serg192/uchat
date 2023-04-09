#include "../../inc/client.h"

char *mx_prepare_str_for_sql(const char *input) {
    if (!input) {
        return NULL;
    }

    size_t input_len = strlen(input);
    size_t output_len = input_len * 2 + 1;
    char *output = malloc(output_len);
    if (!output) {
        return NULL;
    }

    const char *p;
    char *q;
    for (p = input, q = output; *p != '\0';) {
        if (*p == '\'') {
            q[0] = '\'';
            q[1] = '\'';
            p++;
            q += 2;
        }
        else if (*p == '"' || *p == '\\' || *p == '\0') {
            q[0] = '\\';
            q[1] = *p++;
            q += 2;
        }
        else {
            *q++ = *p++;
        }
    }
    *q = '\0';

    return output;
}
