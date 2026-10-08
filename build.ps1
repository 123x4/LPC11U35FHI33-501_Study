param(
    [string]$KeilPath = 'D:\Keil5\Core\UV4\UV4.exe',
    [switch]$Incremental
)
$ErrorActionPreference = 'Stop'
if (-not (Test-Path -LiteralPath $KeilPath -PathType Leaf)) {
    throw "Keil UV4.exe not found: $KeilPath. Supply -KeilPath with your installation path."
}
$project = Join-Path $PSScriptRoot 'Sample.uvproj'
$output = Join-Path $PSScriptRoot 'output'
New-Item -ItemType Directory -Path $output -Force | Out-Null
$log = Join-Path $output 'build.log'
if (Test-Path -LiteralPath $log) { Remove-Item -LiteralPath $log }
$mode = '-r'
if ($Incremental) { $mode = '-b' }
$arguments = "$mode `"$project`" -t `"Target 1`" -o `"$log`""
$process = Start-Process -FilePath $KeilPath -ArgumentList $arguments -WorkingDirectory $PSScriptRoot -WindowStyle Hidden -Wait -PassThru
if (-not (Test-Path -LiteralPath $log)) { throw "Keil produced no build log (exit code $($process.ExitCode))." }
$content = Get-Content -LiteralPath $log -Raw
Write-Output $content
if ($process.ExitCode -notin @(0, 1) -or $content -notmatch '0 Error\(s\)') {
    throw "Build failed (Keil exit code $($process.ExitCode)). See $log"
}
Write-Output "Firmware: $(Join-Path $output 'Sample.hex')"
Write-Output "Debug ELF: $(Join-Path $output 'Sample.axf')"
