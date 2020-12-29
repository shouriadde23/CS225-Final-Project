#include "pageRank.h"

// create function to take in adj list, actual page rank algorithm
void PageRank::initializeWeight(const unordered_map<Vertex, unordered_map<Vertex, Edge>>& adj_list, int numIterations) {
    // clear rank list first
    rank_list_.clear();

    // initializing rank_list_ and set weight for all vectors to 1/size as default
    for (auto key = adj_list.begin(); key != adj_list.end(); ++key) {
        Vertex current_vertex = key->first;
        WeightedVertex weighted_vertex(current_vertex, (1.0 / (double) adj_list.size()));
        rank_list_.push_back(weighted_vertex);
        convert_vertex_map_[current_vertex] = rank_list_.size() - 1;
        // cout << rank_list_ << endl;
    }

    // visit nodes and check if destinations exist in rank_list_
    // if not then add to rank_list_
    // call updateweight for next steps
    updateWeight(adj_list, numIterations);
}

void PageRank::updateWeight(const unordered_map<Vertex, unordered_map<Vertex, Edge> >& adj_list, int numIterations) {
    // set all new rank values to 0;
    for (int i = 0; i < (int) rank_list_.size(); i++) {
        rank_list_[i].newRank = 0;
    }
    
    // we loop through each vertex and increase the new rank for adjacent vertices
    for (auto key = adj_list.begin(); key != adj_list.end(); ++key) {
        Vertex source = key->first;

        // find the weighted vector that source refers to
        int source_index = convert_vertex_map_[source];
        
        // a map of destination vertices representing adjacent nodes
        unordered_map<Vertex, Edge> map = key->second;

        if (map.size() == 0) { 
            for (int i = 0; i < (int) rank_list_.size(); i++) {
                rank_list_[i].newRank += (kDecayRate * (rank_list_[source_index].rank) / rank_list_.size());
            }
            continue;
        }
        
        for (auto second_key = map.begin(); second_key != map.end(); ++second_key) {
            Vertex destination = second_key->first;
            // find the weighted vector that destination refers to

            int destination_index = convert_vertex_map_[destination];
            // formula: decay rate * (old page rank) / number of connections
            double value_to_add = kDecayRate * (rank_list_[source_index].rank) / map.size();
            rank_list_[destination_index].newRank += value_to_add;        
        }
    }

    // adding 1 - decayrate / size for maintaining prob values
    for (int i = 0; i < (int) rank_list_.size(); i++) {
        double adder = (1 - kDecayRate) / rank_list_.size();
        rank_list_[i].newRank += adder;
    }
    
    
    // check to see if we need to recursive call
    double greatest_change = 0.0;
    for (int i = 0; i < (int) rank_list_.size(); i++) {
        if (greatest_change < abs(rank_list_[i].newRank - rank_list_[i].rank)) {
            greatest_change = abs(rank_list_[i].newRank - rank_list_[i].rank);
        }
    }

    for (int i = 0; i < (int) rank_list_.size(); i++) {
        double update_rank = (rank_list_[i].newRank);
        rank_list_[i].rank = update_rank;
    }
    
    if ((greatest_change > kEpsilon) && (count < numIterations)) {
        // move newrank to rank;
        count++;
        updateWeight(adj_list, numIterations);
    }
}

// return the greatest 3 elements of rank_list_
vector<PageRank::WeightedVertex> PageRank::getGreatestThree() {
    vector<WeightedVertex> greatestThree;
    WeightedVertex first, second, third;
    
    // finding the top 3 of rank_list_ with the greatest weight
    for (WeightedVertex i : rank_list_) {
        if (i.newRank > first.newRank) {
            third = second;
            second = first;
            first = i;
        } else if (i.newRank > second.newRank) {
            third = second;
            second = i;
        } else if (i.newRank > third.newRank) {
            third = i;
        }
    }

    greatestThree.push_back(first);
    greatestThree.push_back(second);
    greatestThree.push_back(third);

    return greatestThree;
}

// empty default constructor
PageRank::PageRank() {

}

// prints out the labels of the vertices and top 3 greatest weights and writes them into a file
void PageRank::getStableVector() {
    std::ofstream pagefile;
    pagefile.open("data/pg_new_result.txt");

    vector<WeightedVertex> greatestThreeRanks = getGreatestThree();
    for(unsigned i = 0; i < rank_list_.size(); i++) {
        pagefile << rank_list_[i].rank << "     --------- LABEL: " << rank_list_[i].label << endl;
    }
    pagefile << "------------------------------------------------------------------------------------------------------" << endl;
    for(unsigned i = 0; i < greatestThreeRanks.size(); i++) {
        pagefile << "Greatest Label------- "<< greatestThreeRanks[i].label << "  RANK: " << greatestThreeRanks[i].newRank << endl;
    }

    pagefile.close();
}

// return a new copy of of rank_list_
vector<PageRank::WeightedVertex> PageRank::getRankList() {
    vector<WeightedVertex> copy;
    for (int i = 0; i < (int) rank_list_.size(); i++) {
        copy.push_back(rank_list_[i]);
    }
    return copy;
}
