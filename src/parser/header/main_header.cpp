#include "../include/header/main_header.hpp"

MainHeader::MainHeader(ifstream& file) {
    byte_t block[DEFAULT_HEADER_SIZE];
    file.seekg(0);
    file.read(reinterpret_cast<char *>(block), DEFAULT_HEADER_SIZE);
    // byte_t* cursor = block + CHECKSUM_SIZE;
    // for (auto i = 0; i < 4; i++) {
    //     printf("%02x ", *reinterpret_cast<uint8_t *>(cursor + i));
    // }
    // printf("\n");
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