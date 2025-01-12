param (
	[string]$path = "",
	[string]$profile = ""
)

$exePath = "$path\build\$profile\project\quail\Quail.exe"
$filePath = "$path\project\installer\res\data.h"


# Ensure we're using '.' sing instead of ',' sign for fpn.
$currentThread = [System.Threading.Thread]::CurrentThread
$culture = [System.Globalization.CultureInfo]::InvariantCulture
$currentThread.CurrentCulture = $culture
$currentThread.CurrentUICulture = $culture


# Get the file information data.
$fileInfo = Get-Item $exePath


# Convert to KiloBytes format.
$fileSizeInKiloBytes = [double]($fileInfo.Length / 1024)
$formattedString = "{0:F2}" -f $fileSizeInKiloBytes


# Write to file.
New-Item -Path $filePath -Force -ItemType "file" -Value "#pragma once
#define DISK_SPACE_KB $formattedString
"
