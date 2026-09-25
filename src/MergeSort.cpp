#include "MergeSort.hpp"

#include <algorithm>

const wchar_t* MergeSort::displayName() const
{
    return L"マージソート / Merge Sort";
}

const wchar_t* MergeSort::description() const
{
    return L"小さな整列済みの区間を、値の順番を比べながら結合します。\n"
           L"区間の幅を1、2、4…と広げ、配列全体を整列します。";
}

void MergeSort::reset(const std::vector<int>& values)
{
    resetState(values);
    buffer_ = values;
    width_ = 1;
    start_ = 0;
    beginMerge();
}

void MergeSort::beginMerge()
{
    const auto size = state_.values.size();
    middle_ = start_ + std::min(width_, size - start_);
    end_ = middle_ + std::min(width_, size - middle_);
    left_ = start_;
    right_ = middle_;
    output_ = start_;
}

void MergeSort::step()
{
    if (state_.finished)
        return;

    // Read from a snapshot of the previous pass so writes cannot destroy
    // unread values. Prefer the left run on equality to keep the sort stable.
    state_.highlighted = {output_};
    if (left_ < middle_ && (right_ == end_ || buffer_[left_] <= buffer_[right_]))
        state_.values[output_] = buffer_[left_++];
    else
        state_.values[output_] = buffer_[right_++];
    ++output_;
    ++state_.steps;

    if (output_ == end_)
    {
        start_ = end_;
        if (start_ == state_.values.size())
        {
            const auto size = state_.values.size();
            // Equivalent to width_ *= 2, capped to avoid overflow.
            width_ += std::min(width_, size - width_);
            if (width_ == size)
            {
                state_.finished = true;
                state_.highlighted.clear();
                return;
            }
            buffer_ = state_.values;
            start_ = 0;
        }
        beginMerge();
    }
}
