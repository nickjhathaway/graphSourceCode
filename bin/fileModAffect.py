#!/usr/bin/python

import fnmatch, subprocess, sys, os, argparse, re





#srcDir = sys.argv[1]
#objdir = sys.argv[2]


class node():
    def __init__(self, name, modTime):
        self.value_ = name
        self.modTime_ = modTime
        self.objectModTime_ = 0
        self.children_= []
        self.color_ = ""
        self.visited_ = False
    def changeObjectModTime(self, objModTime):
        self.objectModTime_ = objModTime

class inGraph():
    def __init__(self):
        self.nodes_ = []
        self.nodePositions_ = {}
    
    def addNode(self, name, modTime):
        self.nodePositions_[name] = len(self.nodes_)
        self.nodes_.append(node(name, modTime))
    
    def addPair(self, including, beingIncluded):
        self.nodes_[self.nodePositions_[beingIncluded]].children_.append(self.nodePositions_[including])
    
    def modChildren(self, nodePos, modColor):
        if not self.nodes_[nodePos].visited_:
            self.nodes_[nodePos].visited_ = True;
            self.nodes_[nodePos].color_ = modColor;
            for childPos in self.nodes_[nodePos].children_ :
                self.modChildren(childPos, modColor);
                
    def getChildrenList(self, nodePos):
        allChildren = []
        print str(self.nodes_[nodePos].visited_) + " " + self.nodes_[nodePos].value_
        if not self.nodes_[nodePos].visited_:
            self.nodes_[nodePos].visited_ = True;
            allChildren.append(nodePos)
            for child in self.nodes_[nodePos].children_:
                allChildren = allChildren + self.getChildrenList(child)
        return allChildren
    
    def printChildren(self, nodePos, out):
        if not self.nodes_[nodePos].visited_:
            self.nodes_[nodePos].visited_ = True;
            out.write(self.nodes_[nodePos].value_ + " ");
            for child in self.nodes_[nodePos].children_:
                self.printChildren(child, out)
    
    def addObjecTime(self, name, objModTime):
        self.nodes_[self.nodePositions_[name]].objectModTime_ = objModTime
        
    def reset(self):
        for key, value in self.nodePositions_.iteritems():
            self.nodes_[value].visted_ = False
            #print self.nodes_[value].value_
            #print self.nodes_[value].visted_
    
    def printGraphViz(self, out, title):
        out.write("digraph G  { \n")
        out.write("bgcolor =\"#000000\" \n")
        out.write("labelloc=\"t\" \n")
        out.write("fontcolor = \"#ffffff\"\n")
        out.write("fontsize = 20 \n")
        out.write("label = \"" + title + "\" \n")
        out.write("fixedsize = true; \n")
        for key, value in self.nodePositions_.iteritems():
            if not self.nodes_[value].visited_:
                self.nodes_[value].color_ = "#4dac26"
            out.write(self.nodes_[value].value_ + "[shape=circle,style=filled,fixesize =true, color = \"#000000\", fillcolor =\"" + self.nodes_[value].color_ + "\", width = 1]\n") 
        
        
        for key, value in self.nodePositions_.iteritems():
            for child in self.nodes_[value].children_:
                out.write(self.nodes_[value].value_ + " -> " + self.nodes_[child].value_ + "[penwidth=5, color=\"") 
                if self.nodes_[value].visited_:
                    out.write("#f1b6da");
                else:
                    out.write("#4dac26");
                out.write("\"]\n")
        out.write("}\n")  
    
    
def getCppFiles(searchDir):
    return [os.path.join(dirpath, f)
    for dirpath, dirnames, files in os.walk(searchDir)
    for f in fnmatch.filter(files, '*.c*')]

def getHeaderFiles(searchDir):
    return [os.path.join(dirpath, f)
    for dirpath, dirnames, files in os.walk(searchDir)
    for f in fnmatch.filter(files, '*.h*')]

def getObjectFiles(searchDir):
    return [os.path.join(dirpath, f)
    for dirpath, dirnames, files in os.walk(searchDir)
    for f in fnmatch.filter(files, '*.o')]

def getAllSourceFiles(searchDir):
    return getCppFiles(searchDir) + getHeaderFiles(searchDir);


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
    parser.add_argument('-obj', type=str, nargs=1, required =True);
    parser.add_argument('-out', type=str, nargs=1, required = True);
    parser.add_argument('-outTest', type=str, nargs=1, required = True);
    return parser.parse_args()
