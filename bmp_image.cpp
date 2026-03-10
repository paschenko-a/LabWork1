#include "bmp_image.h"
#include <algorithm>
#include <cmath>

BMP::BMP() : header(std::make_unique<BMPHeader>()) {}

BMP::BMP(const BMP& other) : header(std::make_unique<BMPHeader>(*other.header)), pixel_data(other.pixel_data) {}

BMP& BMP::operator=(const BMP& other) {
    if (this != &other) {
        header = std::make_unique<BMPHeader>(*other.header);
        pixel_data = other.pixel_data;
    }
    return *this;
}

bool BMP::Load(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file || !header->Read(file) || !header->IsValid()) return false;

    uint32_t width = std::abs(header->GetWidth());
    uint32_t height = std::abs(header->GetHeight());
    uint32_t rowSize = header->GetRowSize();
    
    pixel_data.assign(rowSize * height, 0);
    file.seekg(header->GetOffset());

    for (uint32_t y = 0; y < height; ++y) {
        uint32_t rowIdx = (header->GetHeight() > 0) ? (height - 1 - y) : y;
        file.read(reinterpret_cast<char*>(&pixel_data[rowIdx * rowSize]), width * 3);
        file.seekg(rowSize - width * 3, std::ios::cur);
    }
    return true;
}

bool BMP::Save(const std::string& filename) {
    if (!IsLoaded()) return false;
    std::ofstream file(filename, std::ios::binary);
    if (!file) return false;

    header->UpdateFileSize();
    if (!header->Write(file)) return false;

    uint32_t width = Width();
    uint32_t height = Height();
    uint32_t rowSize = header->GetRowSize();
    std::vector<uint8_t> padding(rowSize - width * 3, 0);

    for (uint32_t y = 0; y < height; ++y) {
        uint32_t rowIdx = (header->GetHeight() > 0) ? (height - 1 - y) : y;
        file.write(reinterpret_cast<const char*>(&pixel_data[rowIdx * rowSize]), width * 3);
        if (!padding.empty()) file.write(reinterpret_cast<const char*>(padding.data()), padding.size());
    }
    return true;
}

void BMP::GetPixel(int x, int y, uint8_t& r, uint8_t& g, uint8_t& b) const {
    uint32_t rowSize = header->GetRowSize();
    size_t idx = static_cast<size_t>(y) * rowSize + static_cast<size_t>(x) * 3;
    b = pixel_data[idx];
    g = pixel_data[idx + 1];
    r = pixel_data[idx + 2];
}

void BMP::SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    uint32_t rowSize = header->GetRowSize();
    size_t idx = static_cast<size_t>(y) * rowSize + static_cast<size_t>(x) * 3;
    pixel_data[idx] = b;
    pixel_data[idx + 1] = g;
    pixel_data[idx + 2] = r;
}

std::size_t BMP::Width() const { return std::abs(header->GetWidth()); }
std::size_t BMP::Height() const { return std::abs(header->GetHeight()); }
bool BMP::IsLoaded() const { return !pixel_data.empty(); }
std::size_t BMP::MemoryUsage() const { return pixel_data.size() + sizeof(BMPHeader); }

BMP BMP::RotateClockwise() const {
    BMP res;
    res.header->SetWidth(Height());
    res.header->SetHeight(Width());
    res.pixel_data.assign(res.header->GetRowSize() * res.Height(), 0);
    for (uint32_t y = 0; y < Height(); ++y) {
        for (uint32_t x = 0; x < Width(); ++x) {
            uint8_t r, g, b;
            GetPixel(x, y, r, g, b);
            res.SetPixel(Height() - 1 - y, x, r, g, b);
        }
    }
    return res;
}

BMP BMP::RotateCounterClockwise() const {
    BMP res;
    res.header->SetWidth(Height());
    res.header->SetHeight(Width());
    res.pixel_data.assign(res.header->GetRowSize() * res.Height(), 0);
    for (uint32_t y = 0; y < Height(); ++y) {
        for (uint32_t x = 0; x < Width(); ++x) {
            uint8_t r, g, b;
            GetPixel(x, y, r, g, b);
            res.SetPixel(y, Width() - 1 - x, r, g, b);
        }
    }
    return res;
}

BMP BMP::ApplyGaussianFilter() const {
    static const double k[3][3] = {{1./16, 2./16, 1./16}, {2./16, 4./16, 2./16}, {1./16, 2./16, 1./16}};
    BMP res(*this);
    int w = Width(), h = Height();
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            double r = 0, g = 0, b = 0;
            for (int ky = -1; ky <= 1; ++ky) {
                for (int kx = -1; kx <= 1; ++kx) {
                    int px = std::clamp(x + kx, 0, w - 1);
                    int py = std::clamp(y + ky, 0, h - 1);
                    uint8_t pr, pg, pb;
                    GetPixel(px, py, pr, pg, pb);
                    r += pr * k[ky + 1][kx + 1];
                    g += pg * k[ky + 1][kx + 1];
                    b += pb * k[ky + 1][kx + 1];
                }
            }
            res.SetPixel(x, y, (uint8_t)r, (uint8_t)g, (uint8_t)b);
        }
    }
    return res;
}