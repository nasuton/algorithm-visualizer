#include "QuickSort.hpp"

#include <utility>

const wchar_t* QuickSort::displayName() const { return L"クイックソート / Quick Sort"; }
const wchar_t* QuickSort::description() const
{
    return L"区間の末尾を基準値（ピボット）にして、小さい値を左へ集めます。\n"
           L"基準値の位置を確定し、左右の区間にも同じ処理を繰り返します。";
}

void QuickSort::reset(const std::vector<int>& values)
{
    resetState(values);
    pending_.clear();
    begin_ = end_ = scan_ = boundary_ = 0;
    if (!state_.finished)
    {
        pending_.emplace_back(0, values.size());
        beginPartition();
    }
}

void QuickSort::beginPartition()
{
    if (pending_.empty())
    {
        finish();
        return;
    }
    const auto range = pending_.back();
    pending_.pop_back();
    begin_ = range.first;
    end_ = range.second;
    scan_ = boundary_ = begin_;
}

void QuickSort::step()
{
    if (state_.finished)
        return;
    const auto pivot = end_ - 1;
    ++state_.steps;
    if (scan_ < pivot)
    {
        state_.highlighted = {scan_, boundary_, pivot};
        state_.detail = L"分割: 基準値 " + std::to_wstring(state_.values[pivot]) + L" と比較";
        if (state_.values[scan_] <= state_.values[pivot])
        {
            std::swap(state_.values[scan_], state_.values[boundary_]);
            ++boundary_;
        }
        ++scan_;
    }
    else
    {
        state_.highlighted = {boundary_, pivot};
        std::swap(state_.values[boundary_], state_.values[pivot]);
        state_.detail = L"確定: 基準値を左から " + std::to_wstring(boundary_ + 1) + L" 番目に配置";
        // Half-open ranges; push right first to visit the left partition next.
        if (end_ - (boundary_ + 1) > 1)
            pending_.emplace_back(boundary_ + 1, end_);
        if (boundary_ - begin_ > 1)
            pending_.emplace_back(begin_, boundary_);
        beginPartition();
    }
}
