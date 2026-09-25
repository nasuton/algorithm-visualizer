#include "BubbleSort.hpp"
#include "MergeSort.hpp"
#include "InsertionSort.hpp"
#include "SelectionSort.hpp"
#include "QuickSort.hpp"
#include "HeapSort.hpp"
#include "CountingSort.hpp"
#include "RadixSort.hpp"

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <limits>
#include <random>
#include <stdexcept>
#include <vector>

void require(bool condition, const char* message)
{
    if (!condition)
        throw std::runtime_error(message);
}

void verify(SortAlgorithm& sort, const std::vector<int>& input)
{
    sort.reset(input);
    require(sort.state().values == input, "reset must restore input");
    require(sort.state().steps == 0, "reset must clear step count");
    require(sort.state().highlighted.empty(), "reset must clear highlights");
    require(sort.state().finished == (input.size() < 2), "initial completion state");

    auto expected = input;
    std::stable_sort(expected.begin(), expected.end());
    // Includes up to ten decimal digit passes for full-range 32-bit integers.
    const auto limit = 2 * input.size() * input.size() + 100 * input.size() + 100;
    std::size_t steps = 0;
    while (!sort.state().finished)
    {
        require(steps++ < limit, "sort must terminate");
        const auto previousSteps = sort.state().steps;
        sort.step();
        require(sort.state().steps == previousSteps + 1, "step must advance once");
        require(sort.state().values.size() == input.size(), "size must stay constant");
        for (const auto index : sort.state().highlighted)
            require(index < input.size(), "highlight index must be valid");
    }
    require(sort.state().values == expected, "result must match stable_sort");
    require(sort.state().highlighted.empty(), "completed sort must clear highlights");
    const auto finalSteps = sort.state().steps;
    sort.step();
    require(sort.state().values == expected && sort.state().steps == finalSteps,
            "step after completion must do nothing");
}

