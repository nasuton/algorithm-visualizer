#pragma once

#include "SortAlgorithm.hpp"

class BubbleSort final : public SortAlgorithm
{
public:
    void reset(const std::vector<int>& values) override;
    void step() override;
    [[nodiscard]] const char* name() const override { return "Bubble Sort"; }
    [[nodiscard]] const wchar_t* displayName() const override;
    [[nodiscard]] const wchar_t* description() const override;

private:
    std::size_t position_ = 0;
    std::size_t end_ = 0;
    bool swapped_ = false;
};
