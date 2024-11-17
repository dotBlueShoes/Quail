param (
	[string]$project_dir = "",
	[string]$quail_dir = ""
)

$ori = "'$project_dir\build\x64-win-debug\project\Quail.exe'";
$dst = "'$quail_dir\Quail.exe'";

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

#Start-Process $PSHOME\powershell -ArgumentList "-NoExit -command (Invoke-Command -ScriptBlock {$block} -ArgumentList $ori, $dst)" # Keep console open.
Start-Process $PSHOME\powershell -ArgumentList "-command (Invoke-Command -ScriptBlock {$block} -ArgumentList $ori, $dst)"
