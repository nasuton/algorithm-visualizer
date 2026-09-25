#pragma once

#include <cstddef>
#include <string>
#include <vector>

// The sorting classes have no dependency on the rendering library.
struct SortState
{
    std::vector<int> values;
    std::vector<std::size_t> highlighted;
    std::size_t steps = 0;
    bool finished = true;
    std::wstring detail;
};

class SortAlgorithm
{
public:
    virtual ~SortAlgorithm() = default;
    virtual void reset(const std::vector<int>& values) = 0;
    virtual void step() = 0;
    [[nodiscard]] virtual const char* name() const = 0;
    [[nodiscard]] virtual const wchar_t* displayName() const = 0;
    [[nodiscard]] virtual const wchar_t* description() const = 0;
    [[nodiscard]] const SortState& state() const { return state_; }

protected:
    void resetState(const std::vector<int>& values)
    {
        state_ = {values, {}, 0, values.size() < 2, {}};
    }

    void finish()
    {
        state_.finished = true;
        state_.highlighted.clear();
    }

    SortState state_;
};
