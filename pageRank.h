#include "graph.h"

// decay rate shows how connected two links are, so the more times we call updateWeight, 
// it changes less, allowing us to tell when we "converge" and should stop calling updates
const double kDecayRate = 0.85;
// epsilon value to tell when to stop recursive call
const double kEpsilon = 0.0000001;

class PageRank {
    public:
        struct WeightedVertex {
          public:

            string label;
            double rank;
            double newRank;
            WeightedVertex(string setLabel, double setRank) {
                label = setLabel;
                rank = setRank;
                newRank = -1;
            }
            WeightedVertex(string setLabel) {
                label = setLabel;
                rank = 0;
                newRank = -1;
            }
            WeightedVertex() {
                label = "";
                rank = 0;
                newRank = -1;
            }
        };


        // default constructor
        PageRank();

        // set weight and update rank_list_
        void initializeWeight(const unordered_map<Vertex, unordered_map<Vertex, Edge> >& adj_list, int numIterations);

        // create a function takes in a list of doubles, return the greatest 3
        vector<WeightedVertex> getGreatestThree();

        void getStableVector();

        // get a copy of rank_list_ for testing/comparison
        vector<WeightedVertex> getRankList();

    private:
        // assumes rank_list_ is updated, sets the weight for the vertices in rank_list_
        void updateWeight(const unordered_map<Vertex, unordered_map<Vertex, Edge> >& adj_list, int numIterations);

        // unordered_map<int, Vertex> label_map_;
        vector<WeightedVertex> rank_list_;

        unordered_map<Vertex, int> convert_vertex_map_;

        int count = 0;
        // int twoIters = 2;
        // int hundredIters = 100;

};

// Unused code

/* Considered implementing an adjacency matrix for page rank. Runtime is not ideal
 *
 *
 */
// vector<vector<int> > PageRank::createMatrix(
//     unordered_map<Vertex, unordered_map<Vertex, Edge>> adj_list) {
//     int unique_vertices = adj_list.size();
//     // initializing matrix and weight as 0
//     vector<vector<int> > two_d_vector;
//     for (int row = 0; row < unique_vertices; row++) {
//         vector<int> blank_vector;
//         two_d_vector.push_back(blank_vector);
//         for (int col = 0; col < unique_vertices; col++) {
//             two_d_vector[row].push_back(0);
//         }
//     }

//     // set hashmap by looking at map and adjacent nodes
//     int current = 0;
//     for (auto key = adj_list.begin(); key != adj_list.end(); ++key) {
//         label_map_[current] = (*key).first;
//         current++;
//     }
    

//     return two_d_vector;
// }


// vector<vector<int>> createMatrix(
//unordered_map<Vertex, unordered_map<Vertex, Edge> > adj_list);