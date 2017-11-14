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
    vector<int> tmp_latest;
    int vertex, MWIS_weight = 0;
    char *filename = argv[1];
    int simulation_times = 1000;
    bool isSame = true;
    
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

        srand(time(NULL));
        for (int i = 0; i < mwis.size(); ++i) {
            mwis[i].set_path(fin);
            mwis[i].calculate_degree_priority();
            mwis[i].set_map();

            // Random nodes into the set.
            mwis[i].set_isMWIS(rand() % 2);
        }

        // test
        for (int s = 0; s < mwis.size(); ++s) {
            printf("%d ", mwis[s].get_isMWIS());
        }
        printf("\n");
        
        for (int t = 0; t < simulation_times; ++t) {
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

            MWIS_weight = 0;
            vector<int> tmp;
            for (int i = 0; i < result.size(); ++i) {
                if (result[i]) {
                    tmp.push_back(i);
                    MWIS_weight += mwis[i].get_weight();
                }
            }
            
            isSame = true;
            if (t == 0) {
                tmp_latest.assign(tmp.begin(), tmp.end());
            } else {
                for (int i = 0; i < tmp.size(); ++i) {
                    if (tmp[i] != tmp_latest[i]) {
                        isSame = false;
                        break;
                    }
                }
                tmp_latest.assign(tmp.begin(), tmp.end());
            }
        }

        // Print reuslt
        if (isSame) {
            printf("All %d times results are same.\n", simulation_times);
            printf("MWIS: {");
            for (int i = 0; i < tmp_latest.size(); ++i) {
                if (i == 0) {
                    printf("%d", tmp_latest[i]);
                } else {
                    printf(", %d", tmp_latest[i]);
                }
            }
            printf("}\n");
            printf("Total MWIS weight: %d\n", MWIS_weight);
        }
    }

    return 0;
}