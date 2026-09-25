#include "InsertionSort.hpp"

#include <utility>

const wchar_t* InsertionSort::displayName() const { return L"挿入ソート / Insertion Sort"; }
const wchar_t* InsertionSort::description() const
{
    return L"左側の整列済みの部分に、次の値を挿入します。\n"
           L"左隣の値が大きい間は交換し、適切な位置まで移動させます。";
}

void InsertionSort::reset(const std::vector<int>& values)
{
    resetState(values);
    insertion_ = position_ = 1;
}

void InsertionSort::step()
{
    if (state_.finished)
        return;
    state_.highlighted = {position_ - 1, position_};
    state_.detail = L"挿入: 値 " + std::to_wstring(state_.values[position_]) + L" と左隣を比較";
    ++state_.steps;
    if (state_.values[position_ - 1] > state_.values[position_])
    {
        std::swap(state_.values[position_ - 1], state_.values[position_]);
        --position_;
        if (position_ != 0)
            return;
    }
    position_ = ++insertion_;
    if (insertion_ == state_.values.size())
        finish();
}
