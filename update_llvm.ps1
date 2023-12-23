# ! Check if needed is missing !

$repository_release = 'https://api.github.com/repos/llvm/llvm-project/releases/latest'
$look_for = 'win64.exe'
$name_it = 'LLVM.exe'

# Get Download URL

$json_key_length = 31 
$whole_string = curl -s $repository_release | findstr "browser_download_url" | findstr $look_for
$download_url = $whole_string.Substring($json_key_length, $whole_string.Length - $json_key_length - 1)

# Display it

Write-Host $download_url

# Download it

curl -L $download_url > $name_it





### NOTES
### json_key_length = 31  // "browser_download_url" thing // + 1 - 1 to remove quotes neceserry for curl to work for some reson. 
### curl -L "https://github.com/llvm/llvm-project/releases/download/llvmorg-17.0.6/LLVM-17.0.6-win64.exe" > $name_it # works weirdly but not when a variable.