#include "../../include/graph/graph.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

using namespace std;

void Graph::addVertex(const Vertex& vertex) {
    if (vertices.find(vertex.getId()) != vertices.end()) {
        throw runtime_error("Vertex with ID " + vertex.getId() + " already exists");
    }
    vertices[vertex.getId()] = vertex;
    adjacencyList[vertex.getId()] = {};
}

void Graph::removeVertex(const string& vertexId) {
    if (vertices.find(vertexId) == vertices.end()) {
        throw runtime_error("Vertex with ID " + vertexId + " not found");
    }
    // Remove all edges from adjacencyList
    adjacencyList.erase(vertexId);
    // Remove all edges to this vertex
    for (auto& [src, edges] : adjacencyList) {
        edges.erase(remove_if(edges.begin(), edges.end(),
            [&](const Edge& e) { return e.getDestination() == vertexId; }), edges.end());
    }
    vertices.erase(vertexId);
}

const Vertex& Graph::getVertex(const string& vertexId) const {
    auto it = vertices.find(vertexId);
    if (it == vertices.end()) {
        throw runtime_error("Vertex with ID " + vertexId + " not found");
    }
    return it->second;
}

void Graph::addEdge(const Edge& edge) {
    if (vertices.find(edge.getSource()) == vertices.end() ||
        vertices.find(edge.getDestination()) == vertices.end()) {
        throw runtime_error("One or both vertices not found");
    }
    adjacencyList[edge.getSource()].push_back(edge);
}

void Graph::removeEdge(const string& source, const string& destination) {
    auto it = adjacencyList.find(source);
    if (it == adjacencyList.end()) return;
    auto& edges = it->second;
    edges.erase(remove_if(edges.begin(), edges.end(),
        [&](const Edge& e) { return e.getDestination() == destination; }), edges.end());
}

double Graph::getEdgeWeight(const string& source, const string& destination, int preferenceType) const {
    auto it = adjacencyList.find(source);
    if (it == adjacencyList.end()) throw runtime_error("Source vertex not found");
    for (const auto& edge : it->second) {
        if (edge.getDestination() == destination) {
            return edge.getWeight(preferenceType);
        }
    }
    throw runtime_error("Edge not found");
}

vector<string> Graph::getNeighbors(const string& vertexId) const {
    vector<string> neighbors;
    auto it = adjacencyList.find(vertexId);
    if (it != adjacencyList.end()) {
        for (const auto& edge : it->second) {
            neighbors.push_back(edge.getDestination());
        }
    }
    return neighbors;
}

bool Graph::hasVertex(const string& vertexId) const {
    return vertices.find(vertexId) != vertices.end();
}

bool Graph::hasEdge(const string& source, const string& destination) const {
    auto it = adjacencyList.find(source);
    if (it == adjacencyList.end()) return false;
    for (const auto& edge : it->second) {
        if (edge.getDestination() == destination) return true;
    }
    return false;
}

vector<Vertex> Graph::getAllVertices() const {
    vector<Vertex> result;
    for (const auto& [id, v] : vertices) {
        result.push_back(v);
    }
    return result;
}

vector<Edge> Graph::getAllEdges() const {
    vector<Edge> result;
    for (const auto& [src, edges] : adjacencyList) {
        for (const auto& e : edges) {
            result.push_back(e);
        }
    }
    return result;
}