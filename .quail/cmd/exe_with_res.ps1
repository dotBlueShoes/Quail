param (
	[string]$project_dir = "",
	[string]$project_name = ""
)

# Because sources are not connected to the executable
#  but placed as a linkfile inside build directory...
#  When running from console we simply need to change the path to build's path.

Push-Location $project_dir
& ./$project_name.exe
Pop-Location
