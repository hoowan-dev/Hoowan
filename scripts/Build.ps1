# Builds the Hoowan solution with MSBuild.
# Usage: scripts/Build.ps1 [-Configuration Debug|Release|Dist] [-Project Sandbox|Hoowan-Editor|...] [-Rebuild]
param(
    [ValidateSet("Debug", "Release", "Dist")]
    [string]$Configuration = "Debug",
    [string]$Project = "",
    [switch]$Rebuild
)

$ErrorActionPreference = "Stop"
$root = Split-Path -Parent $PSScriptRoot
$sln = Join-Path $root "Hoowan.sln"

if (-not (Test-Path $sln)) {
    Write-Host "Hoowan.sln not found; running premake first..."
    & (Join-Path $root "vendor\bin\premake\premake5.exe") --file="$root\premake5.lua" vs2019
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
}

# Projects are generated for the v142 (VS2019) toolset, so prefer a VS2019 install,
# then fall back to the newest Visual Studio that has MSBuild.
$vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
$msbuild = & $vswhere -version "[16.0,17.0)" -products * -requires Microsoft.Component.MSBuild -find "MSBuild\**\Bin\MSBuild.exe" | Select-Object -First 1
if (-not $msbuild) {
    $msbuild = & $vswhere -latest -products * -requires Microsoft.Component.MSBuild -find "MSBuild\**\Bin\MSBuild.exe" | Select-Object -First 1
}
if (-not $msbuild) { throw "MSBuild not found. Install Visual Studio with the C++ workload." }

$target = if ($Rebuild) { "Rebuild" } else { "Build" }
if ($Project) {
    # Solution metaproj targets: bare project name builds; "Name:Rebuild" rebuilds. '.' maps to '_'.
    $name = $Project.Replace('.', '_')
    $target = if ($Rebuild) { "$($name):Rebuild" } else { $name }
}

& $msbuild $sln -m -nologo -v:minimal "-t:$target" "-p:Configuration=$Configuration" "-p:Platform=x64"
exit $LASTEXITCODE
