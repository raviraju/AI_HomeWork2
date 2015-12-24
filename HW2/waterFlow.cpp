#include <iostream>
#include <fstream>
#include <cstring> //strncmp
#include <string>
#include <cstdlib>
#include <set>
#include <sstream>

using namespace std;

int main(int argc, char *argv[])
{
	//g++ waterFlow.cpp –o waterFlow.o
	//./waterFlow.o –i inputFile
	
	ofstream ofs("output.txt", ios::out | ios::app);
	if(!ofs.is_open())
	{
		cout << "Cannot open output file stream" << endl;
		return -1;
	}
	if( argc != 3)
	{
		cout << "Invalid input format. Usage : ./waterFlow.o -i inputFile" << endl;
		return 0;
	}
	if ( strncmp(argv[1],"-i",sizeof("-i")) != 0)
	{
		cout << "Invalid option" << argv[1] << " . Usage : ./waterFlow.o -i inputFile" << endl;
		return 0;
	}
	
	ifstream ifs(argv[2]);
	string line;
	int noOfTestCases = 0;
	////////////////////////////NO OF TEST CASES////////////////////////////////////
	getline(ifs, line);
	noOfTestCases = atoi(line.c_str());
    cout << "No of Test Cases : " << noOfTestCases << endl;
	for ( int i =0 ; i< noOfTestCases ; i++)
	{
		cout << endl << endl;
		cout << "Test Case : " << i+1 << endl; 
		////////////////////////////TEST CASE TASK////////////////////////////////////
		getline(ifs,line);
		string curTask = line;
		string bfsTask("BFS"), dfsTask("DFS"), ucsTask("UCS");
		bool bfs = false, dfs = false, ucs = false;
		//cout << curTask << endl;
		if (curTask.compare(bfsTask) == 0)
		{
			cout << "Run BFS" << endl;
			bfs = true;
		}
		else if (curTask.compare(dfsTask) == 0)
		{
			cout << "Run DFS" << endl;
			dfs = true;
		}
		else if (curTask.compare(ucsTask) == 0)
		{
			cout << "Run UCS" << endl;
			ucs = true;
		}
		else
		{
			cout << "Invalid Search Task : " << curTask;
			return 0;
		}
		////////////////////////////SRC NODE////////////////////////////////////
		getline(ifs,line);
		string srcNode = line;
		cout << "srcNode : " << srcNode << endl;
		
		pair<set<string>::iterator,bool> inserted;
		set<string>::iterator strSetItr;
		////////////////////////////DESTINATION NODES////////////////////////////////////
		getline(ifs,line);
		string destNodesString = line;
		//cout << "destNodesString :" << destNodesString;
		stringstream dss (destNodesString);
		set<string> destNodes;
		string destNode;
		while(dss >> destNode)
		{
			inserted = destNodes.insert(destNode);
			if (inserted.second == false)
				cout << "Duplicate destination : " << destNode << " not inserted" << endl;
		}
		cout << "Destination Nodes are:";
		for (strSetItr=destNodes.begin(); strSetItr!=destNodes.end(); ++strSetItr)
			cout << ' ' << *strSetItr;
		cout << endl;
		////////////////////////////MID NODES////////////////////////////////////
		getline(ifs,line);
		string midNodesString = line;
		//cout << "midNodesString :" << midNodesString;
		stringstream mss (midNodesString);
		set<string> midNodes;
		string midNode;
		while(mss >> midNode)
		{
			inserted = midNodes.insert(midNode);
			if (inserted.second == false)
				cout << "Duplicate middle : " << midNode << " not inserted" << endl;
		}
		cout << "Middle Nodes are:";
		for (strSetItr=midNodes.begin(); strSetItr!=midNodes.end(); ++strSetItr)
			cout << ' ' << *strSetItr;
		cout << endl;
		int noOfPipes = 0;
		////////////////////////////NO OF PIPES////////////////////////////////////
		getline(ifs, line);
		noOfPipes = atoi(line.c_str());
		cout << "No of Pipes : " << noOfPipes << endl;
		for ( int i =0 ; i< noOfPipes ; i++)
		{
			getline(ifs, line);
			cout << "Pipe " << i+1 << " : " << line << endl;
		}
		int startTime = 0;
		////////////////////////////START TIME////////////////////////////////////
		getline(ifs, line);
		startTime = atoi(line.c_str());
		cout << "Start Time : " << startTime << endl;
		while(ifs)
		{
			getline(ifs,line);
			if (line.length() == 0)
				break;
			cout << line << endl;
		}
	}
    ifs.close();
		
	
	return 0;
}