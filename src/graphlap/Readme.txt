Prerequisite
You need to have opencv  installed on your computer

Procedure
1)Compile the code using the command : g++ -g -std=c++11 <file name e.g. graphlap_label.cpp> `pkg-config opencv --cflags --libs`
2)The code requires three input files placed in the same folder from where you are executing the code naming spdescrs.txt,adjacency_list.txt,multicolumn_result_label.txt.
3)After you execute  the binary file the program will ask to give the value of alpha.
4)Once you input the alpha value wait for  20-30 seconds and the program will dump the results in the file result_label_graphlap.txt. 
