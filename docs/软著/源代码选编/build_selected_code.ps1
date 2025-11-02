Param()

$ErrorActionPreference = 'Stop'

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$root = Resolve-Path (Join-Path $scriptDir '..\..\..')
$outFile = Join-Path $scriptDir 'selected_code.txt'

$files = @(
    'board\h30.h',
    'board\h30.c',
    'board\my_move.h',
    'board\my_move.c',
    'board\dc_motor_control.h',
    'board\dc_motor_control.c',
    'board\motor_control.h',
    'board\motor_control.c',
    'board\servo_control.h',
    'board\servo_control.c',
    'board\servo2_control.h',
    'board\servo2_control.c',
    'board\hcsr04.h',
    'board\hcsr04.c',
    'board\board_delay.h',
    'board\board_delay.c',
    'src\main.c'
)

"软件名称：嵌入式移动小车姿态矫正控制软件 V1.0`r`n生成时间：$(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')`r`n注意：本合并稿仅包含原创核心功能模块，第三方/SDK 代码不在此列。`r`n`r`n" | Set-Content -Path $outFile -Encoding utf8

foreach ($rel in $files) {
    $path = Join-Path $root $rel
    if (-not (Test-Path $path)) {
        "===== MISSING FILE: $rel =====`r`n" | Add-Content -Path $outFile -Encoding utf8
        continue
    }
    "===== BEGIN FILE: $rel =====`r`n" | Add-Content -Path $outFile -Encoding utf8
    Get-Content -LiteralPath $path -Raw | Add-Content -Path $outFile -Encoding utf8
    "`r`n===== END FILE: $rel =====`r`n`r`n" | Add-Content -Path $outFile -Encoding utf8
}

Write-Host "selected_code.txt 已生成：$outFile"

