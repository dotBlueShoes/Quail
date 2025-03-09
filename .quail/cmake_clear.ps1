param (
	[string]$project_build_dir = ""
)

Remove-Item -Path $project_build_dir -Recurse -Force
