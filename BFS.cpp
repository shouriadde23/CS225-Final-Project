#include "BFS.h"
#include <vector>
#include <queue>


// BFS constructor
BFS::BFS(Graph& graph) {
    vector<Vertex> vertices = graph.getVertices();
    vector<Edge> edges = graph.getEdges();

    for (auto key = vertices.begin(); key != vertices.end(); ++key) {
        vertexVisitedMap[*key] = false;
    }

    for (auto key = edges.begin(); key != edges.end(); ++key) {
        graph.setEdgeLabel((*key).source, (*key).dest, "UNEXPLORED");
    }

    for (Vertex v : vertices) {
        if (vertexVisitedMap[v] == false) {
            traverse(graph, v);
        }
    }
}


// BFS helper function to traverse
// using lecture BFS code
void BFS::traverse(Graph& graph, Vertex v) {
    // initializing queue
    std::queue<Vertex> q;

    // mark initial vertex as visited, add to queue
    vertexVisitedMap[v] = true; 
    q.push(v);
    bfsPath.push_back(v);

    // iterate through queue
    while(q.empty() == false) {
        // dequeue the top-most node; need it for bfs traversal processing
        v = q.front();
        q.pop();
        
        // processing adjacent vertices of current vertex on queue
        for(Vertex w : graph.getAdjacent(v)) {
            // check if unexplored edge, add edge as discovery
            if(vertexVisitedMap[w] == false) {
                graph.setEdgeLabel(v, w, "DISCOVERY");
                discoveryEdges++;
                // set w's vertex value to true since it is now visited
                vertexVisitedMap[w] = true;
                // push the new adjacent node to queue
                q.push(w);
                bfsPath.push_back(w);
            }
            // if explored edge, set edge label as cross
            else if (graph.getEdgeLabel(v, w).compare("UNEXPLORED") == 0 ) {
                graph.setEdgeLabel(v, w, "CROSS");
                crossEdges++;
            }
        }
    }   
}

// Dijkstra's Algo which includes BFS traversal
unordered_map<Vertex, double> BFS::dijkstraTrav(Graph& graph, Vertex startV) {

    unordered_map<Vertex, double> distance_map;
    unordered_map<Vertex, bool> visited_map;

    double inf = std::numeric_limits<double>::infinity();
    vector<Vertex> vertices = graph.getVertices();
    
    distance_map[startV] = 0;
    std::priority_queue<nodeDistance, vector<nodeDistance>, CompareDist> pq;
    
    // Create the first node as the source and put it into the queue
    nodeDistance first = {startV, 0};
    pq.push(first);

    for (int i = 0; i < (int) vertices.size(); i++) {
        if(vertices[i] != startV) {
            distance_map[vertices[i]] = inf;
        }
        visited_map[vertices[i]] = false;
    }

    while (!pq.empty()) {
        nodeDistance temp = pq.top();

        pq.pop();
        vector<Vertex> adj_vertices = graph.getAdjacent(temp.node);

        for (int j = 0; j < (int) adj_vertices.size(); j++) {
            Vertex cur_adj_vertex = adj_vertices[j];
            double cur_distance = distance_map[temp.node]  + graph.getEdgeWeight(temp.node, cur_adj_vertex);

            if (cur_distance < distance_map[cur_adj_vertex]) {
                distance_map[cur_adj_vertex] = cur_distance;
            }
            
            if (!visited_map[cur_adj_vertex]) {
                nodeDistance newNode;                   
                newNode.node = cur_adj_vertex;
                newNode.distance = distance_map[cur_adj_vertex];

                pq.push(newNode);
                visited_map[cur_adj_vertex] = true;
            }
        }
        
    }

    return distance_map;
}

// return the BFS path.
// NOTE: only works after traverse is called
vector<Vertex> BFS::getBFSTraversedPath() {
    return bfsPath;
}