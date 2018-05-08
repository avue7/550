/**
 * @file proj1.cpp
 * @brief
 * 	This program implements finding MBST using camerini's 
 * 	algorithm. BFS is used in conjunction to connect the graphs
 * 	as well as to find the connected components for camerini's 
 * 	algorithm. 
 * 
 * @author Athit Vue
 * @date 02/06/2018
 */

#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <algorithm>
#include <list>
#include <assert.h>
#include <queue>
#include <limits.h>
#define DEBUG 0 // DEBUGGER: change to 1 for debugging.
using namespace std;

struct edge{
  public:
    int v; // Destination vertex
    int weight; // Weight at destination vertex
    int u_original;
    int v_original;
  
    // Default constructor.
    edge(){
      this->v = 0;
      this->weight = 0;
      u_original = 0;
      v_original = 0;
    }
    // Overloaded constructor.
    edge(int u, int v, int w){
      this->v = v;
      this->weight = w;
      u_original = u;
      v_original = v;
    }
};

/**
 * void collect_weights
 *
 * This function is called from MBST() to collect the weights
 * of the vertices that exists in adjA list. The vector weights
 * will record all of the weights that exists on each edge.
 *
 * @param adjA A reference to the address of lists in vector adjA.
 * @param weights A reference to the address of vector weights.
 */
void collect_weights(vector<list<edge>> &adjA, vector<int> &weights){
  // **Important: vector weights needs its own index for the number 
  // of weights collected (will not work correctly without this).
  int temp_index;
  temp_index = 0;  

  // First, iterate thru all adjA vertices
  for (int i = 0; i < (int) adjA.size(); i++){
    // Next, check each neighbor for weight and record it
    for (auto it = adjA[i].begin(); it != adjA[i].end(); ++it){
      // Make sure we record the weight of each edge only once.
      // Check to make sure that we have not visited a neighbor before.
      // Since we know that our vertices has children in increasing 
      // index, we can just check to see if current u, parent, is greater
      // than v (neighbor). If u is greater than neighbor, then it must be
      // true that we have visited the edge on (u, v) before. So don't 
      // record this edge another time; basically, don't record (v, u). 
      if (i < (*it).v){
        weights[temp_index] = (*it).weight;
        // Increment the index for vector weights.
        temp_index++;
      }
    } 
  }
}

/**
 * int select (function provided by: Dr. Harris)
 *
 * Finds the k-th element in the vector v using O(n)-time.
 * Note: initally, start = 0 and fin = v.size()-1.
 *
 * @param v
 * @param start The starting index in the range in v.
 * @param fin The last index in the range in v.
 * @param k The middle index computed from MBST.
 * @return -1 Error
 * @return >0 Success, returns the selected median
 */
int select(vector<int> &v, int start, int fin, int k){

    int vsize = fin - start + 1;

    if(start == fin)
        return v[start];
    else if(start > fin)
        return -1;
	
	if(vsize <= 5){
		sort(v.begin() + start, v.begin() + fin + 1);

		return v[k];
	}
	
	int msize = ceil((double)vsize/5);

    vector<int> medians(msize, 0);
    int med_ind = 0;
    for(int i = start; i <= fin; i = i + 5){
        vector<int> temp(5, 0);
        int j = 0;
        for(; j < 5 && (i + j)<= fin; j++){
            temp[j] = v[i+j];
        }//for j
        if(j < 5)
            temp.resize(j);
        sort(temp.begin(), temp.end());
        if(j == 5)
            medians[med_ind++] = temp[2];
        else{
            int jmid = j >> 1;//divide by 2
            if(j % 2 == 0)
                jmid--;
            medians[med_ind++] = temp[jmid];

            }
    }//for i
    //so far we selected medians of n/5 groups of 5 elem
    int mid = msize >> 1; //divide by 2
    if((msize % 2) == 0)
        mid--;
    int x = select(medians, 0, msize - 1, mid);
    //partition v's elements around x
    //find x
    int indx = -1;
    for(int i = start; i <= fin; i++){
        if(v[i] == x){
            indx = i;
            break;
            }
    }//for i
    if(indx == -1)
        return -2;
    int t = v[start];
    v[start] = x;
    v[indx] = t;
    int i = start + 1, j = fin;
    while(i < j){
        if(v[i] > x && v[j] <= x){
            t = v[j];
            v[j] = v[i];
            v[i] = t;
            i++;
            j--;
        }//if
        else if(v[i] <= x){
            i++;
            if(i > fin){
                i--;
                break;
            }
            }
        else{
            j--;
            if(j == 0){
                j++;
                break;
            }
            }
    }//while partition
    int last = i;
    if(v[i] > x)
        last--;//last index that has less than or equal elemenents than x
//count how many elements are equal to x
    int count_equal = 0;	
    for(int u = start; u <= last; u++){
        if(v[u] == x)
            count_equal++;
    }//for u
    
    if(k > (last - count_equal) && k <= last)
        return x;
    else if(last < k)
        return select(v, last + 1, fin, k);
    else
        return select(v, start, last, k);
}//select

