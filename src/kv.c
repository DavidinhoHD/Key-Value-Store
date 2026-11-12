#include <kv.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define TOMBSTONE 1

size_t hash(char* val, int capacity) {
    size_t hash = 0x13371337deadbeef;

    while (*val) {
        hash ^= *val;
        hash = hash << 8;
        hash += *val;

        val++;
    }
    return hash % capacity;
}

// function kv put:
// returns: the index of the key, on error returns -1, -2 on not found
int kv_put(kv_t* db, char* key, char* value) {
    if (!db || !key || !value) {return -1;}

    size_t idx = hash(key, db->capacity);

    for (size_t i = 0; i < db->capacity - 1; i++) {
        size_t real_idx = (idx + 1) % db->capacity;
        kv_entry_t* entry = &db->entries[real_idx];

        if (entry-> key && entry-> key != (void*)TOMBSTONE && !strcmp(entry->key, key)) {

            char* new_val = strdup(value);
            if (!new_val) return -1;
            entry->value = new_val;
        }

        if (!entry->key || entry->key == (void*)TOMBSTONE) {
            char* new_val = strdup(value);
            char* new_key = strdup(key);
            if (!new_val || !new_key) {
                free(new_key);
                free(new_val);
                return -1;
            }
            entry->value = new_val;
            entry->key = new_key;
            db->count++;
            return real_idx;
        }
    }
    return -2;
}

kv_t* kv_init(size_t capacity) {
    if (capacity == 0) return NULL;

    kv_t* table = malloc(sizeof(kv_t));
    if (table == NULL) {
        return NULL;
    }

    table->capacity = capacity;
    table->count = 0;
    table->entries = calloc(capacity, sizeof(kv_entry_t));
    if (table->entries == NULL) {
        return NULL;
    }

    return table;
}