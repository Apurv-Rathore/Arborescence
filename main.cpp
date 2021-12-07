#include<queue>
#include <bits/stdc++.h>
#include<vector>

#define fo(i,n) for(int i=0;i<n;i++)
using namespace std;

map<int,int> arbo(map<int,vector<pair<int,int>>> adj_in_list, int s, int total , int n);
void solve();
pair<int,int> isCyclic(int V, vector<int> adj[],int presentinthegraphfordetectingcycle[]);
pair<int,int> isUtil(int V,vector<int> adj[] , int node , bool vis[], bool stack[]);
bool sortbysec(const pair<int,int> &a, const pair<int,int> &b) 
{
    return (a.second < b.second); 
}

int main()
{
    int t;
    //no of test cases
    cin>>t;
    while(t--) solve();
    return 0;
}


void solve()
{
        int n,m,s,from,to,edgeweight,i,j,total;
        cin>>n>>m>>s;
        total = n+1;
        int adj_matrix[n+1][n+1];
        //will store the initial weights of all the edges
        map<int,vector<pair<int,int>>> adj_in_list1;
        //fo(i,n+1) adj_in_list.push_back(helper);
        //adj_in_list will contain the to-> from1, from2 , from3 ....
        fo(i,n+1)
        {
            fo(j,n+1)
            {   
                adj_matrix[i][j]=-1;//means edge not present initially
            }
        }
        int incomingnumber[n+1];
        for (int i=0;i<=n;i++) incomingnumber[i]=0;
        int negweightpresent = 0;
        vector<int>  toremove;
        //if any negative edege weight is present, then print -1
        fo(i,m)
        {
            cin>>from>>to>>edgeweight;
            //if (to==s) continue;
            if (edgeweight<0) negweightpresent = 1;
            else incomingnumber[to]+=1;
            if(adj_matrix[from][to]==-1) adj_matrix[from][to]=edgeweight;
            else
            {
                adj_matrix[from][to]=min(adj_matrix[from][to],edgeweight);
            }
            if (from==to) continue;
            adj_in_list1[to].push_back({from,edgeweight});
        }

        map<int,vector<pair<int,int>>> adj_in_list;
        for (auto p:adj_in_list1)
        {
            int to = p.first;
            if (to!=s and incomingnumber[to]==0) continue;
            for (auto p1:p.second)
            {
                if (to==p1.first) continue;
                //cout<<"-> from to "<<p1.first<<" "<<to<<endl;
                if (incomingnumber[p1.first]!=0 or p1.first==s)
                {
                    adj_in_list[to].push_back(p1);
                }
            }
        }


        if (negweightpresent==1)
        {
            cout<<-1<<endl;
            return;
        }
        for (int i=1;i<=n;i++)
        {
            if(i!=s)
            {
                if (incomingnumber[i]==0) toremove.push_back(i);
            }
        }        
        //basically total is the highest order of node present in the graph
        map<int,int> parent = arbo(adj_in_list,s,total,n);
        //we get the parent map
        //cout<<"the final parent array size :"<<parent.size()<<endl;
        //PROBLEM-----------PARENT ARRAY SIZE IS LESS THAN ACTUAL => solved
        //we have the parent map , total sum of weights
        //just we have to construct the graph
        int answer = 0;
        //cout<<"parent of n"<<parent[n]<<endl;
        //cout<<"parent size "<<parent.size()<<endl;
        vector<pair<int,int>> answergraph[n+1];
        //cout<<"n is :"<<n<<endl;
        for (auto p:parent)
        {
            int to = p.first;
            int from = p.second;
            //cout<<"to "<<to<<" from "<<from<<endl;
            //if (from<1 or from>n or to<1 or to>n) continue;
            //cout<<"from to "<<from<<" "<<to<<endl;
            answer+=adj_matrix[from][to];
            answergraph[from].push_back({to,adj_matrix[from][to]});
        }
        //finding the distance of nodes from s
        int distance[n+1];
        queue<int> q;
        q.push(s);
        int cnt = 0;
        for (int i=0;i<=n;i++) distance[i]=-1;
        distance[s] = 0;
        while(q.size())
        {
            for (int i=0;i<q.size();i++)
            {   
                int papa = q.front();
                q.pop();
                for (auto p:answergraph[papa])
                {
                    int child = p.first;
                    distance[child] = adj_matrix[papa][child]+distance[papa];
                    q.push(child);
                }
            }
        }


        // for (auto i:toremove) 
        // {
        //     distance[i]=-1;
        //     parent[i]=-1;
        // }
        for (auto i:toremove) 
        {
            distance[i]=-1;
            parent[i]=-1;
        }
        distance[s]=0;
        parent[s]=0;
        cout<<answer<<" ";
        for (int i=1;i<=n;i++) {
        
        cout<<distance[i]<<" ";}
        cout<<"# ";
        for (int i=1;i<=n;i++) cout<<parent[i]<<" ";
        cout<<endl;
}
pair<int,int> isUtil(int V,vector<int> adj[] , int node , bool vis[], bool stack[])
{
    stack[node] = vis[node] = true;
    //if (V==112) {cout<<"deb in cycle node"<<node<<endl;cout<<adj[1].size()<<endl;}
    
    for (auto child:adj[node])
    {
        if (vis[child]==false)
        {
            pair<int,int> pa = isUtil(V,adj,child,vis,stack);
            if (pa!=make_pair(-1,-1)) return pa;
        }
        else if (stack[child]==true) {return {node,child};}
    }
    stack[node] = false;
    return make_pair(-1,-1);
}
pair<int,int> isCyclic(int V, vector<int> adj[], int presentinthegraphfordetectingcycle[])
{
    bool vis[V];
    bool stack[V];
    for (int i=1;i<=V;i++)
    {
        vis[i] = false;
        stack[i] = false;
    }
    
    for (int i=1;i<=V;i++)
    {

        if (vis[i]==false && presentinthegraphfordetectingcycle[i]==1)
        {
            pair<int,int> pa = isUtil(V,adj,i,vis,stack);
            if (pa!=make_pair(-1,-1)) return pa;
        }
        
    }
    return {-1,-1};
}

