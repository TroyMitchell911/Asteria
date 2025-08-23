#!/usr/bin/env python3

import re
import numpy as np
from PIL import Image
import argparse
import glob
import os

def c_array_to_rgb565a_png(c_file, out_png, width, height):
    with open(c_file, "r", encoding="utf-8") as f:
        txt = f.read()
    # 匹配 uint8_t 数组里的十六进制字节
    bytes_hex = re.findall(r"0x([0-9a-fA-F]{2})", txt)
    data = np.fromiter((int(x,16) for x in bytes_hex), dtype=np.uint8)

    expect = width * height * 3
    if len(data) < expect:
        raise ValueError(f"{c_file} 数据长度不足：{len(data)} vs {expect}")
    # 每像素 3 字节：[lo, hi, alpha]
    data = data[:expect].reshape((height, width, 3))

    lo = data[:,:,0].astype(np.uint16)
    hi = data[:,:,1].astype(np.uint16)
    alpha = data[:,:,2]

    rgb565 = (hi << 8) | lo
    r = ((rgb565 >> 11) & 0x1F) << 3
    g = ((rgb565 >> 5)  & 0x3F) << 2
    b = (rgb565 & 0x1F) << 3

    img = np.dstack((r,g,b,alpha)).astype(np.uint8)
    Image.fromarray(img, "RGBA").save(out_png)
    print(f"{c_file} -> {out_png}")

def main():
    parser = argparse.ArgumentParser(description="Convert LVGL C array (RGB565+Alpha) to PNG")
    parser.add_argument("files", nargs="+", help="C file(s) or wildcard, e.g. 1.c *.c")
    parser.add_argument("--width", type=int, required=True, help="Image width")
    parser.add_argument("--height", type=int, required=True, help="Image height")
    args = parser.parse_args()

    # 支持通配符展开
    all_files = []
    for pattern in args.files:
        all_files.extend(glob.glob(pattern))

    if not all_files:
        print("没有找到匹配的文件")
        return

    for cfile in all_files:
        out_png = os.path.splitext(cfile)[0] + ".png"
        try:
            c_array_to_rgb565a_png(cfile, out_png, args.width, args.height)
        except Exception as e:
            print(f"{cfile} 转换失败: {e}")

if __name__ == "__main__":
    main()

