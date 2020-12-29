#include "graph.h"
#include "edge.h"
#include <vector>
#include <queue>

class BFS {
    public:

        // BFS function
        BFS(Graph & graph);

        // helper BFS function used for traversing
        void traverse(Graph & graph, Vertex v);

        // Dijkstra's Algo which includes BFS traversal
        unordered_map<Vertex, double> dijkstraTrav(Graph & graph, Vertex startV);

        // return the BFS path.
        // ONLY CALL AFTER traverse
        vector<Vertex> getBFSTraversedPath();

        int crossEdges = 0;

        int discoveryEdges = 0;

        struct nodeDistance
        {
            Vertex node;
            double distance;
        };

        // This is the basis on which the elements of a priority queue are sorted and 
        // kept in the queue, here the criteria is that the node with smaller distance should
        // come above the one with larger distance

        class CompareDist
        {
            public:
                bool operator()(nodeDistance& n1, nodeDistance& n2)
                {
                if (n1.distance > n2.distance) 
                        return true;
                else
                        return false;
                }
        };

    private:
    
        // map of vertexes to bools, keeping track of visited vertices
        unordered_map<Vertex, bool> vertexVisitedMap;

        // path taken for BFS traversal
        vector<Vertex> bfsPath;
};
        
