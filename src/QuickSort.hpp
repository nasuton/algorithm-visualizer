#pragma once

#include "SortAlgorithm.hpp"
#include <utility>

class QuickSort final : public SortAlgorithm
{
public:
    void reset(const std::vector<int>& values) override;
    void step() override;
    [[nodiscard]] const char* name() const override { return "Quick Sort"; }
    [[nodiscard]] const wchar_t* displayName() const override;
    [[nodiscard]] const wchar_t* description() const override;

private:
    void beginPartition();
    std::vector<std::pair<std::size_t, std::size_t>> pending_;
    std::size_t begin_ = 0;
    std::size_t end_ = 0;
    std::size_t scan_ = 0;
    std::size_t boundary_ = 0;
};

