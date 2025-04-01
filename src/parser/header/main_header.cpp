#include "../include/header/main_header.hpp"

MainHeader::MainHeader(ifstream& file) {
    file.seekg(0);
    byte_t block[DEFAULT_HEADER_SIZE];
    // byte_t* block = new byte_t[DEFAULT_HEADER_SIZE];
    file.read((char *)block, DEFAULT_HEADER_SIZE);
    byte_t* cursor = block + CHECKSUM_SIZE;
    Reader reader(cursor);
    reader.Read<char>(magic_bytes, 4);
    version = reader.Read<uint64_t>();
    // delete [] block;
}

void MainHeader::operator=(const MainHeader& other) {
    memcpy(magic_bytes, other.magic_bytes, 4);
    version = other.version;
}

void MainHeader::Print() {
    string bytes(magic_bytes, 4);
    cout << "magic bytes: " << bytes << "\nversion: " << version << '\n';
}