/**
 * @file hw1.cpp
 * 
 * @brief
 *    Implementation of a n-ary tree using BFS. This program will use BFS
 *    to figure out the longest path (the diameter) of two endpoints. It 
 *    will then find the core vertex and print out all the paths from this 
 *    core vertex.  
 *
 * @author Athit Vue
 * @date 1/27/2018
 */
#include <iostream>
#include <list>    // For list object
#include <queue>   // For queue object
#include <string>
#include <utility> // For pair object
#include <algorithm> // For sort()
#include <functional> // for less
#include <stack>  // For statck
#define DEBUG 0 // Change this number to 1 for debugging outputs
using namespace std;

class Tree{
  public:
    Tree(int num_vertices);
    ~Tree();
    void add_edge(int u, int v);
    void BFS(int source);
    pair<int,int> find_longest_path();
    int get_parent(int v);
    void reset_arrays();
    void set_flag();
    void print(int core);    

  private:
    int num_vertices;
    int* distance;
    int* parents;
    list<int> *neighbors; // Adjacency list    
    bool print_out; // Flag to check if we need to print
};

/* Constructor */
Tree::Tree(int num_vertices){
  this->num_vertices = num_vertices;  
  /* Dynamically allocate memory for adjacency list */
  neighbors = new list<int>[num_vertices];
  print_out = false;
}

/* Destructor */
Tree::~Tree(){
  delete[] neighbors;
  delete[] parents;
  delete[] distance;
}

/**
 * void add_edge
 * 
 * This member function is used to add the edge of vertex u
 * and vertex v. The neighbors of vertex u will be stored in 
 * a list container that variable neighbor points to. 
 * 
 * @param u The vertex u.
 * @param v The vertex v.
 */
void Tree::add_edge(int u, int v){
  // Add v to u's adjacency list
  neighbors[u].push_back(v);
  // Undirected graph so v will also have u as a neighbor
  neighbors[v].push_back(u);
}

/**
 * void BFS
 * 
 * This is my implementation of the BFS. First it allocates memory
 * for the parents and distance arrays. Then it initializes all the
 * elements in the distance array to -1. A -1 is used as the value to 
 * indicate that the index associated with the vertex u has not been
 * visited. If the vertex is visited, then the index of the element 
 * will be set to > -1. A queue container is used to help with keeping
 * track of the visited vertices and an array called parents is used 
 * to keep track of the parent of vertex v. 
 * 
 * @param source The source or vertex to run BFS on.
 */
void Tree::BFS(int source){
  // Dynamically allocated memory for the parent and distance array
  parents = new int[num_vertices];
  distance = new int[num_vertices];

  // Initialize the distance array to -1 for unvisited vertices
  for (int i = 0; i < num_vertices; i++){
    distance[i] = -1;
  }

  // Instiantiate a queue object for storing visited vertices
  queue<int> q;

  // Add one to the current source node (mark as visited)
  distance[source] = distance[source] + 1;

  // Enqueue the source vertex at the end of the queue
  q.push(source);

  // Parent of source is itself
  parents[source] = source;

  // Sort the neighbors list (must or it will print the wrong paths first)
  for (int i = 0; i < num_vertices; i++){
    for (auto it = neighbors[i].begin(); it != neighbors[i].end();
        it++){
      neighbors[i].sort(less<int>());
    }
  }
  
  while (!q.empty()){
    // Get the value of the vertex in the front of the queue call it u
    int u = q.front();
    // Pop it from the queue
    q.pop();
    
    // Check the current vertex's neighbors
    for (auto it = neighbors[u].begin(); it != neighbors[u].end(); it++){
      int v = *it;

      // If neighbors distance -1, then we know that it has not been visted
      if (distance[v] == -1){
        // Enqueue the unvisited neighbor
        q.push(v);
        // Let the neighbor have a distance of plus 1 of u's 
        // current distance; this also marks it as visited
        distance[v] = distance[u] + 1;
        // Update the parent's array, parent of v is u
        parents[v] = u;
      }
    }
  }

  /* KEEP FOR DEBUGGING IN THE FUTURE */
  if (DEBUG){
    /* DEBUG: distance array */ 
    cout << "#### DISTANCE ARRAY ####" << endl;
    for (int i = 0; i < num_vertices; i++){
      cout << "Vertex <" << i << "> has distance = ";
      cout << distance[i] << endl;
    }
    cout << endl;
    /* DEBUG: parents array */
    cout << "#### PARENTS ARRAY ####" << endl;
    for (int i = 0; i < num_vertices; i++){
      cout << "Parent of v <" << i << "> is: ";
      cout << parents[i] << endl;
    }
    cout << endl;
    /* DEBUG: neighbors list */
    cout << "#### ADJACENCY LIST ####" << endl;
    for (int i = 0; i < num_vertices; i++){
      int count = 0;
      cout << "Vertex <" << i << "> has neighbors: ";
      for (auto it = neighbors[i].begin(); it != neighbors[i].end();
        it++){
        cout << *it;
        list<int>::iterator end = neighbors[i].end();
        count++;
        if (count == *end){
          cout << " "; 
        }
        else{
          cout << ", ";
        }
      }
      cout << endl;
    }
  }
}

