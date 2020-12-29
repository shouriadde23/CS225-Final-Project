#include "graph.h"

const Vertex Graph::InvalidVertex = "_CS225INVALIDVERTEX";
const int Graph::InvalidWeight = INT_MIN;
const string Graph:: InvalidLabel = "_CS225INVALIDLABEL";
const Edge Graph::InvalidEdge = Edge(Graph::InvalidVertex, Graph::InvalidVertex, Graph::InvalidWeight, Graph::InvalidLabel);

// default constructor, assumes directed is false
Graph::Graph(bool weighted) : weighted(weighted),directed(false),random(Random(0))
{
}

Graph::Graph(bool weighted, bool directed) : weighted(weighted),directed(directed),random(Random(0))
{
}

// seed used for random generation, so test code is consistent
Graph::Graph(bool weighted, int numVertices, unsigned long seed)
    :weighted(weighted),
      directed(false),
     random(Random(seed)) 
{
    if (numVertices < 2)
    {
     error("numVertices too low");
     exit(1);
    }

    vector<Vertex> vertices;
    for (int i = 0; i < numVertices; i++)
    {
        insertVertex(to_string(i));
        vertices.push_back(to_string(i));
    }

    // make sure all vertices are connected
    random.shuffle(vertices);
    Vertex cur = vertices[0];
    for (size_t i = 0; i < vertices.size() - 1; ++i)
    {
        Vertex next = vertices[i + 1];
        insertEdge(cur, next);
        if (weighted) 
        {
            int weight = random.nextInt();
            setEdgeWeight(cur, next, weight);
        }
        cur = next;
    }

    // keep the graph from being overpopulated with edges,
    //  while still maintaining a little randomness
    int numFailures = 0;
    int idx = 0;
    random.shuffle(vertices);
    while (numFailures < 2) 
    {
        if (!insertEdge(vertices[idx], vertices[idx + 1])) 
        {
            ++numFailures;
        } 
        else 
        {
            // if insertEdge() succeeds
            if (weighted)
                setEdgeWeight(vertices[idx], vertices[idx + 1],
                              random.nextInt());
            ++idx;
            if (idx >= numVertices - 2) 
            {
                idx = 0;
                random.shuffle(vertices);
            }
        }
    }
}

// finds vertices adjacent to the source parameter
vector<Vertex> Graph::getAdjacent(Vertex source) const 
{
    auto lookup = adjacency_list.find(source);

    if (lookup == adjacency_list.end()) {
        return vector<Vertex>();
    } else {
        vector<Vertex> vertex_list;
        unordered_map <Vertex, Edge> & map = adjacency_list[source];

        for (auto it = map.begin(); it != map.end(); it++)
        {
            vertex_list.push_back(it->first);
        }
        return vertex_list;
    }
}


Vertex Graph::getStartingVertex() const
{
    return adjacency_list.begin()->first;
}

vector<Vertex> Graph::getVertices() const
{
    vector<Vertex> ret;

    for(auto it = adjacency_list.begin(); it != adjacency_list.end(); it++)
    {
        ret.push_back(it->first);
    }

    return ret;
}

Edge Graph::getEdge(Vertex source , Vertex destination) const
{
    if(assertEdgeExists(source, destination, __func__) == false)
        return Edge();
    Edge ret = adjacency_list[source][destination];
    return ret;
}

vector<Edge> Graph::getEdges() const
{
    if (adjacency_list.empty())
        return vector<Edge>();

    vector<Edge> ret;
    set<pair<Vertex, Vertex>> seen;

    for (auto it = adjacency_list.begin(); it != adjacency_list.end(); it++)
    {
        Vertex source = it->first;
        for (auto its = adjacency_list[source].begin(); its != adjacency_list[source].end(); its++)
        {
            Vertex destination = its->first;
            if(seen.find(make_pair(source, destination)) == seen.end())
            {
                //this pair is never added to seen
                ret.push_back(its->second);
                seen.insert(make_pair(source,destination));
                if(!directed)
                {
                    seen.insert(make_pair(destination, source));
                }
            }
        }
    }

    return ret;
}

bool Graph::vertexExists(Vertex v) const
{
    return assertVertexExists(v, "");
}

bool Graph::edgeExists(Vertex source, Vertex destination) const
{
    return assertEdgeExists(source, destination, "");
}

Edge Graph::setEdgeLabel(Vertex source, Vertex destination, string label)
{
    if (assertEdgeExists(source, destination, __func__) == false)
        return InvalidEdge;
    Edge e = adjacency_list[source][destination];
    Edge new_edge(source, destination, e.getWeight(), label);
    adjacency_list[source][destination] = new_edge;

    if(!directed)
    {
        Edge new_edge_reverse(destination,source, e.getWeight(), label);
        adjacency_list[destination][source] = new_edge_reverse;
    }
    return new_edge;
}


