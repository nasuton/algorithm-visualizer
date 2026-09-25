#pragma once

#include "SortAlgorithm.hpp"

class HeapSort final : public SortAlgorithm
{
public:
    void reset(const std::vector<int>& values) override;
    void step() override;
    [[nodiscard]] const char* name() const override { return "Heap Sort"; }
    [[nodiscard]] const wchar_t* displayName() const override;
    [[nodiscard]] const wchar_t* description() const override;

private:
    void endSift();
    std::size_t heapSize_ = 0;
    std::size_t nextRoot_ = 0;
    std::size_t root_ = 0;
    bool building_ = true;
    bool extracting_ = false;
};

