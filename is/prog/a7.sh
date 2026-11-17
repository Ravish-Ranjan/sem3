#!/bin/bash

read -p "Enter file path : " file_path

if [ ! -f "$file_path" ]; then
    echo "Error : file doesn't exists"
    exit 1
fi

read -p "Select hashing algorithm (MD5, SHA256, SHA1, SHA512) : " algo

algo=$(echo "$algo" | tr '[:upper:]' '[:lower:]')

case "$algo" in
    md5) hash_cmd="md5sum" ;;
    sha256) hash_cmd="sha256sum" ;;
    sha1) hash_cmd="sha1sum" ;;
    sha512) hash_cmd="sha512sum" ;;
    *) 
        echo "Error : unsupported algorithm, choose from (MD5, SHA256, SHA1, SHA512)"
        exit 1 ;;
esac

echo "1. Generate baseline hash (Initial state)"
echo "2. Verify file integrity against an existing hash"
read -p "Choose an option (1 or 2): " choice

current_hash=$($hash_cmd "$file_path" | awk '{print $1}')

if [ "$choice" == "1" ]; then
    echo ""
    echo "[${algo^^}] Generated Hash: $current_hash"
    echo "$current_hash" > "./${file_path}_hash"
elif [ "$choice" == "2" ];then
    read -p "Enter the original/expected hash file_path : " original_hash_file_path
        if [ ! -f "$original_hash_file_path" ]; then
        echo "Error: Hash file doesn't exist"
        exit 1
    fi

    read -r original_hash < "$original_hash_file_path"
    echo "Original Hash : $original_hash"
    echo "Current Hash  : $current_hash"

    if [ "$current_hash" == "$original_hash" ]; then
        echo "Status        : Integrity Verified"
    else
        echo "Status        : Integrity Compromised"
    fi
else
    echo "Invalid option selected."
    exit 1
fi