def main():
    args = parse_args()
    outFile = open(args.out[0], "w")
    allFiles = getAllSourceFiles(args.src[0])
    objectFiles = getObjectFiles(args.obj[0])
    outFile.write( "digraph G  { \n" )
    outFile.write("bgcolor =\"#000000\" \n")
    outFile.write("labelloc=\"t\" \n" )
    outFile.write("fontcolor = \"#ffffff\"\n" )
    outFile.write("fontsize = 20 \n" )
    outFile.write("label = \"top\" \n")
    outFile.write("fixedsize = true; \n")
    graph = inGraph();
    objectFilesTrimedDic = {}
    for obj in objectFiles:
        srcName = obj.find(args.src[0])
        objectFilesTrimedDic[obj[srcName:]] = obj
    
    for file in allFiles:
        statbuf = os.stat(file)
        graph.addNode(os.path.basename(file).replace(".", "_"), statbuf.st_mtime)
        if(".h" not in file):
            lastPeriod = file.rfind(".c")
            objNameFile = file[:lastPeriod] + ".o"
            if objNameFile in objectFilesTrimedDic.keys():
                statbuf = os.stat(objectFilesTrimedDic[objNameFile])
                #print objNameFile + " " + str(statbuf.st_mtime)
                graph.addObjecTime(os.path.basename(file).replace(".", "_"), statbuf.st_mtime)
        if(".h" in file):
            outFile.write(os.path.basename(file).replace(".", "_") + " [shape=circle,style=filled,fixesize =true, color = \"#000000\", fillcolor =\"#0571b0\", width = 1]" + "\n")
        else:
            outFile.write(os.path.basename(file).replace(".", "_") + " [shape=circle,style=filled,fixesize =true, color = \"#000000\", fillcolor =\"#ca0020\", width = 1]" + "\n")
    pattern = re.compile("#include.*\".*\.h")
    for file in allFiles:
        for i, line in enumerate(open(file)):
            for match in re.finditer(pattern, line):
                graph.addPair(os.path.basename(file).replace(".", "_"), os.path.basename(re.findall('"([^"]*)"', line)[0]).replace(".", "_") )
                if(".h" in os.path.basename(re.findall('"([^"]*)"', line)[0])): 
                    outFile.write(os.path.basename(re.findall('"([^"]*)"', line)[0]).replace(".", "_") + " -> " + os.path.basename(file).replace(".", "_") + " ")
                    outFile.write("[penwidth=5, color=\"#92c5de\"]\n")
                else:
                    outFile.write(os.path.basename(re.findall('"([^"]*)"', line)[0]).replace(".", "_") + " -> " + os.path.basename(file).replace(".", "_")+  " ") 
                    outFile.write("[penwidth=5, color=\"#f4a582\"]\n")
    outFile.write("}\n")
    chil = graph.getChildrenList(graph.nodePositions_["profilerSetUp_hpp"]);
    print chil
    outFileTest = open(args.outTest[0], "w")
    graph.modChildren(graph.nodePositions_["stringUtils_hpp"], "#d01c8b")
    graph.printGraphViz(outFileTest, "all")
    graph.reset()
    for key, value in graph.nodePositions_.iteritems():
        graph.nodes_[value].visted_ = False
        print graph.nodes_[value].value_
        print graph.nodes_[value].visted_
    chil2 = graph.getChildrenList(graph.nodePositions_["profilerSetUp_hpp"]);
    print chil2
    for key, value in graph.nodePositions_.iteritems():
        graph.reset()
        if not (key.endswith("_c") or key.endswith("_cpp")):
            print key  + " " + str(graph.nodes_[value].modTime_)
            for child in graph.getChildrenList(value):
                if not (graph.nodes_[child].value_.endswith("_h") or graph.nodes_[child].value_.endswith("_hpp") or child == value):
                    print "\t" + graph.nodes_[child].value_
                    print "\t\t" + str(graph.nodes_[child].modTime_) + " " + str(graph.nodes_[child].objectModTime_)
                    if graph.nodes_[value].modTime_ > graph.nodes_[child].objectModTime_:
                        print "\t\t" + "\033[0m", "\033[1;31m header is newer\033[0m"
                    else:
                        print "\t\t" + "\033[0m", "\033[1;32mheader is older\033[0m"
    
    
main()