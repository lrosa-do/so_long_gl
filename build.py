import sys
import os
import json
import glob
from os import walk
from subprocess import STDOUT, PIPE
import subprocess
import shlex
import argparse
import shutil
import time
import json
import re
import subprocess
from subprocess import check_output, CalledProcessError, STDOUT
from datetime import datetime
import logging
import random
import sys
import time

OSP=os.path.sep


EMCC ="/home/djoker/code/emsdk/upstream/emscripten/emcc"
EMCPP ="/home/djoker/code/emsdk/upstream/emscripten/em++"
EMAR ="/home/djoker/code/emsdk/upstream/emscripten/emar"

PORT = 8080


SHOW_COMMAND=False
    
def getFoldersTree(root,max_depth=0):
    path =root
    path = os.path.normpath(path)
    res = []
    for root,dirs,files in os.walk(path, topdown=True):
        depth = root[len(path) + len(os.path.sep):].count(os.path.sep)
        if depth == max_depth:
            res += [os.path.join(root, d) for d in dirs]
            dirs[:] = [] # Don't recurse any deeper
    
    return res
            
def runProcess(command, args=[],wait=True):
    args = [command] + args
    def cmd_args_to_str(cmd_args):
        return ' '.join([arg if not ' ' in arg else '"%s"' % arg for arg in cmd_args])
    global SHOW_COMMAND
    if SHOW_COMMAND:
        print("Execute -> ",cmd_args_to_str(args))
    proc = subprocess.Popen(args,
        stdout = subprocess.PIPE,
        stderr = subprocess.PIPE,
    )
    
    stdout, stderr = proc.communicate()
    if wait:
        proc.wait()
    return proc.returncode, stdout, stderr
    
def cmd_args_to_str(cmd_args):
    return ' '.join([arg if not ' ' in arg else '"%s"' % arg for arg in cmd_args])

