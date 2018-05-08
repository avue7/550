#include <algorithm>
#include <math.h>
#include <vector>
#include <iostream>
#include <string>
#include <list>
using namespace std;

/*********************************************
1. adjD is the answer: adjacency list containing
selected edges, the max-weight of which you need to output

2. On the way down of recursion
adjD has the same number of vertices as orig graph, but
adjacency lists are empty

3. On the way up of recursion,
adjD grows: we add original edges from the Smallest-half
edges (not always, only in those recursive calls where
graph over smallest-half edges was not connected)

4. Initially, in adjA's vector is indexed by vertices of
original graph, and for each edge (u, v),
adjA[u] has v
adjA[v] has u

5. Initially, adjD's vector has the same size as
the number of vertices in the original graph 
(and this does not change through the algorithm)
***********************************************/
int WHITE = 0;
int GRAY = 1;
int BLACK = 2;

struct vertex
{
public:
    int vertexIndex;
    int color;
    vertex * parent;
    int d; // discovery time
    int f; // finish time
    int ccId;

    vertex() {
        color = WHITE;
        d = 0;
        f = 0;
        vertexIndex = 0;
        ccId = -1;
    }
};

struct edge {
public:
    int u_original;
    int v_original;
    //the one being pointed to
    int v;
    //weight
    int w;

    edge(int u, int v, int w) {
        u_original = u;
        v_original = v;
        this->v = v;
        this->w = w;
    }

    edge() {
        u_original = 0;
        v_original = 0;
        this->v = 0;
        this->w = 0;
    }
};

void MBST(vector<list<edge> > &adjA, vector<list<edge> > &adjD);

