#pragma once
#include <cstdint>
#include <fstream>

#pragma pack(push, 1)
struct BMPHeaderData {
    uint16_t type{0x4D42};
    uint32_t size{0};
    uint16_t reserved1{0};
    uint16_t reserved2{0};
    uint32_t offset{54};
    uint32_t info_size{40};
    int32_t width{0};
    int32_t height{0};
    uint16_t planes{1};
    uint16_t bits_per_pixel{24};
    uint32_t compression{0};
    uint32_t image_size{0};
    int32_t x_ppm{0};
    int32_t y_ppm{0};
    uint32_t colors_used{0};
    uint32_t important_colors{0};
};
#pragma pack(pop)

class BMPHeader {
private:
    BMPHeaderData data;

public:
    BMPHeader() = default;
    bool Read(std::ifstream& file);
    bool Write(std::ofstream& file) const;
    bool IsValid() const;
    int32_t GetWidth() const;
    int32_t GetHeight() const;
    uint32_t GetOffset() const;
    void SetWidth(int32_t w);
    void SetHeight(int32_t h);
    void UpdateFileSize();
    uint32_t GetRowSize() const;
};