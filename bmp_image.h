#pragma once
#include "image.h"
#include "bmp_header.h"
#include <vector>
#include <memory>

class BMP : public Image {
private:
    std::unique_ptr<BMPHeader> header;
    std::vector<uint8_t> pixel_data;

    void SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);
    void GetPixel(int x, int y, uint8_t& r, uint8_t& g, uint8_t& b) const;

public:
    BMP();
    BMP(const BMP& other);
    BMP& operator=(const BMP& other);
    BMP(BMP&&) noexcept = default;
    BMP& operator=(BMP&&) noexcept = default;
    ~BMP() override = default;

    bool Load(const std::string& filename) override;
    bool Save(const std::string& filename) override;
    std::size_t Width() const override;
    std::size_t Height() const override;
    std::size_t MemoryUsage() const override;
    bool IsLoaded() const override;

    BMP RotateClockwise() const;
    BMP RotateCounterClockwise() const;
    BMP ApplyGaussianFilter() const;
};