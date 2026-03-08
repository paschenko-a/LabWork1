#include "bmp_header.h"
#include <cmath>

bool BMPHeader::Read(std::ifstream& file) {
    file.read(reinterpret_cast<char*>(&data), sizeof(data));
    return file.good();
}

bool BMPHeader::Write(std::ofstream& file) const {
    file.write(reinterpret_cast<const char*>(&data), sizeof(data));
    return file.good();
}

bool BMPHeader::IsValid() const {
    return data.type == 0x4D42 && data.bits_per_pixel == 24 && data.width > 0 && data.height != 0;
}

int32_t BMPHeader::GetWidth() const { return data.width; }
int32_t BMPHeader::GetHeight() const { return data.height; }
uint32_t BMPHeader::GetOffset() const { return data.offset; }

void BMPHeader::SetWidth(int32_t w) { data.width = w; }
void BMPHeader::SetHeight(int32_t h) { data.height = h; }

void BMPHeader::UpdateFileSize() {
    uint32_t rowSize = GetRowSize();
    data.image_size = rowSize * std::abs(data.height);
    data.size = data.offset + data.image_size;
}

uint32_t BMPHeader::GetRowSize() const {
    return ((std::abs(data.width) * 3 + 3) / 4) * 4;
}