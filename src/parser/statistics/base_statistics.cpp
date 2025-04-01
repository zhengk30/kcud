
#include "../include/statistics/base_statistics.hpp"

void BaseStatistics::Deserialize(field_id_t field_id, Reader& reader) {
    reader.Advance(13);
    do {
        if (reader.Read<field_id_t>() == 201) {
            break;
        } else {
            reader.Unread<uint8_t>();
        }
    } while (1);
    reader.Advance(12);
    if (reader.Read<field_id_t>() != 203) {
        reader.Advance(8);
    }
    reader.Advance(6);
    if (reader.Read<field_id_t>() != OBJECT_END) {
        reader.Advance(6);
        assert(reader.Read<field_id_t>() == OBJECT_END);
    }
}