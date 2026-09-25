# Third-party notices

この配布物は以下の第三者ライブラリ・データを利用しています。
本書と `licenses/` は、実行ファイルと一緒に保持してください。
これらのライセンスはそれぞれの第三者コンポーネントに適用され、本プロジェクト独自コードの利用許諾を定めるものではありません。

| コンポーネント | バージョン / 出典 | ライセンス・表記 |
| --- | --- | --- |
| SFML | 3.1.0 / https://github.com/SFML/SFML | `licenses/SFML.txt` (zlib) |
| FreeType | 2.14.3 / https://freetype.org | `licenses/FreeType-FTL.txt` (本配布ではFTLを選択)、`FreeType-LICENSES.txt` |
| FreeType内のBDF / PCF / hash / gzipコード | 上記FreeTypeに含まれるもの | `licenses/FreeType-BDF.txt`、`FreeType-PCF.txt`、`FreeType-fthash.txt`、`FreeType-zlib.txt` |
| dlg | FreeTypeのサブモジュール | `licenses/dlg.txt` (Boost Software License 1.0) |
| HarfBuzz | 14.1.0 / https://github.com/harfbuzz/harfbuzz | `licenses/HarfBuzz.txt` (Old MIT)、`HarfBuzz-ms-use.txt` (MIT) |
| SheenBidi | 3.0.0 / https://github.com/Tehreer/SheenBidi | Copyright (C) 2014-2026 Muhammad Tayyab Akram; `licenses/Apache-2.0.txt` |
| stb_image / stb_image_write | SFML同梱ヘッダー | `licenses/stb_image.h.txt`、`stb_image_write.h.txt` (本配布ではMITを選択) |
| QOI | SFML同梱ヘッダー / https://github.com/phoboslab/qoi | `licenses/QOI.txt` (MIT) |
| cpp-unicodelib | SFML同梱ヘッダー / https://github.com/yhirose/cpp-unicodelib | `licenses/cpp-unicodelib.txt` (MIT) |
| glad / Khronos headers | glad 2.0.8生成ヘッダー / https://github.com/Dav1dde/glad | `licenses/GLAD-notices.txt`、`Khronos-platform.txt`、`Apache-2.0.txt` |
| Vulkan headers | SFML同梱ヘッダー | `licenses/Vulkan-Headers-notice.txt`、`Apache-2.0.txt` |
| Unicode data | 文字処理ライブラリで使用するデータ / https://www.unicode.org/license.txt | `licenses/Unicode.txt` |

This software is based in part on the work of the FreeType Team.
Portions of this software are copyright © 1996-2026 The FreeType Project (https://freetype.org). All rights reserved.

SFMLの上流CMake処理はFreeType、HarfBuzz、SheenBidiのビルド設定にパッチを適用します。
これらはSFML 3.1.0の `tools/freetype`、`tools/harfbuzz`、`tools/sheenbidi` に定義された変更です。
このプロジェクトで依存ライブラリのアルゴリズムを独自に変更したものではありません。

フォントファイルとMicrosoft Visual C++ランタイムはこのZIPには含まれません。
PCに導入済みのフォントを参照し、MSVCランタイムはMicrosoft公式の配布物を利用してください。

一部の通知ファイルは元ファイルのコメントやREADMEをそのまま保存しているため、実装や説明文を含みます。
