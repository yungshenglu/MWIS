#include "header.h"
#include "MWIS.h"

using namespace std;

// Constructor
MWIS::MWIS() {
    _count = 10;
    _isMWIS = false;
    _path.resize(10);
    _map.resize(10);
}

MWIS::MWIS(int count) {
    _count = count;
    _isMWIS = false;
    _path.resize(count);
    _map.resize(_count);
}

int MWIS::get_weight() {
    return _weight;
}

int MWIS::get_degree() {
    return _degree;
}

int MWIS::get_index() {
    return _index;
}

bool MWIS::get_isMWIS() {
    return _isMWIS;
}

double MWIS::get_priority() {
    return _priority;
}

vector<int> MWIS::get_path() {
    return _path;
}

vector<int> MWIS::get_neighbors() {
    return _neighbors;
}

vector<Node> MWIS::get_map() {
    return _map;
}

void MWIS::set_count(int count) {
    _count = count;
    _path.resize(_count);
    _map.resize(_count);
}

void MWIS::set_index(int index) {
    _index = index;
}

void MWIS::set_weight(ifstream& fin) {
    fin >> _weight;
}

void MWIS::set_map() {
    for (int i = 0; i < _map.size(); ++i) {
        _map[i].isMWIS = false;
    }

    _map[_index].priority = _priority;
}

void MWIS::set_path(ifstream& fin) {
    for (int i = 0; i < _path.size(); ++i) {
        fin >> _path[i];
        if (_path[i] == 1) {
            _degree += 1;
            _neighbors.push_back(i);
        }   
    }
}

void MWIS::set_isMWIS(bool isMWIS) {
    _isMWIS = isMWIS;
}

void MWIS::send_msg() {
    for (int i = 0; i < _neighbors.size(); ++i) {
        Msg msg = {
            _isMWIS,
            _index,
            _priority,
            _neighbors[i],
            _map
        };
        _send_buff.push(msg);
    }
}

void MWIS::recv_msg() {
    MWIS::sort_msg();

    while (!_recv_buff.empty()) {
        Msg recv = _recv_buff.front();
        for (int i = 0; i < _map.size(); ++i) {
            _map[i].priority = recv.map[i].priority;
            _map[i].isMWIS = recv.map[i].isMWIS;
        }
        MWIS::calculate_MWIS(recv);
        _recv_buff.pop();
    }
}

void MWIS::calculate_degree_priority() {
    _priority = (double)_weight / (double)(_degree + 1);
}

void MWIS::calculate_MWIS(Msg msg) {
    if (msg.priority < _priority) {
        _isMWIS = true;
        _map[_index].isMWIS = true;
    } else {
        if (msg.isMWIS == false) {
            _isMWIS = true;
            _map[_index].isMWIS = true;
        } else { 
            _isMWIS = false;
            _map[_index].isMWIS = false;
        }
    }
}

void MWIS::sort_msg() {
    vector<Msg> tmp;
    while (!_recv_buff.empty()) {
        tmp.push_back(_recv_buff.front());
        _recv_buff.pop();
    }

    sort(tmp.begin(), tmp.end(), MWIS::compare_msg);

    for (int i = 0; i < tmp.size(); ++i) {
        _recv_buff.push(tmp[i]);
    }
}

bool MWIS::compare_msg(const Msg& a, const Msg& b) {
    if (a.priority == b.priority) {
        return b.sender < a.sender;
    }
    return b.priority > a.priority;
} 