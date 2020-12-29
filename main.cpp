#include "graph.h"
#include "pageRank.h"
#include "BFS.h"

int main(int argc, char** argv) {
    // 1st cmd line argument: load ALL data or load subset for PageRank
    // 2nd cmd line argument: path to data to run PageRank on, needs to be directed and unweighted
    // 3nd cmd line argument: path to data to run BFS on, needs to be unweighted
    // 4nd cmd line argument: path to data to run Dijkstra's on, needs to be weighted

    bool loadAll = true;
    string pgRankPath = "";
    string bfsPath = "";
    string dijkstraPath = "";
    Vertex startVertex = "";

    // allowing user to load in smaller set of data or larger set of data
    if (string(argv[1]) == "FALSE") {
        loadAll = false;
    } else if (string(argv[1]) == "TRUE") {
        loadAll = true;
    }

    pgRankPath = string(argv[2]);
    bfsPath = string(argv[3]);
    dijkstraPath = string(argv[4]);
    startVertex = string(argv[5]);

    /* PAGE RANK SECTION
     * 
     */

    Graph pageRankGraph(false, true);
    vector<string> test;
    if (loadAll) {
        test = pageRankGraph.loadAllGraphData(pgRankPath);
    } else if (loadAll == false) {
        test = pageRankGraph.loadSmallerGraphData(pgRankPath);
    }
    
   
    pageRankGraph.buildGraph(test);

    PageRank pg;
    pg.initializeWeight(pageRankGraph.getAdjacencyList(), 100);
    pg.getStableVector();

    //------------------------------------------------------------------------------------------------------
    
    /* BFS SECTION
     * 
     */

    // undirected, unweighted graph
    std::ofstream bfsfile;
    bfsfile.open("data/bfs_path_result.txt");
    
    Graph bfs(false, false);
    vector<string> bfsVec = bfs.loadAllGraphData(bfsPath);
    bfs.buildGraph(bfsVec);
    BFS traversal(bfs);
    
    auto path = traversal.getBFSTraversedPath();
    for (unsigned i = 0; i < path.size(); i++) {
        bfsfile << path[i] << " -----Node " << i << " in BFS path" << endl;
    }

    bfsfile.close();
    bfs.writeGraph();

    //------------------------------------------------------------------------------------------------------
    
    /* DIJKSTRA'S SHORTEST PATH SECTION
     * 
     */

    std::ofstream dijkfile;
    dijkfile.open("data/dijk_result.txt");

    Graph dijkstra(true, false);
    vector<string> dijkstraVec = dijkstra.loadAllGraphData(dijkstraPath);
    dijkstra.buildGraph(dijkstraVec);
    BFS dijk(dijkstra);

    unordered_map<Vertex, double> result = dijk.dijkstraTrav(dijkstra, startVertex);
    

    for (auto cur = result.begin(); cur != result.end(); ++cur) {
        double distance = cur->second;
        if (distance == std::numeric_limits<double>::infinity()) {
            dijkfile << "Vertex = " << cur->first << "   Distance = " << " No path found " << std::endl;
        } else {
            dijkfile << "Vertex = " << cur->first << "   Distance = " << distance << std::endl;
        }
    }

    dijkfile.close();
    
    return 0;
}