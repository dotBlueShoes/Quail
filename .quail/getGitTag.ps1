$tag = git tag --sort="-v:refname" | Select -First 1

#Write-Output "$tag"
return $tag