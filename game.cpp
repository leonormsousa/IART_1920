#include <iostream> 
#include <limits>
#include <vector> 
#include <map>
#include <string>
#include <queue>
#include <algorithm>
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
        Move(){}
        Move(char group, int direction){
            this->group=group;
            this->direction=direction;
        }
        char getGroup(){return group;}
        int getDirection(){return direction;}
        void display(){
            cout << "Group: " << group << " | Direction: " ;
            switch(direction){
                case UP:
                    cout << "Up\n";
                    break;
                case DOWN:
                    cout << "Down\n";
                    break;
                case LEFT:
                    cout << "Left\n";
                    break;
                case RIGHT:
                    cout << "Right\n";
                    break;
            }
        }
};

//--------------------------------------------------------------------------------------------------------//
//--------------------------------------------  LEVEL ----------------------------------------------------//
//--------------------------------------------------------------------------------------------------------//
class Level{
    private:
        int number;
        vector<vector<char> > board;
        map<char, pair<int, string> > groups;
        int cost;
        int find_jmax(char group);
        int find_jmin(char group);
        int find_imax(char group);
        int find_imin(char group);
    public:
        Level(){}
        Level(int number, vector< vector<char> > board);
        bool analise_move(Move move);
        void make_move(Move move);
        bool solved();
        void display();
        vector<Move> possible_moves();
        bool group_exists(char group){
            if (groups.find(group) == groups.end())
                return false;
            return true;
        }
        void display_group_colors(){
            for (auto& x: groups) {
                if ((x.first!='_') && (x.first!='0'))
                    cout << "Group " << x.first << ": " << x.second.second << "  " << reset_color << "\n";
            }
        }
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

vector<Move> Level::possible_moves(){
    vector<Move> moves;
    for (auto& x: groups) {
        if ((x.first != '0') && (x.first != '_')){
            Move move_up(x.first, UP);
            if (analise_move(move_up))
                moves.push_back(move_up);
            Move move_down(x.first, DOWN);
            if (analise_move(move_down))
                moves.push_back(move_down);
            Move move_left(x.first, LEFT);
            if (analise_move(move_left))
                moves.push_back(move_left);
            Move move_right(x.first, RIGHT);
            if (analise_move(move_right))
                moves.push_back(move_right);
        }
    }
    return moves;
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
        Node(Level state, Node* father, Move move, int depth);
};

Node::Node(Level state, Node* father, Move move, int depth){
    this->state=state;
    this->father=father;
    this->move=move;
    this->depth=depth;
}

class SearchTree{
    private:
        vector<Node> nodes;
        int index=-1;
    public:    
        SearchTree(Node root){
            nodes.push_back(root);
            index=0;
        }
        Node uniform_cost();
        Node depth();
        Node depth_iterative();
        Node greedy();
};

Node SearchTree::uniform_cost(){
    Node node = nodes[index];
    if (node.state.solved())
        return node;
    vector<Move> moves = node.state.possible_moves();
    for (int i=0; i<moves.size(); i++){
        Level new_level=node.state;
        new_level.make_move(moves[i]);
        Node new_node(new_level, &nodes[index], moves[i], node.depth+1);
        nodes.push_back(new_node);
    }
    index++;
    return uniform_cost();
}

//--------------------------------------------------------------------------------------------------------//
//----------------------------------------  FoldingBlocks ------------------------------------------------//
//--------------------------------------------------------------------------------------------------------//
class FoldingBlocks{
    private:
        vector<Level> levels;
    public:
        FoldingBlocks();
        void print_level(int level){ levels[level].display();}
        void print_possible_moves(int level){
            vector<Move> moves=levels[level].possible_moves();
            for (int i=0; i<moves.size(); i++)
                moves[i].display();
        }
        vector<Move> solve(Level level);
        void play_bot(int mode, int level);
        void play_human(int level);
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
    Move m('0', 0);
    Node first(level, NULL, m, 0);
    SearchTree st(first);
    Node final=st.uniform_cost();
    vector<Move> moves;
    while(final.father!=NULL){
        moves.push_back(final.move);
        final=*(final.father);
    }
    reverse(moves.begin(), moves.end());
    return moves;
}

void FoldingBlocks::play_bot(int mode, int level){
    Level current_level = levels[level];
    vector<Move> moves = solve(current_level);
    current_level.display();
    cout << "\n";
    if (moves.empty())
        cout << "No solution is possible for level " << level << '\n';
    else{
        for (int i=0; i<moves.size(); i++){
                current_level.make_move(moves[i]);
                moves[i].display();
                current_level.display();
                cout << "\n";
        }
        cout << "Level Solved with success.\nFinal Cost: " << moves.size() << "\n"; 
    }
}

void FoldingBlocks::play_human(int level){
    Level current_level = levels[level];
    int cost=0;
    bool out=false;
    while(!out){
        //display board
        current_level.display();
        cout << "Number of Moves: " << cost << "\n\n";

        //group input
        current_level.display_group_colors();
        cout << "To Quit insert Q.\nNext Move?\n" << "Group: ";
        char group;
        cin >> group;
        while(1){
            if(!cin.fail() && (current_level.group_exists(group) || (group=='Q')))
                break;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            cout << "\nYou have entered wrong input" << endl;
            current_level.display_group_colors();
            cout << "To Quit insert Q.\nNext Move?\n" << "Group: ";
            cin >> group;
        }
        
        if (group=='Q')
            break;

        //direction input
        cout << "\nDOWN: 1 \nUP: 2\nLEFT: 3\nRIGHT : 4\n";
        cout << "Direction: ";
        int direction;
        cin >> direction;
        while(1){
            if(!cin.fail() && (direction>=1) && (direction<=4))
                break;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            cout << "\nYou have entered wrong input" << endl;
            cout << "DOWN: 1 \nUP: 2\nLEFT: 3\nRIGHT : 4\n";
            cout << "Direction: ";
            cin >> direction;
        }
        cout << "\n";

        //analise move
        Move move(group, direction);
        if (!current_level.analise_move(move))
            cout << "Move not possible.\n\n";
        else{
            current_level.make_move(move);
            cost++;
        }

        if (current_level.solved()){
            out=true;
            cout << "Congratulations! You have solved this level!\n";
            current_level.display();
            cout << "Final Number of Moves: " << cost << "\n\n";
        }
    }
}

int main (int argc, char *argv[]){
    if ((argc != 3) || (atoi(argv[1]) <= 0) || (atoi(argv[1]) > 3) || (atoi(argv[2]) <= 0) || (atoi(argv[2]) > 7))
    {
        cout << "Usage: ./game <option> <level>\n"
             << "where option must be: 1(human player), 2(uniform-cost solver), 3(depth-first solver) ...\n"
             << "where level must be: an integer between 1 and 7\n\n";
        return -1;
    }
    
    int level=atoi(argv[2])-1;
    int mode=atoi(argv[1]);


    FoldingBlocks foldingBlocks;
    if (mode==1)
        foldingBlocks.play_human(level);
    else
        foldingBlocks.play_bot(mode, level);
}