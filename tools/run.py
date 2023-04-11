import subprocess, globals, os

config = "Debug"
exepath = "{}/bin/{}/{}/".format(os.getcwd(), config, globals.PROJECT_NAME)

if globals.IsWindows():
    subprocess.call(["cmd.exe", "/c", "{}\\run.bat".format(globals.TOOLS_DIR), config, globals.PROJECT_NAME], cwd=os.getcwd())

if globals.IsLinux():
    subprocess.call(["make"], cwd=os.getcwd())