# リリース作成手順

リポジトリのルートで実行します。配布対象はVisual StudioでビルドしたWindows x64 Release版です。

## ローカルの配布物を作る

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DBUILD_SHARED_LIBS=OFF
cmake --build build --config Release --parallel
powershell -File scripts/package-release.ps1
```

PowerShell 7の場合は最後のコマンドを `pwsh -File scripts/package-release.ps1` にできます。
パッケージスクリプトはCTestを実行し、失敗した場合や実行ファイルよりソースが新しい場合は停止します。
バージョンは `CMakeLists.txt` のプロジェクトバージョンから読み取ります。
既存のZIPやチェックサムは上書きしません。再作成時は既存の配布物を別の場所へ保管してから実行してください。

出力先は `dist/` です。

- `algorithm-visualizer-v0.1.0-windows-x64.zip`: 実行ファイル、説明書、第三者ライセンス、ビルド情報。
- `algorithm-visualizer-v0.1.0-source.zip`: プロジェクトのソースとドキュメント。依存ライブラリ本体はCMakeで取得します。
- `SHA256SUMS.txt`: 両ZIPのSHA-256。

## GitHub Releaseとして公開する

公開先リポジトリにこのバージョンのコードをコミット・pushした後、次を行います。

1. そのコミットに `v0.1.0` タグを付けます。
2. GitHubの「Releases」から新しいリリースを作成し、タグを選びます。
3. タイトルを `Algorithm Visualizer v0.1.0` にします。
4. 本文には `RELEASE_NOTES.md` の内容を使います。
5. `dist/` 内の2つのZIPと `SHA256SUMS.txt` を添付して公開します。

公開先: https://github.com/nasuton/algorithm-visualizer
独自コードにオープンソースライセンスを付ける場合は、所有者が利用条件を決めて `LICENSE` を追加し、配布スクリプトにも含めてください。

公式手順: https://docs.github.com/en/repositories/releasing-projects-on-github/managing-releases-in-a-repository
