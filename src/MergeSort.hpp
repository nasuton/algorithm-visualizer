#pragma once

#include "SortAlgorithm.hpp"

// Bottom-up merge sort: merge runs of length 1, 2, 4, ... .
class MergeSort final : public SortAlgorithm
{
public:
    void reset(const std::vector<int>& values) override;
    void step() override;
    [[nodiscard]] const char* name() const override { return "Merge Sort"; }
    [[nodiscard]] const wchar_t* displayName() const override;
    [[nodiscard]] const wchar_t* description() const override;

private:
    void beginMerge();

    std::vector<int> buffer_;
    std::size_t width_ = 1;
    std::size_t start_ = 0;
    std::size_t middle_ = 0;
    std::size_t end_ = 0;
    std::size_t left_ = 0;
    std::size_t right_ = 0;
    std::size_t output_ = 0;
};
