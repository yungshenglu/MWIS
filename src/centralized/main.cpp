#include "header.h"
#include "MWIS.h"

using namespace std;

int main(int argc, char *argv[]) {
    ifstream fin;
    int vertex;
    char *filename = argv[1];
    vector<int> weight;
    vector< vector<int> > path;

    // Read the input file
    fin.open(filename);
    while (!fin.eof()) {
        MWIS mwis(fin);

        for (int i = 0; i < mwis.get_vertex(); ++i)
            mwis.set_weight_vector(i, fin);

        for (int i = 0; i < mwis.get_vertex(); ++i) {
            for (int j = 0; j < mwis.get_vertex(); ++j)
                mwis.set_path_vector(i, j, fin);
        }

        // Calculate the degree and priority for each vertex.
        mwis.calculate_degree_priority();

        // Calculate the MWIS.
        mwis.calculate_MWIS();

        // Calculate the total of MWIS weight.
        mwis.calculate_MWIS_weight();

        vector<int> MWIS_nodes = mwis.get_MWIS();
        printf("MWIS: {");
        for (int i = 0; i < MWIS_nodes.size(); ++i) {
            if (i == 0)
                printf("%d", MWIS_nodes[i]);
            else
                printf(",%d", MWIS_nodes[i]);
        }
        printf("}\n");
        printf("Total MWIS weight: %d\n", mwis.get_MWIS_weight());
    }

    return 0;
}