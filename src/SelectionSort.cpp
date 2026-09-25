#include "SelectionSort.hpp"

#include <utility>

const wchar_t* SelectionSort::displayName() const { return L"選択ソート / Selection Sort"; }
const wchar_t* SelectionSort::description() const
{
    return L"未整列の部分から最小の値を探し、先頭の値と交換します。\n"
           L"これを繰り返し、左端から1つずつ順番を確定させます。";
}

void SelectionSort::reset(const std::vector<int>& values)
{
    resetState(values);
    position_ = minimum_ = 0;
    scan_ = 1;
}

void SelectionSort::step()
{
    if (state_.finished)
        return;
    ++state_.steps;
    if (scan_ < state_.values.size())
    {
        state_.highlighted = {minimum_, scan_};
        if (state_.values[scan_] < state_.values[minimum_])
            minimum_ = scan_;
        state_.detail = L"探索: 現在の最小値は " + std::to_wstring(state_.values[minimum_]);
        ++scan_;
    }
    else
    {
        state_.highlighted = {position_, minimum_};
        std::swap(state_.values[position_], state_.values[minimum_]);
        state_.detail = L"確定: 左から " + std::to_wstring(position_ + 1) + L" 番目に最小値を配置";
        minimum_ = ++position_;
        scan_ = position_ + 1;
        if (scan_ == state_.values.size())
            finish();
    }
}
