
#include "../include/statistics/base_statistics.hpp"

void BaseStatistics::Deserialize(field_id_t field_id, Reader& reader) {
    assert(reader.Read<field_id_t>() == field_id);
    auto has_null = reader.Read<bool>(100);  // has_null
	auto has_no_null = reader.Read<bool>(101);  // has_no_null
	auto distinct_count = reader.ReadEncoded<idx_t>(102);  // distinct_count

    assert(reader.Read<field_id_t>() == 103);  // about to read string stats

    (void)reader.Read<string>(200);
    (void)reader.Read<string>(201);
	(void)reader.Read<bool>(202);
	(void)reader.Read<bool>(203);
	(void)reader.ReadEncoded<uint64_t>(204);
    assert(reader.Read<field_id_t>() == OBJECT_END);  // string stats end
    assert(reader.Read<field_id_t>() == OBJECT_END);  // base stats end
}