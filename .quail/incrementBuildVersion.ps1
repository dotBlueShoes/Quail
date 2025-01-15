param (
    [string]$filepath
)

$buildBumber = Get-Content -Path "$filepath/buildNumber.txt"
$buildBumber = [int]$buildBumber + 1
$buildBumber | Out-File "$filepath/buildNumber.txt"

#TODO: SKIP GITTAG IF FLAG OR NO SOMETHINK FAILS

$gitTag = & "$filepath/.quail/getGitTag.ps1" "$filepath"

$header = "#pragma once
#define QUAIL_FULL_VERSION_W L`"$gitTag.$buildBumber`"
#define QUAIL_FULL_VERSION `"$gitTag.$buildBumber`"
#define QUAIL_BUILD_VERSION $buildBumber
"

$header | Out-File "$filepath/project/global/inc/global/version.h"

Write-Host "- incremented build version."
#return "$buildBumber"
