#include <iostream>
#include <string>
#include <stdexcept>
#include "graph/graph.hpp"
#include "algorithm/dijkstra.hpp"
#include "algorithm/aStar.hpp"
#include "io/csvhandler.hpp"
#include "io/jsonhandler.hpp"
#include "preference/preference.hpp"
#include "utils/logger.hpp"
#include <cstdlib> // Untuk system()
#include <fstream>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

void saveGraphToCSV(const Graph& graph, const string& locationFile, const string& routeFile) {
    ofstream loc(locationFile);
    loc << "id,x,y\n";
    for (const auto& v : graph.getAllVertices()) {
        loc << v.getId() << "," << v.getLatitude() << "," << v.getLongitude() << "\n";
    }
    loc.close();
    ofstream route(routeFile);
    route << "from,to,distance,time,cost\n";
    for (const auto& e : graph.getAllEdges()) {
        route << e.getSource() << "," << e.getDestination() << "," << e.getDistance() << "," << e.getTime() << "," << e.getCost() << "\n";
    }
    route.close();
    cout << "Graph berhasil disimpan ke data/location.csv dan data/route.csv\n";
}

void printPath(const Graph& graph, const vector<string>& path) {
    cout << "\nRecommended Route:\n";
    cout << "=================\n";
    for (size_t i = 0; i < path.size(); ++i) {
        const auto& vertex = graph.getVertex(path[i]);
        cout << i + 1 << ". " << vertex.getId() << " ";
        if (i < path.size() - 1) {
            cout << "-> ";
        }
        if ((i + 1) % 5 == 0) cout << "\n";
    }
    cout << "\n\n";
}

void printPathDetails(const Graph& graph, const vector<string>& path) {
    double totalDistance = Dijkstra::getPathDistance(graph, path);
    double totalTime = Dijkstra::getPathTime(graph, path);
    double totalCost = Dijkstra::getPathCost(graph, path);

    cout << "Path Details:\n";
    cout << "============\n";
    cout << "Total Distance: " << totalDistance << " km\n";
    cout << "Total Time: " << totalTime << " minutes\n";
    cout << "Total Cost: $" << totalCost << "\n\n";
}

void addVertexMenu(Graph& graph) {
    string id;
    double x, y;
    cout << "Masukkan ID vertex (string): "; cin >> id;
    cout << "Masukkan koordinat x: "; cin >> x;
    cout << "Masukkan koordinat y: "; cin >> y;
    graph.addVertex(Vertex(id, x, y));
    cout << "Vertex berhasil ditambahkan!\n";
    saveGraphToCSV(graph, "data/location.csv", "data/route.csv");
}

void editVertexMenu(Graph& graph) {
    string id;
    cout << "Masukkan ID vertex yang ingin diubah: "; cin >> id;
    if (!graph.hasVertex(id)) {
        cout << "Vertex tidak ditemukan!\n";
        return;
    }
    double x, y;
    cout << "Masukkan koordinat x baru: "; cin >> x;
    cout << "Masukkan koordinat y baru: "; cin >> y;
    graph.removeVertex(id);
    graph.addVertex(Vertex(id, x, y));
    cout << "Vertex berhasil diubah!\n";
    saveGraphToCSV(graph, "data/location.csv", "data/route.csv");
}

void deleteVertexMenu(Graph& graph) {
    string id;
    cout << "Masukkan ID vertex yang ingin dihapus: "; cin >> id;
    if (!graph.hasVertex(id)) {
        cout << "Vertex tidak ditemukan!\n";
        return;
    }
    graph.removeVertex(id);
    cout << "Vertex berhasil dihapus!\n";
    saveGraphToCSV(graph, "data/location.csv", "data/route.csv");
}

void addEdgeMenu(Graph& graph) {
    string from, to;
    double distance, time, cost;
    cout << "Masukkan ID asal: "; cin >> from;
    cout << "Masukkan ID tujuan: "; cin >> to;
    cout << "Masukkan jarak: "; cin >> distance;
    cout << "Masukkan waktu: "; cin >> time;
    cout << "Masukkan biaya: "; cin >> cost;
    graph.addEdge(Edge(from, to, distance, time, cost));
    cout << "Edge berhasil ditambahkan!\n";
    saveGraphToCSV(graph, "data/location.csv", "data/route.csv");
}

