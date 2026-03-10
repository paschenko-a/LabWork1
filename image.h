#pragma once
#include <cstddef>
#include <string>

class Image {
public:
    virtual ~Image() = default;
    virtual bool Load(const std::string& filename) = 0;
    virtual bool Save(const std::string& filename) = 0;
    virtual std::size_t Width() const = 0;
    virtual std::size_t Height() const = 0;
    virtual std::size_t MemoryUsage() const = 0;
    virtual bool IsLoaded() const = 0;
};