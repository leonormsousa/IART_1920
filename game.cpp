#include <iostream> 
#include <vector> 
using namespace std; 

const int DOWN = 1;
const int UP = 2;
const int LEFT = 3;
const int RIGHT = 4;

class Level{
    private:
        vector< vector<char> > initial_state;
        vector< vector<char> > board;
        int number;
        vector<char> colours;
        int cost;
    public:
        Level(int number, vector< vector<char> > board){
            this->number=number;
            this->board=board;
            this->initial_state=board;
            this->cost=0;
        };
        int analise_move(char group, int direction);
        void move(char group, int direction, int limit);
};

int Level::analise_move(char group, int direction){
    switch(direction){
        case DOWN:{
            //find imax
            int imax=-1;
            for (int line=0; line<board.size(); line++){
                for (int column=0; column<board[line].size(); column++){
                    if (board[line][column] == group){
                        if (line>imax)
                            imax=line;
                    }
                }
            }
            if (imax==-1)
                return -1;
            //verify if move is possible
            for (int line=0; line<board.size(); line++){
                for (int column=0; column<board[line].size(); column++){
                    if (board[line][column] == group){
                        if (2*imax-line+1>=board.size())
                            return -1;
                        if (board[2*imax-line+1][column]!='0')
                            return -1;
                    }
                }
            }
            return imax;
        }
        case UP:{
            //find imin
            int imin=board.size();
            for (int line=0; line<board.size(); line++){
                for (int column=0; column<board[line].size(); column++){
                    if (board[line][column] == group){
                        if (line<imin)
                            imin=line;
                    }
                }
            }
            if (imin==board.size())
                return -1;
            //verify if move is possible
            for (int line=0; line<board.size(); line++){
                for (int column=0; column<board[line].size(); column++){
                    if (board[line][column] == group){
                        if (2*imin-line-1<0)
                            return -1;
                        if (board[2*imin-line-1][column]!='0')
                            return -1;
                    }
                }
            }
            return imin;
        }
        case RIGHT:{
            //find jmax
            int jmax=-1;
            for (int line=0; line<board.size(); line++){
                for (int column=0; column<board[line].size(); column++){
                    if (board[line][column] == group){
                        if (column>jmax)
                            jmax=column;
                    }
                }
            }
            if (jmax==-1)
                return -1;
            //verify if move is possible
            for (int line=0; line<board.size(); line++){
                for (int column=0; column<board[line].size(); column++){
                    if (board[line][column] == group){
                        if (2*jmax-column+1>=board[0].size())
                            return -1;
                        if (board[line][2*jmax-column+1]!='0')
                            return -1;
                    }
                }
            }
            return jmax;
        }
        case LEFT:{
            //find jmin
            int jmin=board[0].size();
            for (int line=0; line<board.size(); line++){
                for (int column=0; column<board[line].size(); column++){
                    if (board[line][column] == group){
                        if (column<jmin)
                            jmin=column;
                    }
                }
            }
            if (jmin==board[0].size())
                return -1;
            //verify if move is possible
            for (int line=0; line<board.size(); line++){
                for (int column=0; column<board[line].size(); column++){
                    if (board[line][column] == group){
                        if (2*jmin-column-1>=board[0].size())
                            return -1;
                        if (board[line][2*jmin-column-1]!='0')
                            return -1;
                    }
                }
            }
            return jmin;
        }
        default:
            return -1;
    } 
}

void Level::move(char group, int direction, int limit){
    switch(direction){
        case DOWN:{
            //find imax
            int imax=limit;
            //make move
            for (int line=0; line<board.size(); line++){
                for (int column=0; column<board[line].size(); column++){
                    if (board[line][column] == group){
                        board[2*imax-line+1][column]=group;
                    }
                }
            }
            break;
        }
        case UP:{
            //find imin
            int imin=limit;
            //make move
            for (int line=0; line<board.size(); line++){
                for (int column=0; column<board[line].size(); column++){
                    if (board[line][column] == group){
                        board[2*imin-line-1][column]=group;
                    }
                }
            }
            break;
        }
        case RIGHT:{
            //find jmax
            int jmax=limit;
            //make move
            for (int line=0; line<board.size(); line++){
                for (int column=0; column<board[line].size(); column++){
                    if (board[line][column] == group){
                        board[line][2*jmax-column+1]=group;
                    }
                }
            }
            break;
        }
        case LEFT:{
            //find jmin
            int jmin=limit;
            //make move
            for (int line=0; line<board.size(); line++){
                for (int column=0; column<board[line].size(); column++){
                    if (board[line][column] == group){
                        board[line][2*jmin-column-1]=group;
                    }
                }
            }
            break;
        }
    } 
    this->cost++;
}


class FoldingBlocks{
    private:
        vector<Level> levels;
    public:
        FoldingBlocks();
        void print(){ cout<<"Hello";}
};

FoldingBlocks::FoldingBlocks(){
    vector< vector<char> > level1 = {{'A', '0'}, {'0', '0'}};
    vector< vector<char> > level2 = {{'_', '0', '0', '_'}, {'0', '0', '0', '0'}, {'A', 'A', '0', '0'}, {'_', 'A', '0', '_'}};
    vector< vector<char> > level3 = {{'A', '0', '0', '0'}, {'_', '_', '_', '_'}, {'A', '0', '0', '0'}};
    vector< vector<char> > level4 = {{'A', '0', '_', '_'}, {'0', '0', '0', '0'}, {'0', '0', '0', '0'}, {'0', '0', '0', '0'}, {'0', '0', '0', '0'}, {'_', '_', 'B', '0'}};
    vector< vector<char> > level5 = {{'0', '0', '0', '0'}, {'0', '0', '0', '0'}, {'0', '0', '0', '0'}, {'0', '0', '0', 'A'}, {'0', '0', '0', 'B'}, {'0', '0', '0', '0'}, {'0', '0', '0', '0'}, {'0', '0', '0', '0'}};
    vector< vector<char> > level6 = {{'0', '0', '0', '0'}, {'0', '0', '0', 'A'}, {'0', '0', '0', 'B'}, {'0', '0', '0', 'C'}, {'0', '0', '0', '0'}, {'0', '0', '0', '0'}, {'0', '0', '0', '0'}};
    levels.push_back(Level(1, level1));
    levels.push_back(Level(2, level2));
    levels.push_back(Level(3, level3));
    levels.push_back(Level(4, level4));
    levels.push_back(Level(5, level5));
    levels.push_back(Level(6, level6));
}

int main(){
    FoldingBlocks foldingBlocks;
    foldingBlocks.print();
}