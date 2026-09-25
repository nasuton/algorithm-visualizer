#pragma once

#include "SortAlgorithm.hpp"
#include <array>
#include <type_traits>

class RadixSort final : public SortAlgorithm
{
public:
    void reset(const std::vector<int>& values) override;
    void step() override;
    [[nodiscard]] const char* name() const override { return "Radix Sort"; }
    [[nodiscard]] const wchar_t* displayName() const override;
    [[nodiscard]] const wchar_t* description() const override;

private:
    using Unsigned = std::make_unsigned_t<int>;
    [[nodiscard]] Unsigned key(int value) const;
    std::array<std::vector<int>, 10> buckets_;
    int minimum_ = 0;
    Unsigned maximumKey_ = 0;
    Unsigned divisor_ = 1;
    std::size_t position_ = 0;
    std::size_t bucket_ = 0;
    std::size_t bucketPosition_ = 0;
    bool gathering_ = false;
};

