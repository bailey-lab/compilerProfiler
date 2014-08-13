#!/usr/bin/python

import fnmatch, subprocess, sys, os, argparse, re, copy

from headInGraph import fileCollection
from headInGraph import fileNode
from headInGraph import headInGraph





'''
#print "source files"
for f  in allFiles:
    statbuf = os.stat(f)
    #print f + " " + str(statbuf.st_mtime)
#print "Object files: " 
for f  in objectFiles:
    statbuf = os.stat(f)
    #print f + " " + str(statbuf.st_mtime)
    '''
def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('-src', type=str, nargs=1, required =True);
    parser.add_argument('-outMain', type=str, nargs=1, required = True);
    parser.add_argument('-addSystem', dest = 'addSystem', action = 'store_true' );
    return parser.parse_args()
def main():
    args = parse_args()
    allFiles = fileCollection.getAllSourceFiles(args.src[0])
    
    graph = headInGraph();
    for file in allFiles:
        statbuf = os.stat(file)
        if(".h" not in file):
            graph.addNode(os.path.basename(file).replace(".", "_"), fileNode.cppColor, "internal", statbuf.st_mtime)
        else:
            graph.addNode(os.path.basename(file).replace(".", "_"), fileNode.headerColor, "internal", statbuf.st_mtime)
    pattern = re.compile("#include.*\".*\.h")
    patternSystem = re.compile("#include.*\<.*\>")
    for file in allFiles:
        for i, line in enumerate(open(file)):
            for match in re.finditer(pattern, line):
                if(".h" in file): 
                    graph.addPair(os.path.basename(file).replace(".", "_"), os.path.basename(re.findall('"([^"]*)"', line)[0]).replace(".", "_"), fileNode.headToHeadColor)
                else:
                    graph.addPair(os.path.basename(file).replace(".", "_"), os.path.basename(re.findall('"([^"]*)"', line)[0]).replace(".", "_"), fileNode.cppToHeaderColor)
            if args.addSystem:
                for match in re.finditer(patternSystem, line):
                    modifiedHeaderName = ((line[(line.find("<") + 1):line.find(">")]).replace(".", "_")).replace("/", "__");
                    if modifiedHeaderName not in graph.nodePositions_.keys():
                        graph.addNode(modifiedHeaderName, fileNode.externalHeaderColor, "external", 0)
                    if(".h" in file): 
                        graph.addPair(os.path.basename(file).replace(".", "_"), modifiedHeaderName, fileNode.headToHeadColor)
                    else:
                        graph.addPair(os.path.basename(file).replace(".", "_"), modifiedHeaderName, fileNode.cppToHeaderColor)
    
    outMainFile = open(args.outMain[0], "w");
    graph.printGraphViz(outMainFile, "all", args.addSystem)

main()