string Graph::getEdgeLabel(Vertex source, Vertex destination) const
{
    if(assertEdgeExists(source, destination, __func__) == false)
        return InvalidLabel;
    return adjacency_list[source][destination].getLabel();
}

double Graph::getEdgeWeight(Vertex source, Vertex destination) const
{
    if (!weighted)
        error("can't get edge weights on non-weighted graphs!");

    if(assertEdgeExists(source, destination, __func__) == false)
        return InvalidWeight;
    return adjacency_list[source][destination].getWeight();
}

void Graph::insertVertex(Vertex v)
{
    // will overwrite if old stuff was there
    removeVertex(v);
    // make it empty again
    adjacency_list[v] = unordered_map<Vertex, Edge>();
}


Vertex Graph::removeVertex(Vertex v)
{

    if (adjacency_list.find(v) != adjacency_list.end())
    {
        if(!directed){
            for (auto it = adjacency_list[v].begin(); it != adjacency_list[v].end(); it++)
            {
                Vertex u = it->first;
                adjacency_list[u].erase(v); 
            }
            adjacency_list.erase(v);
            return v;
        }
        
        adjacency_list.erase(v);
        for(auto it2 = adjacency_list.begin(); it2 != adjacency_list.end(); it2++)
        {
            Vertex u = it2->first;
            if (it2->second.find(v)!=it2->second.end())
            {
                it2->second.erase(v);
            }
        }
        return v;
    }

    return InvalidVertex;
}

bool Graph::insertEdge(Vertex source, Vertex destination)
{   
    if(adjacency_list.find(source)!= adjacency_list.end() 
    && adjacency_list[source].find(destination)!= adjacency_list[source].end())
    {
        //edge already exit
        return false;
    }

    if(adjacency_list.find(source)==adjacency_list.end())
    {
        adjacency_list[source] = unordered_map<Vertex, Edge>();
    }
        //source vertex exists
    adjacency_list[source][destination] = Edge(source, destination);
    if(!directed)
    {
        if(adjacency_list.find(destination)== adjacency_list.end())
        {
            adjacency_list[destination] = unordered_map<Vertex, Edge>();
        }
        adjacency_list[destination][source] = Edge(source, destination);
    }
    
    return true;
}

Edge Graph::removeEdge(Vertex source, Vertex destination)
{
    if(assertEdgeExists(source, destination, __func__) == false)
        return InvalidEdge;
    Edge e = adjacency_list[source][destination];
    adjacency_list[source].erase(destination);
    // if undirected, remove the corresponding edge
    if(!directed)
    {
        adjacency_list[destination].erase(source);
    }
    return e;
}


Edge Graph::setEdgeWeight(Vertex source, Vertex destination, double weight)
{
    if (assertEdgeExists(source, destination, __func__) == false)
        return InvalidEdge;
    Edge e = adjacency_list[source][destination];
    //std::cout << "setting weight: " << weight << std::endl;
    Edge new_edge(source, destination, weight, e.getLabel());
    adjacency_list[source][destination] = new_edge;

    if(!directed)
        {
            Edge new_edge_reverse(destination,source, weight, e.getLabel());
            adjacency_list[destination][source] = new_edge_reverse;
        }

    return new_edge;
}

bool Graph::assertVertexExists(Vertex v, string functionName) const
{
    if (adjacency_list.find(v) == adjacency_list.end())
    {
        if (functionName != "")
            error(functionName + " called on nonexistent vertices");
        return false;
    }
    return true;
}

bool Graph::assertEdgeExists(Vertex source, Vertex destination, string functionName) const
{
    if(assertVertexExists(source,functionName) == false)
        return false;
    if(adjacency_list[source].find(destination)== adjacency_list[source].end())
    {
        if (functionName != "")
            error(functionName + " called on nonexistent edge " + source + " -> " + destination);
        return false;
    }

    if(!directed)
    {
        if (assertVertexExists(destination,functionName) == false)
            return false;
        if(adjacency_list[destination].find(source)== adjacency_list[destination].end())
        {
            if (functionName != "")
                error(functionName + " called on nonexistent edge " + destination + " -> " + source);
            return false;
        }
    }
    return true;
}

bool Graph::isDirected() const
{
    return directed;
}

void Graph::clear()
{
    adjacency_list.clear();
}


/**
 * Prints a graph error and quits the program.
 * The program is exited with a segfault to provide a stack trace.
 * @param message - the error message that is printed
 */
void Graph::error(string message) const
{
    cerr << "\033[1;31m[Graph Error]\033[0m " + message << endl;
}

/**
 * Prints the graph through stdout, giving the weight and label of each vertice
 */