void editEdgeMenu(Graph& graph) {
    string from, to;
    cout << "Masukkan ID asal edge yang ingin diubah: "; cin >> from;
    cout << "Masukkan ID tujuan edge yang ingin diubah: "; cin >> to;
    if (!graph.hasEdge(from, to)) {
        cout << "Edge tidak ditemukan!\n";
        return;
    }
    graph.removeEdge(from, to);
    double distance, time, cost;
    cout << "Masukkan jarak baru: "; cin >> distance;
    cout << "Masukkan waktu baru: "; cin >> time;
    cout << "Masukkan biaya baru: "; cin >> cost;
    graph.addEdge(Edge(from, to, distance, time, cost));
    cout << "Edge berhasil diubah!\n";
    saveGraphToCSV(graph, "data/location.csv", "data/route.csv");
}

void deleteEdgeMenu(Graph& graph) {
    string from, to;
    cout << "Masukkan ID asal edge yang ingin dihapus: "; cin >> from;
    cout << "Masukkan ID tujuan edge yang ingin dihapus: "; cin >> to;
    if (!graph.hasEdge(from, to)) {
        cout << "Edge tidak ditemukan!\n";
        return;
    }
    graph.removeEdge(from, to);
    cout << "Edge berhasil dihapus!\n";
    saveGraphToCSV(graph, "data/location.csv", "data/route.csv");
}

void showGraphMenu(const Graph& graph) {
    cout << "\nDaftar Vertex:\n";
    for (const auto& v : graph.getAllVertices()) {
        cout << v.getId() << " (" << v.getLatitude() << ", " << v.getLongitude() << ")\n";
    }
    cout << "\nDaftar Edge:\n";
    for (const auto& e : graph.getAllEdges()) {
        cout << e.getSource() << " -> " << e.getDestination()
                  << " | Jarak: " << e.getDistance() << " | Waktu: " << e.getTime() << " | Biaya: " << e.getCost() << "\n";
    }
}

void exportGraphToDOT(const Graph& graph, const string& filename) {
    ofstream ofs(filename);
    ofs << "digraph G {\n";
    // Vertex
    for (const auto& v : graph.getAllVertices()) {
        ofs << "  \"" << v.getId() << "\" [label=\"" << v.getId() << "\n(" << v.getLatitude() << "," << v.getLongitude() << ")\"]\n";
    }
    // Edge
    for (const auto& e : graph.getAllEdges()) {
        ofs << "  \"" << e.getSource() << "\" -> \"" << e.getDestination() << "\" [label=\"Jarak: " << e.getDistance() << "\\nWaktu: " << e.getTime() << "\\nBiaya: " << e.getCost() << "\"]\n";
    }
    ofs << "}\n";
}

void visualizeGraph(const Graph& graph) {
    const string dotFile = "graphviz_output.dot";
    const string pngFile = "graphviz_output.png";
    exportGraphToDOT(graph, dotFile);
    // Jalankan Graphviz
    string cmd = "dot -Tpng " + dotFile + " -o " + pngFile;
    int res = system(cmd.c_str());
    if (res == 0) {
        cout << "Graph image generated: " << pngFile << "\n";
        // Coba buka gambar (Linux: xdg-open, Windows: start, Mac: open)
        #ifdef __linux__
        system(("xdg-open " + pngFile + " &").c_str());
        #elif _WIN32
        system(("start " + pngFile).c_str());
        #elif __APPLE__
        system(("open " + pngFile).c_str());
        #endif
    } else {
        cout << "Gagal menjalankan Graphviz. Pastikan 'dot' sudah terinstall.\n";
    }
}

void savePreferenceToJSON(const Preference& preferences, const string& filename) {
    json j;
    j["weights"]["distance"] = preferences.getDistanceWeight();
    j["weights"]["time"] = preferences.getTimeWeight();
    j["weights"]["cost"] = preferences.getCostWeight();
    ofstream ofs(filename);
    ofs << j.dump(4);
    ofs.close();
    cout << "Preferensi berhasil disimpan ke " << filename << "\n";
}