/**
 * void select_smallest
 *
 * If median weight is m, then this function will select the edges
 * that have weights <= m, and store them into the vector of list adjB.
 *
 * @param adjA The address of vector of list adjA.
 * @param adjB The address of the vector of list adjB.
 * @param med_weight The address of the median weight found in MBST().
 */
void select_smallest(vector<list<edge>> &adjA, 
  vector<list<edge>> &adjB, int &med_weight){
  // We are storing the edges from adjA so we need to iterate thru ajdA.
  // First iterate thru vertices U (parents).
  for (int i = 0; i < (int) adjA.size(); i++){
    // Next, check its neighbors for vertex that has weights
    // less than med_weight and store it in adjB.
    for (auto it = adjA[i].begin(); it != adjA[i].end();){
      // Check for weights less than med_weight
      if ((*it).weight <= med_weight){
        edge temp_edge = *it;
        adjB[i].push_back(temp_edge);
        // Now erase that edge from adjA's list using erase (as specified).
        it = adjA[i].erase(it); 
      }
      else{
        it++;
      }
    }
  }
}

/**
 * int BFS
 *
 * This function is does a breadth first search on vertex u, 
 * that is passed in as a parameter and finds the connected 
 * components. It will set each vertices in the cc vector to
 * the appropriate connected component index.
 *
 * @param adjB The address of vector list adjB.
 * @param cc The address of vector cc.
 * @param vertex The address of the current vertex.
 * @param component_index The address of the current comp. index.
 */
void BFS(vector<list<edge>> &adjB, vector<short int> &cc,
	int &vertex, int &component_index){
  // Put vertex in the current component_index
  cc[vertex] = component_index;

  queue<int> q;

  q.push(vertex);

  while (!q.empty()){
    int u = q.front();
    q.pop();
    
    // Check the current vertex's neighbors
    for (auto it = adjB[u].begin(); it != adjB[u].end(); it++){
      int v = (*it).v;
      
      // If neighbor == -1, then it doesn't belong to any cc yet.
      if (cc[v] == -1){
        if (DEBUG){
          cout << "Vertex not seen yet is: " << v << endl;
        }
        // Push the neighbor v not seen to the queue
        // and set it to the current component index.
        q.push(v);
        cc[v] = component_index;
      }
    } 
  }
}

/**
 * int connected
 *
 * This function calls the BFS function and returns the number 
 * of connected components.
 *
 * @param adjB The address of vector list adjB.
 * @param cc The address of cc vector.
 * @return component_count The number of components after running BFS.
 */
int connected(vector<list<edge>> &adjB, vector<short int> &cc){
  int component_count = 0;
  int num_vertices;
  num_vertices = (int) adjB.size();
  
  // Initialize all vertices of vector cc to -1.
  for (int i = 0; i < num_vertices; i++){
    cc[i] = -1;
  }
  
  if (DEBUG){
    cout << endl;
    cout << "###  CC_VERTICES INITIALIZED ###" << endl;
    for (int i = 0; i < (int) cc.size(); i++){
      cout << i << ": " << cc[i] << endl; 
    }
  }

  // Iterate thru all vertices and find their connected component id.
  for (int vertex = 0; vertex < num_vertices; vertex++){
    // If vertices doesn't belong to any cc, then run BFS.
    if (cc[vertex] == -1){
      BFS(adjB, cc, vertex, component_count);
      component_count++; 
    }
  }
  
  if (DEBUG){
    cout << endl;
    cout << "### CONNECTED COMPONENTS AFTER BFS ###" << endl;
    for (int i = 0; i < num_vertices; i++){
      cout << "Vertex <" << i << "> belongs to cc : " << cc[i] << endl;
    }
    cout << endl;
  }
  
  return component_count;
}

