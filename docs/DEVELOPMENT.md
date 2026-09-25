# Algorithm Visualization

C++ と SFML を使い、8種類のソートの途中経過を棒グラフで表示します。
各アルゴリズムは独立したクラスで実装し、0.25秒ごとに1ステップ進みます。

## 操作方法

ウィンドウを選択してキーを押してください。Enter は不要です。

| キー | 動作 |
| --- | --- |
| `1` / テンキー `1` | マージソートに切り替えて最初から再生（初期表示） |
| `2` / テンキー `2` | バブルソートに切り替えて最初から再生 |
| `3` / テンキー `3` | 挿入ソートに切り替えて最初から再生 |
| `4` / テンキー `4` | 選択ソートに切り替えて最初から再生 |
| `5` / テンキー `5` | クイックソートに切り替えて最初から再生 |
| `6` / テンキー `6` | ヒープソートに切り替えて最初から再生 |
| `7` / テンキー `7` | カウントソートに切り替えて最初から再生 |
| `8` / テンキー `8` | 基数ソートに切り替えて最初から再生 |
| `Space` | 一時停止 / 再開 |
| `R` | 現在のソートを最初から再生 |

同じ番号を再度押した場合も、同じ初期配列から再生します。
画面上部にソート名と2行の日本語説明、実行状態、ステップ数を表示します。
`1`〜`8` で切り替えると説明も連動して変わります。画面下部にはキー操作を表示します。
通常の棒は青、処理対象は黄色、ソート完了後はすべて緑になります。

- **BubbleSort**: 隣り合う2要素を比較し、必要なら交換する処理が1ステップです。
  黄色は比較した2要素です。交換のない周回で早期終了します。
- **MergeSort**: 幅1・2・4…の部分配列を統合するボトムアップ方式です。
  補助配列から1要素を書き戻す処理が1ステップで、書き込み先を黄色にします。
  書き戻し途中は同じ値が一時的に複数表示されますが、未処理の値は補助配列に保持しています。
- **InsertionSort**: 挿入中の値と左隣を1回比較し、必要なら交換します。
  その値を左へ移動して、整列済みの部分に挿入します。
- **SelectionSort**: 最小値候補と1要素を比較するステップと、探索終了後に最小値を先頭へ移すステップに分かれます。
- **QuickSort**: 区間末尾をピボットにする Lomuto 分割です。1要素の比較・必要な交換、またはピボットの確定が1ステップです。
  再帰の代わりに未処理区間のスタックを使い、基準値と比較対象を黄色で表示します。
- **HeapSort**: 最大ヒープを配列として扱います。親と子の比較・最大1段の交換、または最大値を右端へ移す処理が1ステップです。
  説明欄にヒープの構築・修復・最大値の確定を表示します。木そのものの描画は行いません。
- **CountingSort**: 1要素の出現回数を数える段階と、小さい値から1要素ずつ書き戻す段階を表示します。
  負数にも対応しますが、`最大値 − 最小値 + 1` は100万以下に限ります。
  範囲を超える入力は、巨大なメモリ確保を避けるため `reset()` が `std::length_error` を送出します。
- **RadixSort**: 最小値からの差を符号なし整数のキーにし、10進数の下位桁から安定に分類する LSD 基数ソートです。
  1要素のバケットへの振り分け、または1要素の回収が1ステップです。
  説明欄に処理中の桁・バケットを表示し、負数や `INT_MIN` / `INT_MAX` にも対応します。

カウント・基数ソートでも、書き戻し途中は同じ値が一時的に複数表示されます。
各ソートでステップの単位は異なるため、ステップ数や再生時間は実行速度のベンチマークではありません。

| クラス | 計算量（可視化の描画時間を除く） | 補助領域 |
| --- | --- | --- |
| `MergeSort` | O(n log n) | O(n) |
| `BubbleSort` | 平均・最悪 O(n²)、最良 O(n) | O(1) |
| `InsertionSort` | 平均・最悪 O(n²)、最良 O(n) | O(1) |
| `SelectionSort` | O(n²) | O(1) |
| `QuickSort` | 平均 O(n log n)、最悪 O(n²) | 区間スタック O(n) 以下 |
| `HeapSort` | O(n log n) | O(1) |
| `CountingSort` | O(n + k) | O(k) |
| `RadixSort` | O(d(n + 10)) | O(n + 10) |

`n` は要素数、`k` は値の範囲、`d` は最小値からの差の最大値の10進桁数です。
補助領域は表示用の入力コピーを除いた、アルゴリズム自体の作業領域です。

## 採用ライブラリ

