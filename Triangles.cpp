// Triangles.cpp : Tento soubor obsahuje funkci main. Provádění programu se tam zahajuje a ukončuje.
//
#include <stdlib.h>

#include <ctime>
#include <cassert>
#include <iostream>

#include <string>

#include <fstream>
#include <sstream>
#include <memory>
#include <queue>

#include <vector>
#include <unordered_map>
#include <unordered_set>
//#include "../Projekt/Graph2.h"

using namespace std;

void clear_console()
{
#if defined _WIN32
    system("cls");
    //clrscr(); // including header file : conio.h
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    system("clear");
    //std::cout<< u8"\033[2J\033[1;1H"; //Using ANSI Escape Sequences
#elif defined (__APPLE__)
    system("clear");
#endif
}

auto read_graph(std::string filename) {
    auto graph = std::make_unique<
            std::unordered_map<int, unique_ptr<std::unordered_set<int>>>>();

    std::ifstream infile(filename);
    std::string line;
    while (std::getline(infile, line)) {
        if (line[0] != '#') {
            std::istringstream iss(line);
            int a, b;
            iss >> a >> b;
            if (graph->count(a) == 0) {
                graph->insert(std::make_pair(a,
                                             unique_ptr<std::unordered_set<int>>(
                                                     new std::unordered_set<int>())));
            }
            graph->at(a)->insert(b);
            if (graph->count(b) == 0) {
                graph->insert(std::make_pair(b,
                                             unique_ptr<std::unordered_set<int>>(
                                                     new std::unordered_set<int>())));
            }
            graph->at(b)->insert(a);
        }
    }

    return graph;
}

double global_clustering_coefficient(
        const std::unique_ptr<std::unordered_map<int, std::unique_ptr<std::unordered_set<int>>>> &graph) {
    // Initialize variables to keep track of the number of triangles and connected triples
    int triangles = 0;
    int connected_triples = 0;

    // Loop over all vertices in the graph
    std::unordered_set<int> visited;
    std::vector<int> largest_component;
    for (auto iter = graph->begin(); iter != graph->end(); iter++) {
        if (visited.find(iter->first) == visited.end()) {
            // Vertex not visited, start BFS from this vertex
            std::queue<int> queue;
            queue.push(iter->first);
            visited.insert(iter->first);
            std::vector<int> component;
            while (!queue.empty()) {
                int vertex = queue.front();
                queue.pop();
                component.push_back(vertex);
                auto &neighbors = graph->at(vertex);
                for (auto neighbor: *neighbors) {
                    if (visited.find(neighbor) == visited.end()) {
                        queue.push(neighbor);
                        visited.insert(neighbor);
                    }
                }
            }
            if (component.size() > largest_component.size()) {
                largest_component = std::move(component);
            }
        }
    }
    std::cout << largest_component.size() << std::endl;
    // Loop over all vertices in the largest connected component
    int current = 0;
    for (int vertex: largest_component) {
        ++current;
        // Get the set of neighbors of the current vertex
        const auto &neighbors = *graph->at(vertex);

        // Loop over all pairs of neighbors
        for (auto i = neighbors.begin(); i != neighbors.end(); i++) {
            for (auto j = std::next(i); j != neighbors.end(); j++) {
                // Check if there is an edge between the two neighbors
                if (graph->at(*i)->count(*j) > 0) {
                    // Increment the count of triangles
                    triangles++;
                }
            }
        }

        // Compute the number of connected triples
        connected_triples += neighbors.size() * (neighbors.size() - 1);
        clear_console();
        std::cout << "Computing " << current << "/" << largest_component.size() << flush;
    }

    // Compute the global clustering coefficient
    cout << "triangles: " << triangles << endl;
    cout << "Num of connected_triples = " << connected_triples << endl;

    cout << "         " << triangles * 2 << "\nFrac = -------- \n        " << connected_triples << endl;
    double clustering_coefficient = (double) triangles * 2 / (connected_triples);
    return clustering_coefficient;
}

//https://gist.github.com/andrewcchen/e9d24b42504a442141f2
int main(int args, char *argv[]) {
    assert(args == 2);

    auto graph = read_graph(argv[1]);
    printf("Number of vertices: %d\n", graph->size());
    int edge_count = 0;
    for (auto iter = graph->begin(); iter != graph->end(); iter++) {
        edge_count += iter->second->size();
    }
    edge_count /= 2;
    printf("Number of edges: %d\n", edge_count);

    int start_time = clock();
//    int trios = count_triangles(std::move(graph), trios);
    double gcc = global_clustering_coefficient(graph);
    int end_time = clock();

//    printf("Global clustering coeficient: %f , %d\n", gcc, trios);
    printf("Global clustering coeficient: %f\n", gcc);
    printf("Time elapsed: %f\n", (end_time - start_time) / (double) CLOCKS_PER_SEC);
}

// Spuštění programu: Ctrl+F5 nebo nabídka Ladit > Spustit bez ladění
// Ladění programu: F5 nebo nabídka Ladit > Spustit ladění

// Tipy pro zahájení práce:
//   1. K přidání nebo správě souborů použijte okno Průzkumník řešení.
//   2. Pro připojení ke správě zdrojového kódu použijte okno Team Explorer.
//   3. K zobrazení výstupu sestavení a dalších zpráv použijte okno Výstup.
//   4. K zobrazení chyb použijte okno Seznam chyb.
//   5. Pokud chcete vytvořit nové soubory kódu, přejděte na Projekt > Přidat novou položku. Pokud chcete přidat do projektu existující soubory kódu, přejděte na Projekt > Přidat existující položku.
//   6. Pokud budete chtít v budoucnu znovu otevřít tento projekt, přejděte na Soubor > Otevřít > Projekt a vyberte příslušný soubor .sln.
