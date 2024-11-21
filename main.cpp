#include<iostream>
#include<string>
#include<cstdlib>
#include<time.h>
#include "doublelinklist.h"

using namespace std;

// Class representing Pokémon attacks
struct attack{   // class for pokemon attacks
    private:
        string name;     // Name of the attack
        int pp;          // Power Points for the attack
        int accuracy;    // Accuracy of the attack (percentage)
        int damage;      // Damage inflicted by the attack
        int first;       // First level at which the attack can be used
    public:
        attack(const string&, int, int, int, int);
        int get_first();    // Returns the first level the attack can be used
        int get_accuracy(); // Returns the accuracy of the attack
        int get_damage();   // Returns the damage of the attack
        int get_pp();       // Returns the PP cost of the attack
        string get_name();  // Returns the name of the attack
};
// Constructor to initialize an attack
attack::attack(const string& name_in, int p, int a, int d, int f){
    name = name_in;
    pp = p;
    accuracy = a;
    damage = d;
    first = f;
}

// Constructor to initialize an attack
string attack::get_name(){
    return name;
}

// Getter method to return the PP cost of the attack
int attack::get_pp(){
    return pp;
}

// Getter method to return the damage of the attack
int attack::get_damage(){
    return damage;
}

// Getter method to return the first level the attack can be used
int attack::get_first(){
    return first;
}

// Getter method to return the accuracy of the attack
int attack::get_accuracy(){
    return accuracy;
}

struct pokemon{                         // class for pokemons
    int pp;                          // Power Points of the Pokémon
    int hp;                          // Health Points of the Pokémon
    string name;                         // Name of the Pokémon
    DoublyList<attack*> attacks; // List of attacks available to the Pokémon
    pokemon(const string &name_in, int, int); // Constructor
    string get_name();              // Returns the name of the Pokémon
    void set(char, int to_reduce); // Updates the PP or HP of the Pokémon
};

// Constructor to initialize a Pokémon with name, PP, and HP
pokemon::pokemon(const string &name_in, int p, int h){
    name = name_in;
    pp = p;
    hp = h;
}

// Method to update the PP or HP of the Pokémon
void pokemon::set(char which, int to_reduce){
    if(which == 'p'){
        pp += to_reduce;
    }else{
        hp -= to_reduce;
    }
}

// Class representing a node in the graph
struct node{  // class for the nodes of the graph
    string ifef;  // attack is efficient or nonefficent
    string curattack;  // attack name
    int num;  // node num
    pokemon* pikachu;  // current pikachu
    pokemon* blastoise;  // currrent blastoise
    char status;  // whose turn?
    int level;  // level of the node
    bool isleaf;  // node is a leaf or not
    double prob;  // prob of the node happening
     
    DoublyList<node*> child;  // children nodes of the node
    node* parent; // parent pointer to print path after finding the winnernode in part-2 
  

    // Constructor for initializing a node with default values
    node(pokemon* p, pokemon* b, char s = 'P', int lvl = 0, bool leaf = false, double probability = 1.0, node* par = nullptr)
        : pikachu(p), blastoise(b), status(s), level(lvl), isleaf(leaf), prob(probability), parent(par) {
        ifef = "";
        curattack = "";
        num = 0;
    }

