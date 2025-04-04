#pragma once
#include "../../common.hpp"
#include "../decoder/decoder.hpp"

#define IS_NEXT_BLOCK(offset) (((offset) % METADATA_BLOCK_SIZE) == 0)

class Reader {
public:
    Reader(byte_t* cursor) : cursor_(cursor), offset_(0) {}
    //
    // normal read operations
    //
    //
    void Reassign(ifstream& file, uint64_t file_size, idx_t next_id, idx_t next_index) {
        auto fileoff = DEFAULT_HEADER_SIZE * 3 + DEFAULT_BLOCK_SIZE * next_id + CHECKSUM_SIZE;
        file.seekg(fileoff, ios::beg);
        file.read(reinterpret_cast<char *>(cursor_), GET_READ_SIZE(file, file_size));
        cursor_ += METADATA_BLOCK_SIZE * next_index;
        offset_ = 0;
    }

    template <typename T>
    void Read(T* dest, size_t n) {
        uint64_t i = 0;
        auto read_size = n * sizeof(T);
        byte_t buffer[1024];
        while (i < read_size) {
            if (IS_NEXT_BLOCK(offset_)) {
                offset_ += 8;
            }
            buffer[i++] = cursor_[offset_++];
        }
        memcpy(dest, buffer, sizeof(T) * n);
    }

    template <typename T>
    T Read() {
        uint64_t i = 0;
        byte_t buffer[sizeof(T)];
        while (i < sizeof(T)) {
            if (IS_NEXT_BLOCK(offset_)) {
                offset_ += 8;
            }
            buffer[i++] = cursor_[offset_++];
        }
        T val;
        memcpy(&val, buffer, sizeof(T));
        return val;
    }

    template <typename T>
    T Read(field_id_t field_id) {
        uint64_t tmp_offset = offset_;
        field_id_t actual_field_id = PeekFieldId(&tmp_offset);
        constexpr bool is_string = is_same<T, string>::value;
        if (field_id != actual_field_id) {
            if constexpr (is_string) {
                return "";
            } else {
                return static_cast<T>(0);
            }
        }
        offset_ = tmp_offset;
        if constexpr (is_string) {
            if (IS_NEXT_BLOCK(offset_)) offset_ += 8;
            auto size = cursor_[offset_++];
            char buffer[size];
            Read<char>(buffer, size);
            string result(buffer, size);
            return result;
        } else {
            return Read<T>();
        }
    }

    //
    // decode + read
    //
    //
    template <typename T>
    T ReadEncoded() {
        byte_t bytes[16] = {0};
        Peek(bytes, 16);
        T val;
        size_t size;
        if (is_unsigned<T>::value) {
            size = unsigned_decode(bytes, reinterpret_cast<uint64_t *>(&val));
        } else {
            size = signed_decode(bytes, reinterpret_cast<int64_t *>(&val));
        }
        Advance(size);
        return val;
    }

    template <typename T>
    T ReadEncoded(field_id_t field_id) {
        // 
        // peek the next field id
        // 
        // 
        uint64_t tmp_offset = offset_;
        field_id_t actual_field_id = PeekFieldId(&tmp_offset);
        if (actual_field_id != field_id) {
            return static_cast<T>(0);
        }
        offset_ = tmp_offset;

        // 
        // read and decode
        //
        //
        byte_t bytes[16];
        Peek(bytes, 16);
        T val;
        size_t size;
        if (is_unsigned<T>::value) {
            size = unsigned_decode(bytes, reinterpret_cast<uint64_t *>(&val));
        } else {
            size = signed_decode(bytes, reinterpret_cast<int64_t *>(&val));
        }
        Advance(size);
        return val;
    }

    template <typename T>
    bool ReadEncoded(field_id_t field_id, T* val) {
        *val = ReadEncoded<T>(field_id);
        return static_cast<uint64_t>(*val) == 0;
    }

    idx_t ReadMetaBlockPtr() {
        assert(IS_NEXT_BLOCK(offset_));
        //
        // DON'T ADVANCE!
        //
        //
        idx_t* ptr = reinterpret_cast<idx_t *>(cursor_ + offset_);
        return *ptr;
    }

    void Advance(size_t nbytes) {
        uint64_t total = 0;
        while (total < nbytes) {
            if (IS_NEXT_BLOCK(offset_)) {
                offset_ += CHECKSUM_SIZE;
            }
            total++;
            offset_++;
        }
    }

    void UnalignedAdvance(size_t nbytes) {
        offset_ += nbytes;
        // for (auto i = offset_; i < offset_ + 16; i++) {
        //     printf("%02x ", (uint8_t)cursor_[i]);
        // }
        // printf("\n");
    }

    uint64_t CurrentPosition() {
        return offset_;
    }


private:
    byte_t* cursor_;
    uint64_t offset_;

    field_id_t PeekFieldId(uint64_t* tmp_offset) {
        byte_t actual_field_id_buffer[sizeof(field_id_t)];
        uint64_t i = 0;
        while (i < sizeof(field_id_t)) {
            if (IS_NEXT_BLOCK(*tmp_offset)) {
                *tmp_offset += 8;
            }
            actual_field_id_buffer[i++] = cursor_[(*tmp_offset)++];
        }
        field_id_t actual_field_id;
        memcpy(&actual_field_id, actual_field_id_buffer, sizeof(field_id_t));
        return actual_field_id;
    }

    void Peek(byte_t* dest, uint64_t nbytes) {
        uint64_t i = 0;
        auto tmp_offset = offset_;
        while (i < nbytes) {
            if (IS_NEXT_BLOCK(tmp_offset)) {
                tmp_offset += 8;
            }
            dest[i++] = cursor_[tmp_offset++];
        }
    }
};

class DataReader {
public:
    DataReader(byte_t* cursor) : cursor_(cursor) {}
    template <typename T>
    void Read(T* dest, uint64_t n) {
        T* ptr = reinterpret_cast<T *>(cursor_);
        memcpy(dest, ptr, sizeof(T) * n);
        cursor_ += sizeof(T) * n;
    }
    template <typename T> T Read() {
        T* ptr = reinterpret_cast<T *>(cursor_);
        T val = *ptr;
        cursor_ += sizeof(T);
        return val;
    }
private:
    byte_t* cursor_;
};