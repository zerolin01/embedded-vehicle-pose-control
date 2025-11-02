Param()

$ErrorActionPreference = 'Stop'

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$root = Resolve-Path (Join-Path $scriptDir '..\..\..')
$dest = Join-Path $scriptDir 'code'

if (Test-Path $dest) { Remove-Item -Recurse -Force $dest }
New-Item -ItemType Directory -Force -Path $dest | Out-Null

$listFile = Join-Path $scriptDir '文件清单.txt'
$files = Get-Content -Path $listFile | Where-Object { $_ -and (-not $_.StartsWith('#')) }

foreach ($rel in $files) {
    $src = Join-Path $root $rel
    if (-not (Test-Path $src)) {
        Write-Warning "缺失文件: $rel"
        continue
    }
    $target = Join-Path $dest $rel
    $targetDir = Split-Path -Parent $target
    New-Item -ItemType Directory -Force -Path $targetDir | Out-Null
    Copy-Item -LiteralPath $src -Destination $target -Force
}

Write-Host "精简提交代码已导出至: $dest"

