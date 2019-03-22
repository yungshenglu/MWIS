#include "header.h"

using namespace std;

typedef struct node {
    bool isMWIS;
    double priority;
} Node;

typedef struct msg {
    bool isMWIS;
    int sender;
    double priority;
    int receiver;
    vector<Node> map;
} Msg;

class MWIS {
public:
    MWIS();
    MWIS(int);

    // Get functions
    int get_weight();
    int get_degree();
    int get_index();
    bool get_isMWIS();
    double get_priority();
    vector<int> get_path();
    vector<int> get_neighbors();
    vector<Node> get_map();
    
    // Set functions
    void set_count(int);
    void set_index(int);
    void set_weight(ifstream& fin);
    void set_path(ifstream& fin);
    void set_map();

    void send_msg();
    void recv_msg();
    void calculate_degree_priority();
    void calculate_MWIS(Msg msg);
    void sort_msg();

    static bool compare_msg(const Msg&, const Msg&);

    queue<Msg> _send_buff;
    queue<Msg> _recv_buff;

private:
    bool _isMWIS;
    int _count;
    int _index;
    int _weight;
    int _degree;
    double _priority;
    vector<int> _path;
    vector<int> _neighbors;
    vector<Node> _map;
};