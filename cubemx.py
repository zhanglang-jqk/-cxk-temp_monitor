import os
import re

root_dir = os.getcwd()
def deleteFileByPattern(path,pattern):
    os.chdir(root_dir)
    os.chdir(path)
    files = os.listdir()
    for file in files:
        result = re.match(pattern,file)
        if(result):
            print("delte match file %s"%file)
            os.remove(file)

# delete match files
deleteFileByPattern("Inc","stm32f\w+.h")
deleteFileByPattern("src","stm32f\w+.c")
deleteFileByPattern("src","system_stm32\w+.c")

# remove match "void Error_Handler(void);"
os.chdir(root_dir + "\\" + "Inc")
text = ""
with open("main.h","r",encoding="utf-8") as f:
    line = f.readline()
    while line:
        if(line.__eq__("void Error_Handler(void);\n")):
            print("delete line %s"%line)
            line = f.readline()
        text += line
        line = f.readline()
with open("main.h","w",encoding="utf-8") as f:
    f.write(text)

os.chdir(root_dir + "\\" + "src")
# rename the file is mattch filename
try:
    os.rename("main.c","cubemx.c")
except FileNotFoundError:
    print("main.c file not found")
except FileExistsError:
    print("remove file is cubemx.c")
    os.remove("cubemx.c")
    os.rename("main.c","cubemx.c")
else:
    print("rename main.c to cubemx.c")

text = ""
with open("cubemx.c","r",encoding="utf-8") as f:
    line = f.readline()
    while line:
        if(line == "static void MX_GPIO_Init(void);\n"):
            # line = f.readline()
            line = "void MX_GPIO_Init(void);\n"
            print("modify static void MX_GPIO_Init(void); to void MX_GPIO_Init()")
        if(line == "static void MX_GPIO_Init(void)\n"):
            line = "void MX_GPIO_Init()\n"
            print("modify static void MX_GPIO_Init(void); to void MX_GPIO_Init()")
        text += line
        line = f.readline()
with open("cubemx.c","w",encoding="utf-8") as f:
    f.write(text)