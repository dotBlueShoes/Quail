param (
	[string]$mt_path = "",
	[string]$path = "",
	[string]$profile = "",
	[string]$project = "",
	[string]$file = ""
)

#Write-Host "$mt_path"
#Write-Host "$path"
#Write-Host "$profile"

.$mt_path -nologo -manifest "$path\project\$project\res\$file.manifest" -outputresource:"$path\build\$profile\project\$project\$file;#1"