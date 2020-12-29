#include "../cs225/catch/catch.hpp"
#include <iostream>
#include "../graph.h"
#include "../edge.h"
#include "../pageRank.h"
#include "../BFS.h"

TEST_CASE("Graph::loadSmallerGraphData data is randomly chosen") {
    Graph g(false, true);
    
    vector<string> test_random1 = g.loadSmallerGraphData("data/web-Google_copy.txt");
    srand(9);
    vector<string> test_random2 = g.loadSmallerGraphData("data/web-Google_copy.txt");
    REQUIRE(test_random1.size() == 500);
    REQUIRE(test_random2.size() == 500);
    REQUIRE(test_random1 != test_random2);
}

TEST_CASE("PageRank::getGreatestThree - Test with vertex with no outgoing direction") {
  Graph g(false, true);
  vector<string> test = g.loadAllGraphData("data/pageRank_complex.txt");
  
  g.buildGraph(test);
  g.print();

  PageRank p;
  p.initializeWeight(g.getAdjacencyList(), 99);
  p.getStableVector();
  vector<PageRank::WeightedVertex> greatest_three = p.getGreatestThree();

  REQUIRE(greatest_three[0].newRank > 0.30);
  REQUIRE(greatest_three[0].newRank < 0.32);

  REQUIRE(greatest_three[1].newRank > 0.30);
  REQUIRE(greatest_three[1].newRank < 0.32);

  REQUIRE(greatest_three[2].newRank > 0.18);
  REQUIRE(greatest_three[2].newRank < 0.20);
}

TEST_CASE("PageRank::getGreatestThree - Test every vertex with no outgoing direction") {
  Graph g(false, true);
  vector<string> test = g.loadAllGraphData("data/pageRank_complex.txt");
  
  g.buildGraph(test);
  g.print();

  PageRank p;
  p.initializeWeight(g.getAdjacencyList(), 99);
  p.getStableVector();
  vector<PageRank::WeightedVertex> rank_list = p.getRankList();

  REQUIRE(rank_list[0].newRank > 0.30);
  REQUIRE(rank_list[0].newRank < 0.32);
  REQUIRE(rank_list[rank_list.size() - 1].newRank > 0.04);
  REQUIRE(rank_list[rank_list.size() - 1].newRank < 0.08);
}

TEST_CASE("PageRank:updateWeight - twoIterationsSimpleGraph") {

  Graph g(false, true);
  vector<string> test = g.loadAllGraphData("data/pageRank_simple.txt");
  
  g.buildGraph(test);
  g.print();

  PageRank p;
  p.initializeWeight(g.getAdjacencyList(), 1);
  p.getStableVector();
  vector<PageRank::WeightedVertex> greatest_three = p.getGreatestThree();

  REQUIRE(greatest_three[0].newRank > 0.30);
  REQUIRE(greatest_three[0].newRank < 0.32);
  
  REQUIRE(greatest_three[1].newRank > 0.21);
  REQUIRE(greatest_three[1].newRank < 0.23);
  
  REQUIRE(greatest_three[2].newRank > 0.21);
  REQUIRE(greatest_three[2].newRank < 0.23);
}

TEST_CASE("PageRank:updateWeight - oneHundredIterationsSimpleGraph") {

  Graph g(false, true);
  vector<string> test = g.loadAllGraphData("data/pageRank_simple.txt");
  
  g.buildGraph(test);
  g.print();

  PageRank p;
  p.initializeWeight(g.getAdjacencyList(), 99);
  p.getStableVector();
  vector<PageRank::WeightedVertex> greatest_three = p.getGreatestThree();


  REQUIRE(greatest_three[0].newRank > 0.47);
  REQUIRE(greatest_three[0].newRank < 0.49);
  
  REQUIRE(greatest_three[1].newRank > 0.16);
  REQUIRE(greatest_three[1].newRank < 0.18);
  
  REQUIRE(greatest_three[2].newRank > 0.16);
  REQUIRE(greatest_three[2].newRank < 0.18);
}

// TEST_CASE("PageRank::getStableVector - Test stable vector generated correctly") {

// }
TEST_CASE("PageRank::updateWeight simple graph cycle") {
  Graph g(false, true);
  vector<string> test = g.loadAllGraphData("data/pageRank_simple_cycle.txt");
  
  g.buildGraph(test);
  g.print();

  PageRank p;
  p.initializeWeight(g.getAdjacencyList(), 99);
  p.getStableVector();
  vector<PageRank::WeightedVertex> greatest_three = p.getGreatestThree();
  
  REQUIRE(greatest_three[0].newRank == greatest_three[1].newRank);
  REQUIRE(greatest_three[1].newRank  ==  greatest_three[2].newRank);
  REQUIRE(greatest_three[0].newRank  ==  greatest_three[2].newRank);
}

