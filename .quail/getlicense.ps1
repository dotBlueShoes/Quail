param (
	[string]$path = ""
)

$out = "$path\project\installer\res\license.h"
$temp = "$path\temp"
$in = "$path\LICENSE"


$header = "#pragma once
#define LICENSE \"

$comma = '"'
$newline = "\n`"\"
$footer = "`""

(Get-Content $in) | ForEach-Object { $_ -replace "`"", "`"`"" } | Add-Content $temp

$header | Set-Content $out
(Get-Content $temp) | ForEach-Object { $comma + $_ + $newline } | Add-Content $out
#$footer | Add-Content $out

Remove-Item $temp


# $newline = "\n`"`"\" - additional string breaking 
#(Get-Content $in) | ForEach-Object { $_ + $newline } | Add-Content $out
# (Get-Content $in) | ForEach-Object { $_ + $newline -replace "`"", "\`""} | Add-Content $out - Replace variant
# Write to file.
#New-Item -Path $filePath -Force -ItemType "file" -Value "#pragma once
##define DISK_SPACE_KB $formattedString
#"