int main() {
    try {
        Logger::init("app.log");
        Logger::info("Application started");

        // Load graph data
        cout << "Loading graph data...\n";
        Graph graph = CSVHandler::loadGraphFromCSV(
            "data/location.csv",
            "data/route.csv"
        );
        Logger::info("Graph data loaded successfully");

        // Load user preferences
        cout << "Loading user preferences...\n";
        Preference preferences = JSONHandler::loadPreferencesFromJSON(
            "data/preference.json"
        );
        Logger::info("User preferences loaded successfully");

        // Loop utama aplikasi
        while (true) {
            // CRUD menu
            int menu = -1;
            do {
                cout << "\n==== MENU CRUD GRAPH ====\n";
                cout << "1. Tampilkan graph\n";
                cout << "2. Tambah vertex\n";
                cout << "3. Ubah vertex\n";
                cout << "4. Hapus vertex\n";
                cout << "5. Tambah edge\n";
                cout << "6. Ubah edge\n";
                cout << "7. Hapus edge\n";
                cout << "8. Lanjut ke pencarian rute\n";
                cout << "9. Visualisasikan graph\n";
                cout << "0. Keluar\n";
                cout << "Pilih menu: ";
                cin >> menu;
                switch(menu) {
                    case 1: showGraphMenu(graph); break;
                    case 2: addVertexMenu(graph); break;
                    case 3: editVertexMenu(graph); break;
                    case 4: deleteVertexMenu(graph); break;
                    case 5: addEdgeMenu(graph); break;
                    case 6: editEdgeMenu(graph); break;
                    case 7: deleteEdgeMenu(graph); break;
                    case 8: break;
                    case 9: visualizeGraph(graph); break;
                    case 0: cout << "Keluar aplikasi.\n"; return 0;
                    default: cout << "Menu tidak valid!\n";
                }
            } while(menu != 8);

            // Input preferensi user
            int prefMenu = 0;
            while (true) {
                cout << "\nPilih preferensi utama pencarian rute:\n";
                cout << "1. Jarak terpendek\n";
                cout << "2. Waktu tercepat\n";
                cout << "3. Biaya termurah\n";
                cout << "Pilihan (1/2/3): ";
                cin >> prefMenu;
                if (prefMenu < 1 || prefMenu > 3 || cin.fail()) {
                    cout << "Input tidak valid! Pilih 1, 2, atau 3.\n";
                    cin.clear(); cin.ignore(1000, '\n');
                    continue;
                }
                break;
            }
            double wDist = 0, wTime = 0, wCost = 0;
            if (prefMenu == 1) wDist = 1.0;
            else if (prefMenu == 2) wTime = 1.0;
            else if (prefMenu == 3) wCost = 1.0;
            preferences.setDistanceWeight(wDist);
            preferences.setTimeWeight(wTime);
            preferences.setCostWeight(wCost);
            savePreferenceToJSON(preferences, "data/preference.json");

            // Get user input lokasi
            string start, goal;
            while (true) {
                cout << "\nAvailable locations:\n";
                for (const auto& vertex : graph.getAllVertices()) {
                    cout << vertex.getId() << " ";
                }
                cout << "\n\nEnter start location: ";
                cin >> start;
                cout << "Enter destination: ";
                cin >> goal;
                if (!graph.hasVertex(start) || !graph.hasVertex(goal)) {
                    cout << "Lokasi tidak ditemukan! Ulangi input.\n";
                    continue;
                }
                if (start == goal) {
                    cout << "Lokasi asal dan tujuan tidak boleh sama!\n";
                    continue;
                }
                break;
            }

            // Find path using both algorithms
            cout << "\nMencari rute optimal...\n";
            vector<string> dijkstraPath, aStarPath;
            bool dijkstraSuccess = false, astarSuccess = false;
            try {
                dijkstraPath = Dijkstra::findShortestPath(
                    graph, start, goal, preferences
                );
                dijkstraSuccess = !dijkstraPath.empty();
            } catch (const exception& e) {
                cout << "Error Dijkstra: " << e.what() << "\n";
            }
            try {
                aStarPath = AStar::findPath(
                    graph, start, goal
                );
                astarSuccess = !aStarPath.empty();
            } catch (const exception& e) {
                cout << "Error A*: " << e.what() << "\n";
            }
            cout << "\n==============================\n";
            cout << "Hasil Algoritma Dijkstra:\n";
            if (dijkstraSuccess) {
                printPath(graph, dijkstraPath);
                printPathDetails(graph, dijkstraPath);
            } else {
                cout << "Tidak ada rute ditemukan oleh Dijkstra.\n";
            }
            cout << "\n------------------------------\n";
            cout << "Hasil Algoritma A*:\n";
            if (astarSuccess) {
                printPath(graph, aStarPath);
                printPathDetails(graph, aStarPath);
            } else {
                cout << "Tidak ada rute ditemukan oleh A*.\n";
            }
            cout << "==============================\n";
            if (dijkstraSuccess && astarSuccess) {
                if (dijkstraPath == aStarPath) {
                    cout << "Kedua algoritma menemukan rute optimal yang sama!\n";
                } else {
                    cout << "Algoritma menemukan rute yang berbeda.\n";
                }
            }
            Logger::info("Path finding completed successfully");
            // Kembali ke menu utama
        }

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        Logger::error(string("Application error: ") + e.what());
        return 1;
    }
}
