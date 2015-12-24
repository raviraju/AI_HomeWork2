#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring> //strncmp
#include <cstdlib>//atoi
#include <queue>//pits

using namespace std;

int main(int argc, char *argv[])
{
	//g++ mancala.cpp –o mancala.o
	//./mancala.o –i inputFile
	
	ofstream ofs("next_state.txt", ios::out | ios::trunc);
	if(!ofs.is_open())
	{
		cout << "Cannot open output file stream" << endl;
		return -1;
	}
	if( argc != 3)
	{
		cout << "Invalid input format. Usage : ./mancala.o -i inputFile" << endl;
		return 0;
	}
	if ( strncmp(argv[1],"-i",sizeof("-i")) != 0)
	{
		cout << "Invalid option" << argv[1] << " . Usage : ./mancala.o -i inputFile" << endl;
		return 0;
	}
	
	ifstream ifs(argv[2]);
	string line;
	
	////////////////////////////TASK////////////////////////////////////
	int task = 0;//<Task#> Greedy=1, MiniMax=2, Alpha-Beta=3, Competition=4
	getline(ifs, line);
	task = atoi(line.c_str());
	bool greedy = false, miniMax = false, alphaBeta = false, competition = false;
	switch(task)
	{
		case 1:
				cout << "Run Greedy Algo" << endl;			greedy = true;
				break;
		case 2:
				cout << "Run MiniMax Algo" << endl;			miniMax = true;
				break;
		case 3:
				cout << "Run Alpha-Beta Algo" << endl;		alphaBeta = true;
				break;
		case 4:
				cout << "Run in Competition Mode" << endl;	competition = true;
				break;
		default:
				cout << "Invalid task : " << task << endl;
				return 0;
	}
	////////////////////////////PLAYER////////////////////////////////////
	int playerNo = 0;//<Your player: 1 or 2>
	getline(ifs, line);
	playerNo = atoi(line.c_str());
	cout << "playerNo : " << playerNo << endl;
	////////////////////////////CUT OF DEPTH////////////////////////////////////
	int cutOfDepth = 0;//<Cutting off depth>
	getline(ifs, line);
	cutOfDepth = atoi(line.c_str());
	cout << "cutOfDepth : " << cutOfDepth << endl;
	////////////////////////////Board state for player-2////////////////////////////////////
	getline(ifs,line);
	string pitsStatesString = line;
	cout << "pitsStatesString for player-2 : " << pitsStatesString << endl;
	stringstream pss1 (pitsStatesString);
	queue<string> pits1;
	string pit;
	while(pss1 >> pit)
		pits1.push(pit);
	int noOfPits = pits1.size();//N
	int** board = new int*[2];
	for(int i = 0; i < 2; ++i)
		board[i] = new int[noOfPits+2];
	int i=1;
	while (!pits1.empty())
	{
		board[0][i++] = atoi((pits1.front()).c_str());
		pits1.pop();
	}
	////////////////////////////Board state for player-1////////////////////////////////////
	getline(ifs,line);
	pitsStatesString = line;
	cout << "pitsStatesString for player-1 : " << pitsStatesString << endl;
	stringstream pss2 (pitsStatesString);
	queue<string> pits2;
	while(pss2 >> pit)
		pits2.push(pit);
	if(noOfPits != pits2.size())
	{
		cout << "Invalid inputs as the no of pits arent equal for two players" << endl;
		return 0;
	}
	i=1;
	while (!pits2.empty())
	{
		board[1][i++] = atoi((pits2.front()).c_str());
		pits2.pop();
	}
	
	int mancala = 0;
	////////////////////////////<#stones in player-2’s mancala>////////////////////////////////////
	getline(ifs, line);
	mancala = atoi(line.c_str());
	cout << "#stones in player-2 mancala : " << mancala << endl;
	board[0][0] = mancala;board[1][0] = mancala;
	////////////////////////////<#stones in player-2’s mancala>////////////////////////////////////
	getline(ifs, line);
	mancala = atoi(line.c_str());
	cout << "#stones in player-1 mancala : " << mancala << endl;
	board[0][noOfPits+1] = mancala;board[1][noOfPits+1] = mancala;
	
	cout << "Board State : " << endl;
	for(int i =0; i<2 ; i++)
	{
		for(int j =0; j<(noOfPits+2); j++)
			cout << board[i][j] << " , ";
		cout << endl;
	}
	
    ifs.close();
		
	
	return 0;
}