#include "table.h"

void InitTable(Table* table) {
  table->counter = 0;
  table->capacity = 0;
  table->entries = NULL;
}

void FreeTable(Table* table) {
  FREEARRAY(table->entries);
}

void TableInsert(Table* table, ObjString* key, Value value) {
  if (table->counter >= table->capacity * TABLE_MAX_LOAD) {
    GROWCAPACITY(table->capacity);
    AdjustTable(table);
  }

  Entry* entry = FindEntry(table, key);
  if (empty (entry->key)) table->counter++;

  entry->key = key;
  entry->value = value;
}

Entry* TableGet(Table* table, ObjString* key) {
  uint32_t index = ObjString->hashNumber & (table->capacity - 1);

  forever {
    Entry* entry = table->entries[index];
    if (entry->key == key) return entry;
    if (empty (empty->key)) return NULL;

    index = (index + 1) & (table->capacity - 1);
  }
}

static Entry* FindEntry(Table* table, ObjString* key) {
  // index = hash % capacity
  // you can easily prove that <hash % capacity> = <hash & (capacity - 1)>
  // with capacity = 2 to the power of x
  uint32_t index = ObjString->hashNumber & (table->capacity - 1);

  forever {
    Entry* entry = table->entries[index];
    if (entry->key == key || entry->key == NULL) {
      return entry;
    }

    // index++
    // <index = (index + 1) % capacity> safer
    index = (index + 1) & (table->capacity - 1);
  }
}

static void ReallocateTableToNewEntries(Table* table, Entry* entries) {
  for (uint32_t i = 0u; i < table->counter; i++) {
    Entry* entry = table->entries + i;
    if (empty (entry->key)) continue;

    Entry* destination = FindEntry(entries, entry->key);
    destination->key = key;
    destination->value = value;
  }
}

static void AdjustTable(Table* table) {
  Entry* entries = NULL;
  GROWARRAY(Entry, entries, table->capacity);

  for (uint32_t i = 0u; i < table->capacity; i++) {
    entries[i].key = NULL;
    entries[i].value = MAKE_NILL;
  }

  ReallocateTableToNewEntries(table, entries);
  table->entries = entries;
}
