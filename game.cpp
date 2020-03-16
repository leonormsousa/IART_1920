#include <iostream> 
#include <vector> 
#include <map>
#include <string>
using namespace std; 

const int DOWN = 1;
const int UP = 2;
const int LEFT = 3;
const int RIGHT = 4;
const string reset_color = "\033[0m";
const string empty_color = "\33[0;30;40m";
const string grey_color = "\33[0;97;107m";
const vector<string> colors = {"\33[0;31;41m", "\33[0;32;42m", "\33[0;33;43m", "\33[0;34;44m", "\33[0;35;45m", "\33[0;36;46m", "\33[0;37;47m"};

class Level{
    private:
        vector< vector<char> > initial_state;
        vector< vector<char> > board;
        int number;
        map<char, pair<int, string> > groups;
        int cost;
    public:
        Level(int number, vector< vector<char> > board);
        int analise_move(char group, int direction);
        void move(char group, int direction, int limit);
        void display();
};

Level::Level(int number, vector< vector<char> > board){
    this->number=number;
    this->board=board;
    this->initial_state=board;
    this->cost=0;
    int color_counter=0;
    for (int line=0; line<board.size(); line++){
        for (int column=0; column<board[line].size(); column++){
            if (groups.find(board[line][column]) == groups.end()){
                if (board[line][column]=='_')
                    groups['_'] = pair<int, string> (1, empty_color);
                else if (board[line][column]=='0')
                    groups['0'] = pair<int, string> (1, grey_color);
                else{
                    groups[board[line][column]] = pair<int, string> (1, colors[color_counter]);
                    color_counter++;
                }
            }
            else
            {
                pair<int, string> previous_pair=groups.at(board[line][column]);
                groups[board[line][column]] = pair<int, string> (previous_pair.first+1, previous_pair.second);
            }
            
        }
    }
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

void Level::display(){
    cout << "  ";
    for (int column=0; column<board[0].size(); column++){
        cout << column +1 << " ";
    }
    cout << "\n";
    for (int line=0; line<board.size(); line++){
        cout << line +1 << " ";
        for (int column=0; column<board[line].size(); column++){
            string color=groups.at(board[line][column]).second;
            cout << color << board[line][column] << " " << reset_color;
        }
        cout << '\n';
    }
}


class FoldingBlocks{
    private:
        vector<Level> levels;
    public:
        FoldingBlocks();
        void print(){ levels[3].display();}
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