#include "../include/block/storage_block.hpp"

StorageBlock::StorageBlock(idx_t block_id, idx_t block_offset) : block_id_(block_id), block_offset_(block_offset) {
}

StorageBlock StorageBlock::Deserialize(field_id_t field_id, Reader& reader) {
    assert(reader.Read<field_id_t>() == field_id);
    StorageBlock block;
    block.block_id_ = reader.ReadEncoded<idx_t>(100);
    block.block_offset_ = reader.ReadEncoded<idx_t>(101);
    if (reader.Read<field_id_t>() != OBJECT_END) {
        reader.Advance(8);
        assert(reader.Read<field_id_t>() == OBJECT_END);
    }
    return block;
}

void StorageBlock::operator=(StorageBlock& other) {
    block_id_ = other.block_id_;
    block_offset_ = other.block_offset_;
}

idx_t StorageBlock::GetBlockId() {
    return block_id_;
}

idx_t StorageBlock::GetBlockOffset() {
    return block_offset_;
}