map<int,int> arbo(map<int,vector<pair<int,int>>> adj_in_list, int s, int total , int n)
{
    //cout<<total-n<<endl;
    //first finding the min incoming edge from the adj_in_list, then subtracting the minimum from the rest 
    int i;//used as iterator
    int min_incoming[total+1];//stores the minimum incoming edges
    map<int,int> parent; //our function will return this
    vector<int> normal_adj_list[total+1];//will be used to check for cycle

    //  problem_____________________________________________________________________
    //1. parent map is not correct =>solved
    //2. adj_in_list is not correct =>solved

    //cout<<"size of adj_in_list :"<<adj_in_list.size()<<endl;
    int presentinthegraphfordetectingcycle[total+1];
    for (int i=0;i<=total;i++) presentinthegraphfordetectingcycle[i]=-1;
    presentinthegraphfordetectingcycle[s]=1;
    for(auto &p:adj_in_list)
    {
        int to = p.first;
        if (s!=to)
        {
            //cout<<"deb"<<endl;
            sort(p.second.begin(),p.second.end(),sortbysec);
            //sorting to get the minimumm at the first
            min_incoming[to] = p.second[0].second;
            parent[to] = p.second[0].first;
            //cout<<"deb "<<p.second[0].first<<endl;
            normal_adj_list[parent[to]].push_back(to);
            presentinthegraphfordetectingcycle[parent[to]]=1;
            //cout<<" normal_adj_list will contain from , to  : "<<parent[to]<<" "<<to<<endl;
            //cout<<"from to "<<parent[to]<<" "<<to<<endl;
            for (auto &p1:p.second) p1.second-=min_incoming[to];
        }
    }
    //till now we have made parent array , min_incoming array , normal_adj_list for checking the cycles
    //now we have to find if any cycle exists or not
    //if (total-n==12) cout<<"dub"<<endl;//problem in isCyclic
    pair<int,int> cycleutil = isCyclic(total,normal_adj_list,presentinthegraphfordetectingcycle);
    bool cyclepresent = true;
    if (cycleutil==make_pair(-1,-1)) cyclepresent = false;
    vector<int> nodes_in_cycle;
    //if (total==n) cout<<"size of parent "<<parent.size()<<endl;
    //cout<<"is cycle persent "<<cyclepresent<<endl;
    // for (auto p:parent)
    //     {
    //         int to = p.first;
    //         int fro = p.second;
    //         cout<<"to "<<to<<" from "<<from<<endl;
    //         cout<<"-----------"<<endl;

    //     }
    if (cyclepresent==false) return parent;
    //cout<<"cycle present"<<endl;
    int child = cycleutil.first;
    int ancesstor = cycleutil.second;
    int temp = child;
    nodes_in_cycle.push_back(temp);
    //cout<<" child "<< child <<" ancesstor "<<ancesstor<<endl;
    //map<int , int > presentincycle;
    bool presentincycle[total+1];
    for(int i=0;i<=total;i++) presentincycle[i]=false;
    presentincycle[temp]=true;
    map<int,int> edgesincycle;
    edgesincycle[ancesstor]=temp;
    while(temp!=ancesstor)
    {
        edgesincycle[temp]=parent[temp];
        temp = parent[temp];
        presentincycle[temp]=true;
        nodes_in_cycle.push_back(temp);
    }

    //cout<<"nodes_in_cycle size : "<<nodes_in_cycle.size()<<endl;
    //now as a cycle is present and we have all the nodes present in the cycle , hence what we do is that we contract the cycle to from a supernode
    //total = total + 1;//increasing total nodes
    //new_for_rec is the new adj_list created in order to call for recursion with it
    //fo(i,total+1) new_for_rec.push_back(helper);
    //now creating the new graph
    //nodes in cycle
    //cout<<"nodes in cycle are : ";
    //for (auto i:nodes_in_cycle) cout<<i<<" ";
    //cout<<endl;
    map<int,vector<pair<int,int>>> new_for_rec;
    for (auto p:adj_in_list) 
    {
        int to = p.first;
        for (auto p1:p.second)
        {
            //p is vector of pairs
            //p1 is a pair
            int from = p1.first;
            int edgeWeight = p1.second;
            //now we have to make cases and check if they lie on cycle or not
            //both lie on cycle and have 0 weight
            //both lie on cycle and don't have 0 weight
            //edge from cycle to other
            //edge from other to cycle
            //edge from other to other
            //cout<<"for new_rec from "<<from<<" to "<<to<<endl;
            if (!presentincycle[to] and !presentincycle[from]) {new_for_rec[to].push_back({from,edgeWeight});}
            else
            {
                if (presentincycle[to] and !presentincycle[from]) 
                {
                    new_for_rec[total].push_back({from,edgeWeight});
                    //cout<<"from to  deb "<<from<<" "<<to<<endl;
                }
                else if (!presentincycle[to] and presentincycle[from]) 
                {    
                    //cout<<"yes2 ------------------------"<<total<<" "<<to<<endl;
                    //cout<<"to deb "<<to<<" "<<total<<endl;
                    new_for_rec[to].push_back({total,edgeWeight});
                }
                //else if (edgeWeight==0) {edgesincycle[to]=from;}
            }
        }
    }
    //cout<<"size of new_for_rec : "<<new_for_rec.size()<<endl;



    //now we have compressed the node and now we just have to call recursion
    //cout<<"---------------------------------------------"<<endl;
    //cout<<"---------------------------------------------"<<total<<endl;
    map<int,int> newparent = arbo(new_for_rec,s,total+1,n);
    //cout<<"---------------------------------------------"<<endl;
    //total-=1;
    //if (total==n+1)cout<<"newparent size: "<<newparent.size()<<endl;
    // for (auto p:newparent)
    // {
    //     cout<<" from "<<p.second<<" to "<<p.first<<endl;
    // }
    //now we have to merge
    int parentofsuper = newparent[total];
    //cout<<"parentofsuper "<<parentofsuper<<endl;
    //now finding to which node does the new parent node of the super node points to
    int tempweight = 1e7;
    int sonofparentofsuper;
    //sonofparentofsuper is the son node of parentof super node and in cycle
    for (auto node:nodes_in_cycle)
    {
        //cout<<"node "<<node<<endl;
        for (auto p:adj_in_list[node])
        {
            if (p.first==parentofsuper)
            {
                if(tempweight>p.second)
                {
                    tempweight = p.second;
                    sonofparentofsuper = node;
                }
            }
        }
    }
    map<int,int> finalmap;
    finalmap[sonofparentofsuper]=parentofsuper;
    //cout<<"sonofparentofsuper "<<sonofparentofsuper<<endl;
    //now with the help of newparent we have to uncompress the cycle
    //cout<<"edgesincycle.size() : "<<edgesincycle.size()<<endl;
    for (auto p:edgesincycle)
    {
        int to = p.first;
        int from = p.second;
        if (to!=sonofparentofsuper) finalmap[to]=from;
        //cout<<"-> "<<to<<" "<<from<<endl;
    }
    for (auto p:newparent)
    {
        //{to,from}
        int to = p.first;
        int from = p.second;
        //some cases
        if (to!=total)
        {
            if (from!=total && to!=total)
            {
                finalmap[to]=from;
                //cout<<"-> "<<to<<" "<<from<<endl;
            }
            else
            {
                //from is in cycle , but to not in cycle
                // finalmap[to] = adj_in_list[to][0].first;
                int mw = 1e7;
                int mfrom = 0;
                for (auto p1:adj_in_list[to])
                {
                    if (presentincycle[p1.first] and mw>p1.second)
                    {
                        mw = p1.second;
                        mfrom = p1.first;
                    }
                }
                if (total==n and to==n) cout<<"total is n"<<finalmap[to]<<endl;
                finalmap[to] = mfrom;
                //finalmap[to] = adj_in_list[to][0].first;
                //cout<<"-> "<<to<<" "<<from<<endl;
            }
        }
    }
    return finalmap;
}
