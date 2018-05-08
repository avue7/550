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

            .................................

            return;
        }

        /***********   Collect weights of edges **********/
        vector<int> weights(m, 0);
        //You need to write this function:
        collect_weights(adjA, weights);



        /*******   Find the median of weights *********/
        int k = m >> 1;//divide by 2
        if(m % 2 == 0)
                k--;
        int median_weight = select(weights, 0, m-1,k);
        //Function "select" is provided

        
        /*****  Collect edges with weights <= median_weight
                from adjA into adjB
                Erase an edge from adjA:
                it takes O(1) to erase (use erase) from
                a list
        *************************************************/
        vector< list<edge> > adjB(adjA.size());//smallest edges

        //You need to write this function:
        select_smallest(adjA, adjB, median_weight);


        /******* Find connected components in adjB 
                in linear time 
         ***************************************/
        vector<short int> cc(adjB.size());
        int total_cc = connected(adjB, cc);//number of comp



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

                return;
        }//else not connected
}//MBST recursive
