#include "../include/header/main_header.hpp"

MainHeader::MainHeader(int fildes) {
    byte_t block[DEFAULT_HEADER_SIZE];
    lseek(fildes, 0, SEEK_SET);
    assert(read(fildes, block, DEFAULT_HEADER_SIZE) == DEFAULT_HEADER_SIZE);
    Reader reader(block);
    reader.Read<char>(magic_bytes, 4);
    version = reader.Read<uint64_t>();
}

void MainHeader::operator=(const MainHeader& other) {
    memcpy(magic_bytes, other.magic_bytes, 4);
    version = other.version;
}

void MainHeader::Print() {
    string bytes(magic_bytes, 4);
    cout << "[MainHeader]"
         << "\nmagic bytes: " << bytes
         << "\nversion: " << version << '\n';
}