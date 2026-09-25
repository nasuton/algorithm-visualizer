#pragma once

#include "SortAlgorithm.hpp"

class CountingSort final : public SortAlgorithm
{
public:
    // Reject huge value ranges instead of attempting an unbounded allocation.
    static constexpr std::size_t maximumRange = 1'000'000;
    void reset(const std::vector<int>& values) override;
    void step() override;
    [[nodiscard]] const char* name() const override { return "Counting Sort"; }
    [[nodiscard]] const wchar_t* displayName() const override;
    [[nodiscard]] const wchar_t* description() const override;

private:
    std::vector<std::size_t> counts_;
    int minimum_ = 0;
    std::size_t position_ = 0;
    std::size_t bucket_ = 0;
    bool writing_ = false;
};