/**
 * pair<int, int> find_longest_path
 *
 * This member function of the Tree class will iterate thru the 
 * distance array and find the largest value. 
 * 
 * @return vertex The vertex of which has the largest distance.
 * @return larger_distance The largest distance of the graph.
 */
pair<int,int> Tree::find_longest_path(){
  int larger_distance = 0;
  int vertex;
  for (int i = 0; i < num_vertices; i++){
    if (distance[i] > larger_distance){
      larger_distance = distance[i];
      vertex = i;
    }
  }
  // Return the pair of vertex index and distance
  return make_pair(vertex, larger_distance);
}

/**
 * int get_parent
 *
 * A member function to get the parent of v.
 * 
 * @param v The vertex v of which we want to find the parent of.
 * @return u The vertex u of which is the parent of vertex v.
 */
int Tree::get_parent(int v){
  int u;
  u = parents[v];

  if (DEBUG){
    cout << "DEBUGGING::Get_parent(): parent of <" << v << ">";
    cout << " is : <" << u << ">" << endl;
  }  
  return u;
}

/**
 * void reset_arrays
 * 
 * A member function to help reset the distance or parents array.
 * It basically deallocates the memory and resources that is pointed
 * to by the distance and parents variables.
 */
void Tree::reset_arrays(){
  delete[] distance;
  delete[] parents;
}

/**
 * void print
 * 
 * This member function prints the vertices to standard output.
 * A stack container is used to help with the lifo order. 
 * 
 * @param core The core vertex of the graph.
 */
void Tree::print(int core){
  stack<int> print_stack;
  int u;
  int v = 0;
  int count;

  for (int i = 0; i < num_vertices; i++){
    print_stack.push(i);
    u = i;
    count = 1;
    if (core == i){
      continue;
    }
    else{
      for (int j = 0; j < distance[i]; j++){
        v = get_parent(u);
        print_stack.push(v);     
        u = v;      
        count++;
      }
      while (count != 0){
        cout << print_stack.top() << " ";
        print_stack.pop();
        count--;
      }
      cout << endl;
    }
  }
}

/**
 * int main
 *
 * The main function. This function instintiates a Tree object and 
 * asks the user for some inputs (number of vertices and (u, v) that 
 * will help connect the vertices to make a tree). BFS and other 
 * member functions will also be called to help determine the diameter
 * of the graph and find its core. Once all that is done, then it will 
 * call the member function print to print out the paths of the graph
 * starting from the core. 
 * 
 * @return -1 Returns an error.
 * @return 0 Program executed with no problem. 
 */
int main(){
  int u, v, i, num_vertices, source, core;
  pair<int, int> first_endpoint, second_endpoint, core_endpoint;

  // Prompt: enter the number of vertices
  cin >> num_vertices;

  if (num_vertices == 0){
    cerr << "ERROR: graph is empty." << endl;
    return -1;
  }

  // Instantiate a Tree object
  Tree tree(num_vertices);

  // Get user input and add edges to the u and v vertices
  for (i = 0; i < (num_vertices-1); i++){
    cin >> u >> v;
    tree.add_edge(u, v);
  }

  // User does not specify source so we are assuming source will begin
  // from index 0 or vertex 0
  source = 0;
  tree.BFS(source);
  // Get the first end_point
  first_endpoint = tree.find_longest_path(); 
  
  if (DEBUG){
    cout << endl;
    cout << "DEBUGGING::First endpoint is : <" << first_endpoint.first;
    cout << ">" << endl;
  }

  // Reset parents and distance arrays
  tree.reset_arrays();

  if (DEBUG){
    cout << endl;
    cout << "----------Second Endpoint-----------" << endl;
    cout << endl;
  }

  // Now run bfs on the first endpoints vertex to get the second endpoint
  tree.BFS(first_endpoint.first);
  second_endpoint = tree.find_longest_path();

  // Now we can find the core by dividing the diameter (longest path)
  // and divide it by 2
  int h = 0;
  h = second_endpoint.second / 2;

  int end_point = second_endpoint.first;

  if (DEBUG){
    cout << endl;
    cout << "-----------DEBUGGING----------------" << endl;
    cout << endl;
    cout << "DEBUGGING::Second endpoint is : <" << end_point << ">" << endl;
  }

  // Backtrace h edges from the second endpoint using parents array
  for (int i = 0; i < h; i++){  
    core = tree.get_parent(end_point);
    end_point = core;
  }
 
  if (DEBUG){
    cout << "DEBUGGING::Core vertex is : <" << core << ">" << endl;
    cout << endl;
    cout << "DEBUGGING::The longest_path is from vertex <";
    cout << first_endpoint.first;
    cout << "> to vertex <" << second_endpoint.first << ">," << endl;
    cout << "with a distance of <" << second_endpoint.second;
    cout << ">" << endl;
  }

  if (DEBUG){
    cout << endl;
    cout << "---------- Process Core -----------" << endl;
    cout << endl;
  }

  // Reset the parent's and distance's arrays
  tree.reset_arrays();

  // Run BFS on T and the core, use the parent's array to backtrace
  // paths from the core to each vertex in T and print it accordingly
  tree.BFS(core);
  core_endpoint = tree.find_longest_path();
 
  if (DEBUG){
    cout << endl;
    cout << "DEBUGGING::Core max distance is : ";
    cout << core_endpoint.second << endl;
    cout << endl;
  }

  // Print the paths
  tree.print(core);

  return 0;
}
