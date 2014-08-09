#include <stdio.h>
#include <cstddef>
#include <utility>
#include <iterator>
#include <iostream>
#include <vector>
#include <array>
#include <stdint.h>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <algorithm>
#include <random>
#include <sstream>
#include <fstream>
#include <dirent.h>
#include <errno.h>
#include <exception>
#include <stdexcept>
#include <memory>
#include <thread>
#include <deque>
// For toupper()
#include <cctype>

// For isdigit()
#include <ctype.h>

// Below is for file modification times
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <ctime>
#include <sys/types.h>
#include <regex.h>
#include <functional>
#include <chrono>

#include <cppitertools/enumerate.hpp>
#include <cppitertools/range.hpp>
#include <cppprogutils.h>
#include <cppitertools/range.hpp>
#include <memory>
typedef std::vector<std::string> VecStr;
VecStr tokenizeString(const std::string& str, const std::string& delim,
                      bool addEmptyToEnd = false) {
  VecStr output;
  if("whitespace" == delim){
    std::stringstream tempStream(str);
    while (!tempStream.eof()) {
      std::string tempName;
      tempStream >> tempName;
      output.emplace_back(tempName);
    }
  }else{
    if (str.find(delim.c_str()) == std::string::npos) {
      output.push_back(str);
    } else {
      std::size_t pos = str.find(delim, 0);
      std::size_t oldPos = -delim.size();
      while (pos != std::string::npos) {
        output.push_back(
            str.substr(oldPos + delim.size(), pos - oldPos - delim.size()));
        oldPos = pos;
        pos = str.find(delim, pos + 1);
      }
      if (oldPos + delim.size() == str.size()) {
        if (addEmptyToEnd) {
          output.push_back("");
        }
      } else {
        output.push_back(str.substr(oldPos + delim.size(), str.size() - 1));
      }
    }
  }
  return output;
}
void trimEndWhiteSpace(std::string& str) {
  size_t lastPlacePos = str.size() - 1;
  char lastPlace = str[str.size() - 1];
  while (isspace(lastPlace)) {
    lastPlacePos--;
    lastPlace = str[lastPlacePos];
  }
  size_t firstPlacePos = 0;
  char firstPlace = str[0];
  while (isspace(firstPlace)) {
    firstPlacePos++;
    firstPlace = str[firstPlacePos];
  }
  str = str.substr(firstPlacePos, lastPlacePos - firstPlacePos + 1);
}
std::size_t findFirstWhitespace(const std::string & str){
	return str.find_first_of(" \f\n\r\t\v");
}
bool trimAtFirstWhitespace(std::string & str){
	std::size_t firstWhitespace = findFirstWhitespace(str);
	if(firstWhitespace == std::string::npos	){
		return false;
	}else{
		str.erase(firstWhitespace);
		return true;
	}
}
std::string replaceString(std::string theString,
                          const std::string& replaceSpace,
                          const std::string& newSpace) {
  size_t spaceSize = replaceSpace.size();
  size_t currPos = theString.find(replaceSpace);
  while (currPos != std::string::npos) {
    theString.replace(currPos, spaceSize, newSpace);
    currPos = theString.find(replaceSpace, currPos + newSpace.size());
  }
  return theString;
}
bool fexists(const std::string &filename) {
  /*boost::filesystem::path p(filename);
  if(boost::filesystem::exists(p)){
          std::cout << "exists" << std::endl;
  }*/
  std::ifstream ifile(filename.c_str());
  if (ifile) {
    return true;
  } else {
    return false;
  }
}
void openTextFile(std::ofstream &file, std::string filename,
                  std::string fileExtention, bool overWrite,
                  bool exitOnFailure) {
  if (filename.find(fileExtention) == std::string::npos) {
    filename.append(fileExtention);
  } else if (filename.substr(filename.size() - fileExtention.size(),
                             fileExtention.size()) != fileExtention) {
    filename.append(fileExtention);
  }
  // std::ofstream file;
  if (fexists(filename) && !overWrite) {
    std::cout << filename << " already exists" << std::endl;
    if (exitOnFailure) {
      exit(1);
    }
  } else {
    file.open(filename.data());
    if (!file) {
      std::cout << "Error in opening " << filename << std::endl;
      if (exitOnFailure) {
        exit(1);
      }
    } else {
      // chmod(filename.c_str(), S_IRWU|S_IRGRP|S_IWGRP|S_IROTH);
      chmod(filename.c_str(),
            S_IWUSR | S_IRUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
      // chmod(filename.c_str(), S_IRWXU|S_IRGRP|S_IXGRP|S_IWGRP|S_IROTH);
    }
  }
}
/* modified = #d01c8b
 * connection to mod = #f1b6da
 * connection to unmod = #b8e186
 * unmod = #4dac26
 */

namespace graphColors{
static std::string modColor = "#d01c8b";
static std::string modEdgeColor = "#f1b6da";
static std::string unModColor = "#4dac26";
static std::string unModEdgeColor = "#b8e186";
}

class includeGraph {
public:

	struct node{
		node(const std::string & str): value_(str), color_(""){}
		std::string value_;
		std::vector<std::shared_ptr<node>> children_;
		bool visited_ = false;
		std::string color_;

	};

	std::vector<std::shared_ptr<node>> nodes_;
	std::unordered_map<std::string, uint32_t> nodesPosition_;



	void addNode(const std::string & str){
		nodesPosition_[str] = nodes_.size();
		nodes_.emplace_back(std::make_shared<node>(str));
	}

	void addPair(const std::string & including, const std::string & beingIncluded){
		nodes_[nodesPosition_[beingIncluded]]->children_.emplace_back(nodes_[nodesPosition_[including]]);
	}

	void modChildren(std::shared_ptr<node> & n,
			const std::string & modColor){
		if(!n->visited_){
			n->visited_ = true;
			n->color_ = modColor;
			for(auto & child : n->children_){
				modChildren(child, modColor);
			}
		}
	}

	void printChildren(const std::shared_ptr<node> & n, std::ostream & out){
		if(!n->visited_){
			n->visited_ = true;
			out << n->value_ << " ";
			for(const auto & child : n->children_){
				printChildren(child, out);
			}
		}
	}

	void reset(){
		std::for_each(nodes_.begin(), nodes_.end(), [&](std::shared_ptr<node> & n){ n->visited_ = false;});
	}

	void printGraphViz(std::ostream & out, const std::string & title) const {
		out << "digraph G  { \n"
		"bgcolor =\"#000000\" \n"
		"labelloc=\"t\" \n"
		"fontcolor = \"#ffffff\"\n"
		"fontsize = 20 \n"
		"label = \"" + title + "\" \n"
		"fixedsize = true; \n";
		for(auto & n : nodes_){
			if(!n->visited_){
				n->color_ = graphColors::unModColor;
			}
			out << n->value_ <<
					"[shape=circle,style=filled,fixesize =true, color = \"#000000\", fillcolor =\""
					<< n->color_ << "\", width = 1]\n";
		}
		for(const auto & n : nodes_){
			for(const auto & child : n->children_){
				out << n->value_ << " -> " << child->value_ <<
									"[penwidth=5, color=\"";
				if(n->visited_){
					out << graphColors::modEdgeColor;
				}else{
					out << graphColors::unModEdgeColor;
				}
				out << "\"]\n";
			}
		}
		out << "}\n";
	}
};

/*
 * 	std::cout << inGraph.nodes_[inGraph.nodesPosition_[headerName]]->value_ << std::endl;
	for(const auto & n : inGraph.nodes_){
		std::cout << n->value_ << std::endl;

		for(const auto & child : n->children_){
			std::cout << "\t" << child->value_ << std::endl;
		}
	}
	for(const auto & pos : inGraph.nodesPosition_){
		std::cout << pos.first << " " << pos.second << std::endl;
	}
 */

int main(int argc, char* argv[]) {
	cppprogutils::programSetUp setUp(argc, argv);
	std::string filename = "";
	std::string headerName = "";
	std::string outFilename = "";
	bool printGraphViz = false;
	bool printAll = false;
	bool overWrite = false;
	setUp.setOption(filename, "-file,-filename", "filename", true);
	if(!setUp.setOption(printAll, "-printAll,-all", "printAllchanges")){
		setUp.setOption(headerName, "-header,-headerName", "header", true);
		printGraphViz = setUp.setOption(outFilename, "-out,-outFile", "outfilename", true);
	}
	setUp.setOption(overWrite, "-overWrite", "overWrite");
	printGraphViz = setUp.setOption(outFilename, "-out,-outFile", "outfilename");


	setUp.finishSetUp(std::cout);
	//setUp.startARunLog(setUp.directoryName_);
	std::ifstream inFile(filename);
	includeGraph inGraph;
	for(std::string line; std::getline(inFile, line);){
		if(line.find("->") != std::string::npos){
			auto toks = tokenizeString(line, "->");
			std::for_each(toks.begin(), toks.end(), [&](std::string & str){ trimEndWhiteSpace(str);});
			trimAtFirstWhitespace(toks[1]);
			inGraph.addPair(toks[1], toks[0]);
		}else if (line.find("_h") != std::string::npos ||
				line.find("_c") != std::string::npos){
			std::stringstream tempStream;
			tempStream << line;
			std::string tempStr = "";
			tempStream >> tempStr;
			inGraph.addNode(tempStr);
		}
	}
	if(headerName != ""){
		headerName = replaceString(headerName, ".h", "_h");
		inGraph.printChildren(inGraph.nodes_[inGraph.nodesPosition_[headerName]], std::cout);
		inGraph.reset();
		inGraph.modChildren(inGraph.nodes_[inGraph.nodesPosition_[headerName]], graphColors::modColor);
		inGraph.nodes_[inGraph.nodesPosition_[headerName]]->color_ = "#ff0000";
	}

	//
	if(printGraphViz){
		std::ofstream outFile;
		openTextFile(outFile, outFilename, ".dot", overWrite, false);
		if(printAll){
			for(auto & file : inGraph.nodesPosition_){
				if(file.first.find("_c") != std::string::npos){
					continue;
				}
				inGraph.reset();
				inGraph.modChildren(inGraph.nodes_[file.second], graphColors::modColor);
				inGraph.nodes_[file.second]->color_ = "#ff0000";
				inGraph.printGraphViz(outFile, file.first);
			}
		}else{
			inGraph.printGraphViz(outFile, headerName);
		}
	}
	setUp.logRunTime(std::cout);
	return 0;
}
