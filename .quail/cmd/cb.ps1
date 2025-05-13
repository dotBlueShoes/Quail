param (
	[string]$project_dir = "",
	[string]$quail_dir = "",
	[string]$profile = ""
)

$ori = "'$project_dir\build\$profile\project\quail\Quail.exe'";
$dst = "'$quail_dir\Quail.exe'";

$fm_uni = "$project_dir\build\$profile\project\myuninstall\uninstall.exe";
$to_uni = "$quail_dir\uninstall.exe";
Copy-Item -Path "$fm_uni" -Destination "$to_uni";

$block = { 
	param([string]$ori, [string]$dst); 
	
	Write-Host "$ori, $dst";
	
	do {
		try {
			# Try opening the file in exclusive mode (read-write)
			$fileStream = [System.IO.File]::Open(
				$dst, 
				[System.IO.FileMode]::Open, 
				[System.IO.FileAccess]::ReadWrite, 
				[System.IO.FileShare]::None
			);
			
			# If successful, file is not in use, so we can break the loop
			$fileStream.Close();
			#Write-Host "File is not in use and can be accessed.";
			Copy-Item -Path "$ori" -Destination "$dst";
			break;
		}
		catch {
			# If an exception is thrown, the file is in use, so wait for a while and retry
			Write-Host "File is in use. Waiting...";
			Start-Sleep -Seconds 2;
		}
	} while ($true);
}

# Keep console open varaint
# Start-Process powershell -ArgumentList "-NoExit -command (Invoke-Command -ScriptBlock {$block} -ArgumentList $ori, $dst)" 
Start-Process powershell -ArgumentList "-command (Invoke-Command -ScriptBlock {$block} -ArgumentList $ori, $dst)"
