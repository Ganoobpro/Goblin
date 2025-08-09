#include "table.h"

#define IS_EMPTY_ENTRY(entryPtr) ((entryPtr)->key == NULL && IS_NILL((entryPtr)->value))
#define IS_TOMBSTONE(entryPtr)   ((entryPtr)->key == NULL && (!IS_NILL((entryPtr)->value)))

static Entry* FindEntry(Table* table, ObjString* key);
static void ReallocateTableToNewEntries(Table* table, Entry* entries);
static void AdjustTable(Table* table);

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
    if (IS_EMPTY_ENTRY(entry)) return NULL;

    index = (index + 1) & (table->capacity - 1);
  }
}

void TableDelete(Table* table, ObjString* key) {
  Entry* entry = FindEntry(table, key);
  if (empty (entry->key)) return;

  entry->key = NULL;
  if (IS_NILL(entry->value))
    entry->value = MAKE_NUMBER(0);
}

static Entry* FindEntry(Table* table, ObjString* key) {
  // index = hash % capacity
  // you can easily prove that <hash % capacity> = <hash & (capacity - 1)>
  // with capacity = 2 to the power of x
  uint32_t index = ObjString->hashNumber & (table->capacity - 1);

  forever {
    Entry* entry = table->entries[index];
    // Dont use IS_EMPTY_ENTRY to reuse TOMBSTONE
    if (entry->key == key || empty (entry->key)) {
      return entry;
    }

    // index++
    // <index = (index + 1) % capacity> safer
    index = (index + 1) & (table->capacity - 1);
  }
}

// TODO: I must build something to clean old table, right?
static uint32_t ReallocateTableToNewEntries(Table* table, Entry* entries) {
  uint32_t newCounter = table->counter;

  for (uint32_t i = 0u; i < table->counter; i++) {
    Entry* entry = table->entries + i;
    // If <EMPTY ENTRY> (but shorter in this condition)
    if (empty (entry->key)) {
      newCounter--;
    }

    Entry* destination = FindEntry(entries, entry->key);
    destination->key = key;
    destination->value = value;
  }

  return newCounter;
}

static void AdjustTable(Table* table) {
  Entry* entries = NULL;
  GROWARRAY(Entry, entries, table->capacity);

  for (uint32_t i = 0u; i < table->capacity; i++) {
    entries[i].key = NULL;
    entries[i].value = MAKE_NILL;
  }

  table->counter = ReallocateTableToNewEntries(table, entries);
  table->entries = entries;
}

#undef IS_EMPTY_ENTRY
#undef IS_TOMBSTONE