TEST_CASE("PageRank::updateWeight - twoIterationsComplexGraph") {
  Graph g(false, true);
  vector<string> test = g.loadAllGraphData("data/pageRank_complex.txt");
  
  g.buildGraph(test);
  g.print();

  PageRank p;
  p.initializeWeight(g.getAdjacencyList(), 1);
  p.getStableVector();
  vector<PageRank::WeightedVertex> greatest_three = p.getGreatestThree();

  REQUIRE(greatest_three[0].newRank > 0.39);
  REQUIRE(greatest_three[0].newRank < 0.4);
  
  REQUIRE(greatest_three[1].newRank > 0.27);
  REQUIRE(greatest_three[1].newRank < 0.28);
  
  REQUIRE(greatest_three[2].newRank > 0.15);
  REQUIRE(greatest_three[2].newRank < 0.158);
}

TEST_CASE("PageRank::updateWeight - oneHundredIterationsComplexGraph") {
  Graph g(false, true);
  vector<string> test = g.loadAllGraphData("data/pageRank_complex.txt");
  
  g.buildGraph(test);
  g.print();

  PageRank p;
  p.initializeWeight(g.getAdjacencyList(), 99);
  p.getStableVector();
  vector<PageRank::WeightedVertex> greatest_three = p.getGreatestThree();

  REQUIRE(greatest_three[0].newRank > 0.315);
  REQUIRE(greatest_three[0].newRank < 0.319);
  
  REQUIRE(greatest_three[1].newRank > 0.30);
  REQUIRE(greatest_three[1].newRank < 0.32);
  
  REQUIRE(greatest_three[2].newRank > 0.18);
  REQUIRE(greatest_three[2].newRank < 0.20);
}



// BFS SECTION
//      A
//    /    \
//   C  -   B
TEST_CASE("BFS::traverse - simple cycle") {
  Graph g(false, false);
  vector<string> test = g.loadAllGraphData("data/bfs_simple_cycle.txt");
  
  int crossCount = 1;
  int discoverCount = 2;
  g.buildGraph(test);

  BFS bfs(g);
  std::vector<Vertex> path = bfs.getBFSTraversedPath();

  REQUIRE(bfs.crossEdges == crossCount);
  REQUIRE(bfs.discoveryEdges == discoverCount);
}

TEST_CASE("BFS::traverse - complex") {
  Graph g(false, false);
  vector<string> test = g.loadAllGraphData("data/bfs_complex.txt");
  
  int crossCount = 6;
  int discoverCount = 7;
  g.buildGraph(test);

  BFS bfs(g);
  std::vector<Vertex> path = bfs.getBFSTraversedPath();

  REQUIRE(bfs.crossEdges == crossCount);
  REQUIRE(bfs.discoveryEdges == discoverCount);
}

TEST_CASE("BFS::traverse - disconnected undirected graph") {
  Graph g(false, false);
  vector<string> test = g.loadAllGraphData("data/bfs_disconnected.txt");
  
  int crossCount = 0;
  int discoverCount = 2;
  g.buildGraph(test);

  BFS bfs(g);
  std::vector<Vertex> path = bfs.getBFSTraversedPath();

  g.print();
  
  REQUIRE(bfs.crossEdges == crossCount);
  REQUIRE(bfs.discoveryEdges == discoverCount);
}

// Dijkstra's Shortest Path

TEST_CASE("BFS::dijkstraTrav - simple shortest path") {
  Graph g(true, false);
  vector<string> test = g.loadAllGraphData("data/dijkstra_simple.txt");
  
  g.buildGraph(test);
  BFS bfs(g);

  // start from A
  // Graph looks like A - B - C - D - E 
  unordered_map<Vertex, double> test_map = bfs.dijkstraTrav(g, "A");

  REQUIRE(test_map["A"] == 0);
  REQUIRE(test_map["B"] == 1);
  REQUIRE(test_map["C"] == 2);
  REQUIRE(test_map["D"] == 3);
  REQUIRE(test_map["E"] == 4);
}

TEST_CASE("BFS::dijkstraTrav - complex shortest path") {
  Graph g(true, false);
  vector<string> test = g.loadAllGraphData("data/dijkstra_complex.txt");
  
  g.buildGraph(test);
  BFS bfs(g);

  // start from Home, target is School
  unordered_map<Vertex, double> test_map = bfs.dijkstraTrav(g, "Home");
  
  REQUIRE(test_map["Home"] == 0);
  REQUIRE(test_map["A"] == 3);
  REQUIRE(test_map["B"] == 2);
  REQUIRE(test_map["C"] == 5);
  REQUIRE(test_map["D"] == 3);
  REQUIRE(test_map["E"] == 7);
  REQUIRE(test_map["F"] == 7);  
  REQUIRE(test_map["School"] == 9);
}

TEST_CASE("BFS::dijkstraTrav - simple disconnected graph") {
  Graph g(true, false);
  vector<string> test = g.loadAllGraphData("data/dijkstra_disconnected.txt");
  
  g.buildGraph(test);
  BFS bfs(g);

  // start from Home, target is School
  unordered_map<Vertex, double> test_map = bfs.dijkstraTrav(g, "A");
  
  REQUIRE(test_map["A"] == 0);
  REQUIRE(test_map["B"] == 1);
  REQUIRE(test_map["C"] == std::numeric_limits<double>::infinity());
  REQUIRE(test_map["D"] == std::numeric_limits<double>::infinity());
  REQUIRE(test_map["E"] == std::numeric_limits<double>::infinity());
}