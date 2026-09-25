#include "HeapSort.hpp"

#include <utility>

const wchar_t* HeapSort::displayName() const { return L"ヒープソート / Heap Sort"; }
const wchar_t* HeapSort::description() const
{
    return L"親の値が子以上になる最大ヒープを作り、最大値を右端へ移します。\n"
           L"残ったヒープを修復して繰り返します。木構造は配列で表しています。";
}

void HeapSort::reset(const std::vector<int>& values)
{
    resetState(values);
    heapSize_ = values.size();
    nextRoot_ = heapSize_ / 2;
    root_ = 0;
    building_ = true;
    extracting_ = false;
    if (!state_.finished)
        root_ = --nextRoot_;
}

void HeapSort::endSift()
{
    if (building_ && nextRoot_ != 0)
        root_ = --nextRoot_;
    else
    {
        building_ = false;
        extracting_ = true;
    }
}

void HeapSort::step()
{
    if (state_.finished)
        return;
    ++state_.steps;
    if (extracting_)
    {
        state_.highlighted = {0, heapSize_ - 1};
        std::swap(state_.values[0], state_.values[heapSize_ - 1]);
        --heapSize_;
        state_.detail = L"最大値を右端に確定 / 残りのヒープ: " + std::to_wstring(heapSize_) + L" 要素";
        if (heapSize_ < 2)
            finish();
        else
        {
            root_ = 0;
            extracting_ = false;
        }
        return;
    }

    // One step sifts down by at most one level, comparing a parent and its children.
    auto largest = root_;
    const auto child = root_ * 2 + 1;
    state_.highlighted = {root_};
    if (child < heapSize_)
    {
        state_.highlighted.push_back(child);
        if (state_.values[child] > state_.values[largest])
            largest = child;
    }
    if (child + 1 < heapSize_)
    {
        state_.highlighted.push_back(child + 1);
        if (state_.values[child + 1] > state_.values[largest])
            largest = child + 1;
    }
    state_.detail = building_ ? L"ヒープ構築: 親と子を比較" : L"ヒープ修復: 親と子を比較";
    if (largest != root_)
    {
        std::swap(state_.values[root_], state_.values[largest]);
        root_ = largest;
        if (root_ * 2 + 1 < heapSize_)
            return;
    }
    endSift();
}
