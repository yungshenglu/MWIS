#include "header.h"
#include "MWIS.h"

using namespace std;

bool compare_result(vector<bool> latest, vector<bool> current) {
    for (int i = 0; i < latest.size(); ++i) {
        if (latest[i] != current[i])
            return false;
    }

    return true;
}

int main(int argc, char *argv[]) {
    ifstream fin;
    vector<bool> latest, result;
    int vertex;
    char *filename = argv[1];
    
    // Read the input file
    fin.open(filename);
    while (!fin.eof()) {
        fin >> vertex;
        vector<MWIS> mwis(vertex);

        latest.resize(vertex, true);
        result.resize(vertex, false);

        for (int i = 0; i < mwis.size(); ++i) {
            mwis[i].set_weight(fin);
            mwis[i].set_index(i);
        }

        for (int i = 0; i < mwis.size(); ++i) {
            mwis[i].set_path(fin);
            mwis[i].calculate_degree_priority();
            mwis[i].set_map();
        }

        while (!compare_result(latest, result)) {
            for (int i = 0; i < result.size(); ++i)
                latest[i] = result[i];
            
            for (int j = 0; j < mwis.size(); ++j) {
                // Calculate the degree and priority for own vertex.
                mwis[j].send_msg();
                
                // Send msg to each vertex's neighbors.
                while (!mwis[j]._send_buff.empty()) {
                    Msg msg = mwis[j]._send_buff.front();
                    mwis[msg.receiver]._recv_buff.push(msg);
                    mwis[j]._send_buff.pop();
                }
            }
           
            for (int j = 0; j < mwis.size(); ++j) {
                // Receive all msg from own neighbors.
                mwis[j].recv_msg();
                result[j] = mwis[j].get_map()[j].isMWIS;
            }
        }

        vector<int> tmp;
        int MWIS_weight = 0;
        for (int i = 0; i < result.size(); ++i) {
            if (result[i]) {
                tmp.push_back(i);
                MWIS_weight += mwis[i].get_weight();
            }
        }

        // Print reuslt
        printf("MWIS: {");
        for (int i = 0; i < tmp.size(); ++i) {
            if (i == 0) {
                printf("%d", tmp[i]);
            } else {
                printf(", %d", tmp[i]);
            }
        }
        printf("}\n");
        printf("Total MWIS weight: %d\n", MWIS_weight);
    }

    return 0;
}