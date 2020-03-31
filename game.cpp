#include <iostream> 
#include <limits>
#include <vector> 
#include <map>
#include <string>
#include <list>
#include <algorithm>
using namespace std;

const int DOWN = 1;
const int UP = 2;
const int LEFT = 3;
const int RIGHT = 4;
const string reset_color = "\033[0m";
const string empty_color = "\33[0;30;40m";
const string grey_color = "\33[0;97;107m";
const vector<string> colors = { "\33[0;31;41m", "\33[0;32;42m", "\33[0;33;43m", "\33[0;34;44m", "\33[0;35;45m", "\33[0;36;46m", "\33[0;37;47m" };

//--------------------------------------------------------------------------------------------------------//
//--------------------------------------------  MOVE -----------------------------------------------------//
//--------------------------------------------------------------------------------------------------------//
class Move {
private:
	int direction;
	char group;
public:
	Move() {}
	Move(char group, int direction) {
		this->group = group;
		this->direction = direction;
	}
	char getGroup() { return group; }
	int getDirection() { return direction; }
	void display() {
		cout << "Group: " << group << " | Direction: ";
		switch (direction) {
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
class Level {
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
	Level() {}
	Level(int number, vector< vector<char> > board);
	bool analise_move(Move move);
	void make_move(Move move);
	bool solved();
	void display();
	vector<vector<char> > getBoard() { return board; }
	map<char, pair<int, string> > getGroups() { return groups; }
	vector<Move> possible_moves();
	bool group_exists(char group) {
        return !(groups.find(group) == groups.end());
    }
	void display_group_colors() {
		for (auto& x : groups) {
			if ((x.first != '_') && (x.first != '0'))
				cout << "Group " << x.first << ": " << x.second.second << "  " << reset_color << "\n";
		}
	}

    int getValue();
};

Level::Level(int number, vector< vector<char> > board) {
	this->number = number;
	this->board = board;
	this->cost = 0;
	int color_counter = 0;
	for (int line = 0; line < board.size(); line++) {
		for (int column = 0; column < board[line].size(); column++) {
			if (groups.find(board[line][column]) == groups.end()) {
				if (board[line][column] == '_')
					groups['_'] = pair<int, string>(1, empty_color);
				else if (board[line][column] == '0')
					groups['0'] = pair<int, string>(1, grey_color);
				else {
					groups[board[line][column]] = pair<int, string>(1, colors[color_counter]);
					color_counter++;
				}
			}
			else
			{
				pair<int, string> previous_pair = groups.at(board[line][column]);
				groups[board[line][column]] = pair<int, string>(previous_pair.first + 1, previous_pair.second);
			}

		}
	}
};

int Level::find_imax(char group) {
	int imax = -1;
	for (int line = 0; line < board.size(); line++) {
		for (int column = 0; column < board[line].size(); column++) {
			if (board[line][column] == group) {
				if (line > imax)
					imax = line;
			}
		}
	}
	return imax;
}

int Level::find_jmax(char group) {
	int jmax = -1;
	for (int line = 0; line < board.size(); line++) {
		for (int column = 0; column < board[line].size(); column++) {
			if (board[line][column] == group) {
				if (column > jmax)
					jmax = column;
			}
		}
	}
	return jmax;
}

int Level::find_imin(char group) {
	int imin = board.size();
	for (int line = 0; line < board.size(); line++) {
		for (int column = 0; column < board[line].size(); column++) {
			if (board[line][column] == group) {
				if (line < imin)
					imin = line;
			}
		}
	}
	return imin;
}

int Level::find_jmin(char group) {
	int jmin = board[0].size();
	for (int line = 0; line < board.size(); line++) {
		for (int column = 0; column < board[line].size(); column++) {
			if (board[line][column] == group) {
				if (column < jmin)
					jmin = column;
			}
		}
	}
	return jmin;
}

bool Level::analise_move(Move move) {
	switch (move.getDirection()) {
	case DOWN: {
		//find imax
		int imax = find_imax(move.getGroup());
		if (imax == -1)
			return false;
		//verify if move is possible
		for (int line = 0; line < board.size(); line++) {
			for (int column = 0; column < board[line].size(); column++) {
				if (board[line][column] == move.getGroup()) {
					if (2 * imax - line + 1 >= board.size())
						return false;
					if (board[2 * imax - line + 1][column] != '0')
						return false;
				}
			}
		}
		return true;
	}
	case UP: {
		//find imin
		int imin = find_imin(move.getGroup());
		if (imin == board.size())
			return false;
		//verify if move is possible
		for (int line = 0; line < board.size(); line++) {
			for (int column = 0; column < board[line].size(); column++) {
				if (board[line][column] == move.getGroup()) {
					if (2 * imin - line - 1 < 0)
						return false;
					if (board[2 * imin - line - 1][column] != '0')
						return false;
				}
			}
		}
		return true;
	}
	case RIGHT: {
		//find jmax
		int jmax = find_jmax(move.getGroup());
		if (jmax == -1)
			return false;
		//verify if move is possible
		for (int line = 0; line < board.size(); line++) {
			for (int column = 0; column < board[line].size(); column++) {
				if (board[line][column] == move.getGroup()) {
					if (2 * jmax - column + 1 >= board[0].size())
						return false;
					if (board[line][2 * jmax - column + 1] != '0')
						return false;
				}
			}
		}
		return true;
	}
	case LEFT: {
		//find jmin
		int jmin = find_jmin(move.getGroup());
		if (jmin == board[0].size())
			return false;
		//verify if move is possible
		for (int line = 0; line < board.size(); line++) {
			for (int column = 0; column < board[line].size(); column++) {
				if (board[line][column] == move.getGroup()) {
					if (2 * jmin - column - 1 >= board[0].size())
						return false;
					if (board[line][2 * jmin - column - 1] != '0')
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

void Level::make_move(Move move) {
	switch (move.getDirection()) {
	case DOWN: {
		//find imax
		int imax = find_imax(move.getGroup());
		//make move
		for (int line = 0; line < board.size(); line++) {
			for (int column = 0; column < board[line].size(); column++) {
				if (board[line][column] == move.getGroup()) {
					board[2 * imax - line + 1][column] = move.getGroup();
				}
			}
		}
		break;
	}
	case UP: {
		//find imin
		int imin = find_imin(move.getGroup());
		//make move
		for (int line = 0; line < board.size(); line++) {
			for (int column = 0; column < board[line].size(); column++) {
				if (board[line][column] == move.getGroup()) {
					board[2 * imin - line - 1][column] = move.getGroup();
				}
			}
		}
		break;
	}
	case RIGHT: {
		//find jmax
		int jmax = find_jmax(move.getGroup());
		//make move
		for (int line = 0; line < board.size(); line++) {
			for (int column = 0; column < board[line].size(); column++) {
				if (board[line][column] == move.getGroup()) {
					board[line][2 * jmax - column + 1] = move.getGroup();
				}
			}
		}
		break;
	}
	case LEFT: {
		//find jmin
		int jmin = find_jmin(move.getGroup());
		//make move
		for (int line = 0; line < board.size(); line++) {
			for (int column = 0; column < board[line].size(); column++) {
				if (board[line][column] == move.getGroup()) {
					board[line][2 * jmin - column - 1] = move.getGroup();
				}
			}
		}
		break;
	}
	}
	this->cost++;
}

bool Level::solved() {
	for (int line = 0; line < board.size(); line++) {
		for (int column = 0; column < board[line].size(); column++) {
			if (board[line][column] == '0')
				return false;
		}
	}

	return true;
}

void Level::display() {
	cout << "  ";
	for (int column = 0; column < board[0].size(); column++) {
		cout << column + 1 << " ";
	}
	cout << "\n";
	for (int line = 0; line < board.size(); line++) {
		cout << line + 1 << " ";
		for (int column = 0; column < board[line].size(); column++) {
			string color = groups.at(board[line][column]).second;
			cout << color << board[line][column] << " " << reset_color;
		}
		cout << '\n';
	}
}

vector<Move> Level::possible_moves() {
	vector<Move> moves;
	for (auto& x : groups) {
		if ((x.first != '0') && (x.first != '_')) {
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

int Level::getValue(){
    int value=0;

    for (int line = 0; line < board.size(); line++) {
        for (int column = 0; column < board[line].size(); column++) {
            if  (board[line][column] == '0')
                value++;
        }
    }
    return value;
}
//--------------------------------------------------------------------------------------------------------//
//--------------------------------------------  NODE -----------------------------------------------------//
//--------------------------------------------------------------------------------------------------------//
class Node {
public:
	Level state;
	int father_index;
	Move move;
	int depth;
	vector<Move> moves;
	int value;
	Node(Level state, int father_index, Move move, int depth);
	Node() {};
};

Node::Node(Level state, int father_index, Move move, int depth) {
	this->state = state;
	this->father_index=father_index;
	this->move = move;
	this->depth = depth;
	this->moves=state.possible_moves();
}

class SearchTree {
private:
    list<Node> nodes_unsearched;
    list<Node> nodes;
    int index;
public:
	SearchTree(Node root) {
		nodes.push_back(root);
		index = 0;

	}
    Node* getNodeAt(int index);
    Node *getUnsearchedNodeAt(int index);
    void resetIterative() {
	    index = 0;
	    Node temp = nodes.front();
        temp.moves=temp.state.possible_moves();
	    nodes.clear();
	    nodes.push_back(temp);};
	Node breadth_first();
	Node depth_first(int max_depth);
	Node iterative_deepening(int max_depth);
    Node greedy();
    Node greedy_aux();
    Node astar();


    void startAstar();
};

Node* SearchTree::getNodeAt(int index) {
    auto l_front = nodes.begin();
    advance(l_front, index);
    return &(*l_front);
}

Node* SearchTree::getUnsearchedNodeAt(int index) {
    auto l_front = nodes_unsearched.begin();
    advance(l_front, index);
    return &(*l_front);
}

void SearchTree::startAstar(){
    Node node = nodes.front();
    nodes.clear();
    nodes_unsearched.push_back(node);
}

Node SearchTree::breadth_first() {
    Node* node = getNodeAt(index);
    if (node->state.solved()) {
		cout << "Nodes visited: " << index << "\n";
		return *node;
	}
	vector<Move> moves = node->state.possible_moves();
	for (int i = 0; i < moves.size(); i++) {
		Level new_level = node->state;
		new_level.make_move(moves[i]);
		Node new_node(new_level, index, moves[i], node->depth + 1);
		nodes.push_back(new_node);
	}
	index++;
	if (index < nodes.size())
		return breadth_first();
	else
		return nodes.front();
}

Node SearchTree::depth_first(int max_depth) {
	Node* node = getNodeAt(index);
	if (node->state.solved()) {
		cout << "Nodes visited: " << index << "\n";
		return *node;
	}
	if (node->depth >= max_depth)
		return nodes.front();
	if (node->moves.size()>0){
        Level new_level = node->state;
        new_level.make_move(node->moves.front());
        //new_level.display();
        Node new_node(new_level, index,node->moves.front(), node->depth + 1);
        //cout<<"MaxDepth: "<<max_depth<<"\nMoves available: "<<node->moves.size()<<"\nDepth: "<<node->depth<<"\nIndex: "<<index<<endl;
        index=nodes.size();
        nodes.push_back(new_node);
        node->moves.erase(node->moves.begin());
        return depth_first(max_depth);}
	else if (node->father_index!=-1){
	    index=node->father_index;
        return depth_first(max_depth);
	}
	else return nodes.front();
}

Node SearchTree::iterative_deepening(int max_depth) {
	Node final_node;
	for (int i = 0; i <= max_depth; i++) {
		resetIterative();
		final_node = depth_first(i);
		if (final_node.father_index != -1)
			return final_node;
	}
	return nodes.front();
}

void grouping(vector<vector<char> > &board, vector<vector<char> > &visited_board, int j, int k, int &partial_grp) {
    if (j < board.size() && k < board[j].size() && board[j][k] == '0' && j>=0 && k>=0 && visited_board[j][k] == '0') {
        partial_grp++;
        visited_board[j][k] = '_';
        grouping(board, visited_board, j, k - 1, partial_grp);
        grouping(board, visited_board, j, k + 1, partial_grp);
        grouping(board, visited_board, j - 1, k, partial_grp);
        grouping(board, visited_board, j + 1, k, partial_grp);
    }
}
Node SearchTree::greedy() {
    Node *node = getNodeAt(index);

    int grp_size = INT16_MAX;
    int partial_grp = 0;
    int k_temp, j_temp;

    vector<vector<char> > board = node->state.getBoard();
    vector<vector<char> > visited_board = node->state.getBoard();
    for (int j = 0; j < board.size(); j++) {//find smallest empty group
        for (int k = 0; k < board[j].size(); k++) {
            if (board[j][k] == '0' && visited_board[j][k] == '0') {
                /*		partial_grp++;
                        visited_board[j][k] = '_';*/
                partial_grp = 0;
                grouping(board, visited_board, j, k, partial_grp);
                if (partial_grp < grp_size) {
                    grp_size = partial_grp;
                    k_temp = k;
                    j_temp = j;
                }
            }
        }
    }
    vector<Move> altered;
    vector<Move> partially_altered;
    vector<Move> unaltered;
    for (int i=0;i<node->moves.size();i++){
        Level new_level = node->state;
        new_level.make_move(node->moves[i]);
        if (board[j_temp][k_temp] == '0') {
            visited_board = new_level.getBoard();
            partial_grp = 0;
            grouping(board, visited_board, j_temp, k_temp, partial_grp);
            if (partial_grp < grp_size-1)//if size changed more than 1 (because if its the big group it always changes 1)
                altered.push_back(node->moves[i]);
            else if (partial_grp + 1 == grp_size)
                partially_altered.push_back(node->moves[i]);
            else unaltered.push_back(node->moves[i]);
        }
        else altered.push_back(node->moves[i]);
    }
    node->moves.clear();
    altered.insert(altered.end(), partially_altered.begin(), partially_altered.end());
    altered.insert(altered.end(), unaltered.begin(), unaltered.end());
    node->moves=altered;
    return greedy_aux();
}

Node SearchTree::greedy_aux() {
    Node *node = getNodeAt(index);
    if (node->state.solved()) {
        cout << "Nodes visited: " << index << "\n";
        return *node;
    }
    if (node->moves.size()>0){
        Level new_level = node->state;
        new_level.make_move(node->moves.front());
        //new_level.display();
        Node new_node(new_level, index,node->moves.front(), node->depth + 1);
        index=nodes.size();
        nodes.push_back(new_node);
        node->moves.erase(node->moves.begin());
        //cout<<"\nMoves available: "<<node->moves.size()<<"\nDepth: "<<node->depth<<"\nIndex: "<<index<<endl;
        return greedy();}
    else if (node->father_index!=-1){
        index=node->father_index;
        return greedy_aux();
    }
    else return nodes.front();
}

Node SearchTree::astar() {
    int temp_value=INT16_MAX,index=0;
    for (int i=0;i<nodes_unsearched.size();i++){
        if (getUnsearchedNodeAt(i)->value < temp_value){
            temp_value=getUnsearchedNodeAt(i)->value;
            index=i;}
    }
    Node* node = getUnsearchedNodeAt(index);
    nodes.push_back(*node);


    if (node->state.solved()) {
        cout << "Nodes visited: " << nodes.size() << "\n";
        return *node;
    }

    for (int i=0;i<node->moves.size();i++){
        Level new_level = node->state;
        new_level.make_move(node->moves[i]);
        Node new_node(new_level, nodes.size()-1,node->moves[i], node->depth + 1);
        new_node.value=new_level.getValue()+new_node.depth;
        nodes_unsearched.push_back(new_node);}


    auto it = nodes_unsearched.begin();
    advance(it,index);
    nodes_unsearched.erase(it);
    return astar();
}

//--------------------------------------------------------------------------------------------------------//
//----------------------------------------  FoldingBlocks ------------------------------------------------//
//--------------------------------------------------------------------------------------------------------//




//--------------------------------------------------------------------------------------------------------//
//----------------------------------------  FoldingBlocks ------------------------------------------------//
//--------------------------------------------------------------------------------------------------------//
class FoldingBlocks {
private:
	vector<Level> levels;
public:
	FoldingBlocks();
	void print_level(int level) { levels[level].display(); }
	void print_possible_moves(int level) {
		vector<Move> moves = levels[level].possible_moves();
		for (int i = 0; i < moves.size(); i++)
			moves[i].display();
	}
	vector<Move> solve(int mode, Level level);
	void play_bot(int mode, int level);
	void play_human(int level);
};

FoldingBlocks::FoldingBlocks() {
	vector< vector<char> > level1 = { {'A', '0'}, {'0', '0'} };
	vector< vector<char> > level2 = { {'_', '0', '0', '_'}, {'0', '0', '0', '0'}, {'A', 'A', '0', '0'}, {'_', 'A', '0', '_'} };
	vector< vector<char> > level3 = { {'A', '0', '0', '0'}, {'_', '_', '_', '_'}, {'A', '0', '0', '0'} };
	vector< vector<char> > level4 = { {'A', '0', '_', '_'}, {'0', '0', '0', '0'}, {'0', '0', '0', '0'}, {'0', '0', '0', '0'}, {'_', '_', 'B', '0'} };
	vector< vector<char> > level5 = { {'A', '0', '_', '_'}, {'0', '0', '0', '0'}, {'0', '0', '0', '0'}, {'0', '0', '0', '0'}, {'0', '0', '0', '0'}, {'_', '_', 'B', '0'} };
	vector< vector<char> > level6 = { {'0', '0', '0', '0'}, {'0', '0', '0', '0'}, {'0', '0', '0', '0'}, {'0', '0', '0', 'A'}, {'B', '0', '0', '0'}, {'0', '0', '0', '0'}, {'0', '0', '0', '0'}, {'0', '0', '0', '0'} };
	levels.push_back(Level(1, level1));
	levels.push_back(Level(2, level2));
	levels.push_back(Level(3, level3));
	levels.push_back(Level(4, level4));
	levels.push_back(Level(5, level5));
	levels.push_back(Level(6, level6));
}

vector<Move> FoldingBlocks::solve(int mode, Level level) {
	Move m('0', 0);
	Node first(level,-1, m, 0);
    first.value=level.getValue();
	SearchTree st(first);
	Node final;

	switch (mode) {
	case 2:
		final = st.breadth_first();
		break;
	case 3:
		final = st.depth_first(100);
		break;
	case 4:
		final = st.iterative_deepening(100);
		break;
    case 5:
        final= st.greedy();
        break;
    case 6:
        st.startAstar();
        final=st.astar();
        break;
    }


	vector<Move> moves;
	while (final.father_index != -1) {
        moves.push_back(final.move);
		final = *(st.getNodeAt(final.father_index));
	}
	reverse(moves.begin(), moves.end());

    return moves;
}

void FoldingBlocks::play_bot(int mode, int level) {
	Level current_level = levels[level];
	//cout << current_level.getGroups()['0'].first;
	vector<Move> moves = solve(mode, current_level);
	current_level.display();
	cout << "\n";
	if (moves.empty())
		cout << "No solution is possible for level " << level+1 << '\n';
	else {
		for (int i = 0; i < moves.size(); i++) {
			current_level.make_move(moves[i]);
			moves[i].display();
			current_level.display();
			cout << "\n";
		}
		cout << "Level Solved with success.\nFinal Cost: " << moves.size() << "\n";
	}
}

void FoldingBlocks::play_human(int level) {
	Level current_level = levels[level];
	int cost = 0;
	bool out = false;
	while (!out) {
		//display board
		current_level.display();
		cout << "Number of Moves: " << cost << "\n\n";

		//group input
		current_level.display_group_colors();
		cout << "To Quit insert Q.\nNext Move?\n" << "Group: ";
		char group;
		cin >> group;
		while (1) {
			if (!cin.fail() && (current_level.group_exists(group) || (group == 'Q')))
				break;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "\nYou have entered wrong input" << endl;
			current_level.display_group_colors();
			cout << "To Quit insert Q.\nNext Move?\n" << "Group: ";
			cin >> group;
		}

		if (group == 'Q')
			break;

		//direction input
		cout << "\nDOWN: 1 \nUP: 2\nLEFT: 3\nRIGHT : 4\n";
		cout << "Direction: ";
		int direction;
		cin >> direction;
		while (1) {
			if (!cin.fail() && (direction >= 1) && (direction <= 4))
				break;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
		else {
			current_level.make_move(move);
			cost++;
		}

		if (current_level.solved()) {
			out = true;
			cout << "Congratulations! You have solved this level!\n";
			current_level.display();
			cout << "Final Number of Moves: " << cost << "\n\n";
		}
	}
}

int main(int argc, char *argv[]) {
	if ((argc != 3) || (atoi(argv[1]) <= 0) || (atoi(argv[1]) > 6) || (atoi(argv[2]) <= 0) || (atoi(argv[2]) > 6))
	{
		cout << "Usage: ./game <option> <level>\n"
			<< "where option must be: 1(human player), 2(breadth-first solver), 3(depth-first solver), 4(iterative deepening), 5(greedy), 6(A*)\n"
			<< "where level must be: an integer between 1 and 6\n\n";
		return -1;
	}

	int level = atoi(argv[2]) - 1;
	int mode = atoi(argv[1]);


	FoldingBlocks foldingBlocks;
	if (mode == 1)
		foldingBlocks.play_human(level);
	else
		foldingBlocks.play_bot(mode, level);
}