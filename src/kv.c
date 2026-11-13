#include <kv.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>


size_t hash(const char* val, int capacity) {
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
// returns: the index of the key, on error returns -1
int kv_put(kv_t* db, char* key, char* value) {
    if (!db || !key || !value) return -1;

    size_t idx = hash(key, db->capacity);
    kv_entry_t* first_tomb = NULL;
    kv_entry_t* target = NULL;

    for (size_t i = 0; i < db->capacity; i++) {
        kv_entry_t* entry = &db->entries[(idx + i) % db->capacity];

        if (entry->key == NULL) {
            // empty slot: the chain ends here, key doesn't exist
            target = first_tomb ? first_tomb : entry;
            break;
        }

        if (entry->key == TOMBSTONE) {
            if (!first_tomb) first_tomb = entry;
            continue;
        }

        if (strcmp(entry->key, key) == 0) {
            // update existing key
            char* new_val = strdup(value);
            if (!new_val) return -1;
            free(entry->value);
            entry->value = new_val;
            return 0;
        }
    }

    // probed the whole table without finding an empty slot
    if (!target) target = first_tomb;
    if (!target) return -1;

    // insert new entry
    char* new_key = strdup(key);
    char* new_val = strdup(value);
    if (!new_key || !new_val) {
        free(new_key);
        free(new_val);
        return -1;
    }

    target->key = new_key;
    target->value = new_val;
    db->count++;
    return 0;
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
        free(table);
        return NULL;
    }

    return table;
}