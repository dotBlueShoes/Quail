param (
	[string]$mt_path = "",
	[string]$path = "",
	[string]$profile = "",
	[string]$file = ""
)

#Write-Host "$mt_path"
#Write-Host "$path"
#Write-Host "$profile"

.$mt_path -nologo -manifest "$path\project\installer\res\Installer.exe.manifest" -outputresource:"$path\build\$profile\project\installer\$file;#1"