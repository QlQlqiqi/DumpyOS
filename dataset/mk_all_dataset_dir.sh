#!/bin/bash

create_dir() {
    local target_dir="$1"  
    mkdir -p "${target_dir}"
    mkdir -p "${target_dir}/data"
    mkdir -p "${target_dir}/in-memory"
    mkdir -p "${target_dir}/paa"
    mkdir -p "${target_dir}/query"
    mkdir -p "${target_dir}/res"
    mkdir -p "${target_dir}/sax"
}

main() {
    create_dir "chlorine"
    create_dir "gas"
    create_dir "stock"
    create_dir "synthetic"
}

main