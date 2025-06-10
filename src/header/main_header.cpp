#include "../include/header/main_header.hpp"

MainHeader::MainHeader(ifstream& file) {
    byte_t block[DEFAULT_HEADER_SIZE];
    file.seekg(CHECKSUM_SIZE, ios::beg);
    file.read(reinterpret_cast<char *>(block), DEFAULT_HEADER_SIZE);
    memcpy(magic_bytes, block, 4);
    memcpy(&version, block+4, sizeof(uint64_t));
}

//MainHeader::MainHeader([[maybe_unused]] int _unused) {
//    assert(binary_file != nullptr);
//    byte_t* block = binary_file;
//    Reader reader(block);
//    reader.Read<char>(magic_bytes, 4);
//    version = reader.Read<uint64_t>();
//}

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
