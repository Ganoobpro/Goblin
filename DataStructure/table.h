#ifndef GOBLIN_TABLE_H
#define GOBLIN_TABLE_H

#define TABLE_MAX_LOAD .75

typedef struct {
  ObjString* key;
  Value value;
  uint32_t hashNumber;
} Entry;

typedef struct {
  uint32_t counter;
  uint32_t capacity;
  Entry* entries;
} Table;

void InitTable(Table* table);
void FreeTable(Table* table);
void TableInsert(Table* table, ObjString* key, Value value);
Entry* TableGet(Table* table, ObjString* key);
void TableDelete(Table* table, ObjString* key);

#endif
