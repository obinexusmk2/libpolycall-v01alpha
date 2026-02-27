#include <stdio.h>
#include <string.h>

void trigger_if_changed(const char* key, const char* new_value) {
    static char last_value[64] = "";
    if (strcmp(last_value, new_value) != 0) {
        printf("trigger update key=%s value=%s\n", key, new_value);
        strncpy(last_value, new_value, sizeof(last_value) - 1);
        last_value[sizeof(last_value) - 1] = '\0';
    } else {
        printf("cache hit key=%s value=%s\n", key, new_value);
    }
}
