#include "CountingSort.hpp"

#include <algorithm>
#include <cstdint>
#include <stdexcept>

const wchar_t* CountingSort::displayName() const { return L"カウントソート / Counting Sort"; }
const wchar_t* CountingSort::description() const
{
    return L"各整数が何回現れるかを数え、小さい値から回数分だけ書き戻します。\n"
           L"値の範囲が狭い配列に向いています。負の整数も扱えます。";
}

void CountingSort::reset(const std::vector<int>& values)
{
    // Validate before changing state. Use wide arithmetic to avoid signed overflow.
    std::int64_t range = 0;
    int minimum = 0;
    if (values.size() > 1)
    {
        const auto extrema = std::minmax_element(values.begin(), values.end());
        minimum = *extrema.first;
        range = static_cast<std::int64_t>(*extrema.second) - minimum + 1;
        if (range > static_cast<std::int64_t>(maximumRange))
            throw std::length_error("CountingSort supports a value range of at most 1,000,000.");
    }
    resetState(values);
    minimum_ = minimum;
    counts_.assign(static_cast<std::size_t>(range), 0);
    position_ = bucket_ = 0;
    writing_ = false;
}

void CountingSort::step()
{
    if (state_.finished)
        return;
    ++state_.steps;
    state_.highlighted = {position_};
    if (!writing_)
    {
        const int value = state_.values[position_];
        const auto index = static_cast<std::size_t>(static_cast<std::int64_t>(value) - minimum_);
        ++counts_[index];
        state_.detail = L"集計: 値 " + std::to_wstring(value) + L" の出現回数 = " + std::to_wstring(counts_[index]);
        if (++position_ == state_.values.size())
        {
            position_ = 0;
            writing_ = true;
        }
    }
    else
    {
        while (counts_[bucket_] == 0)
            ++bucket_;
        const int value = static_cast<int>(static_cast<std::int64_t>(minimum_) + static_cast<std::int64_t>(bucket_));
        state_.values[position_] = value;
        --counts_[bucket_];
        state_.detail = L"書き戻し: 値 " + std::to_wstring(value) + L" / この値の残り " + std::to_wstring(counts_[bucket_]);
        if (++position_ == state_.values.size())
            finish();
    }
}
