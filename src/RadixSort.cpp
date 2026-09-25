#include "RadixSort.hpp"

#include <algorithm>

const wchar_t* RadixSort::displayName() const { return L"基数ソート / Radix Sort"; }
const wchar_t* RadixSort::description() const
{
    return L"最小値からの差をキーにし、1の位、10の位…の順で分類します。\n"
           L"各桁を0〜9のバケットへ振り分け、順序を保って集め直します。";
}

RadixSort::Unsigned RadixSort::key(int value) const
{
    // Unsigned subtraction represents value - minimum_ without signed overflow,
    // including inputs containing both INT_MIN and INT_MAX.
    return static_cast<Unsigned>(value) - static_cast<Unsigned>(minimum_);
}

void RadixSort::reset(const std::vector<int>& values)
{
    resetState(values);
    for (auto& bucket : buckets_)
        bucket.clear();
    minimum_ = 0;
    maximumKey_ = 0;
    divisor_ = 1;
    position_ = bucket_ = bucketPosition_ = 0;
    gathering_ = false;
    if (!values.empty())
    {
        const auto extrema = std::minmax_element(values.begin(), values.end());
        minimum_ = *extrema.first;
        maximumKey_ = key(*extrema.second);
    }
}

void RadixSort::step()
{
    if (state_.finished)
        return;
    ++state_.steps;
    state_.highlighted = {position_};
    if (!gathering_)
    {
        const int value = state_.values[position_];
        const auto digit = static_cast<std::size_t>((key(value) / divisor_) % 10);
        buckets_[digit].push_back(value);
        state_.detail = std::to_wstring(divisor_) + L" の位を分類: 値 " + std::to_wstring(value) +
            L" → バケット " + std::to_wstring(digit) + L" (" + std::to_wstring(buckets_[digit].size()) + L" 個)";
        if (++position_ == state_.values.size())
        {
            position_ = 0;
            gathering_ = true;
        }
    }
    else
    {
        while (bucketPosition_ == buckets_[bucket_].size())
        {
            ++bucket_;
            bucketPosition_ = 0;
        }
        state_.values[position_] = buckets_[bucket_][bucketPosition_++];
        state_.detail = std::to_wstring(divisor_) + L" の位を回収: バケット " + std::to_wstring(bucket_);
        if (++position_ == state_.values.size())
        {
            if (maximumKey_ / divisor_ < 10)
                finish();
            else
            {
                divisor_ *= 10;
                position_ = bucket_ = bucketPosition_ = 0;
                gathering_ = false;
                for (auto& bucket : buckets_)
                    bucket.clear();
            }
        }
    }
}
