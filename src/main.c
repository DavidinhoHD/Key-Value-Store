#include <stddef.h>
#include <stdio.h>
#include <kv.h>

int main() {
    kv_t* table = kv_init(3);
    printf("%p\n", (void*)table);
    printf("%ld\n", table->capacity);


    kv_put(table, "hehe", "haha");
    kv_put(table, "hehe", "hoho");
    kv_put(table, "lala", "baba");

    for (size_t i = 0; i < table-> capacity; i++) {
        if (table->entries[i].key) {
           printf("%s: %s\n", table->entries[i].key, table->entries->value);
        }
    }

    char* val = kv_get(table, "hehe");
    char* val2 = kv_get(table, "lala");
    char* val3 = kv_get(table, "this doesn't exist");

    printf("%s %s %s\n", val, val2, val3);
}