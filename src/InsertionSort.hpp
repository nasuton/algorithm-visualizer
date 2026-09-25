#pragma once

#include "SortAlgorithm.hpp"

class InsertionSort final : public SortAlgorithm
{
public:
    void reset(const std::vector<int>& values) override;
    void step() override;
    [[nodiscard]] const char* name() const override { return "Insertion Sort"; }
    [[nodiscard]] const wchar_t* displayName() const override;
    [[nodiscard]] const wchar_t* description() const override;

private:
    std::size_t insertion_ = 1;
    std::size_t position_ = 1;
};

