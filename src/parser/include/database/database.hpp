#pragma once
#include "../../common.hpp"
#include "../header/database_header.hpp"
#include "../block/storage_block.hpp"
#include "../statistics/base_statistics.hpp"
#include "../statistics/distinct_statistics.hpp"
#include "../header/main_header.hpp"
#include "../schema/schema.hpp"
#include "../table/table.hpp"
#include "../type_info/type_info.hpp"

class Database {
public:
    Database(const char*);
    void LoadExistingDatabase();
    string operator[](idx_t);
    typename vector<string>::iterator begin();
    typename vector<string>::iterator end();
    uint64_t GetRowCount();
private:
    ifstream file;
    fsize_t file_size;
    vector<Schema*> schemas;
    vector<Table*> tables;
    MainHeader main_header;
    DatabaseHeader db_header;
    vector<string> data;
    
    uint64_t GetReadSize();
    void LoadListEntries();
    void LoadRowGroups(Table*);
    void LoadColumnData(Table*);
    void LoadColumnDataPointer(MetadataBlock&);
    void LoadData(uint64_t, uint64_t, StorageBlock&);
};