**[SFML 3.1.0](https://www.sfml-dev.org/)** を使用します。
図形・線・文字の2D描画、ウィンドウ管理、キーボード・マウス入力を扱えるため、
ソートの棒グラフ、探索アルゴリズムのマス目、グラフの頂点・辺の可視化に適しています。
Graphics モジュールをリンクすると、必要な Window / System もリンクされます。
今回不要な Audio / Network はビルドしません。

導入方法は [SFML 公式 CMake テンプレート](https://github.com/SFML/cmake-sfml-project) を参考にしています。
CMake の設定ファイル名は `CMakeLists.txt` です。
`FetchContent` が SFML をビルドフォルダー内に取得し、アプリと一緒にビルドします。
システム全体へのライブラリのインストールや `cmake --install` は不要です。

## 必要なもの（Windows）

- CMake 3.28 以上
- Git（PATH に登録）
- Visual Studio 2022 または Build Tools 2022 の「C++ によるデスクトップ開発」
  （MSVC と Windows SDK を含む）
- 初回の依存ライブラリ取得に必要なインターネット接続

CMake はライブラリを取得しますが、コンパイラや Windows SDK 自体はインストールしません。
C++17 を使用します。

## ビルドと実行（Windows / PowerShell）

このフォルダーで次を実行してください。

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release --parallel
.\build\bin\Release\algorithm_visualization.exe
```

棒グラフのウィンドウが表示され、マージソートの再生が始まります。閉じるボタンで終了します。
デバッグ用には `Release` を `Debug` に置き換えてください。
初回は SFML とその依存ライブラリの取得・ビルドに時間がかかります。

既定では SFML を静的リンクします。Windows の MSVC ランタイムは動的リンクなので、
別の PC に配布する場合は対応する Visual C++ ランタイムが必要になる場合があります。

### Git の証明書エラーが出る場合

Windows で `SSL certificate problem: unable to get local issuer certificate` が出る場合は、
新しい PowerShell で次を設定してから、上の CMake コマンドを再実行してください。
Git for Windows が Windows の証明書ストアを使うようになります。
証明書の検証は有効なままで、Git のグローバル設定も変更しません。
この設定は、その PowerShell を閉じると終了します。

```powershell
$env:GIT_CONFIG_COUNT = '1'
$env:GIT_CONFIG_KEY_0 = 'http.sslBackend'
$env:GIT_CONFIG_VALUE_0 = 'schannel'
```

## 実装を追加するとき

- `src/main.cpp`: キー入力、再生タイミング、描画を担当します。
- `src/SortAlgorithm.hpp`: `reset`、`step` と表示状態の共通インターフェースです。
- `src/BubbleSort.hpp` / `.cpp`: バブルソート（時間 O(n²)、整列済み入力では O(n)）。
- `src/MergeSort.hpp` / `.cpp`: マージソート（時間 O(n log n)、補助領域 O(n)）。
- 追加した6種類も `src/InsertionSort`、`SelectionSort`、`QuickSort`、`HeapSort`、`CountingSort`、`RadixSort` の `.hpp` / `.cpp` に分けています。
- ソートのクラスは SFML に依存しません。ソート用 `.cpp` を増やす際は CMake の `sorting` に追加してください。
- 初期配列と再生間隔は `src/main.cpp` の `initialValues` と `stepSeconds` で設定します。
  現在の描画配置は10個の正の値（1〜10）向けです。ソートのクラス自体は `int` 配列を扱いますが、カウントソートには上記の範囲制限があります。
- ソート名と説明は各クラスの `displayName()` / `description()` に定義しています。

## 日本語フォント

起動時に、作業ディレクトリの `assets/font.ttf`、`C:/Windows/Fonts/meiryo.ttc`、
`C:/Windows/Fonts/YuGothM.ttc` の順にフォントを探します。
このPCではインストール済みのメイリオを使うため、追加ダウンロードは不要です。
Windows のフォントファイルをプロジェクトへコピー・同梱することはありません。

別のフォントや Windows 以外の環境では、日本語を含むフォントのパスを引数で指定できます。
明示したフォントを読み込めない場合、または既定のフォントが見つからない場合は、
コンソールにエラーを表示して終了します。

```powershell
.\build\bin\Release\algorithm_visualization.exe "C:/Windows/Fonts/meiryo.ttc"
```

## テスト

```powershell
cmake --build build --config Release --parallel
ctest --test-dir build -C Release --output-on-failure
```

空配列、1要素、整列済み、逆順、重複、負数、整数の最大・最小値、乱数配列を
`std::stable_sort` の結果と比較します。2〜6要素の全順列も確認します。
途中でのリセット、1ステップずつの更新、カウントソートの範囲制限と境界値も検証します。
CLion の既定のビルド先を使う場合は `build` を `cmake-build-debug` に読み替えてください。

Linux / macOS でも SFML を利用できますが、上記は Windows 向けの手順です。
他の環境のコンパイラ・システム依存パッケージについては
[公式導入手順](https://www.sfml-dev.org/tutorials/3.1/getting-started/cmake/) を参照してください。
