/**
 * @file main.cpp
 *
 * @brief
 *    This program helps RedNova and StarTrace's managers to choose the best
 *    strategy, based on a 10-year planning, that will minimize the expenses
 *    over the first 10 years. This program solves the problem when managers
 *    must decide whether to add two new wormholes between A (RedNova) and C
 *    (StarTrace) and between B (RedNova) and D (StarTrace) and remove one of
 *    these new wormholes.
 *
 * @author Athit Vue
 * @date 03/20/2018
 */
#include <iostream>
#include <list>
#include <vector>
#define DEBUG 0
using namespace std;

// Struct for vertex
struct vertex{
  public:
    int weight;
    int parent;
    int visited = false;

    // Constructor
    vertex(){
      parent = -1;
      weight = -1;
    }
};

// Struct for edge
struct edge{
  int b; // Destinaton vertex
  int weight;

  // Overloaded Constructor
  edge(int v, int w){
    b = v;
    weight = w;
  }
};

// Struct for largest_edge
struct largest_edge{
  int a; // from
  int b; // to
  int weight;

  largest_edge(){
    a = 0;
    b = 0;
    weight = 0;
  }

  // Overloaded Constructor
  largest_edge(int u, int v, int w){
    a = u;
    b = v;
    weight = w;
  }
};

// Global Dummy Vertex
largest_edge dummy_vertex(-1, -1 , -1);

// Prototypes:
void get_RedNovas_inputs(vector<vertex> &RedNova_vertices, vector<list<edge>> &RedNova_adj_list, int &total_cost, int n);
void get_StarTraces_inputs(vector<vertex> &StarTrace_vertices, vector<list<edge>> &StarTrace_adj_list, int &total_cost, int m);
largest_edge largest_weight_edge(int u, int v, vector<vertex> &vertices,vector<list<edge>> &adj_list);
void DFS(vector<list<edge>> &adj_list, vector<vertex> &vertices, int u);
void decide_and_print(int r_A, int r_B, int s_C, int s_D, int c1, int c2, int w1, int w2, largest_edge existing_max_edge, int &total_cost, int wormhole);

int main(){
  int n; // RedNova's size
  int r_A; // Id of a RedNova's station
  int s_C; // Id of a StarTrace's station
  int w1; // Maintainence fee of wormhole between A and C.
  int c1; // One time cost to create a wormhole.
  int r_B;
  int s_D;
  int w2;
  int c2;
  int m; // StarTrace's size
  int total_cost = 0; // Total maintainence fee

  // First get n, the total number of stations in RedNova's wormhole network.
  cin >> n;

  // Create a vector array that will store RedNova's vertices.
  vector<vertex> RedNova_vertices(n);
  // Create a vector list to store the adjacency list for RedNova's vertices.
  vector<list<edge>> RedNova_adj_list(n);

  // Get RedNova's vertices from input.
  get_RedNovas_inputs(RedNova_vertices, RedNova_adj_list, total_cost, n);

  /* KEEP FOR DEBUGGING IN THE FUTURE */
  if (DEBUG){
    /* DEBUG: neighbors list */
    cout << "#### ADJACENCY LIST RedNova ####" << endl;
    for (int i = 0; i < n; i++){
      cout << "Vertex <" << i << "> : ";
      for (auto it = RedNova_adj_list[i].begin();
        it != RedNova_adj_list[i].end(); it++){
        cout << (*it).b << "(" << (*it).weight << "), ";
      }
      cout << endl;
    }
  }

  // Get from stdinput StarTrace's size
  cin >> m;

  // Create vectors for StarTrace
  vector<vertex> StarTrace_vertices(m);
  vector<list<edge>> StarTrace_adj_list(m);

  get_StarTraces_inputs(StarTrace_vertices, StarTrace_adj_list, total_cost, m);

  /* KEEP FOR DEBUGGING IN THE FUTURE */
  if (DEBUG){
    /* DEBUG: neighbors list */
    cout << "#### ADJACENCY LIST STARTRACE ####" << endl;
    for (int i = 0; i < m; i++){
      cout << "Vertex <" << i << "> : ";
      for (auto it = StarTrace_adj_list[i].begin();
        it != StarTrace_adj_list[i].end(); it++){
        cout << (*it).b << "(" << (*it).weight << "), ";
      }
      cout << endl;
    }
  }

  // Get the next needed requirements: A - id of a RedNova's station,
  // C - id of a StarTrace's station, w1 - the weight, c1 - the cost to prod.
  cin >> r_A >> s_C >> w1 >> c1;

  // Next same as above but for "to" vertex...
  cin >> r_B >> s_D >> w2 >> c2;

  // Need to keep track of the largest edge somehow...
  // Make a struct type for holding (u, v) and weight...
  largest_edge RedNova_largest_edge;
  largest_edge StarTrace_largest_edge;

  // Find the largest weighted edge for RedNova
  RedNova_largest_edge = largest_weight_edge(r_B, r_A, RedNova_vertices,
    RedNova_adj_list);

  // Find the largest weighted edge for StarTrace
  StarTrace_largest_edge = largest_weight_edge(s_D, s_C, StarTrace_vertices,
    StarTrace_adj_list);

  largest_edge existing_max_edge = dummy_vertex;

  int wormhole = 0;

  // Decide which wormhole to add.
  if (RedNova_largest_edge.weight >= StarTrace_largest_edge.weight){
    existing_max_edge.weight = RedNova_largest_edge.weight;
    existing_max_edge.a = RedNova_largest_edge.a;
    existing_max_edge.b = RedNova_largest_edge.b;
    wormhole = 1;
  }
  else{
    existing_max_edge.weight = StarTrace_largest_edge.weight;
    existing_max_edge.a = StarTrace_largest_edge.a;
    existing_max_edge.b = StarTrace_largest_edge.b;
    wormhole = 2;
  }

  decide_and_print(r_A, r_B, s_C, s_D, c1, c2, w1, w2,
    existing_max_edge, total_cost, wormhole);

  return 0;
}

