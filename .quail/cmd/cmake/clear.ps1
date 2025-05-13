param (
	[string]$project_build_dir = "",
	[string]$cmake_cache_dir = ""
)

Remove-Item -Path $project_build_dir -Recurse -Force -ErrorAction SilentlyContinue
if (-not $?) { Write-Warning "Could not remove build directory." }

Remove-Item -Path $cmake_cache_dir -Recurse -Force -ErrorAction SilentlyContinue
if (-not $?) { Write-Warning "Could not remove cache directory." }
	
