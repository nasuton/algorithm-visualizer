param(
    [string]$BuildDirectory = 'build'
)

$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path -Parent $PSScriptRoot
$buildRoot = [IO.Path]::GetFullPath((Join-Path $projectRoot $BuildDirectory))
$cmakeText = Get-Content -LiteralPath (Join-Path $projectRoot 'CMakeLists.txt') -Raw
if ($cmakeText -notmatch 'project\(AlgorithmVisualization VERSION ([0-9]+\.[0-9]+\.[0-9]+)') {
    throw 'Could not read the project version from CMakeLists.txt.'
}
$releaseVersion = $Matches[1]
$cache = Get-Content -LiteralPath (Join-Path $buildRoot 'CMakeCache.txt') -Raw
if ($cache -notmatch 'CMAKE_GENERATOR:INTERNAL=Visual Studio' -or
    $cache -notmatch 'CMAKE_GENERATOR_PLATFORM:INTERNAL=x64' -or
    $cache -notmatch 'BUILD_SHARED_LIBS:BOOL=OFF') {
    throw 'Packaging requires a Visual Studio x64 build with BUILD_SHARED_LIBS=OFF.'
}
$exe = Join-Path $buildRoot 'bin/Release/algorithm_visualization.exe'
$exeInfo = Get-Item -LiteralPath $exe
$inputs = @(Get-ChildItem -LiteralPath (Join-Path $projectRoot 'src') -File -Recurse)
$inputs += Get-Item -LiteralPath (Join-Path $projectRoot 'CMakeLists.txt')
if ($inputs | Where-Object { $_.LastWriteTimeUtc -gt $exeInfo.LastWriteTimeUtc }) {
    throw 'Source files are newer than the executable. Build Release before packaging.'
}

& ctest --test-dir $buildRoot -C Release --output-on-failure
if ($LASTEXITCODE -ne 0) { throw 'CTest failed; no release was packaged.' }

$outputRoot = Join-Path $projectRoot 'dist'
$binaryName = "algorithm-visualizer-v$releaseVersion-windows-x64"
$sourceName = "algorithm-visualizer-v$releaseVersion-source"
$binaryZip = Join-Path $outputRoot ($binaryName + '.zip')
$sourceZip = Join-Path $outputRoot ($sourceName + '.zip')
$hashPath = Join-Path $outputRoot 'SHA256SUMS.txt'
foreach ($outputPath in @($binaryZip, $sourceZip, $hashPath)) {
    if (Test-Path -LiteralPath $outputPath) { throw "Refusing to overwrite existing release: $outputPath" }
}
# Unique staging folders avoid deleting files or mixing old and new packages.
$staging = Join-Path $buildRoot ('release-staging/' + [guid]::NewGuid().ToString('N'))
$binaryStage = Join-Path $staging $binaryName
$sourceStage = Join-Path $staging $sourceName
New-Item -ItemType Directory -Force -Path $outputRoot, $binaryStage, $sourceStage | Out-Null

Copy-Item -LiteralPath $exe -Destination $binaryStage
foreach ($file in @('README.md', 'RELEASE_NOTES.md', 'THIRD_PARTY_NOTICES.md')) {
    Copy-Item -LiteralPath (Join-Path $projectRoot $file) -Destination $binaryStage
}
foreach ($directory in @('docs', 'licenses')) {
    Copy-Item -LiteralPath (Join-Path $projectRoot $directory) -Destination $binaryStage -Recurse
}
Copy-Item -LiteralPath (Join-Path $projectRoot 'docs/QUICK_START.txt') -Destination (Join-Path $binaryStage 'README.txt')

$buildInfo = @(
    "Algorithm Visualizer v$releaseVersion"
    'Platform: Windows x64 / MSVC / Release'
    'SFML linkage: static; MSVC runtime: dynamic'
    ('Executable SHA256: ' + (Get-FileHash -LiteralPath $exe -Algorithm SHA256).Hash.ToLowerInvariant())
    ('Packaged (UTC): ' + [DateTime]::UtcNow.ToString('o'))
    ''
    'Dependency source commits:'
)
foreach ($dependency in @('sfml', 'freetype', 'harfbuzz', 'sheenbidi')) {
    $dependencyRoot = Join-Path $buildRoot ("_deps/$dependency-src")
    $commit = & git -C $dependencyRoot rev-parse HEAD
    if ($LASTEXITCODE -ne 0) { throw "Could not identify $dependency source revision." }
    $buildInfo += "$dependency $commit"
}
$buildInfo | Set-Content -LiteralPath (Join-Path $binaryStage 'BUILD_INFO.txt') -Encoding utf8

foreach ($item in @('src', 'tests', 'docs', 'licenses', 'scripts', 'CMakeLists.txt',
                    '.gitignore', 'README.md', 'RELEASE_NOTES.md', 'THIRD_PARTY_NOTICES.md')) {
    Copy-Item -LiteralPath (Join-Path $projectRoot $item) -Destination $sourceStage -Recurse
}
# .NET's ZIP API includes .gitignore as well as ordinary files.
Add-Type -AssemblyName System.IO.Compression.FileSystem
[IO.Compression.ZipFile]::CreateFromDirectory($binaryStage, $binaryZip)
[IO.Compression.ZipFile]::CreateFromDirectory($sourceStage, $sourceZip)
$hashLines = foreach ($archive in @($binaryZip, $sourceZip)) {
    $hash = (Get-FileHash -LiteralPath $archive -Algorithm SHA256).Hash.ToLowerInvariant()
    "$hash  $([IO.Path]::GetFileName($archive))"
}
$hashLines | Set-Content -LiteralPath $hashPath -Encoding ascii
Write-Output "Created $binaryZip"
Write-Output "Created $sourceZip"
Write-Output "Created $hashPath"
