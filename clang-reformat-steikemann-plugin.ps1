$searchPath = "gameplay-plugin"

$files = Get-ChildItem -Path $searchPath -Recurse -Include *.h, *.cpp
foreach ($file in $files){
    clang-format -i $file.FullName
}