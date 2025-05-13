param (
    [string]$filepath
)

Set-Location $filepath
$tag = git tag --sort="-v:refname" | Select -First 1

#Write-Output "$tag"
return $tag