#include "header.h"
#include "MWIS.h"

using namespace std;

typedef struct result {
    vector<int> result_list;
    int count;
    int weight;
} Result;

vector<Result> result;

bool compare_result(vector<bool> latest, vector<bool> current) {
    for (int i = 0; i < latest.size(); ++i) {
        if (latest[i] != current[i])
            return false;
    }

    return true;
}

void store_result(vector<int> current, int weight) {
    bool isSameWithLatest = true;
    Result latest;

    if (result.empty()) {
        latest.result_list.assign(current.begin(), current.end());
        latest.count = 1;
        latest.weight = weight;
        result.push_back(latest);
        return;
    }

    for (int i = 0; i < result.size(); ++i) {
        for (int j = 0; j < result[i].result_list.size(); ++j) {
            if (current[j] != result[i].result_list[j]) {
                latest.result_list.assign(current.begin(), current.end());
                latest.count = 1;
                latest.weight = weight;
                result.push_back(latest);
                isSameWithLatest = false;
                break;
            }
        }

        if (isSameWithLatest) {
            result[i].count += 1;
        }
    }
}

int main(int argc, char *argv[]) {
    ifstream fin;
    vector<bool> latest, current;
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

        for (int i = 0; i < mwis.size(); ++i) {
            mwis[i].set_weight(fin);
            mwis[i].set_index(i);
            mwis[i].set_path(fin);
            mwis[i].calculate_degree_priority();
            mwis[i].set_map();
        }

        // Set random seeds
        srand(time(NULL));

        for (int t = 0; t < simulation_times; ++t) {
            latest.resize(vertex, true);
            current.resize(vertex, false);

            for (int i = 0; i < mwis.size(); ++i) {
                // Random nodes into the set.
                srand(time(NULL));
                mwis[i].set_isMWIS(rand() % 2);
            }
            
            while (!compare_result(latest, current)) {
                for (int i = 0; i < current.size(); ++i)
                    latest[i] = current[i];
                
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
                    current[j] = mwis[j].get_map()[j].isMWIS;
                }
            }

            MWIS_weight = 0;
            vector<int> tmp;
            for (int i = 0; i < current.size(); ++i) {
                if (current[i]) {
                    tmp.push_back(i);
                    MWIS_weight += mwis[i].get_weight();
                }
            }
            
            isSame = true;
            if (t == 0) {
                tmp_latest.assign(tmp.begin(), tmp.end());
                store_result(tmp, MWIS_weight);
            } else {
                store_result(tmp, MWIS_weight);
                for (int i = 0; i < tmp.size(); ++i) {
                    if (tmp[i] != tmp_latest[i]) {
                        isSame = false;
                    }
                }
                tmp_latest.assign(tmp.begin(), tmp.end());
            }

            // test
            for (int i = 0; i < tmp_latest.size(); ++i) {
                printf("%d ", tmp_latest[i]);
            }
            printf("\n---");
        }

        // Print reuslt
        if (isSame) {
            printf("All %d times results are the same.\n", simulation_times);
            for (int i = 0; i < result.size(); ++i) {
                printf("MWIS: {");
                for (int j = 0; j < result[i].result_list.size(); ++j) {
                    if (j == 0) {
                        printf("%d", result[i].result_list[j]);
                    } else {
                        printf(", %d", result[i].result_list[j]);
                    }
                }
                printf("}, counts: %d, perc: %lf\n", result[i].count, (double)(result[i].count / simulation_times));
                printf("Total MWIS weight: %d\n", result[i].weight);
            }
        } else {
            printf("All %d times results have some different.\n", simulation_times);
            for (int i = 0; i < result.size(); ++i) {
                printf("MWIS: {");
                for (int j = 0; j < result[i].result_list.size(); ++j) {
                    if (j == 0) {
                        printf("%d", result[i].result_list[j]);
                    } else {
                        printf(", %d", result[i].result_list[j]);
                    }
                }
                printf("}, counts: %d, perc: %lf\n", result[i].count, (double)(result[i].count / simulation_times));
                printf("Total MWIS weight: %d\n", result[i].weight);
            }
        }
    }

    return 0;
}