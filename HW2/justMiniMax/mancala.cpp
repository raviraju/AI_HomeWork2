#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring> 		//strncmp
#include <cstdlib>		//atoi
#include <queue>		//pits
#include <utility>      //std::pair
#include <map>			//pitNamePositionMap
#include <set>			//playerValidMovesMap

using namespace std;

typedef map<string,pair<int,int> >::iterator pitNamePositionMapItr;
typedef map<pair<int,int>, string >::iterator positionMancalaMapItr;
typedef map<int, set<string> >::iterator playerValidMovesMapItr;
typedef map< int,set<string> >::iterator evalMovesMapItr;
typedef set<string>::iterator setStringsItr;

bool distributeStonesP1(int playerTurn,int& pitValue, int i, int j, int** board, int noOfPits, map<pair<int,int>, string >& positionMancalaMap, map<int, set<string> >& playerValidMovesMap);
bool distributeStonesP2(int playerTurn,int& pitValue, int i, int j, int** board, int noOfPits, map<pair<int,int>, string >& positionMancalaMap, map<int, set<string> >& playerValidMovesMap);
void displayBoard(int noOfPits, int** board, map<int, set<string> >& playerValidMovesMap);
int H_MinValue(int** board,int noOfPits,int cutOfDepth,int playerNo,int curDepth);

bool distributeStonesP1(int playerTurn,int& pitValue, int i, int j, int** board, int noOfPits, map<pair<int,int>, string >& positionMancalaMap, map<int, set<string> >& playerValidMovesMap)
{
	while( (j <= noOfPits+1) && (pitValue > 0) )
	{
		//test for mancala
		positionMancalaMapItr it = positionMancalaMap.find(make_pair(i,j));
		if(it != positionMancalaMap.end())
		{
			if((playerTurn==1)&&(it->second == "M1"))//player-1 adding to M1
			{
				--pitValue; board[i][j] = board[i][j] + 1;//add stone to next pit
				cout << " board[" << i << "][" << j << "] = " << board[i][j];
				if(pitValue == 0)//last stone added to mancala by owner of Mancala
					return true;//to get another chance
				j = j+1;continue;
			}
			if((playerTurn==2)&&(it->second == "M1"))//player-2 adding to M1
			{
				cout << " Player " << playerTurn << ", will skip adding stone to M1" << endl;
				return distributeStonesP2(playerTurn, pitValue,0,noOfPits,board,noOfPits, positionMancalaMap, playerValidMovesMap);
			}
		}
		--pitValue; board[i][j] = board[i][j] + 1;//add stone to next pit
		cout << " board[" << i << "][" << j << "] = " << board[i][j];
		if ((pitValue == 0) && (board[i][j]==1) && (playerTurn==1))//last stone added to a former empty pit owned by player
		{
			cout<< "\n Before Grab" << endl;
			displayBoard(noOfPits, board, playerValidMovesMap);
			cout << "\n last stone added to a former empty pit owned by player, hence grabbing stones across and last stone to mancala" << endl;
			//grab board[i=1][j] and board[0][j] into board[i=1][noOfPits+1]
			board[i][noOfPits+1] = board[i][noOfPits+1] + board[i][j] + board[0][j];
			board[i][j]=0;
			board[0][j]=0;
		}
		j = j+1;//move to next pit
		
	}
	if(pitValue > 0)//remaining stones add to other player's pits
		return distributeStonesP2(playerTurn, pitValue,0,noOfPits,board,noOfPits, positionMancalaMap, playerValidMovesMap);
	return false;
}

