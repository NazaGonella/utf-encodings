mkdir -p ./build
rm -rf ./build/*

pushd ./build > /dev/null

    gcc -g -Wall -Wextra -pedantic ../utf_encoding.c -o utf_encoding
    
popd > /dev/null