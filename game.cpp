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

//--------------------------------------------------------------------------------------------------------//
//--------------------------------------------  MOVE -----------------------------------------------------//
//--------------------------------------------------------------------------------------------------------//
class Move{
    private:
        int direction;
        char group;
    public:
        char getGroup(){return group;}
        int getDirection(){return direction;}
        void display(){
            cout << "Direction: " << direction << " | Group: " << group << "\n";
        }
};

//--------------------------------------------------------------------------------------------------------//
//--------------------------------------------  LEVEL ----------------------------------------------------//
//--------------------------------------------------------------------------------------------------------//
class Level{
    private:
        int number;
        vector< vector<char> > board;
        map<char, pair<int, string> > groups;
        int cost;
        int find_jmax(char group);
        int find_jmin(char group);
        int find_imax(char group);
        int find_imin(char group);
    public:
        Level(int number, vector< vector<char> > board);
        bool analise_move(Move move);
        void make_move(Move move);
        bool solved();
        void display();
};

Level::Level(int number, vector< vector<char> > board){
    this->number=number;
    this->board=board;
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

int Level::find_imax(char group){
    int imax=-1;
    for (int line=0; line<board.size(); line++){
        for (int column=0; column<board[line].size(); column++){
            if (board[line][column] == group){
                if (line>imax)
                    imax=line;
            }
        }
    }
    return imax;
}

int Level::find_jmax(char group){
    int jmax=-1;
    for (int line=0; line<board.size(); line++){
        for (int column=0; column<board[line].size(); column++){
            if (board[line][column] == group){
                if (column>jmax)
                    jmax=column;
            }
        }
    }
    return jmax;
}

int Level::find_imin(char group){
    int imin=board.size();
    for (int line=0; line<board.size(); line++){
        for (int column=0; column<board[line].size(); column++){
            if (board[line][column] == group){
                if (line<imin)
                    imin=line;
            }
        }
    }
    return imin;
}

int Level::find_jmin(char group){
    int jmin=board[0].size();
    for (int line=0; line<board.size(); line++){
        for (int column=0; column<board[line].size(); column++){
            if (board[line][column] == group){
                if (column<jmin)
                    jmin=column;
            }
        }
    }
    return jmin;
}

bool Level::analise_move(Move move){
    switch(move.getDirection()){
        case DOWN:{
            //find imax
            int imax=find_imax(move.getGroup());
            if (imax==-1)
                return false;
            //verify if move is possible
            for (int line=0; line<board.size(); line++){
                for (int column=0; column<board[line].size(); column++){
                    if (board[line][column] == move.getGroup()){
                        if (2*imax-line+1>=board.size())
                            return false;
                        if (board[2*imax-line+1][column]!='0')
                            return false;
                    }
                }
            }
            return true;
        }
        case UP:{
            //find imin
            int imin=find_imin(move.getGroup());
            if (imin==board.size())
                return false;
            //verify if move is possible
            for (int line=0; line<board.size(); line++){
                for (int column=0; column<board[line].size(); column++){
                    if (board[line][column] == move.getGroup()){
                        if (2*imin-line-1<0)
                            return false;
                        if (board[2*imin-line-1][column]!='0')
                            return false;
                    }
                }
            }
            return true;
        }
        case RIGHT:{
            //find jmax
            int jmax=find_jmax(move.getGroup());
            if (jmax==-1)
                return false;
            //verify if move is possible
            for (int line=0; line<board.size(); line++){
                for (int column=0; column<board[line].size(); column++){
                    if (board[line][column] == move.getGroup()){
                        if (2*jmax-column+1>=board[0].size())
                            return false;
                        if (board[line][2*jmax-column+1]!='0')
                            return false;
                    }
                }
            }
            return true;
        }
        case LEFT:{
            //find jmin
            int jmin=find_jmin(move.getGroup());
            if (jmin==board[0].size())
                return false;
            //verify if move is possible
            for (int line=0; line<board.size(); line++){
                for (int column=0; column<board[line].size(); column++){
                    if (board[line][column] == move.getGroup()){
                        if (2*jmin-column-1>=board[0].size())
                            return false;
                        if (board[line][2*jmin-column-1]!='0')
                            return false;
                    }
                }
            }
            return true;
        }
        default:
            return false;
    } 
}

void Level::make_move(Move move){
    switch(move.getDirection()){
        case DOWN:{
            //find imax
            int imax=find_imax(move.getGroup());
            //make move
            for (int line=0; line<board.size(); line++){
                for (int column=0; column<board[line].size(); column++){
                    if (board[line][column] == move.getGroup()){
                        board[2*imax-line+1][column]=move.getGroup();
                    }
                }
            }
            break;
        }
        case UP:{
            //find imin
            int imin=find_imin(move.getGroup());
            //make move
            for (int line=0; line<board.size(); line++){
                for (int column=0; column<board[line].size(); column++){
                    if (board[line][column] == move.getGroup()){
                        board[2*imin-line-1][column]=move.getGroup();
                    }
                }
            }
            break;
        }
        case RIGHT:{
            //find jmax
            int jmax=find_jmax(move.getGroup());
            //make move
            for (int line=0; line<board.size(); line++){
                for (int column=0; column<board[line].size(); column++){
                    if (board[line][column] == move.getGroup()){
                        board[line][2*jmax-column+1]=move.getGroup();
                    }
                }
            }
            break;
        }
        case LEFT:{
            //find jmin
            int jmin=find_jmin(move.getGroup());
            //make move
            for (int line=0; line<board.size(); line++){
                for (int column=0; column<board[line].size(); column++){
                    if (board[line][column] == move.getGroup()){
                        board[line][2*jmin-column-1]=move.getGroup();
                    }
                }
            }
            break;
        }
    } 
    this->cost++;
}

bool Level::solved(){
    for (int line=0; line<board.size(); line++){
        for (int column=0; column<board[line].size(); column++){
            if (board[line][column] == '0')
                return false;
        }
    }
    return true;
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

//--------------------------------------------------------------------------------------------------------//
//--------------------------------------------  NODE -----------------------------------------------------//
//--------------------------------------------------------------------------------------------------------//
class Node{
    public:
        Level state;
        Node* father;
        Move move;
        int depth;
        int cost;
};

class SearchTree{
    public:
        vector<Node> nodes;
};

//--------------------------------------------------------------------------------------------------------//
//----------------------------------------  FoldingBlocks ------------------------------------------------//
//--------------------------------------------------------------------------------------------------------//
class FoldingBlocks{
    private:
        vector<Level> levels;
    public:
        FoldingBlocks();
        void print(int level){ levels[level].display();}
        void run_level(int level);
        vector<Move> solve(Level level);
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

vector<Move> FoldingBlocks::solve(Level level){
    /////////////////////////////THIS>////////////////////////777777777777////////////7777777777777777777777777/////////////////////////////////////777
}

void FoldingBlocks::run_level(int level){
    Level current_level = levels[level];
    vector<Move> moves = solve(current_level);
    current_level.display();
    if (moves.empty())
        cout << "No solution is possible for level " << level << '\n';
    else{
        for (int i=0; i<moves.size(); i++){
                current_level.make_move(moves[i]);
                moves[i].display();
                current_level.display();
        }
        cout << "Level Solved with success.\nFinal Cost: " << moves.size() << "\n"; 
    }
}

int main(){
    FoldingBlocks foldingBlocks;
    foldingBlocks.print(1);
}