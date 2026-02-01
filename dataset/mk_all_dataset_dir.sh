#!/bin/bash

# 定义源根路径（即你实际存放数据的地方）
SOURCE_BASE="/home/qiqi/similar-search/test/dataset/dumpy_dataset"

create_dir() {
    local dataset="$1"
    
    echo "正在为 [${dataset}] 初始化目录结构..."

    # 1. 创建普通目录
    mkdir -p "${dataset}"
    mkdir -p "${dataset}/in-memory"
    mkdir -p "${dataset}/paa"
    mkdir -p "${dataset}/sax"
    mkdir -p "${dataset}/index"
    mkdir -p "${dataset}/fuzzy"

    # 2. 创建软连接目录 (data, query, res)
    # 逻辑：将远程的目录链接到当前执行路径下的子目录中
    local special_dirs=("data" "query" "res")
    
    for dir in "${special_dirs[@]}"; do
        local source_path="${SOURCE_BASE}/${dataset}"
        local target_path="${dataset}/${dir}"

        # 检查源路径是否存在，不存在则提醒（可选）
        if [ ! -d "${source_path}" ]; then
            echo "警告: 源路径 ${source_path} 不存在，软连接可能失效。"
        fi

        # 如果目标已存在（如旧的软连接），先删除再创建
        rm -f "${target_path}"
        ln -s "${source_path}" "${target_path}"
        echo "已建立软连接: ${target_path} -> ${source_path}"
    done
}

main() {
    if [ $# -eq 0 ]; then
        echo "使用方法: $0 dataset1 dataset2 ..."
        exit 1
    fi

    for arg in "$@"; do
        create_dir "$arg"
    done
    echo "全部任务完成！"
}

main "$@"