#include "header.h"

using namespace std;

typedef struct node {
    bool isMWIS;
    int index;
    int weight;
    int degree;
    double priority;
} Node;

class MWIS {
public:
    MWIS();
    MWIS(ifstream& fin);
    MWIS(int, vector<int>, vector< vector<int> >);

    // Get functions
    int get_vertex();
    int get_MWIS_weight();
    vector<int> get_weight();
    vector<int> get_degree();
    vector<int> get_MWIS();
    vector<double> get_priority();
    vector<Node> get_nodes();
    vector< vector<int> > get_path();
    
    // Set functions
    void set_vertex(int);
    void set_weight(vector<int>);
    void set_weight_vector(int, ifstream& fin);
    void set_path(vector< vector<int> >);
    void set_path_vector(int, int, ifstream& fin);

    void resize_vertex(int);
    void calculate_degree_priority();
    void calculate_MWIS();
    void calculate_MWIS_weight();
    void update_degree_priority(int);
    void remove_MWIS(int);

    static bool compare_priority_max(const Node&, const Node&); 
    static bool compare_MWIS_min(const int&, const int&); 

private:
    int _vertex;
    int _MWIS_weight;
    vector<int> _MWIS_nodes;
    vector<Node> _nodes;
    vector< vector<int> > _path;
};