#!/usr/bin/env python 

import sys
import re

prev_line = ""

keys = [
        'name',
        'date',
        'home_build',
        'picture',
        'description',
        'cpu_name',
        'cpu_speed',
        'l1_size',
        'l2_size',
        'l3_size',
        'ram_size',
        'ram_type',
        'chipset',
        'disk',
        'video',
        'case_name',
        'os',
        'gcc',
        'whetstone',
        'dhrystone',
        'dhrystonem',
        'block_write',
        'block_read',
        'l1_speed',
        'l2_speed',
        'l3_speed',
        'ram_speed',
        'pcmark05',
        'pcmark07',
        'pcmark10',
        'dmark03',
        'dmark05',
        'dmark06',
        'firestrike',
        'geekbench',
        ]

def get_line(file):
    global prev_line
    if prev_line != "":
        ret = prev_line
        prev_line = ""
#print("PREV LINE ",ret)
        return ret
    while True:
        line = file.readline()
        line = line.lstrip().rstrip()
        if line != '':
#print("LINE ",line)
            return line

def remove_string(rem, str):
    return re.sub(rem,'',str,flags=re.IGNORECASE).lstrip().rstrip()

def expect_line(rem, str, default = ""):
    global prev_line
#print('EXPECT ',rem)
    if rem.lower() in str.lower():
        prev_line = ''
        return remove_string(rem, str)
    else:
        prev_line = str
        return default

def expect_boolean(rem, str, default = ""):
    global prev_line
#print('EXPECT ',rem)
    if rem.lower() in str.lower():
        prev_line = ''
        return True
    else:
        prev_line = str
        return False

def make_number(rem, str):
    numstr = remove_string(",",expect_line(rem,str))
    numarr = numstr.split(" ");
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
    benchmark = {}
    with open(filename, "r") as file:
        benchmark['name'] = get_line(file)
        benchmark['date'] = get_line(file)
        if expect_boolean("Home build",get_line(file)):
            benchmark['home_build'] = 1
        else:
            benchmark['home_build'] = 0
        benchmark['picture'] = expect_line("Picture",get_line(file))
        benchmark['description'] = expect_line("Description",get_line(file))
        benchmark['cpu_name'] = expect_line("model name",get_line(file))
        benchmark['cpu_speed'] = make_number("cpu MHz",get_line(file))
        benchmark['l1_size'] = make_number("l1 cache",get_line(file))
        benchmark['l2_size'] = make_number("l2 cache",get_line(file))
        benchmark['l3_size'] = make_number("l3 cache",get_line(file))
        benchmark['ram_size'] = make_number("RAM",get_line(file))
        benchmark['ram_type'] = expect_line("RAM Type",get_line(file))
        benchmark['chipset'] = expect_line("Chipset",get_line(file))
        benchmark['disk'] = expect_line("Disk",get_line(file))
        benchmark['video'] = expect_line("video",get_line(file))
        benchmark['case_name'] = expect_line("case",get_line(file))
        benchmark['os'] = get_line(file)
        benchmark['gcc'] = get_line(file)
        benchmark['linebreak'] = get_line(file)
        benchmark['whetstone'] = make_number("Whetstone",get_line(file))
        benchmark['dhrystone'] = make_number("dhrystones",get_line(file))
        benchmark['dhrystonem'] = make_number("dhrystones4c",get_line(file))
        benchmark['block_write'] = make_number("Block Writes MB/s",get_line(file))
        benchmark['block_read'] = make_number("Block Reads MB/s",get_line(file))
        benchmark['l1_speed'] = make_number("L1 Cache MB/s",get_line(file))
        benchmark['l2_speed'] = make_number("L2 Cache MB/s",get_line(file))
        benchmark['l3_speed'] = make_number("L3 Cache MB/s",get_line(file))
        benchmark['ram_speed'] = make_number("Ram MB/s",get_line(file))
        benchmark['pcmark05'] = make_number("pcmark05",get_line(file))
        benchmark['pcmark07'] = make_number("pcmark07",get_line(file))
        benchmark['pcmark10'] = make_number("pcmark10",get_line(file))
        benchmark['dmark03'] = make_number("3dmark03",get_line(file))
        benchmark['dmark05'] = make_number("3dmark05",get_line(file))
        benchmark['dmark06'] = make_number("3dmark06",get_line(file))
        benchmark['firestrike'] = make_number("firestrike",get_line(file))
        benchmark['geekbench'] = make_number("geekbench",get_line(file))
    return benchmark


def print_sql_benchmark(benchmark,filename):
    with open(filename, "a") as f:
        print("INSERT INTO computers (",end="",file=f)
        for key in keys:
            if key == keys[-1]:
                comma = ""
            else:
                comma = ","
            print(f"{key}{comma}",end="",file=f);
        print(") ",file=f);
        print("VALUES (",end="",file=f);
        for key in keys:
            if key == keys[-1]:
                comma = ""
            else:
                comma = ","
            val = benchmark[key]
            if type(val) == int:
                print(f"{val}{comma}",end="",file=f);
            else:
                print(f"\"{val}\"{comma}",end="",file=f);
        print(");",file=f);


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
print_mongo_benchmark(benchmark,"benchmarks.mongo")
print_sql_benchmark(benchmark,"benchmarks.sql")