int main()
{
    try
    {
        BubbleSort bubble;
        MergeSort merge;
        InsertionSort insertion;
        SelectionSort selection;
        QuickSort quick;
        HeapSort heap;
        CountingSort counting;
        RadixSort radix;
        const std::vector<SortAlgorithm*> sorts{
            &bubble, &merge, &insertion, &selection, &quick, &heap, &counting, &radix
        };
        std::vector<std::vector<int>> cases{
            {}, {42}, {2, 1}, {1, 2}, {7, 7, 7},
            {1, 2, 3, 4, 5}, {5, 4, 3, 2, 1},
            {5, 2, 8, 4, 10, 3, 7, 1, 9, 6},
            {-3, 0, -1, -3, 2},
            {std::numeric_limits<int>::max(), 0, std::numeric_limits<int>::min()},
            {100, 10, 1, 0, 1000, 99, 101, 10},
            {std::numeric_limits<int>::min() + 2, std::numeric_limits<int>::min()},
            {std::numeric_limits<int>::max(), std::numeric_limits<int>::max() - 2}
        };
        std::mt19937 random(42);
        std::uniform_int_distribution<int> value(-20, 20);
        for (std::size_t size = 0; size <= 100; ++size)
        {
            std::vector<int> input(size);
            for (auto& element : input)
                element = value(random);
            cases.push_back(input);
        }

        // Exhaustive small permutations exercise partition and heap boundaries.
        for (int size = 2; size <= 6; ++size)
        {
            std::vector<int> input;
            for (int i = 0; i < size; ++i)
                input.push_back(i - 3);
            do
            {
                cases.push_back(input);
            } while (std::next_permutation(input.begin(), input.end()));
        }
        std::uniform_int_distribution<int> fullRange(
            std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
        for (int trial = 0; trial < 30; ++trial)
        {
            std::vector<int> input(30);
            for (auto& element : input)
                element = fullRange(random);
            cases.push_back(input);
        }

        std::size_t verified = 0;
        std::size_t rejected = 0;
        for (auto* sort : sorts)
        {
            // Calls before reset and resets in the middle of an animation are safe.
            sort->step();
            for (const auto& input : cases)
            {
                if (sort == &counting && input.size() > 1)
                {
                    const auto extrema = std::minmax_element(input.begin(), input.end());
                    const auto range = static_cast<std::int64_t>(*extrema.second) - *extrema.first + 1;
                    if (range > static_cast<std::int64_t>(CountingSort::maximumRange))
                    {
                        bool threw = false;
                        try { counting.reset(input); }
                        catch (const std::length_error&) { threw = true; }
                        require(threw, "counting sort must reject huge ranges");
                        ++rejected;
                        continue;
                    }
                }
                sort->reset({9, 8, 7, 6, 5});
                sort->step();
                verify(*sort, input);
                ++verified;
            }

            // Interrupt at every step of a complete run, including later passes,
            // then reset to a different data set to detect stale auxiliary state.
            const std::vector<int> interrupted{5, 2, 8, 4, 10, 3, 7, 1, 9, 6};
            verify(*sort, interrupted);
            const auto totalSteps = sort->state().steps;
            for (std::size_t prefix = 0; prefix <= totalSteps; ++prefix)
            {
                sort->reset(interrupted);
                for (std::size_t step = 0; step < prefix; ++step)
                    sort->step();
                verify(*sort, {-2, 9, 0, -2, 1});
            }
        }

        bubble.reset({3, 2, 1});
        bubble.step();
        require(bubble.state().values == std::vector<int>({2, 3, 1}),
                "bubble animation must perform just one adjacent comparison");
        require(!bubble.state().finished, "bubble animation must remain in progress");
        merge.reset({3, 2, 1});
        merge.step();
        require(merge.state().values == std::vector<int>({2, 2, 1}),
                "merge animation must write just one value from its buffer");
        require(!merge.state().finished, "merge animation must remain in progress");

        insertion.reset({3, 2, 1});
        insertion.step();
        require(insertion.state().values == std::vector<int>({2, 3, 1}) && !insertion.state().finished,
                "insertion must advance by one adjacent comparison");
        selection.reset({3, 2, 1});
        selection.step();
        require(selection.state().values == std::vector<int>({3, 2, 1}) && !selection.state().finished,
                "selection must scan before swapping the minimum");
        quick.reset({3, 2, 1});
        quick.step();
        require(quick.state().values == std::vector<int>({3, 2, 1}) && !quick.state().finished,
                "quick must compare against the pivot before final placement");
        heap.reset({1, 2, 3});
        heap.step();
        require(heap.state().values == std::vector<int>({3, 2, 1}) && !heap.state().finished,
                "heap must build a max heap before extraction");
        counting.reset({2, 1, 2});
        for (int i = 0; i < 3; ++i)
        {
            counting.step();
            require(counting.state().values == std::vector<int>({2, 1, 2}),
                    "counting must not overwrite input during counting");
        }
        counting.step();
        require(counting.state().values == std::vector<int>({1, 1, 2}), "counting writes one value");
        radix.reset({12, 11, 0});
        for (int i = 0; i < 3; ++i)
        {
            radix.step();
            require(radix.state().values == std::vector<int>({12, 11, 0}),
                    "radix must distribute the whole pass before gathering");
        }
        radix.step();
        require(radix.state().values == std::vector<int>({0, 11, 0}), "radix gathers one value");

        verify(counting, {0, static_cast<int>(CountingSort::maximumRange - 1)});
        bool rejectedBoundary = false;
        try { counting.reset({0, static_cast<int>(CountingSort::maximumRange)}); }
        catch (const std::length_error&) { rejectedBoundary = true; }
        require(rejectedBoundary, "counting range limit must be enforced exactly");
        verify(counting, {2, 1});

        std::cout << "Passed " << verified << " sorting cases, " << rejected
                  << " range rejection cases, reset checks, and animation step checks.\n";
    }
    catch (const std::exception& error)
    {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