bool distributeStonesP2(int playerTurn,int& pitValue, int i, int j, int** board, int noOfPits, map<pair<int,int>, string >& positionMancalaMap, map<int, set<string> >& playerValidMovesMap)
{
	while( (j >= 0) && (pitValue > 0) )
	{	
		//test for mancala
		positionMancalaMapItr it = positionMancalaMap.find(make_pair(i,j));
		if(it != positionMancalaMap.end())
		{
			if((playerTurn==2)&&(it->second == "M2"))//player-2 adding to M2
			{
				--pitValue; board[i][j] = board[i][j] + 1;//add stone to next pit
				cout << " board[" << i << "][" << j << "] = " << board[i][j];
				if(pitValue == 0)//last stone added to mancala by owner of Mancala
					return true;//to get another chance
				j = j-1;continue;
			}
			if((playerTurn==1)&&(it->second == "M2"))//player-1 adding to M2
			{
				cout << " Player " << playerTurn << ", will skip adding stone to M2" << endl;
				return distributeStonesP1(playerTurn, pitValue,1,1,board,noOfPits, positionMancalaMap, playerValidMovesMap);
			}
		}
		--pitValue; board[i][j] = board[i][j] + 1;//add stone to next pit
		cout << " board[" << i << "][" << j << "] = " << board[i][j];
		if ((pitValue == 0) && (board[i][j]==1) && (playerTurn==2))//last stone added to a former empty pit owned by player
		{
			cout<< "\n Before Grab" << endl;
			displayBoard(noOfPits, board, playerValidMovesMap);
			cout << "\n last stone added to a former empty pit owned by player, hence grabbing stones across and last stone to mancala" << endl;
			//grab board[i=0][j] and board[1][j] into board[i=0][0]
			board[0][0] = board[0][0] + board[i][j] + board[1][j];
			board[i][j]=0;
			board[1][j]=0;
		}
		j = j-1;//move to next pit
	}
	if(pitValue > 0)
		return distributeStonesP1(playerTurn, pitValue,1,1,board,noOfPits, positionMancalaMap, playerValidMovesMap);
	return false;
}

bool pickPit(int playerTurn,string pitName, int noOfPits, int** board, map<string,pair<int,int> >& pitNamePositionMap, map<pair<int,int>, string >& positionMancalaMap, map<int, set<string> >& playerValidMovesMap)
{
	pitNamePositionMapItr it = pitNamePositionMap.find(pitName);
	if(it == pitNamePositionMap.end())
	{
		cout << "PitName : " << pitName << " not found!!!" << endl;
		return true;
	}
	int i = (it->second).first;
	int j = (it->second).second;
	int pitValue = board[i][j];
	cout << "PitName : " << pitName << " has : " << pitValue << " stones " << endl;
	if (pitValue == 0)
	{
		cout << "Invalid move!!!" << endl;
		return true;
	}
	board[i][j] = 0;//pick all stones
	if(i==1)//Player1
	{
		return distributeStonesP1(playerTurn,pitValue,i,j+1,board,noOfPits,positionMancalaMap, playerValidMovesMap);
	}
	if(i==0)//Player2
	{
		return distributeStonesP2(playerTurn,pitValue,i,j-1,board,noOfPits,positionMancalaMap, playerValidMovesMap);
	}
	return false;
}

void displayValidMoves(int playerNo, map<int, set<string> >& playerValidMovesMap)
{
	cout << "\nValid Moves for Player : " << playerNo << " are : ";
	playerValidMovesMapItr it;
	it = playerValidMovesMap.find(playerNo);
	if(it != playerValidMovesMap.end())
	{
		for(setStringsItr sItr = (it->second).begin(); sItr != (it->second).end(); sItr++)
			cout << " " << *sItr;
		cout << endl;
	}
	else
		cout<< "No Valid moves for Player : " << playerNo << endl;
		
}

bool validMove(string move, int playerNo, map<int, set<string> >& playerValidMovesMap)
{
	playerValidMovesMapItr it;
	it = playerValidMovesMap.find(playerNo);
	if(it != playerValidMovesMap.end())
	{
		for(setStringsItr sItr = (it->second).begin(); sItr != (it->second).end(); sItr++)
			if(*sItr==move)
				return true;
	}
	return false;	
}

void displayBoard(int noOfPits, int** board, map<int, set<string> >& playerValidMovesMap)
{
	cout << "\nBoard State : " << endl;
	cout << "P M ";
	set<string> moves = playerValidMovesMap[2];
	for(setStringsItr sItr = moves.begin(); sItr != moves.end(); sItr++)
			cout << "  " << *sItr;
	cout << endl;

	cout << 2 << " ";
	int i = 0;
	for(int j =0; j<(noOfPits+2); j++)
		if(board[i][j] == -1)
			cout << "* | ";
		else
			cout << board[i][j] << " | ";
	cout << endl;
	cout << 1 << " ";
	i = 1;
	for(int j =0; j<(noOfPits+2); j++)
		if(board[i][j] == -1)
			cout << "* | ";
		else
			cout << board[i][j] << " | ";
	cout << endl;

	cout << "P   ";
	moves = playerValidMovesMap[1];
	for(setStringsItr sItr = moves.begin(); sItr != moves.end(); sItr++)
			cout << "  " << *sItr;
	cout << "  M " << endl;
}

