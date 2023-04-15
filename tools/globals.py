ENGINE_NAME = "calliope"
PROJECT_NAME = "calliopeeditor"
TOOLS_DIR = "tools"

VERSION_MAJOR = 0
VERSION_MINOR = 0
VERSION_REVISION = 2

ENGINE_NAME = "calliope"
PROJECT_NAME = "calliopeeditor"
TOOLS_DIR = "tools"

import sys, platform
PLATFORM = sys.platform
for x in platform.uname():
    if "microsoft" in x.lower():
        PLATFORM = "windows"
        break

def IsWindows():
    return PLATFORM == "windows" or PLATFORM == "win32"

def IsLinux():
    return PLATFORM == "linux"

def IsMac():
    return PLATFORM == "darwin"

def ProcessArguments(argv):
    ret = {} # return a key:value dict
    for arg in argv:
        try:
            k = arg[0:arg.index("=")]
            v = arg[arg.index("=")+1:]
        except:
            k = arg
            v = 0
        ret[k] = v
    return ret

def GetArgumentValue(args, name, default):
    return args[name] if name in args else default