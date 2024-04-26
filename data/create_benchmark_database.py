#!/usr/bin/env python 

import sys
import re

keys = [
        'name',
        'date',
        'home_build',
        'picture',
        'description',
        'cpu_name',
        'cpu_speed',
        'ram_size',
        'ram_type',
        'chipset',
        'disk',
        'video',
        'case_name',
        'whetstone',
        'dhrystone',
        'dhrystonem',
        'block_write',
        'block_read',
        'ram_speed',
        'pcmark07',
        'pcmark10',
        'dmark06',
        'geek_cpu_single',
        'geek_cpu_multi',
        'geek_gpu',
        ]


def remove_string(rem, str):
    return re.sub(rem,'',str,flags=re.IGNORECASE).lstrip().rstrip()

def expect_line(rem, str, default = ""):
    for line in str:
        if line.lower().startswith(rem.lower()):
            return remove_string(rem, line)
    return default

def expect_boolean(rem, str, default = ""):
    for line in str:
        if line.lower().startswith(rem.lower()):
            return True
    return False

def make_number(rem, str):
    numstr = remove_string(",",expect_line(rem,str))
    numarr = numstr.split(" ")
    if len(numarr) == 0:
        num = 0
    else:
        if numarr[0] == "" :
            num = 0
        else:
            num = int(numarr[0])
            if len(numarr) > 1:
                if numarr[1] == "MB":
                    num = num * 1024
                elif numarr[1] == "GB":
                    num = num * 1024 * 1024
    return num

def parse_benchmark(filename):
    file = open(filename,mode='r')
    benchmark_text = file.read(80*100).split('\n');
    file.close()

    benchmark = {}

    benchmark['name'] = expect_line("Name",benchmark_text)
    benchmark['date'] = expect_line("Date",benchmark_text)
    if expect_boolean("Home build",benchmark_text):
        benchmark['home_build'] = 1
    else:
        benchmark['home_build'] = 0
    benchmark['picture'] = expect_line("picture",benchmark_text)
    benchmark['description'] = expect_line("description",benchmark_text)
    benchmark['cpu_name'] = expect_line("model name",benchmark_text)
    benchmark['cpu_speed'] = make_number("cpu mhz",benchmark_text)
    benchmark['ram_size'] = make_number("RAM",benchmark_text)
    benchmark['ram_type'] = expect_line("RAM Type",benchmark_text)
    benchmark['chipset'] = expect_line("Chipset",benchmark_text)
    benchmark['disk'] = expect_line("Disk",benchmark_text)
    benchmark['video'] = expect_line("video",benchmark_text)
    benchmark['case_name'] = expect_line("case",benchmark_text)
    benchmark['whetstone'] = make_number("Whetstone",benchmark_text)
    benchmark['dhrystone'] = make_number("dhrystones",benchmark_text)
    benchmark['dhrystonem'] = make_number("dhrystones4c",benchmark_text)
    benchmark['block_write'] = make_number("Block Writes MB/s",benchmark_text)
    benchmark['block_read'] = make_number("Block Reads MB/s",benchmark_text)
    benchmark['ram_speed'] = make_number("Ram MB/s",benchmark_text)
    benchmark['pcmark07'] = make_number("pcmark07",benchmark_text)
    benchmark['pcmark10'] = make_number("pcmark10",benchmark_text)
    benchmark['dmark06'] = make_number("3dmark06",benchmark_text)
    benchmark['geek_cpu_single'] = make_number("GeekBench cpu single",benchmark_text)
    benchmark['geek_cpu_multi'] = make_number("GeekBench cpu multi",benchmark_text)
    benchmark['geek_gpu'] = make_number("GeekBench gpu",benchmark_text)
    return benchmark


def print_sql_benchmark(benchmark,filename):
    with open(filename, "a") as f:
        print("INSERT INTO computers (",end="",file=f)
        for key in keys:
            if key == keys[-1]:
                comma = ""
            else:
                comma = ","
            print(f"{key}{comma}",end="",file=f)
        print(") ",file=f)
        print("VALUES (",end="",file=f)
        for key in keys:
            if key == keys[-1]:
                comma = ""
            else:
                comma = ","
            val = benchmark[key]
            if type(val) == int:
                print(f"{val}{comma}",end="",file=f)
            else:
                print(f"\"{val}\"{comma}",end="",file=f)
        print(");",file=f)


def print_mongo_item(output, benchmark, key, last):
    val = benchmark[key]
    if not last:
        comma = ','
    else:
        comma = ''
    print(f"	\"{key}\": \"{val}\"{comma}",file=output)

def print_mongo_benchmark(benchmark,filename):
    with open(filename, "a") as f:
        print("{",file=f)

        for key in keys:
            last = (key == keys[-1])
            print_mongo_item(f,benchmark, key, last)
        print("}",file=f)

##### MAIN #####

if len(sys.argv) < 2:
    print(f"	usage: {sys.argv[0]} <benchmark file>")
    exit(1)

computer = 0
benchmark = parse_benchmark(sys.argv[1])
print_mongo_benchmark(benchmark,"../benchmarks.mongo")
print_sql_benchmark(benchmark,"../benchmarks.sql")

