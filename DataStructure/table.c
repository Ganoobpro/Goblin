#include "table.h"

#define IS_EMPTY_ENTRY(entryPtr) (IS_NILL((entryPtr)->value))
#define IS_TOMBSTONE(entryPtr)   (!IS_NILL((entryPtr)->value))

static Entry* FindEntry(Entry* entries, uint32_t capacity,
                        ObjString* key, bool returnTombstone);
static uint32_t ReallocateTableToNewEntries(Table* table, Entry* entries);
static void AdjustTable(Table* table);

void InitTable(Table* table) {
  table->counter = 0;
  table->capacity = 0;
  table->entries = NULL;
}

void FreeTable(Table* table) {
  table->entries = FREE_ARRAY(table->entries);
}

void TableInsert(Table* table, ObjString* key, Value value) {
  if (table->counter >= table->capacity * TABLE_MAX_LOAD) {
    table->capacity = GROW_CAPACITY(table->capacity);
    AdjustTable(table);
  }

  Entry* entry = FindEntry(table->entries, table->capacity, key, true);
  if (empty (entry->key) && IS_EMPTY_ENTRY(entry)) table->counter++;

  entry->key = key;
  entry->value = value;
}

Entry* TableGet(Table* table, ObjString* key) {
  Entry* entry = FindEntry(table->entries, table->capacity, key, false);
  return entry;
}

void TableDelete(Table* table, ObjString* key) {
  Entry* entry = FindEntry(table->entries, table->capacity, key, true);
  if (empty (entry->key)) return;

  entry->key = NULL;
  if (IS_NILL(entry->value))
    entry->value = MAKE_NUMBER(0);
}

static Entry* FindEntry(Entry* entries, uint32_t capacity,
                        ObjString* key, bool returnTombstone) {
  // index = hash % capacity
  // you can easily prove that <hash % capacity> = <hash & (capacity - 1)>
  // with capacity = 2 to the power of x
  uint32_t index = key->hash & (capacity - 1);

  forever {
    Entry* entry = &entries[index];
    if (entry->key == key) return entry;

    if (empty (entry->key)) {
      if (returnTombstone && IS_TOMBSTONE(entry))
        return entry;
      if (IS_EMPTY_ENTRY(entry))
        return entry;
    }

    // index++
    // <index = (index + 1) % capacity> safer
    index = (index + 1) & (capacity - 1);
  }
}

static uint32_t ReallocateTableToNewEntries(Table* table, Entry* entries) {
  uint32_t newCounter = 0u;

  for (uint32_t i = 0u; i < table->counter; i++) {
    Entry* oldEntry = table->entries + i;
    // If <EMPTY ENTRY> (but shorter in this condition)
    if (empty (oldEntry->key)) {
      newCounter--;
    }

    Entry* destination = FindEntry(entries, table->capacity,
                                   oldEntry->key, true);
    destination->key = oldEntry->key;
    destination->value = oldEntry->value;
    newCounter++;
  }

  return newCounter;
}

static void AdjustTable(Table* table) {
  Entry* entries = NULL;
  entries = GROW_ARRAY(Entry, entries, table->capacity);

  for (uint32_t i = 0u; i < table->capacity; i++) {
    entries[i].key = NULL;
    entries[i].value = MAKE_NILL;
  }

  table->counter = ReallocateTableToNewEntries(table, entries);

  table->entries = FREE_ARRAY(table->entries);
  table->entries = entries;
}

#undef IS_EMPTY_ENTRY
#undef IS_TOMBSTONE
