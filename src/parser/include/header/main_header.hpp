#pragma once

#include "../../common.hpp"
#include "../reader/reader.hpp"

class MainHeader {
public:
    MainHeader() = default;
    MainHeader(int fildes);
    void Print();
    void operator=(const MainHeader&);
    ~MainHeader() = default;
private:
    char magic_bytes[4];
	uint64_t version;
};