void Graph::print() const
{
    for (auto it = adjacency_list.begin(); it != adjacency_list.end(); ++it) 
    {
        cout << it->first << endl;
        for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) 
        {
            std::stringstream ss;
            ss << it2->first; 
            string vertexColumn = "    => " + ss.str();
            vertexColumn += " " ;
            cout << std::left << std::setw(26) << vertexColumn;
            string edgeColumn = "edge label = \"" + it2->second.getLabel()+ "\"";
            cout << std::left << std::setw(26) << edgeColumn;
            if (weighted)
                cout << "weight = " << it2->second.getWeight();
            cout << endl;
        }
        cout << endl;
    }
}

void Graph::writeGraph() const {
    // create a file that we write into
    std::ofstream myfile;
    myfile.open("data/bfs_result.txt");

    for (auto it = adjacency_list.begin(); it != adjacency_list.end(); ++it) 
    {
        myfile << it->first << endl;
        for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) 
        {
            std::stringstream ss;
            ss << it2->first; 
            string vertexColumn = "    => " + ss.str();
            vertexColumn += " " ;
            myfile << std::left << std::setw(26) << vertexColumn;
            string edgeColumn = "edge label = \"" + it2->second.getLabel()+ "\"";
            myfile << std::left << std::setw(26) << edgeColumn;
            if (weighted)
                myfile << "weight = " << it2->second.getWeight();
            myfile << endl;
        }
        myfile << endl;
    }

    myfile.close();
}

// helper function implementation
vector<string> Graph::loadSmallerGraphData(const string & filename) {
    vector<string> randStrs;

    std::ifstream text(filename);

	vector<string> out;
    double count = 0;
    int value = 0;
    // 100 - probability = chance of adding current line to graph
    // Ex. prob = 99, means 1% chance of a line being added to graph
    int probability = 99;
    // Seed of rand is constant for testing for now
    //srand(6);
    bool skip_line = false;
    if (text.is_open()) {
		std::istream_iterator<string> iter(text);
        cout << "file opened" << endl;
		while (!text.eof() && count < 500) {
            value = rand() % 100;
            skip_line = true;
            // code processes each pair as 2 lines, 
            // so both lines must be added if one line was chosen
            // and we iter in 2 lines(in istream) and 1 line in txt.
            if (value >= probability) {
                out.push_back(*iter);
                count++;
                
                if (text.eof()) {
                    break;
                }
                ++iter;
                out.push_back(*iter);
                count++;
                skip_line = false;
            }
            if (skip_line) {
                ++iter;
            }
            ++iter;
		}
	}
	return out;
}

// same as load small graph data, but all data is loaded instead
vector<string> Graph::loadAllGraphData(const string & filename) {
    vector<string> randStrs;

    std::ifstream text(filename);

	vector<string> out;
    
    if (text.is_open()) {
		std::istream_iterator<string> iter(text);
        cout << "file opened" << endl;
        while (!text.eof()) {
            out.push_back(*iter);
            ++iter;
        }
	}
	return out;
}

// connect graph properly after randomly choosing vertices using loadSmallerGraphData
void Graph::buildGraph(const vector<string> & vect) {
    // loop through the vector and connect edges between each pair of nodes
    if (weighted == false) {
        for (unsigned i = 0; i < vect.size(); i += 2) {
            Vertex source = vect[i];
            Vertex dest = vect[i + 1];
            if(source == "?" || dest == "?" ) {
                if(source == "?") {
                    insertVertex(dest);
                }
                else {
                    insertVertex(source);
                }
            } else {
                // not including any self loops while updating weights
                if(source != dest) {
                    insertEdge(source, dest);
                }
            }
        }
    } else if (weighted == true) {
        for (unsigned i = 0; i < vect.size(); i += 3) {
            Vertex source = vect[i];
            Vertex dest = vect[i + 1];
            double weight = stold(vect[i + 2]);
            // not including any self loops while updating weights
            if(source != dest) {
                insertEdge(source, dest);
                setEdgeWeight(source, dest, weight);
            }
        }
    }
    
    // after building the graph through a first pass, we need to check if there are nodes
    // that do not point to anywhere, they still need to be added to the adjacency list
    for (auto key = adjacency_list.begin(); key != adjacency_list.end(); ++key) {
        vector<Vertex> adjacentNodes = getAdjacent(key->first);

        for (auto dest = adjacentNodes.begin(); dest != adjacentNodes.end(); ++dest) {
            Vertex destination = *dest;
            if (adjacency_list.find(destination) == adjacency_list.end()) {
                insertVertex(destination);
            }
        }
    }
}

// getter function for private adjacency_list
unordered_map<Vertex, unordered_map<Vertex, Edge>> Graph::getAdjacencyList() {
    return adjacency_list;
}