/**
 * void print_out
 * 
 * This function prints the max weight of MBST.
 *
 * @param adjD The address to adj D.
 */
void print_out(vector<list<edge>> &adjD){
  int max_weight = -INT_MAX;
  for (int i = 0; i < (int) adjD.size(); ++i) {
    for (auto it = adjD[i].begin(); it != adjD[i].end(); it++){
      max_weight = max(max_weight, (*it).weight);
    }
  }
  cout << max_weight << endl;
}

/**
 * void MBST
 *
 * Skeleton code provided by Professor Harris.
 * 1. adjD is the answer: adjacency list containing
 * selected edges, the max-weight of which you need to output
 *
 * 2. On the way down of recursion
 * adjD has the same number of vertices as orig graph, but
 * adjacency lists are empty

 * 3. On the way up of recursion,
 * adjD grows: we add original edges from the Smallest-half
 * edges (not always, only in those recursive calls where
 * graph over smallest-half edges was not connected)

 * 4. Initially, in adjA's vector is indexed by vertices of
 * original graph, and for each edge (u, v),
 * adjA[u] has v
 * adjA[v] has u

 * 5. Initially, adjD's vector has the same size as
 * the number of vertices in the original graph 
 * (and this does not change through the algorithm)
 *
 * @param adjA The address of vector containing the original list 
 * of struct edge.
 * @param adjD The address of the vector containing the selected edges
 * that will need to be output.
 */
void MBST( vector<list<edge> > &adjA, 
                vector<list<edge> > &adjD){
        //Count the number of edges in adjA
        int m = 0;
        int size = adjA.size();

        for(int i = 0; i < size; i++){
                m += adjA[i].size();
        }   
        m = m >> 1;//divide by 2, each edge occurs twice in Adj
        //BASE case, if number of edges is 1, return
        //after adding the first edge to adjD
        if(m == 1){
            //Here you need to add edge to adjD
            //from adjA (add original name of the edge)

            // If a original vertex has neighbor then add the edge to 
            // adjD (the new subgraph).
            for (int i = 0; i < size; i++){
              // Graph should be connected when running MBST.
              // If not, we can't continue (something is wrong).
              if (DEBUG){
                assert ((adjA[i].size() != 0) && 
                "Edge == 1, graph must be connected to run MBST!");
              }
              if (adjA[i].size() > 0){
                adjD.push_back(adjA[0]);
                break;
              }
            }
            return;
        }

        /***********   Collect weights of edges **********/

        // M = the index of all weights, initialized to 0
        vector<int> weights(m, 0);
        //You need to write this function:
        collect_weights(adjA, weights);

        if (DEBUG){
          cout << endl;
          cout << "DEBUG::MBST(): the <";
          cout << weights.size() << "> weights collected are: " << endl;
          cout << "  ";
          for (auto it = weights.begin(); ; it++){
            cout << *it;
            if (it == (weights.end()-1)){
              cout << ". ";
              break;
            }
            else{
              cout << ", ";
            }
          }
          cout << endl;
        }

        /*******   Find the median of weights *********/
        int k = m >> 1;//divide by 2
        if(m % 2 == 0)
                k--;
        int median_weight = select(weights, 0, m-1, k);
        //Function "select" is provided
        
        if (DEBUG){
          cout << endl;
          cout << "DEBUG::MBST(): the median weight after select()";
          cout << " is called is: ";
          cout << median_weight << endl;
        }
        
        /*****  Collect edges with weights <= median_weight
                from adjA into adjB
                Erase an edge from adjA:
                it takes O(1) to erase (use erase) from
                a list
        *************************************************/
        vector< list<edge> > adjB(adjA.size());//smallest edges

        //You need to write this function:
        select_smallest(adjA, adjB, median_weight);

        /*** ADJ_A ***/
        if (DEBUG){
          cout << endl;
          cout << "#### ADJ_A LIST (size: " << (int) adjA.size();
          cout << ") AFTER CALLING SELECT_SMALLEST ####" << endl;
          for (int i = 0; i < (int) adjA.size(); i++){
            cout << "Vertex <" << i << "> : ";
            for (auto it = adjA[i].begin(); it != adjA[i].end(); it++){
              cout << (*it).v << "(" << (*it).weight << "), ";
            }
            cout << endl;
          }
        }
        /** ADJ_B ***/
        if (DEBUG){
          cout << endl;
          cout << "#### ADJ_B LIST (size: " << (int) adjB.size();
          cout << ") AFTER CALLING SELECT_SMALLEST ####" << endl;
          for (int i = 0; i < (int) adjB.size(); i++){
            cout << "Vertex <" << i << "> : ";
            for (auto it = adjB[i].begin(); it != adjB[i].end(); it++){
              cout << (*it).v << "(" << (*it).weight << "), ";
            }
            cout << endl;
          }
        }

        /******* Find connected components in adjB 
                in linear time 
         ***************************************/

        // Create vector of list to store vertices of BFS
        // connected components
        //vector<list<vertex>> cc((int) adjB.size());

        vector<short int> cc(adjB.size());
        int total_cc = connected(adjB, cc);//number of comp
        
        if(DEBUG){
          cout << "TOTAL COMPONENTS = : " << total_cc << endl;
        }

        /**********     If connected (only one component)
                we don't need edges of A anymore
                just call recursively MBST and return 
                after the recursive call returns 
                i.e. do nothing after recursive call returns
        ****************************************/

       if(total_cc == 1){
               MBST(adjB, adjD);
               return;
       }else{
        
            /******     If not connected, 
                contract connected components
                construct adjC from adjA and 
                super vertices (each represents a 
                connected component)
                
                Rename edges according to the names
                of new supervertices (0, 1,.., total_cc-1)
                call recursively MBST on adjC: 
				
				MBST(adjC, adjD);
                
                When call returns, 
                Add edges of adjB (use original names)
                into adjD
            *****************************************/ 
	       // Note: adjA: > median_weight
               
               // First, create adjC
               int size_of_adjC = total_cc;
               vector<list<edge>> adjC(size_of_adjC);

               // Contract for the edges that are <= median_weight.
               for (int i = 0; i < (int) adjA.size(); i++){
                 int u_cc_index;
                 u_cc_index = cc[i];
                 for (auto it = adjA[i].begin(); it != adjA[i].end(); it++){
                   int v_cc_index;
                   v_cc_index = cc[(*it).v];
                   // Connect the components if index are different
                   if (u_cc_index != v_cc_index){
                     // Set new names
                     edge new_edge(u_cc_index, v_cc_index, (*it).weight);
                     new_edge.u_original = (*it).u_original;
                     new_edge.v_original = (*it).v_original;
                     adjC[u_cc_index].push_back(new_edge);
                   }
                 }
               }

              // Call recursive MBST on adjC.
              MBST(adjC, adjD);

              // Call returns, add edges of adjB to adjD, using original
              // names.
              for (int i = 0; i < (int) adjB.size(); i++){
                for (auto it = adjB[i].begin(); it != adjB[i].end(); it++){
                  edge new_edge = *it;
                  new_edge.v = (*it).v_original;
                  adjD[(*it).u_original].push_back(new_edge);
                }
              }

             return;
        }//else not connected
}//MBST recursive