    // This function calculates all possible new states from the current node and creates child nodes
   void expandGraph(DoublyList<node*>& nodeList, int Depth) {
    if (isleaf || level >= Depth) return;

    // Get the list of attacks available to the current Pokémon
    DoublyList<attack*> attacks = (status == 'P') ? pikachu->attacks : blastoise->attacks;
    Node<attack*>* attackNode = attacks.head;
    
    // Calculate the number of available attacks based on level and first use condition
    int availableAttacksCount = 0;
    Node<attack*>* tempNode = attacks.head;
    while (tempNode != NULL) {
        if (level + 1 >= tempNode->data->get_first()) {
            availableAttacksCount++;
        }
        tempNode = tempNode->next;
    }

    // Iterate over the list of attacks and generate new nodes
    while (attackNode != NULL) {
        attack* act = attackNode->data;

        // Check if the current Pokémon has enough PP to use the attack
        if ((status == 'P' && pikachu->pp < act->get_pp()) || (status == 'B' && blastoise->pp < act->get_pp())) {
            attackNode = attackNode->next;
            continue;
        }

        // Ensure the attack can be used starting from its first usage level
        if (level + 1 < act->get_first()) {
            attackNode = attackNode->next;
            continue;
        }

        double effectiveProbability = act->get_accuracy() / 100.0;
        double notEffectiveProbability = 1.0 - effectiveProbability;

        // Generate child nodes for effective and not effective cases
        for (int i = 0; i < (act->get_accuracy() == 100 ? 1 : 2); i++) {
            bool isEffective = (i == 0);
            double childProb = prob * (1.0 / availableAttacksCount) * (isEffective ? effectiveProbability : notEffectiveProbability);

            // Create new Pokémon instances to represent the new state
            pokemon* newPika = new pokemon(*pikachu);
            pokemon* newBlas = new pokemon(*blastoise);

            // Update the PP and HP based on the attack's effectiveness
            if (status == 'P') {
                newPika->pp += act->get_pp();
                if (isEffective) newBlas->hp -= act->get_damage();
                
            } else {
                newBlas->pp += act->get_pp();
                if (isEffective) newPika->hp -= act->get_damage();
                
            }
            // Determine if the new node is a leaf node
            bool newIsLeaf = (newPika->hp <= 0 || newBlas->hp <= 0 || level + 1 == Depth);
            node* newNode = new node(newPika, newBlas, status == 'P' ? 'B' : 'P', level + 1, newIsLeaf, childProb, this);

            newNode->curattack = act->get_name();
            newNode->ifef = isEffective ? "effective" : "not effective";
            child.addBack(newNode);    // Add new node as a child
            nodeList.addBack(newNode);  // Add new node to the node list for BFS
        }
        attackNode = attackNode->next;
    }
}

};


// Class representing the graph of game states
struct graph{
    node* root; // Root node of the graph
    DoublyList<node*> nodes; // DoublyList used to store nodes in BFS order

    // Constructor to initialize the graph with a root node
    graph(pokemon* p, pokemon* b) {
        root = new node(p, b);  // Create the root node
        nodes.addBack(root);     // Add the root node to the list
    }

    // Function to expand the graph and process nodes
    void expand(int Depth, bool shouldPrint = true) {

    // Process nodes until the list is empty
    while (nodes.head != NULL) {
        node* current = nodes.head->data;
        nodes.removeFront();
        if (!current->isleaf && current->level < Depth) {
            current->expandGraph(nodes, Depth); // Expand current node to generate child nodes
            if (shouldPrint && current->level == Depth - 1) {
                printNode(current); // Print the node details
            }
        }
    }
}

    // Function to print node details
    void printNode(node* n) {
    // Assuming you will handle output prefixes outside this function or you adjust this as needed
    cout << "P_HP:" << n->pikachu->hp << " P_PP:" << n->pikachu->pp;
    cout << " B_HP:" << n->blastoise->hp << " B_PP:" << n->blastoise->pp << " PROB:" << n->prob << endl;
}

    // Function to find the shortest winning path for a participant
    void findShortestWinningPath(string participant) {
        DoublyList<node*> queue;
        queue.addBack(root); // Add the root node to the queue

        node* shortestWin = nullptr; // Pointer to store the shortest winning node
        int shortestDepth = 2147483647; // Initialize to a large value

        // Process nodes in BFS manner to find the shortest winning path
        while (queue.head != NULL) {
            node* current = queue.get(0);
            queue.removeFront();

            // Determine if the current node is a winning node for the participant
            bool isWinningNode = (participant == "pikachu" && current->blastoise->hp <= 0 && current->pikachu->hp > 0) ||
                                 (participant == "blastoise" && current->pikachu->hp <= 0 && current->blastoise->hp > 0);

            if (current->isleaf && isWinningNode) {
                if (current->level < shortestDepth) {
                    shortestWin = current;
                    shortestDepth = current->level;
                }
            }

            // Add child nodes to the queue for further processing
            Node<node*>* childNode = current->child.head;
            while (childNode != NULL) {
                queue.addBack(childNode->data);
                childNode = childNode->next;
            }
        }

        // Print the shortest winning path if found
        if (shortestWin != nullptr) {
            DoublyList<node*> bestPath;
            node* current = shortestWin;
            while (current != nullptr) {
                bestPath.addFront(current);
                current = current->parent;
            }
            printWinPath(bestPath, shortestWin);
        } else {
            cout << "No winning path found." << endl;
        }
    }

