# argument 1 is the shaders directory (eg "shaders/"), argument 2 is the output path (eg "bin/ShaderSource.hpp")

# create the header of the header
header=$(cat <<-HEADER
#pragma once

namespace ShaderSource
{
HEADER
)

# for each shader file in the given path
shaders_path=$(realpath $1)
for path in $(ls $shaders_path/*.vert $shaders_path/*.frag); do
    # get the contents of the current shader file
    contents="$(cat $path)"

    # get the name for the constant in the header
    name=$(basename "$path" | sed -E 's/./\U&/g')
    name=$(echo "$name" | sed -E 's/.VERT/_VERTEX/g')
    name=$(echo "$name" | sed -E 's/.FRAG/_FRAGMENT/g')

    # append the shader to the header
    header=$(cat <<-SOURCE
$header
    static const std::string $name = R"SOURCE(
$contents
)SOURCE";

SOURCE
)
done

# append the end to the header
header="$header};"

# write the header file
header_path=$(realpath $2)
echo "$header" > $header_path