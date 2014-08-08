graphSourceCode
===============

Some scripts for graphing out c++ source code


-----------
Installing

clone the repo and change to the directory to download the needed libraries.
Then run make (g++-4.8 needs to be in your path)


cd graphSourCode
./setup.py cppprogutils
./setup.py cppitertools
make 

------------
running scripts 
all scripts are in bin in graphSourceCode

run testIncludeDir.sh on souce directory and tell it a file to log to 

testIncludeDir.sh src/ srcMapped.dot

You can then run grpahviz on this dot file to simply map your src code
blue nodes are headers, red nodes are cpp files.  
A red edge is cpp including a header and a blue edge is a header including a header

dot -T pdf -O scrMapped.dot

Then running on file modification effects, give the previous mapped file and give a header that was changed
and a name for 
fileModAffect -file scrMapped.dot -header utils.hpp -out utilsModAffect.dot

Give -overWrite to overwrite the file if it already exists 
fileModAffect -file scrMapped.dot -header utils.hpp -out utilsModAffect.dot -overWrite

dot -T pdf -O utilsModAffect.dot


And then to get all affects give 
fileModAffect -file scrMapped.dot -all -out modAll.dot -overWrite



