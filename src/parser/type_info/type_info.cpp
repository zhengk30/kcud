#include "../include/type_info/type_info.hpp"

LogicalType LogicalType::Deserialize(field_id_t field_id, Reader& reader) {
    assert(reader.Read<field_id_t>() == field_id);
    uint8_t id = reader.Read<uint8_t>(100);
    assert(reader.Read<field_id_t>() == OBJECT_END);
    return LogicalType(id);
}

ColumnType ColumnType::Deserialize(field_id_t field_id, Reader& reader) {
    assert(reader.Read<field_id_t>() == field_id);
    uint8_t id = reader.Read<uint8_t>();
    return ColumnType(id);
}

CompressionType CompressionType::Deserialize(field_id_t field_id, Reader& reader) {
    auto actual = reader.Read<field_id_t>();
    if (actual != field_id && actual == field_id + 0xbc) {
        reader.Advance(8);
    }
    uint8_t id = reader.Read<uint8_t>();
    if (static_cast<int8_t>(id) < 0) {
        reader.Advance(7);
        id = reader.Read<uint8_t>();
    }
    return CompressionType(id);
}