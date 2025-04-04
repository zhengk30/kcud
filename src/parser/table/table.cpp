#include "../include/table/table.hpp"

Table::Table(CatalogType type, string catalog, string schema, uint8_t on_conflict,
            bool temporary, bool internal, string sql, string table, uint8_t ncols,
            uint64_t nrows, uint64_t row_group_count) :
            type_(type), catalog_name_(catalog), schema_name_(schema), on_conflict_(on_conflict),
            temporary_(temporary), internal_(internal), sql_(sql), table_name_(table), ncols_(ncols),
            nrows_(nrows), row_group_count_(row_group_count) {
}

ColumnInfo::ColumnInfo() {}

void Table::LoadTableColumns(field_id_t field_id, Reader& reader) {
    assert(reader.Read<field_id_t>() == field_id);
    ncols_ = reader.Read<uint8_t>(100);
    for (auto i = 0; i < ncols_; i++) {
        ColumnInfo column_info;
        ColumnInfo::Deserialize(column_info, reader);
        columns_meta_.push_back(column_info);
        assert(reader.Read<field_id_t>() == OBJECT_END);
    }
    assert(reader.Read<field_id_t>() == OBJECT_END);
}

idx_t Table::GetTableStartBlockId() {
    return table_start_.GetBlockId();
}

idx_t Table::GetTableStartBlockIndex() {
    return table_start_.GetBlockIndex();
}

idx_t Table::GetTableStartBlockOffset() {
    return table_start_.GetBlockOffset();
}

uint64_t Table::GetRowCount() {
    return nrows_;
}

void Table::SetRowGroupCount(uint64_t count) {
    row_group_count_ = count;
}

void Table::ReadRowCount(field_id_t field_id, Reader& reader) {
    nrows_ = reader.ReadEncoded<uint64_t>(field_id);
}

void Table::ReadIndexPointers(field_id_t field_id, Reader& reader) {
    auto count = reader.ReadEncoded<uint64_t>(field_id);
    assert(count == 0);
}

void ColumnInfo::Deserialize(ColumnInfo& info, Reader& reader) {
    info.name = reader.Read<string>(100);
    info.logical_type = LogicalType::Deserialize(101, reader);
    info.column_type = ColumnType::Deserialize(103, reader);
    info.compression_type = CompressionType::Deserialize(104, reader);
}

void Table::Deserialize(Reader& reader, Table* table) {
    uint8_t count = reader.Read<uint8_t>(100);
    assert(count == 1);

    // read in table info
    table->type_ = (CatalogType)reader.Read<uint8_t>(100);
    table->catalog_name_ = reader.Read<string>(101);
    table->catalog_name_ = reader.Read<string>(102);
    table->temporary_ = reader.Read<bool>(103);
    table->internal_ = reader.Read<bool>(104);
    table->on_conflict_ = reader.Read<uint8_t>(105);
    table->sql_ = reader.Read<string>(106);
    
    table->table_name_ = reader.Read<string>(200);
    // printf("catalog_name=%s\nschema_name=%s\ntable_name=%s\n", table->catalog_name_.c_str(), table->catalog_name_.c_str(), table->table_name_.c_str());
    table->LoadTableColumns(201, reader);
    assert(reader.Read<field_id_t>() == OBJECT_END);
    
    // printf("about to load table_start_...\n");
    table->table_start_ = MetadataBlock::Deserialize(101, reader);

    // printf("about to load row count...\n");
    table->ReadRowCount(102, reader);
    table->ReadIndexPointers(103, reader);
}

void Table::AddRowGroup(RowGroup* row_group) {
    row_groups_.push_back(row_group);
}

uint64_t Table::GetRowGroupCount() {
    return row_group_count_;
}

RowGroup* Table::GetRowGroup(idx_t i) {
    assert(i < row_group_count_);
    return row_groups_[i];
}