#include "header.h"
#include "MWIS.h"

using namespace std;

// Constructor
MWIS::MWIS() {
    MWIS::set_vertex(10);
    MWIS::resize_vertex(10);
    _MWIS_weight = 0;
}

MWIS::MWIS(ifstream& fin) {
    fin >> _vertex;
    MWIS::set_vertex(_vertex);
    MWIS::resize_vertex(_vertex);
    _MWIS_weight = 0;
}

MWIS::MWIS(int vertex, vector<int> weight, vector< vector<int> > path) {
    MWIS::set_vertex(vertex);
    MWIS::resize_vertex(vertex);
    MWIS::set_weight(weight);
    MWIS::set_path(path);
    _MWIS_weight = 0;
}

int MWIS::get_vertex() {
    return _vertex;
}

int MWIS::get_MWIS_weight() {
    return _MWIS_weight;
}

vector<int> MWIS::get_weight() {
    vector<int> weight(_vertex);

    for (int i = 0; i < weight.size(); ++i)
        weight[i] = _nodes[i].weight;

    return weight;
}

vector<int> MWIS::get_degree() {
    vector<int> degree(_vertex);
    
    for (int i = 0; i < degree.size(); ++i)
        degree[i] = _nodes[i].degree;

    return degree;
}

vector<int> MWIS::get_MWIS() {
    // Sorting into minimum priority order
    sort(_MWIS_nodes.begin(), _MWIS_nodes.end(), MWIS::compare_MWIS_min);
    return _MWIS_nodes;
}

vector<double> MWIS::get_priority() {
    vector<double> priority(_vertex);
    
    for (int i = 0; i < priority.size(); ++i)
        priority[i] = _nodes[i].priority;

    return priority;
}

vector<Node> MWIS::get_nodes() {
    return _nodes;
}

vector< vector<int> > MWIS::get_path() {
    return _path;
}

void MWIS::set_vertex(int vertex) {
    _vertex = vertex;
}

void MWIS::set_weight(vector<int> weight) {
    for (int i = 0; i < weight.size(); ++i) {
        _nodes[i].weight = weight[i];
        _nodes[i].index = i;
    }
}

void MWIS::set_weight_vector(int i, ifstream& fin) {
    fin >> _nodes[i].weight;
    _nodes[i].index = i;
}

void MWIS::set_path(vector< vector<int> > path) {
    int count;

    for (int i = 0; i < path.size(); ++i) {
        for (int j = 0; j < path[i].size(); ++j) {
            _path[i][j] = path[i][j];
            if (path[i][j] == 1)
                _nodes[i].degree += 1;
        }
        _nodes[i].priority = (double)_nodes[i].weight / (double)_nodes[i].degree;
    }
}

void MWIS::set_path_vector(int i, int j, ifstream& fin) {
    fin >> _path[i][j];  
}

void MWIS::resize_vertex(int vertex) {
    // Resize nodes vector
    _nodes.resize(vertex);

    // Resize path vector
    _path.resize(vertex);
    for (int i = 0; i < vertex; ++i) {
        _path[i].resize(vertex);
        _nodes[i].isMWIS = true;
    }
}

void MWIS::calculate_degree_priority() {
    for (int i = 0; i < _path.size(); ++i) {
        for (int j = 0; j < _path[i].size(); ++j) {
            if (_path[i][j] == 1)
                _nodes[i].degree += 1;
        }

        _nodes[i].priority = (double)_nodes[i].weight / ((double)_nodes[i].degree + 1);
    }

    // Sorting into maximum priority order
    sort(_nodes.begin(), _nodes.end(), MWIS::compare_priority_max);
}

void MWIS::calculate_MWIS() {
    for (int i = 0; i < _nodes.size(); ++i) {
        if (_nodes[i].isMWIS) {
            _MWIS_nodes.push_back(_nodes[i].index);
            for (int j = 0; j < _path[_nodes[i].index].size(); ++j) {
                if (_path[_nodes[i].index][j] == 1) {
                   MWIS::remove_MWIS(j);
                   _path[_nodes[i].index][j] = 0;
                   _path[j][_nodes[i].index] = 0;
                   MWIS::update_degree_priority(_nodes[i].index);
                   MWIS::update_degree_priority(j);
                }
            }
        }
    }
}

void MWIS::calculate_MWIS_weight() {
    for (int i = 0; i < _MWIS_nodes.size(); ++i) {
        for (int j = 0; j < _nodes.size(); ++j) {
            if (_nodes[j].index == _MWIS_nodes[i])
                _MWIS_weight += _nodes[j].weight;
        }
    }
}

void MWIS::update_degree_priority(int index) {
    _nodes[index].degree -= 1;
    _nodes[index].priority = (double)_nodes[index].weight / ((double)_nodes[index].degree + 1);
}

void MWIS::remove_MWIS(int index) {
    for (int i = 0; i < _nodes.size(); ++i) {
        if (_nodes[i].index == index) {
            _nodes[i].isMWIS = false;
            break;
        }
    }
}

bool MWIS::compare_priority_max(const Node& a, const Node& b) {
    if (a.priority == b.priority)
        return b.index > a.index;
    return b.priority < a.priority;
}


bool MWIS::compare_MWIS_min(const int& a, const int& b) {
    return b > a;
}