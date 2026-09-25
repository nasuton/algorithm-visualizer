#include "BubbleSort.hpp"

#include <utility>

const wchar_t* BubbleSort::displayName() const
{
    return L"バブルソート / Bubble Sort";
}

const wchar_t* BubbleSort::description() const
{
    return L"隣り合う2つの値を比較し、順番が逆なら交換します。\n"
           L"これを繰り返すと、大きな値から右端に順番が確定していきます。";
}

void BubbleSort::reset(const std::vector<int>& values)
{
    resetState(values);
    position_ = 0;
    end_ = values.size();
    swapped_ = false;
}

void BubbleSort::step()
{
    if (state_.finished)
        return;

    // One step compares adjacent values and swaps them if necessary.
    state_.highlighted = {position_, position_ + 1};
    if (state_.values[position_] > state_.values[position_ + 1])
    {
        std::swap(state_.values[position_], state_.values[position_ + 1]);
        swapped_ = true;
    }
    ++state_.steps;
    ++position_;

    if (position_ + 1 == end_)
    {
        // The largest remaining value is now at the end of this pass.
        --end_;
        state_.finished = !swapped_ || end_ < 2;
        position_ = 0;
        swapped_ = false;
    }
    if (state_.finished)
        state_.highlighted.clear();
}
