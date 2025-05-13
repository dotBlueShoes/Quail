param (
	[string]$path = ""
)

$out = "$path\project\global\inc\global\license.h"
$temp = "$path\temp"
$in = "$path\LICENSE"


$header = "#pragma once
#define LICENSE \
L`"\n`"\"

$comma = '"'
$newline = "\n`"\"
$footer = "`""

(Get-Content $in) | ForEach-Object { $_ -replace "`"", "`"`"" } | Add-Content $temp

$header | Set-Content $out
(Get-Content $temp) | ForEach-Object { $comma + $_ + $newline } | Add-Content $out
#$footer | Add-Content $out

Remove-Item $temp