/**
 * int main
 *
 * Takes in input for original graph and calls recursive MBST
 * to find the maximum weight of the MBST.
 *
 * @return 0 The program excecuted correctly
 */
int main(){
  int num_vertices, num_edges;
  int u, v, w; 

  // Get number of vertices and edges.
  cin >> num_vertices >> num_edges;

  // AdjA is the original graph.
  vector<list<edge>> adjA(num_vertices);

  // AdjD is the list containing the MBST;
  // the max-weight of MST's that needs to output.
  vector<list<edge>> adjD(num_vertices);
    
  // Get the vertices u v and the weight
  for (int i = 0; i < num_edges; ++i){
    cin >> u >> v >> w;

    // AdjA[u] will have edge(u,v) and adj[v] will have edge(v,u).
    adjA[u].push_back(edge(u,v,w));
    adjA[v].push_back(edge(v,u,w));     
  }

  /* KEEP FOR DEBUGGING IN THE FUTURE */
  if (DEBUG){
    cout << endl;
    /* DEBUG: neighbors list */
    cout << "#### ADJACENCY LIST ####" << endl;
    for (int i = 0; i < num_vertices; i++){
      cout << "Vertex <" << i << "> : ";
      for (auto it = adjA[i].begin(); it != adjA[i].end(); it++){
        cout << (*it).v << "(" << (*it).weight << "), ";
      }
      cout << endl;
    }
  }

  // Call Minimum Bottleneck Spanning Tree to find the max length of the 
  // min spanning trees.
  MBST(adjA, adjD);

  // All is done when we reach here, print out max of min weights
  print_out(adjD);
 
  return 0;
}
