#include "header.h"
#include "MWIS.h"

using namespace std;

typedef struct table {
    vector<int> result;
    int count;
    int weight;
} Table;

vector<Table> result_table;

void append_result(vector<int> result, int weight) {
    Table new_result = { 
        result, 1, weight
    };

    result_table.push_back(new_result);
}

void store_result(vector<int> result, int weight) {
    if (!result_table.size()) {
        append_result(result, weight);
    } else {
        bool flag = true;

        for (int i = 0; i < result_table.size(); ++i) {
            flag = true;
            for (int j = 0; j < result_table[i].result.size(); ++j) {
                if (result[j] != result_table[i].result[j]) {
                    flag = false;
                    break;
                }

                if (j == result_table[i].result.size() - 1 && flag) {
                    result_table[i].count += 1;
                    return;
                }
            }
        }

        if (!flag) {
            append_result(result, weight);
        }
    }
}

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
    //bool isSame = true;

    // Set random seeds.
    srand(time(NULL));

    // Read the input file
    fin.open(filename);
    while (!fin.eof()) {
        fin >> vertex;
        vector<MWIS> mwis(vertex);

        for (int i = 0; i < mwis.size(); ++i) {
            mwis[i].set_weight(fin);
            mwis[i].set_index(i);
        }

        for (int t = 0; t < simulation_times; ++t) {
            latest.resize(vertex, true);
            result.resize(vertex, false);

            for (int i = 0; i < mwis.size(); ++i) {
                mwis[i].set_path(fin);
                mwis[i].calculate_degree_priority();
                mwis[i].set_map();

                // Random nodes into the set.
                mwis[i].set_isMWIS(true);
            }

            int count = 0;

            while (1) {
                if (compare_result(latest, result)) {
                    ++count;
                    if (count == 100)
                        break;
                } else {
                    for (int i = 0; i < result.size(); ++i)
                        latest[i] = result[i];
                }

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

                int k = rand() % 10;
                mwis[k].recv_msg(true);
                result[k] = mwis[k].get_map()[k].isMWIS;
            
                /*for (int j = 0; j < mwis.size(); ++j) {
                    // Receive all msg from own neighbors.
                    if (j == k) {
                        mwis[j].recv_msg(true);
                    } else {
                        mwis[j].recv_msg(false);
                    }

                    result[j] = mwis[j].get_map()[j].isMWIS;
                }*/
            }

            // test
            /*for (int i = 0; i < result.size(); ++i) {
                if (result[i]) {
                    printf("%d ", i);
                }
            }
            printf("\n");*/

            MWIS_weight = 0;
            vector<int> tmp;
            for (int i = 0; i < result.size(); ++i) {
                if (result[i]) {
                    tmp.push_back(i);
                    MWIS_weight += mwis[i].get_weight();
                }
            }

            // Store possible results
            store_result(tmp, MWIS_weight);
        }

        printf("Simulation results:\n");
        for (int i = 0; i < result_table.size(); ++i) {
            printf("MWIS: {");
            for (int j = 0; j < result_table[i].result.size(); ++j) {
                if (j == 0) {
                    printf("%d", result_table[i].result[j]);
                } else {
                    printf(", %d", result_table[i].result[j]);
                }
            }
            printf("}, probability: %d%%, Total MWIS weight: %d\n", (result_table[i].count / simulation_times) * 100, result_table[i].weight);
        }
    }

    return 0;
}