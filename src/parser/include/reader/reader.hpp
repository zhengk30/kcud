#pragma once
#include "../../common.hpp"
#include "../decoder/decoder.hpp"

class Reader {
public:
    Reader(byte_t* cursor) : cursor_(cursor) {}
    template <typename T> T Read() {
        T* ptr = reinterpret_cast<T *>(cursor_);
        T val = *ptr;
        cursor_ += sizeof(T);
        return val;
    }
    template <typename T> void Read(T* dest, size_t n) {
        T* ptr = reinterpret_cast<T *>(cursor_);
        memcpy(dest, ptr, n * sizeof(T));
        cursor_ += n * sizeof(T);
    }
    template <typename T> T Read(field_id_t field_id) {
        constexpr bool is_string = is_same<T, string>::value;
        field_id_t* ptr = reinterpret_cast<field_id_t *>(cursor_);
        field_id_t actual_field_id = *ptr;
        if (actual_field_id != field_id) {
            if (actual_field_id - 0xbc00 == field_id) {
                Advance(10);
                goto read;
            } else if (actual_field_id == 0x2bc) {
                Advance(10);
                goto read;
            }
            // read default value
            if constexpr (is_string) {
                return "";
            } else {
                return static_cast<T>(0);
            }
        }
        cursor_ += sizeof(field_id_t);
    read:
        if constexpr (is_string) {
            uint8_t size = *cursor_;
            cursor_ += sizeof(uint8_t);
            char buffer[size];
            Read<char>(buffer, size);
            string result(buffer, size);
            return result;
        } else {
            T result = *reinterpret_cast<T *>(cursor_);
            cursor_ += sizeof(T);
            return result;
        }
        
    }
    template <typename T> T ReadEncoded() {
        T val;
        if (is_unsigned<T>::value) {
            val = unsigned_decode(&cursor_);
        } else {
            val = signed_decode(&cursor_);
        }
        return val;
    }
    template <typename T> T ReadEncoded(field_id_t field_id) {
        field_id_t* ptr = reinterpret_cast<field_id_t *>(cursor_);
        field_id_t actual_field_id = *ptr;
        if (actual_field_id != field_id) {
            if (actual_field_id - 0xbc00 == field_id) {
                Advance(10);
                goto read_encoded;       
            } else if (actual_field_id == 0x2bc) {
                Advance(10);
                goto read_encoded;
            } else {
                return 0;
            }
        }
        cursor_ += sizeof(field_id_t);
    read_encoded:
        T val;
        if (is_unsigned<T>::value) {
            val = unsigned_decode(&cursor_);
        } else {
            val = signed_decode(&cursor_);
        }
        ptr = reinterpret_cast<field_id_t *>(cursor_);
        if (*ptr == 0x2bc) {
            Advance(8);
        }
        return val;
    }
    template <typename T> bool ReadEncoded(field_id_t field_id, T* val) {
        field_id_t* ptr = reinterpret_cast<field_id_t *>(cursor_);
        field_id_t actual_field_id = *ptr;
        if (actual_field_id != field_id) {
            return true;  // is default value
        } else {
            cursor_ += sizeof(field_id_t);
            if (is_unsigned<T>::value) {
                *val = unsigned_decode(&cursor_);
            } else {
                *val = signed_decode(&cursor_);
            }
            ptr = reinterpret_cast<field_id_t *>(cursor_);
            if (*ptr == 0x2bc) {
                Advance(8);
            }
            return false;  // is not default value
        }
    }
    template <typename T> T ReadEncoded(byte_t* cursor) {
        T val;
        if (is_unsigned<T>::value) {
            val = unsigned_decode(&cursor);
        } else {
            val = signed_decode(&cursor);
        }
        return val;
    }
    template <typename T> bool TentativeRead(T expected) {
        T* ptr = reinterpret_cast<T *>(cursor_);
        T actual = *ptr;
        if (actual == expected) {
            cursor_ += sizeof(T);
        }
        return actual == expected;
    }
    template <typename T> void Unread() {
        cursor_ -= sizeof(T);
    }
    void Advance(size_t nbytes) {
        cursor_ += nbytes;
    }
private:
    byte_t* cursor_;
    uint64_t pos_;
};