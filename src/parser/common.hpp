#pragma once
#include <assert.h>
#include <unistd.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <type_traits>
#include <filesystem>
#include <chrono>
#include <thread>
#include <future>
#include <functional>

using namespace std;

typedef uint64_t idx_t;
typedef uint64_t fsize_t;
typedef uint8_t byte_t;
typedef uint16_t field_id_t;

#define CHECKSUM_SIZE sizeof(uint64_t)
#define POINTER_SIZE sizeof(uint64_t)
#define DEFAULT_HEADER_SIZE 4096U
#define HEADER_SIZE DEFAULT_HEADER_SIZE  // configurable
#define METADATA_BLOCK_COUNT 64
#define METADATA_BLOCK_SIZE (DEFAULT_HEADER_SIZE - POINTER_SIZE)
#define OBJECT_END 0xffff
#define DEFAULT_BLOCK_SIZE (1 << 18)
#define DEFAULT_DICTIONARY_HEADER_SIZE (sizeof(uint32_t) + sizeof(uint32_t))
#define DEFAULT_VECTOR_SIZE (2048)
#define STRING_MAX_LENGTH (4294967295U)