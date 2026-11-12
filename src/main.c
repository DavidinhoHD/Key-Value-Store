#include <stddef.h>
#include <stdio.h>
#include <kv.h>

int main() {
    kv_t* table = kv_init(3);
    printf("%p\n", (void*)table);
    printf("%ld\n", table->capacity);


    kv_put(table, "hehe", "haha");

    for (size_t i = 0; i < table-> capacity; i++) {
        if (table->entries[i].key) {
           printf("%s: %s\n", table->entries[i].key, table->entries->value);
        }
    }
}