def call_cmd(command, args=[]):
    args = [command] + args
    result = subprocess.Popen([command], shell=True,close_fds=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    result.wait()
    chkdata = result.stdout.readlines()
    for line in chkdata:
        lstr=line.decode('utf-8')
        print(lstr)
        

        


def getParentDir(path, level=1):
  return os.path.normpath( os.path.join(path, *([".."] * level)) )

def cleanString(string):
    return "-".join(string.split())

def createPath(root,sub):
    path = os.path.join(os.path.dirname(os.path.abspath(root)), sub)
    print("Create path ",path)
    if not os.path.exists(path):
            os.mkdir(path)

def createFolderTree(maindir):
    if not os.path.exists(maindir):
        try:
                    os.makedirs(maindir)
        except OSError as e:
                 print('Something else happened'+str(e))

rsrcPath = os.getcwd()+os.path.sep+"res"
templatesPath = os.getcwd()+os.path.sep+"Templates"
modulesPath = os.getcwd()+os.path.sep+"modules"
projectsPath = os.getcwd()+os.path.sep+"projects"

#debugs

showSRCLoad=False

isLinux=True

def WebCompile(folderRoot ,name, srcs ,CARGS,CPPARGS, fullBuild=False):
    useCPP=False
    outFolder=folderRoot+os.path.sep+"obj"+os.path.sep+"Web"+os.path.sep+name
    createFolderTree(outFolder)
    objsList=[]
    

    index=0
    numSrc=len(srcs)
    for src in srcs:
        if not os.path.isfile(src):
            print("File ",src,"not exists")
            continue
    
        args=[]
        srcFolder = os.path.dirname(os.path.abspath(src))
        objFolder =outFolder +  srcFolder.replace(folderRoot,"")
        
        createFolderTree(objFolder)
        filename, file_extension = os.path.splitext(src)
        basename = os.path.basename(src)
        basename_without_ext = os.path.splitext(os.path.basename(src))[0]
        objName = objFolder+os.path.sep+basename_without_ext+".o"
        objsList.append(objName)
        src_modified_time = os.path.getmtime(src)
        src_convert_time = time.ctime(src_modified_time)
        
        index +=1
     
        cType = EMCC
        if len(file_extension)>=3:
                cType=EMCPP
                useCPP=True
                
        
        if not fullBuild:       
            if os.path.exists(objName):
                obj_modified_time = os.path.getmtime(objName)
                obj_convert_time   = time.ctime(obj_modified_time)
                if (src_convert_time<obj_convert_time):
                    print("Skip  file"+ src)
                    continue
            
                
    
        
        print (cType," ",os.path.basename(src),">",os.path.basename(objName))

            
                
        args.append("-c")
        args.append(src)
        args.append("-o")
        args.append(objName)
        #args.append("-s")
        #args.append("ASYNCIFY")
        args.append("-DPLATFORM_WEB")
        
        


        if useCPP:
            for arg in CPPARGS: 
                value =arg.strip()
                if len(value)>=2:
                    args.append(arg)
                    
        else:
            for arg in CARGS: 
                value =arg.strip()
                if len(value)>=2:
                    args.append(arg)
                    


                        
        
        final_cmd =str(args)
        final_cmd=final_cmd.replace("'",' ')
        final_cmd=final_cmd.replace(",",' ')
        final_cmd=final_cmd.replace("[",' ')
        final_cmd=final_cmd.replace("]",' ')
        print(cType," ",final_cmd)    
        code, out, err=runProcess(cType,args)
        #print("err: '{}'".format(str(err)))
        #print("exit: {}".format(code))
        if code!=0:
            print(err.decode("utf-8") )
            rexp=':(.*?):(.*?): error: '
            #match = re.compile(rexp, re.DOTALL | re.IGNORECASE).findall(err.decode("utf-8"))
            erro =re.search(rexp,err.decode("utf-8"))
            try:    
                lineY=0
                lineX=0
                if erro:
                    linhas = erro.group().split(":")
                    lineX=int(linhas[2])
                    lineY=int(linhas[1])
            except Exception as e:
                print('Error on line {}'.format(sys.exc_info()[-1].tb_lineno), type(e).__name__, e)
            return False
        print(out.decode("utf-8") )
    print("Compiling completed")
    return True
    
       
def WebBuild(folderRoot ,appName, useCPP,LDARGS , isStatic=True):
    args=[]

    outFolder=folderRoot+os.path.sep+"Web"+os.path.sep
    createFolderTree(outFolder)
    
    ListOBJS=[]
    webObjRoot =folderRoot+OSP+"obj"+ OSP+"Web"+OSP+appName

    for root, directories, files in os.walk(webObjRoot, topdown=False):
        
        for name in files:
            ext = os.path.splitext(name)[1]
            if (ext==".o"):
                if not name in ListOBJS:
                    ListOBJS.append(root+OSP+name)
                 
                    
            
    cType = EMCC
    if useCPP:
            cType=EMCPP
            
    if isStatic==False:
        print("Build EMSDK aplication")
        args.append("-o")
        export = outFolder+os.path.sep+appName+".html"
        args.append(export)
        objs=""
        for obj in ListOBJS:
            objs+=obj+' '
            args.append(obj)
        #args.append('-s')


        for arg in LDARGS:
            value =arg.strip()
            if len(value)>1:
                args.append(value)
        rootFolder=os.getcwd()+os.path.sep+"libs"+os.path.sep+"Web"
        args.append("-L"+rootFolder)
        args.append("-s")
        args.append("ASYNCIFY")
        print("Add folder lib:",rootFolder)
        '''
        args.append("-s")
        args.append("USE_SDL=2")
        args.append("-s")
        args.append("FULL_ES2=1")
        args.append("-s") 
        args.append("TOTAL_MEMORY=67108864")
        args.append("-s")
        args.append("FORCE_FILESYSTEM=1")
        '''
    
            
        print("Build to "+cType+" "+appName)
        
        final_cmd =str(args)
        final_cmd=final_cmd.replace("'",' ')
        final_cmd=final_cmd.replace(",",' ')
        final_cmd=final_cmd.replace("[",' ')
        final_cmd=final_cmd.replace("]",' ')
        print(cType," ",final_cmd)
        code, out, err=runProcess(cType,args)
        if code!=0:
            print(err.decode("utf-8") )
            print("Operation Fail  .. ")
            return False
        
        
    
        
        
    if isStatic==True:
        print("Build EMSDK static lib")
        #export =folderRoot+os.path.sep+appName+".a"
        export =folderRoot+os.path.sep+"Web"+os.path.sep+'lib'+appName+".a"
        args.append("rcs")
        args.append(export)
        print("Save to ",export)
        for obj in ListOBJS:
            args.append(obj)
        
        final_cmd =str(args)
        final_cmd=final_cmd.replace("'",' ')
        final_cmd=final_cmd.replace(",",' ')
        final_cmd=final_cmd.replace("[",' ')
        final_cmd=final_cmd.replace("]",' ')
        print(cType," ",final_cmd)
        code, out, err=runProcess(EMAR,args)
        if code!=0:
            print(err.decode("utf-8") )
            print("Operation Fail  .. ")
            return False
    print("Done :) ")
    return True


NAME ="so_long"


DIR="/media/djoker/data/code/c/solonggl/git"


SRC =[]
#SRC.append("/media/djoker/data/code/c/solonggl/src/build/src/main.c")




for file in glob.glob(glob.escape(DIR+OSP+"src"+OSP) + "*.c"):
    SRC.append(file)


ARGCPP=[]

ARGCC=[]
ARGCC.append("-I")
ARGCC.append("/home/djoker/code/crosside/modules/sdl2/include")
ARGCC.append("-I")
ARGCC.append("/media/djoker/data/code/c/solonggl/git/src")
ARGCC.append("-I")
ARGCC.append("/media/djoker/data/code/c/solonggl/git/include")




compile = WebCompile(DIR,NAME,SRC,ARGCC,ARGCPP,False)

ARGLD=[]
ARGLD.append("-s")
ARGLD.append("FORCE_FILESYSTEM=1")
ARGLD.append("-s")
ARGLD.append("USE_SDL=2")
ARGLD.append("-s")
ARGLD.append("FULL_ES3=1")
ARGLD.append("-s")
ARGLD.append("MIN_WEBGL_VERSION=2")
ARGLD.append('--shell-file')
ARGLD.append('shell.html')
if os.path.exists(DIR+OSP+"assets"):
    print("*** add",DIR+OSP+"assets")
    ARGLD.append('--preload-file')
    ARGLD.append(DIR+OSP+"assets"+"@assets")

app=DIR + OSP + "Web"+OSP  + NAME+ ".html"
if compile:
    if WebBuild(DIR ,NAME, True,ARGLD , False):
        print("Run ",app)
        runProcess(os.getcwd()+OSP+"cserver_linux",[app],False)
else:
    print("abort")