int select(vector<int> &v, int start, int fin, int k) {
    int vsize = fin - start + 1;

    if (start == fin)
        return v[start];
    else if (start > fin)
        return -1;

    int msize = ceil((double)vsize/5);

    vector<int> medians(msize, 0);
    int med_ind = 0;
    for(int i = start; i <= fin; i = i + 5) {
        vector<int> temp(5, 0);
        int j = 0;
        for(; j < 5 && (i + j)<= fin; j++) {
            temp[j] = v[i+j];
        }//for j

        if(j < 5)
            temp.resize(j);

        sort(temp.begin(), temp.end());

        if(j == 5)
            medians[med_ind++] = temp[2];
        else {
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
        if(v[i] == x) {
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
        if(v[i] > x && v[j] <= x) {
            t = v[j];
            v[j] = v[i];
            v[i] = t;
            i++;
            j--;
        } else if(v[i] <= x) {
            i++;
            if(i > fin) {
                i--;
                break;
            }
        } else {
            j--;
            if(j == 0) {
                j++;
                break;
            }
        }
    }//while partition

    int last = i;
    if(v[i] > x)
        last--;

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

void collect_weights(vector<list<edge>> &adjList, vector<int> &weights) {
    int weightIndex = 0;
    for (int i = 0; i < (int) adjList.size(); ++i) {
        for (auto it = adjList[i].begin(); it != adjList[i].end(); ++it) {
            //if we haven't seen this edge before
            if (i < (*it).v) {
                weights[weightIndex] = (*it).w;
                weightIndex++;
            }
        }
    }
}

//adjB will contain the edges <= median_weight
void select_smallest(vector<list<edge>> &adjA,  vector<list<edge>> &adjB,
        int median_weight) {
    for (int i = 0; i < (int) adjA.size(); ++i) {
        for (auto j = adjA[i].begin(); j != adjA[i].end();) {
            if ((*j).w <= median_weight) {
                adjB[i].push_back(*j);
                j = adjA[i].erase(j);
            } else {
                ++j;
            }
        }
    }
}

int currentTime;

//dfs but sets the ccId of v to u's ccId
void dfsVisit(vector<list<edge>> &adjList, 
            vector<vertex> &vertices, vertex * u) {
    currentTime = currentTime + 1;
    u->d = currentTime;
    u->color = GRAY;
    
    for (auto it = adjList[u->vertexIndex].begin(); it != adjList[u->vertexIndex].end(); ++it) {
        vertex * v = &vertices[(*it).v];

        v->ccId = u->ccId;

        if (v->color == WHITE) {
            v->parent = u;
            dfsVisit(adjList, vertices, v);
        }
    }

    u->color = BLACK;
    currentTime = currentTime + 1;
    u->f = currentTime;
}

int dfs(vector<list<edge>> &adjList, vector<vertex> &vertices) {
    currentTime = 0;

    for (int i = 0; i < (int) vertices.size(); ++i) {
        /*for (auto j = adjList[i].begin(); j != adjList[i].end();) {
            (*j).parent = (*j).v;
            (*j).color = WHITE;
        }*/
        vertices[i].parent = &vertices[i];
        vertices[i].vertexIndex = i;
        vertices[i].color = WHITE;
    }

    int nextBccId = 0;
    for (int i = 0; i < (int) vertices.size(); ++i) {
        vertex * u = &vertices[i];

        if (u->color == WHITE) {
            u->ccId = nextBccId;
            nextBccId++;
            dfsVisit(adjList, vertices, u);
        }
    }

    return nextBccId;
}

//returns how many connected components there are
int connected(vector<list<edge>> &adjB, vector<vertex> &vertices) {
    return dfs(adjB, vertices);
}

int main() {
    //# of vertices
    int n;
    cin >> n;

    //# of edges
    int m;
    cin >> m;

    vector<list<edge>> adjA(n);

    //cout << "about to start reading in edges" << endl;

    //read in the edges
    for (int i = 0; i < m; ++i) {
        int x, y, w;
        cin >> x;
        cin >> y;
        cin >> w;

        adjA[x].push_back(edge(x, y, w));
        adjA[y].push_back(edge(y, x, w));
    }

    //cout << "done reading in edges" << endl;

    vector<list<edge>> adjD(n);
    MBST(adjA, adjD);

    //cout << "mbst finished" << endl;

    int theMax = -1000000;
    for (int i = 0; i < (int) adjD.size(); ++i) {
        for (auto j = adjD[i].begin(); j != adjD[i].end(); ++j) {
            theMax = max(theMax, (*j).w);
        }
    }

    //print the max weight found in the mbst
    cout << theMax << endl;
}

void MBST(vector<list<edge> > &adjA, 
                vector<list<edge> > &adjD) {
        //Count the number of edges in adjA
        int m = 0;
        int size = adjA.size();
        for(int i = 0; i < size; i++){
            m += adjA[i].size();
        }
        //at this point m is the number of edges in adjA (times 2)

        m = m >> 1;//divide by 2, each edge occurs twice in Adj

        //cout << "\n--new iteration-- and m is " << m << endl;

        //BASE case, if number of edges is 1, return
        //after adding the first edge to adjD
        if (m == 1) {
            //Here you need to add edge to adjD
            //from adjA (add original name of the edge)

            for(int i = 0; i < size; i++) {
                if (adjA[i].size() > 0) {
                    adjD.push_back(adjA[0]);
                    break;
                }
            }

            return;
        }

        if (m == 0) {
            //cout << "error: m shouldn't equal zero" << endl;
            return;
        }

        /***********   Collect weights of edges **********/
        vector<int> weights(m, 0);
        //You need to write this function:
        collect_weights(adjA, weights);

        //cout << "weights collected are: " << endl;
        for (int i = 0; i < (int) weights.size(); ++i) {
            //cout << "weight: " << weights[i] << endl;
        }
        //cout << endl;

        /*******   Find the median of weights *********/
        int k = m >> 1;//divide by 2
        if(m % 2 == 0)
            k--;
        int median_weight = select(weights, 0, m-1, k);
        //Function "select" is provided

        //cout << "median is " << median_weight << endl;

        
        /*****  Collect edges with weights <= median_weight
                from adjA into adjB
                Erase an edge from adjA:
                it takes O(1) to erase (use erase) from
                a list
        *************************************************/
        vector< list<edge> > adjB(adjA.size());//smallest edges

        //You need to write this function:
        //adjB will contain the edges <= median_weight
        select_smallest(adjA, adjB, median_weight);

        //cout << "the weights <= median done being selected. they are: "<< endl;

        for (int i = 0; i < (int) adjB.size(); ++i) {
            for (auto j = adjB[i].begin(); j != adjB[i].end(); ++j) {
                //cout << "small weight " << (*j).w << " from " << i << " to " << (*j).v << endl;
            }
        }



        /******* Find connected components in adjB 
                in linear time 
         ***************************************/
        vector<vertex> vertices((int) adjB.size());
        int total_cc = connected(adjB, vertices);//number of comp

        //cout << "num of connected components: " << total_cc << endl;



        /**********     If connected (only one component)
                we don't need edges of A anymore
                just call recursively MBST and return 
                after the recursive call returns 
                i.e. do nothing after recursive call returns
        ****************************************/

        if(total_cc == 1) {
            MBST(adjB, adjD);
            return;
        } else {
            //adjA is the ones bigger than median and thrown out
            //adjB is <= median
            //adjD will be returned
            
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
                vector< list<edge> > adjC(total_cc);
                int nextBccId = total_cc;

                //for the edges <= median
                for (int i = 0; i < (int) adjA.size(); ++i) {
                    int uCcId = vertices[i].ccId;
                    if (uCcId == -1) {
                        uCcId = nextBccId;
                        vertices[i].ccId = nextBccId;
                        nextBccId++;
                    }

                    for (auto j = adjA[i].begin(); j != adjA[i].end(); ++j) {
                        int vCcId = vertices[(*j).v].ccId;

                        if (vCcId == -1) {
                            vCcId = nextBccId;
                            vertices[(*j).v].ccId = nextBccId;
                            nextBccId++;
                        }

                        ////cout << "same bcc ids of: " << uCcId << " to " << vCcId << endl;

                        //(*j) is the edge from u to v

                        if (uCcId != vCcId) {
                            ////cout << "dif bcc ids of: " << uCcId << " to " << vCcId << endl;

                            edge e(uCcId, vCcId, (*j).w);
                            e.u_original = (*j).u_original;
                            e.v_original = (*j).v_original;
                            adjC[uCcId].push_back(e);

                            /*edge e2(vCcId, uCcId, (*j).w);
                            e2.u_original = (*j).v_original;
                            e2.v_original = (*j).u_original;
                            adjC[vCcId].push_back(e2);*/
                        }
                    }
                }
                //adjC with contracted vertices is now complete


                MBST(adjC, adjD);

                //Add edges of adjB (use original names) into adjD
                for (int i = 0; i < (int) adjB.size(); ++i) {
                    for (auto j = adjB[i].begin(); j != adjB[i].end(); ++j) {
                        //(*j) is the edge from u to v

                        edge e = *j;
                        e.v = (*j).v_original;

                        adjD[(*j).u_original].push_back(e);
                    }
                }

                return;
        }//else not connected
}//MBST recursive
