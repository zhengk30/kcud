#pragma once
#include "../block/metadata_block.hpp"
#include "../reader/reader.hpp"
#include "../type_info/type_info.hpp"
#include "../row_group/row_group.hpp"

class ColumnInfo {
public:
    ColumnInfo();
    static void Deserialize(ColumnInfo&, Reader&);
private:
    string name;
    LogicalType logical_type;
    ColumnType column_type;
    CompressionType compression_type;
};

class Table {
public:
    Table() = default;
    Table(CatalogType, string, string, uint8_t, bool, bool, string, string, uint8_t, uint64_t, uint64_t);
    static void Deserialize(Reader&, Table*);
    idx_t GetTableStartBlockId();
    idx_t GetTableStartBlockIndex();
    idx_t GetTableStartBlockOffset();
    void SetRowGroupCount(uint64_t);
    void AddRowGroup(RowGroup*);
    uint64_t GetRowGroupCount();
    RowGroup* GetRowGroup(idx_t i);
private:
    CatalogType type_;
    string catalog_name_;
    string schema_name_;
    uint8_t on_conflict_;
    bool temporary_;
    bool internal_;
    string sql_;
    // table-specific fields
    string table_name_;
    uint8_t ncols_;
    uint64_t nrows_;
    uint64_t row_group_count_;
    vector<RowGroup*> row_groups_;
    vector<ColumnInfo> columns_meta_;
    MetadataBlock table_start_;
    void LoadTableColumns(field_id_t, Reader&);
    void ReadRowCount(field_id_t, Reader&);
    void ReadIndexPointers(field_id_t, Reader&);
};