bool checkGameOver(int noOfPits,int** board,map<string,pair<int,int> >& pitNamePositionMap)
{
	int noOfEmptyPits[2] = {0,0};
	for(pitNamePositionMapItr it = pitNamePositionMap.begin(); it!= pitNamePositionMap.end(); it++)
	{
		int i = (it->second).first;
		int j = (it->second).second;
		if(board[i][j]==0)
			noOfEmptyPits[i]++;
		if(noOfEmptyPits[i] == noOfPits) 
			return true;
	}
	return false;
}

void copy(int** board, int** tempBoard,int noOfPits)
{
	for(int i = 0; i < 2; ++i)
		for(int j = 0; j < noOfPits+2; ++j)
			tempBoard[i][j] = board[i][j];
}

int Eval(int** board,int noOfPits,int playerTurn)
{
	if(playerTurn==1)
		return board[1][noOfPits+1] - board[0][0];
	else
		return board[0][0] - board[1][noOfPits+1];
}

int opponent(int playerNo)
{
	if(playerNo == 1)
		return 2;
	else //if(playerNo == 2)
		return 1;
}

int result(int playerTurn,string pitName, int noOfPits, int** board, map<string,pair<int,int> >& pitNamePositionMap, map<pair<int,int>, string >& positionMancalaMap, map<int, set<string> >& playerValidMovesMap)
{
	bool anotherChance=false;
	do
	{
		anotherChance = pickPit(playerTurn,pitName, noOfPits, board, pitNamePositionMap, positionMancalaMap, playerValidMovesMap);
		cout << "\nAfter " << pitName << " : ";
		displayBoard(noOfPits, board, playerValidMovesMap);
		if(anotherChance == true)
		{
			cout <<"Player : " << playerTurn << " got another chance!!!!!!!!!!!!" << endl;
			anotherChance=false;
			cout<<"But we are forcing him to stop" << endl;
		}
	}while(anotherChance);
	return Eval(board,noOfPits,playerTurn);
}

string H_MiniMax(int** board, map<string,pair<int,int> >& pitNamePositionMap, map<pair<int,int>, string >& positionMancalaMap,map<int, set<string> >& playerValidMovesMap, int noOfPits,int cutOfDepth,int playerNo,int curDepth)
{
	string action="NULL";
	displayValidMoves(playerNo,playerValidMovesMap);
	int maxEval = 0;
	map< int,set<string> > evalMovesMap;
	playerValidMovesMapItr it;
	it = playerValidMovesMap.find(playerNo);
	if(it != playerValidMovesMap.end())
	{
		cout << "*************************************************************************" << endl;
		for(setStringsItr sItr = (it->second).begin(); sItr != (it->second).end(); sItr++)
		{
			
			//cout << "Action : " << *sItr << endl;
			int** tempBoard = new int*[2];
			for(int i = 0; i < 2; ++i)
				tempBoard[i] = new int[noOfPits+2];
			copy(board,tempBoard,noOfPits);
			//cout << "Current Board : " << endl;
			//displayBoard(noOfPits,board,playerValidMovesMap);
			int val = result(playerNo,*sItr, noOfPits, tempBoard, pitNamePositionMap, positionMancalaMap, playerValidMovesMap);
			cout << "FINAL Temp Board after Action : " << *sItr << " fetched Eval = " << val << endl;
			evalMovesMap[val].insert(*sItr);
			if(val>maxEval)
				maxEval = val;
			displayBoard(noOfPits,tempBoard,playerValidMovesMap);
			//H_MinValue(board, noOfPits,cutOfDepth,opponent(playerNo),curDepth+1);
			cout << "*************************************************************************" << endl;
		}
	}
	//evalMovesMap
	cout << "*************************************************************************" << endl;
	cout << "evalMovesMap : " << endl;
	for(evalMovesMapItr it = evalMovesMap.begin();it != evalMovesMap.end(); it++)
	{
		cout << it->first << " : ";
		for(setStringsItr sItr = (it->second).begin(); sItr != (it->second).end(); sItr++)
			cout << " " << *sItr;
		cout << endl;
	}
	
	cout << "For Max Eval : " << maxEval << ", we have moves :";
	evalMovesMapItr itr = evalMovesMap.find(maxEval);
	if(itr != evalMovesMap.end())
	{
		for(setStringsItr sItr = (itr->second).begin(); sItr != (itr->second).end(); sItr++)
			cout << " " << *sItr;
		cout << endl;
	}
	else
		cout<< "No Valid moves for Max Eval : " << maxEval << endl;
	action = *((itr->second).begin());
	cout << "Hence returning action : " << action << endl;
	return action;
}



