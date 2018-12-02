
#pragma once
#include <algorithm>
#include <cmath>
#include "Dictionary.h"

template <typename KeyType, typename ValueType, typename Hasher=std::hash<KeyType>>
  class DictHash : public Dictionary<KeyType,ValueType> {
 public:
 typedef DictHash<KeyType,ValueType,Hasher> DictType;

 // Big
 DictHash() : table(nullptr), tablesize( 0 ), currentSize( 0 ) {}
 ~DictHash() {
   delete[] table;
 }
 DictHash(const DictType& other) : DictType() {
   *this = other;
 }
 DictHash(DictType&& temp) : DictType() {
   *this = temp;
 }
 DictType&operator=(const DictType& other) {
   if (&other != this) {
     delete[] table;
     tablesize = other.tablesize;
     currentSize = other.currentSize;
     table = new Entry[tablesize];
     for (int i = 0  ; i < tablesize; i++)
       table[i] = other.table[i];
   }
   return *this;
 }
 DictType& operator=(DictType&& temp) {
   std::swap(table, temp.table);
   std::swap(tablesize, temp.tablesize);
   std::swap(currentSize, temp.currentSize);
   return *this;
 }

 bool has(const KeyType& key) const {
   if (tablesize == 0  )
     return false;
   Entry &entry = getBucket(key);
   return entry.status == ACTIVE;
 }

 void add(const KeyType& key, const ValueType& value) {
   checksize();
   Entry &entry = getBucket(key);
   if (entry.status != ACTIVE) {
     // new entry
     if (entry.status == EMPTY)
       currentSize++;
     entry.status = ACTIVE;
     entry.key = key;
     entry.value = value;
   } else {
     // existing entry -- just replace value
     entry.value = value;
   }
 }

 ValueType& get(const KeyType& key) {
   checksize();
   Entry &entry = getBucket(key);
   if (entry.status != ACTIVE) {
     if (entry.status == EMPTY)
       currentSize++;
     entry.status = ACTIVE;
     entry.key = key;
   }
   return entry.value;
 }

 const ValueType& get(const KeyType& key) const {
   if (tablesize ==  0 )
     throw std::invalid_argument("not found");
   Entry &entry = getBucket(key);
   if (entry.status != ACTIVE)
     throw std::invalid_argument("not found");
   return entry.value;
 }

 void remove(const KeyType& key) {
   Entry &entry = getBucket(key);
   if (entry.status == ACTIVE) {
     entry.status = DELETED;
     entry.key = KeyType();
     entry.value = ValueType();
   }
 }

 double loadfactor() const {
   if (tablesize == 0  )
     return   0.0  ;
   return static_cast<double>(currentSize) / tablesize;
 }

 class const_iterator {
 public:
 const_iterator(const DictType *dict, size_t current) : dict(dict), current(current) {}

 const KeyType &operator*() const {
   return dict->table[current].key;
 }

 const_iterator& operator++() {
   current++;
   while (current < dict->tablesize && dict->table[current].status != ACTIVE)
     current++;
   return *this;
 }

 bool operator!=(const const_iterator& other) const {
   return dict != other.dict || current != other.current;
 }
 private:
 const DictType *dict;
 size_t current;
 friend class DictHash<KeyType,ValueType,Hasher>;
 };

 const_iterator begin() const {
   size_t first = 0  ;
   while (first < tablesize && table[first].status != ACTIVE)
     first++;
   return const_iterator(this, first);
 }

 const_iterator end() const {
   return const_iterator(this, tablesize);
 }

 private:
 enum BucketStatus {ACTIVE, EMPTY, DELETED};
 struct Entry {
   BucketStatus status;
   KeyType key;
   ValueType value;

 Entry() : status(EMPTY), key(), value() {}
 };
 Entry *table;
 size_t tablesize;
 size_t currentSize;  // count of ACTIVE and DELETED buckets

 Entry& getBucket(const KeyType &key) const {
   Hasher h;
   size_t hash = h(key);
   size_t bucket;
   long deleted = -1  ;
   for (bucket = hash % tablesize; table[bucket].status != EMPTY; bucket = (bucket+1  ) % tablesize) {
     if (table[bucket].status == DELETED) {
       if (deleted == -1  )
         deleted = bucket;  // we can re-use this one if we need a new bucket
     } else if (table[bucket].key == key) {
       break;
     }
   }
   if (deleted != -1  )
     bucket = static_cast<size_t>(deleted);
   return table[bucket];
 }

 static bool isprime(size_t n) {
   size_t limit = static_cast<size_t>(std::sqrt(n)) + 1  ;
   for (int factor = 2  ; factor < limit; factor += 2  )
     if (n % factor ==  0 )
       return false;
   return true;
 }

 static size_t nextprime(size_t n) {
   if (n < 7  )
     return 7  ;
   while (!isprime(n)) {
     n++;
   }
   return n;
 }

 void checksize() {
   if (currentSize * 2   >= tablesize) {
     size_t oldtablesize = tablesize;
     Entry *oldtable = table;
     tablesize = nextprime(tablesize*2  );
     table = new Entry[tablesize];
     currentSize = 0  ;
     for (int i = 0  ; i < oldtablesize; i++)
       if (oldtable[i].status == ACTIVE)
         add(oldtable[i].key, oldtable[i].value);
     delete[] oldtable;
   }
 }
};
