import os
import re
# Import("env") 

# Arduino
# SRC_PATH = "src/"
# TMP_PATH = SRC_PATH + "_tmp.eebf"
# DEST_PATH = "include/eebrainfuck_code.h"

SRC_PATH = "./"
TMP_PATH = SRC_PATH + "_tmp.eebf"
DEST_PATH = "eebrainfuck_code.h"


def insert_ebbf_file():
  print("Start converting")
  print("Now work with only one file!")

  files = os.listdir(SRC_PATH)
  eebf_files = []
  for file in files:
    if file.find(".eebf") != -1:
      eebf_files.append(file)

  for eebf_file in eebf_files:
    with open(SRC_PATH+eebf_file,"r") as file:
      data =  file.read(-1)
      good_data = ""
      raw_data = ""
      # while len(data) > 0:
      good_data += re.sub(r"|[A-Z]|[a-z]|[0-9]|[\s]", '', data)
      # good_data += re.sub(r"[\s]", '', tmp)
      raw_data += data
        # data = file.read(100)
      # print("-------Raw data-------")
      # print(raw_data)
      # print("-------Good data-------")
      # print(good_data)
      with open(TMP_PATH, "a+") as good_file:
        good_file.write(good_data)
  with open(DEST_PATH, "w") as good_file: 
    with open(TMP_PATH, "r") as tmp_file:
      tmp_data = tmp_file.read(-1)

      good_file.write(f"#pragma once\n")
      # good_file.write(f"#include <Arduino.h>\n")
      good_file.write(f"const int eebfSize= {len(tmp_data)};\n")
      good_file.write("const char  eebf[] = \"")
      good_file.write(tmp_data)
      good_file.write("\";")
      print(f"Wrote to: {DEST_PATH}")
      print(f"Wrote {len(tmp_data)} Extended Embedded Brainfuck instructions")
  os.remove(TMP_PATH)

insert_ebbf_file()