int H_MinValue(int** board,int noOfPits,int cutOfDepth,int playerNo,int curDepth)
{
	/*if(curDepth == cutOfDepth)
		return Eval(board,noOfPits);
	int v = 99999;*/
}

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
	
	map<string,pair<int,int> > pitNamePositionMap;
	map<pair<int,int>, string > positionMancalaMap;
	map<int, set<string> > playerValidMovesMap;
	
	
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
		ostringstream convert;
		convert << i+1;
		string key = string("A")+convert.str();
		playerValidMovesMap[2].insert(key);
		pitNamePositionMap[key] = make_pair(0,i);
		board[0][i++] = atoi((pits1.front()).c_str());
		pits1.pop();
	}
	positionMancalaMap[make_pair(0,0)] = "M2";
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
		ostringstream convert;
		convert << i+1;
		string key = string("B")+convert.str();
		playerValidMovesMap[1].insert(key);
		pitNamePositionMap[key] = make_pair(1,i);
		board[1][i++] = atoi((pits2.front()).c_str());
		pits2.pop();
	}
	positionMancalaMap[make_pair(1,noOfPits+1)] = "M1";
	
	
	int mancala = 0;
	////////////////////////////<#stones in player-2’s mancala>////////////////////////////////////
	getline(ifs, line);
	mancala = atoi(line.c_str());
	cout << "#stones in player-2 mancala : " << mancala << endl;
	board[0][0] = mancala;board[1][0] = -1;
	////////////////////////////<#stones in player-2’s mancala>////////////////////////////////////
	getline(ifs, line);
	mancala = atoi(line.c_str());
	cout << "#stones in player-1 mancala : " << mancala << endl;
	board[0][noOfPits+1] = -1;board[1][noOfPits+1] = mancala;
	
	displayBoard(noOfPits, board, playerValidMovesMap);

	/*cout << "\npitNamePositionMap : " << endl;
	for(pitNamePositionMapItr it = pitNamePositionMap.begin(); it!= pitNamePositionMap.end(); it++)
	{
		int i = (it->second).first;
		int j = (it->second).second;
		cout << it->first << " : board(" << i << ", " << j << ") = " << board[i][j]<< endl;
	}*/
	
	/*displayValidMoves(1,playerValidMovesMap);
	displayValidMoves(2,playerValidMovesMap);
	displayValidMoves(3,playerValidMovesMap);*/

	H_MiniMax(board,pitNamePositionMap, positionMancalaMap,playerValidMovesMap,noOfPits,cutOfDepth,playerNo,0);
	return 0;
	
	
	//INTERACTIVE MODE
	{
	bool player1Turn = true;
	while( !checkGameOver(noOfPits, board, pitNamePositionMap) )
	{
		if(player1Turn)
		{
			displayValidMoves(1,playerValidMovesMap);
			cout << "Enter Player 1 move : ";
			string move;
			cin >> move;
			if(validMove(move,1,playerValidMovesMap))
			{
				bool anotherChance=false;
				anotherChance = pickPit(1,move, noOfPits, board, pitNamePositionMap, positionMancalaMap, playerValidMovesMap);
				cout << "\nAfter " << move << " : ";
				displayBoard(noOfPits, board, playerValidMovesMap);
				if(anotherChance == true)
				{
					player1Turn=true;
					cout <<"Player : 1 got another chance!!!!!!!!!!!!" << endl;
				}
				else
					player1Turn=false;//give chance to player2
			}
		}
		else
		{
			displayValidMoves(2,playerValidMovesMap);
			cout << "Enter Player 2 move : ";
			string move;
			cin >> move;
			if(validMove(move,2,playerValidMovesMap))
			{
				bool anotherChance=false;
				anotherChance = pickPit(2,move, noOfPits, board, pitNamePositionMap, positionMancalaMap, playerValidMovesMap);
				cout << "\nAfter " << move << " : ";
				displayBoard(noOfPits, board, playerValidMovesMap);
				if(anotherChance == true)
				{
					player1Turn=false;
					cout <<"Player : 2 got another chance!!!!!!!!!!!!" << endl;
				}
				else
					player1Turn=true;//give chance to player1
			}
		}
	}
	cout << "GAME OVER" << endl;
	//move all stones from pits to respective mancalas
	for(pitNamePositionMapItr it = pitNamePositionMap.begin(); it!= pitNamePositionMap.end(); it++)
	{
		int i = (it->second).first;
		int j = (it->second).second;
		if(board[i][j]!=0)
		{
			if(i==0)
			{
				board[0][0] += board[i][j];//move stones to mancala
			}
			if(i==1)
			{
				board[1][noOfPits+1] += board[i][j];//move stones to mancala
			}
			board[i][j]=0;
		}
	}	
	displayBoard(noOfPits, board, playerValidMovesMap);
	if(board[0][0] > board[1][noOfPits+1] )
		cout << "Player 2 is WINNER!!!" << endl;
	else if (board[0][0] < board[1][noOfPits+1] )
		cout << "Player 1 is WINNER!!!" << endl;
	else
		cout << "Its a DRAW...." << endl;
	}
	
	/*bool anotherChance;
	anotherChance = pickPit(playerNo,"B3", noOfPits, board, pitNamePositionMap, positionMancalaMap);
	cout << "After B3: ";
	displayBoard(noOfPits, board);
	if(anotherChance == true)
		cout <<"Player : " << playerNo << " got another chance!!!!!!!!!!!!" << endl;
	
	anotherChance = pickPit(playerNo,"B3", noOfPits, board, pitNamePositionMap, positionMancalaMap);
	cout << "After B3: ";
	displayBoard(noOfPits, board);
	if(anotherChance == true)
		cout <<"Player : " << playerNo << " got another chance!!!!!!!!!!!!" << endl;
	
	anotherChance = pickPit(playerNo,"B2", noOfPits, board, pitNamePositionMap, positionMancalaMap);
	cout << "After B2: ";
	displayBoard(noOfPits, board);
	if(anotherChance == true)
		cout <<"Player : " << playerNo << " got another chance!!!!!!!!!!!!" << endl;
	
	anotherChance = pickPit(playerNo,"A4", noOfPits, board, pitNamePositionMap, positionMancalaMap);
	cout << "After A4: ";
	displayBoard(noOfPits, board);
	if(anotherChance == true)
		cout <<"Player : " << playerNo << " got another chance!!!!!!!!!!!!" << endl;
	
	
	anotherChance = pickPit(playerNo,"B3", noOfPits, board, pitNamePositionMap, positionMancalaMap);
	cout << "After B3: ";
	displayBoard(noOfPits, board);
	if(anotherChance == true)
		cout <<"Player : " << playerNo << " got another chance!!!!!!!!!!!!" << endl;

	anotherChance = pickPit(playerNo,"A3", noOfPits, board, pitNamePositionMap, positionMancalaMap);
	cout << "After A3: ";
	displayBoard(noOfPits, board);
	if(anotherChance == true)
		cout <<"Player : " << playerNo << " got another chance!!!!!!!!!!!!" << endl;
	
	anotherChance = pickPit(playerNo,"B4", noOfPits, board, pitNamePositionMap, positionMancalaMap);
	cout << "After B4: ";
	displayBoard(noOfPits, board);
	if(anotherChance == true)
		cout <<"Player : " << playerNo << " got another chance!!!!!!!!!!!!" << endl;
	*/
	
    ifs.close();
		
	
	return 0;
}

