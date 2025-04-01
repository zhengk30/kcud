#include "../include/statistics/distinct_statistics.hpp"

void DistinctStatistics::Deserialize(field_id_t field_id, Reader& reader) {
    
    uint16_t val = reader.Read<field_id_t>();
    assert(val == field_id);
    reader.Advance(1);
    uint8_t byte;
    // sample count
    (void)reader.ReadEncoded<uint64_t>(100);
    // total count
    (void)reader.ReadEncoded<uint64_t>(101);
    (void)reader.Read<uint8_t>(102);
    // hyperloglog type
    (void)reader.Read<uint8_t>(100);
    // hyperloglog size
    auto size = reader.ReadEncoded<uint64_t>(101);
    assert(size == HLL_DENSE_SIZE);
    // printf("size=%llu, HLL_DENSE_SIZE=%llu\n", size, HLL_DENSE_SIZE);
    reader.Advance(HLL_DENSE_SIZE);
    
    if (reader.Read<field_id_t>() != OBJECT_END) {
        reader.Advance(6);
        assert(reader.Read<field_id_t>() == OBJECT_END);
    }

    assert(reader.Read<field_id_t>() == OBJECT_END);  // ???
    assert(reader.Read<field_id_t>() == OBJECT_END);  // ???
    
    val = reader.Read<field_id_t>();
    if (val != 101) {
        return;
    }
    // table sample
    assert(reader.Read<uint8_t>() == 1);
    val = reader.Read<field_id_t>();
    if (val == 100) {
        assert(reader.Read<uint8_t>() == 1);
        if (val == 100) {
            (void)reader.Read<idx_t>(100);
            (void)reader.Read<idx_t>(101);
            (void)reader.Read<idx_t>(102);
            (void)reader.Read<idx_t>(103);
            (void)reader.Read<idx_t>(104);
            assert(reader.Read<field_id_t>() == OBJECT_END);
            val = reader.Read<field_id_t>();
        }
        
    }
    assert(val == 101);
    (void)reader.Read<uint8_t>();
    (void)reader.Read<bool>(102);
    (void)reader.ReadEncoded<uint64_t>(200);
    (void)reader.Read<uint64_t>(201);
    assert(reader.Read<field_id_t>() == OBJECT_END);
    assert(reader.Read<field_id_t>() == OBJECT_END);
    

    // do {
    //     byte = reader.Read<uint8_t>();
    //     if (byte == 0xff) {
    //         reader.Unread<uint8_t>();
    //         break;
    //     }
    // } while (1);
    // reader.Advance(8);
}