/* To get the RedNova inputs */
void get_RedNovas_inputs(vector<vertex> &RedNova_vertices, vector<list<edge>> &RedNova_adj_list, int &total_cost, int n){
  for (int i = 0; i < (n-1); i++){
    int a;
    int b;
    int w;

    // Get input from standard input
    cin >> a >> b >> w;

    // Update total cost for 9 years, for each edge added.
    total_cost = total_cost + (w*10);

    // Add to RedNova's adjacency list
    RedNova_adj_list[a].push_back(edge(b, w));
    // Since undirected, add b to a.
    RedNova_adj_list[b].push_back(edge(a, w));
  }
}

/* To get the StarTrace inputs */
void get_StarTraces_inputs(vector<vertex> &StarTrace_vertices, vector<list<edge>> &StarTrace_adj_list, int &total_cost, int m){
  for (int i = 0; i < (m-1); i++){
    int a;
    int b;
    int w;

    // Get input from standard input
    cin >> a >> b >> w;

   // Update total cost for 9 years, for each edge added.
    total_cost = total_cost + (w*10);

    // Add to StarTrace's adjacency list
    StarTrace_adj_list[a].push_back(edge(b, w));
    // Since undirected, add b to a.
    StarTrace_adj_list[b].push_back(edge(a, w));
  }
}

/* This function will ultilize the recurrence function DFS and backtrack
   the parents of each vertex comparing the max weight. */
largest_edge largest_weight_edge(int u, int v, vector<vertex> &vertices, vector<list<edge>> &adj_list){
  // Need to keep track of the max weighted edge.
  largest_edge cur_max_weight_edge;
  cur_max_weight_edge = dummy_vertex;

  // Use dfs to search the graph
  DFS(adj_list, vertices, u);

  // Now we can backtrack, going to use infinite loop
  int cur_vertex; // Current vertex
  cur_vertex = v;
  while(true){
    int cur_parent;
    int cur_weight;
    cur_parent = vertices[cur_vertex].parent;
    cur_weight = vertices[cur_vertex].weight;

    // Check the current weight, if it is less than current max weight edge then
    // set the weight to the current max weight edge and get that edge's parent.
    // Repeat until we have fully tranverse up the tree.
    if (cur_max_weight_edge.weight < cur_weight){
      cur_max_weight_edge.weight = cur_weight;
      cur_max_weight_edge.a = cur_parent;
      cur_max_weight_edge.b = cur_vertex;
    }
    else{
      // If cur_vertex == u, then we are done; we are at the root.
      if (cur_vertex == u){
        break;
      }
      cur_vertex = cur_parent;
    }
  }

  // Return the largest edge
  return cur_max_weight_edge;
}

/* Recursive function to search the graph */
void DFS(vector<list<edge>> &adj_list, vector<vertex> &vertices, int u){
  int v;

  // Set the current vertex visited property to true.
  vertices[u].visited = true;

  // Iterate through the neighbors
  for (auto it = adj_list[u].begin(); it != adj_list[u].end(); it++){
    v = (*it).b;

    // If have not visted edges then...
    if (vertices[v].visited == false){
      vertices[v].parent = u;
      vertices[v].weight = (*it).weight;
      // Recurse for next Vertex
      DFS(adj_list, vertices, v);
    }
  }
}

/* Decide and print on which is the most cost effective for a period of
   10 years. */
void decide_and_print(int r_A, int r_B, int s_C, int s_D, int c1, int c2,
  int w1, int w2, largest_edge existing_max_edge, int &total_cost,
  int wormhole){
  // Cost of wormhole between A (RedNova) and C (StarTrace)
  int ten_year_cost_AC = c1 + (w1 * 10);
  // Cost of wormhole between B (RedNova) and D (StarTrace)
  int ten_year_cost_BD = c2 + (w2 * 10);
  // Cost of existing max edge from graph after 10 ten years
  int ten_year_cost_cur_max_edge = existing_max_edge.weight * 10;

  // Get the max of new wormholes to compare with current max edge
  int max_new_wormholes = max(ten_year_cost_AC, ten_year_cost_BD);

  // Check to see if existing max edge will cost more than the two
  // new wormholes added, based on 10 years.
  if (ten_year_cost_cur_max_edge < max_new_wormholes){
    // then add only one wormhole needs to be added print that associated
    // with the total cost after 10 ten years
    if (ten_year_cost_AC <= ten_year_cost_BD){
      total_cost += ten_year_cost_AC;
      cout << r_A << " " << s_C << " " << total_cost << endl;
    }
    else{
      total_cost += ten_year_cost_BD;
      cout << r_B << " " << s_D << " " << total_cost << endl;
    }
  }
  else{ // Need to add both wormholes
    // remove the existing max edge's cost
    total_cost -= (existing_max_edge.weight * 10);
    // Add new costs to total
    total_cost += (ten_year_cost_AC+ten_year_cost_BD);

    // Print the wormhole between AC or BD that needs to be removed and
    // print the total cost.
    cout << min(existing_max_edge.a, existing_max_edge.b) << " ";
    cout << max(existing_max_edge.a, existing_max_edge.b) << " ";
    cout << wormhole << " " << total_cost << endl;
  }
}
