param (
	[string]$upx_path = "",
	[string]$in = "",
	[string]$out = ""
)

if (Test-Path $out) {
	Write-Host "- Recreating compressed file.`r`n"
	Remove-Item -Path $out
} else {
	Write-Host "- Creating new compressed file.`r`n"
}

.$upx_path -9 $in -o $out

Write-Host ""
