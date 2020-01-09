from os import system
import sys
CC = ["gcc", "icc"]
CC_FLAGS = {"gcc" : "-std=gnu99 -fopenmp",\
            "icc" : "-std=gnu99 -qopenmp"}
CC_OPTIM_LEVEL = ["-O{}".format(i) for i in range(4)]
app_name = "prod"
src_name = "prod.c"

if sys.argv[1] in ["compile", "all"]:
  # Compile with option
  for compiler in CC:
    flags = CC_FLAGS[compiler]
    for opt_level in CC_OPTIM_LEVEL:
      command = compiler + " " + flags + " " + opt_level
      command += " {} -o {}-{}{}".format(src_name, app_name, compiler, opt_level) 
      print(command)
      if system(command):
        print("Command \"{}\" has failed".format(command))

if sys.argv[1] in ["run", "all"]:
  for opt_level in CC_OPTIM_LEVEL:
      command = "./" + app_name + "-gcc" + opt_level
      if system(command):
        print("Command \"{}\" has failed".format(command))