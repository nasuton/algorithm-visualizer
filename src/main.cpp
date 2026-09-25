#include <SFML/Graphics.hpp>

#include "BubbleSort.hpp"
#include "MergeSort.hpp"
#include "InsertionSort.hpp"
#include "SelectionSort.hpp"
#include "QuickSort.hpp"
#include "HeapSort.hpp"
#include "CountingSort.hpp"
#include "RadixSort.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace
{
bool loadFont(sf::Font& font, int argc, char* argv[])
{
    // An explicit path supports other operating systems and custom fonts.
    if (argc > 1)
        return font.openFromFile(std::filesystem::path(argv[1]));

    const std::vector<std::filesystem::path> candidates{
        "assets/font.ttf",
        "C:/Windows/Fonts/meiryo.ttc",
        "C:/Windows/Fonts/YuGothM.ttc"
    };
    for (const auto& path : candidates)
    {
        std::error_code error;
        if (std::filesystem::is_regular_file(path, error) && font.openFromFile(path))
            return true;
    }
    return false;
}
}

int main(int argc, char* argv[])
{
    sf::Font font;
    if (!loadFont(font, argc, argv))
    {
        std::cerr << "Could not load a Japanese font. Pass a font file path as the first argument.\n"
                  << "Example: algorithm_visualization.exe C:/Windows/Fonts/meiryo.ttc\n";
        return 1;
    }

    sf::RenderWindow window(sf::VideoMode({1040, 760}), "Algorithm Visualization");
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);

    const std::vector<int> initialValues{5, 2, 8, 4, 10, 3, 7, 1, 9, 6};
    MergeSort mergeSort;
    BubbleSort bubbleSort;
    InsertionSort insertionSort;
    SelectionSort selectionSort;
    QuickSort quickSort;
    HeapSort heapSort;
    CountingSort countingSort;
    RadixSort radixSort;
    const std::array<SortAlgorithm*, 8> sorts{
        &mergeSort, &bubbleSort, &insertionSort, &selectionSort,
        &quickSort, &heapSort, &countingSort, &radixSort
    };
    using Key = sf::Keyboard::Key;
    const std::array<Key, 8> numberKeys{
        Key::Num1, Key::Num2, Key::Num3, Key::Num4,
        Key::Num5, Key::Num6, Key::Num7, Key::Num8
    };
    const std::array<Key, 8> numpadKeys{
        Key::Numpad1, Key::Numpad2, Key::Numpad3, Key::Numpad4,
        Key::Numpad5, Key::Numpad6, Key::Numpad7, Key::Numpad8
    };
    SortAlgorithm* activeSort = sorts.front();
    activeSort->reset(initialValues);
    bool paused = false;
    sf::Clock stepClock;
    constexpr float stepSeconds = 0.25f;

    sf::Text heading(font, activeSort->displayName(), 28);
    heading.setPosition({40.f, 20.f});
    heading.setFillColor(sf::Color(240, 244, 250));

    sf::Text explanation(font, activeSort->description(), 19);
    explanation.setPosition({40.f, 72.f});
    explanation.setFillColor(sf::Color(195, 208, 225));
    explanation.setLineSpacing(1.25f);

    sf::Text progress(font, L"", 16);
    progress.setPosition({40.f, 182.f});

    sf::Text detail(font, L"", 17);
    detail.setPosition({40.f, 145.f});
    detail.setFillColor(sf::Color(125, 205, 245));

    sf::Text controls(font,
        L"1: マージ    2: バブル    3: 挿入    4: 選択\n"
        L"5: クイック    6: ヒープ    7: カウント    8: 基数\n"
        L"Space: 一時停止 / 再開    R: 最初から    数字キーで切り替え", 18);
    controls.setPosition({40.f, 625.f});
    controls.setLineSpacing(1.2f);
    controls.setFillColor(sf::Color(195, 208, 225));

    sf::RectangleShape separator({960.f, 1.f});
    separator.setPosition({40.f, 220.f});
    separator.setFillColor(sf::Color(65, 77, 96));

    const auto restart = [&]
    {
        activeSort->reset(initialValues);
        paused = false;
        stepClock.restart();
        heading.setString(activeSort->displayName());
        explanation.setString(activeSort->description());
    };

    std::string previousTitle;

    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (const auto* key = event->getIf<sf::Event::KeyPressed>())
            {
                for (std::size_t i = 0; i < sorts.size(); ++i)
                {
                    if (key->code == numberKeys[i] || key->code == numpadKeys[i])
                    {
                        activeSort = sorts[i];
                        restart();
                        break;
                    }
                }
                switch (key->code)
                {
                case Key::Space:
                    paused = !paused;
                    stepClock.restart();
                    break;
                case Key::R:
                    restart();
                    break;
                default:
                    break;
                }
            }
        }

        if (!window.isOpen())
            break;

        if (!paused && !activeSort->state().finished &&
            stepClock.getElapsedTime().asSeconds() >= stepSeconds)
        {
            activeSort->step();
            stepClock.restart();
        }

        const auto& state = activeSort->state();
        detail.setString(state.finished ? L"配列全体の整列が完了しました。" : state.detail);
        const std::wstring visibleStatus = state.finished ? L"完了" : (paused ? L"一時停止中" : L"実行中");
        progress.setString(visibleStatus + L"  |  ステップ: " + std::to_wstring(state.steps) +
            L"  |  黄色: 処理対象 / 緑: 整列完了");
        progress.setFillColor(state.finished ? sf::Color(90, 205, 140) : sf::Color(255, 200, 80));
        const std::string status = state.finished ? "Done" : (paused ? "Paused" : "Running");
        const std::string title = std::string(activeSort->name()) + " | " + status +
            " | Steps: " + std::to_string(state.steps) +
            " | 1-8: Select  Space: Pause  R: Restart";
        if (title != previousTitle)
        {
            window.setTitle(title);
            previousTitle = title;
        }

        window.clear(sf::Color(24, 28, 38));
        window.draw(heading);
        window.draw(explanation);
        window.draw(progress);
        window.draw(detail);
        window.draw(separator);
        window.draw(controls);
        for (std::size_t i = 0; i < state.values.size(); ++i)
        {
            const float height = static_cast<float>(state.values[i]) * 30.f;
            sf::RectangleShape bar({64.f, height});
            bar.setPosition({70.f + static_cast<float>(i) * 92.f, 580.f - height});
            const bool highlighted = std::find(state.highlighted.begin(), state.highlighted.end(), i)
                != state.highlighted.end();
            bar.setFillColor(state.finished ? sf::Color(90, 205, 140)
                : (highlighted ? sf::Color(255, 200, 80) : sf::Color(80, 180, 230)));
            window.draw(bar);
        }
        window.display();
    }
}