    // Function to print the winning path
    void printWinPath(DoublyList<node*>& bestPath, node* winner) {
        cout << "Winning path with probability: " << winner->prob << endl << endl ;
        Node<node*>* n = bestPath.head;
        while (n != NULL) {
            string currentPokemon = (n->data->status == 'P') ? "Blastoise"  : "Pikachu";  // Check the turn
            if( n->data->curattack !="")
            {
            cout << currentPokemon << " used  " << n->data->curattack << " : " << n->data->ifef << endl;
            }else{
                cout<<"Beginning state: No action Yet"<<endl;
            }
            cout << "Level : " <<n->data->level << " Pikachu HP: "  << n->data->pikachu->hp << " PP: " << n->data->pikachu->pp;
            cout << ", Blastoise HP: " << n->data->blastoise->hp << " PP: " << n->data->blastoise->pp << endl << endl;
            n = n->next;
        }
        // To print the winner node game state
        if (winner != NULL) {              
            cout << "Final state at Level " << winner->level << ": Pikachu HP: " << winner->pikachu->hp << ", PP: " << winner->pikachu->pp
                << "; Blastoise HP: " << winner->blastoise->hp << ", PP: " << winner->blastoise->pp << endl;
        }
    }

};


int main(int argc, char** argv) {
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " part1 [maxDepth] or " << argv[0] << " part2 [participant]" << endl;
        return -1;
    }

    string part = argv[1];
    string participant;

    // Initialize Pokémon with 100 PP and 200 HP
    pokemon* pikachu = new pokemon("Pikachu", 100, 200); // Initialize with 100 PP and 200 HP
    pokemon* blastoise = new pokemon("Blastoise", 100, 200); // Initialize with 100 PP and 200 HP

    // Define attacks for both Pokémon
    pikachu->attacks.addBack(new attack("Thundershock", -10, 100, 40, 0));
    pikachu->attacks.addBack(new attack("Skull Bash", -15, 70, 50, 0));
    pikachu->attacks.addBack(new attack("Slam", -20, 80, 60, 0));
    pikachu->attacks.addBack(new attack("Skip", 100, 100, 0, 3)); // Skip attack restores PP

    blastoise->attacks.addBack(new attack("Tackle", -10, 100, 30, 0));
    blastoise->attacks.addBack(new attack("Water Gun", -20, 100, 40, 0));
    blastoise->attacks.addBack(new attack("Bite", -25, 100, 60, 0));
    blastoise->attacks.addBack(new attack("Skip", 100, 100, 0, 3)); // Skip attack restores PP

    graph g(pikachu, blastoise);

   // Part 1: Expand the graph to a specified depth and print the nodes
    if (part == "part1") {
        if (argc != 3) {
            cout << "Usage: " << argv[0] << " part1 [Depth]" << endl;
            return -1;
        }
        int Depth = atoi(argv[2]);
        g.expand(Depth, true);  // Enable node printing for part1
    // Part 2: Expand the graph to a depth of 9 and find the shortest winning path
    } else if (part == "part2") {
        if (argc != 3) {
            cout << "Usage: " << argv[0] << " part2 [participant]" << endl;
            return -1;
        }
        participant = argv[2];
        g.expand(9, false);  // Disable node printing for part2
        g.findShortestWinningPath(participant);
    } else {
        cout << "Invalid part specified." << endl;
        return -1;
    }

    return 0;
}

