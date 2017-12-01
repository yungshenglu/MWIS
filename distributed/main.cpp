#include "header.h"
#include "MWIS.h"

using namespace std;

typedef struct table {
    vector<int> result;
    int count;
    int weight;
    int convergence;
} Table;

vector<Table> result_table;

void append_result(vector<int> result, int weight, int convergence) {
    Table new_result = { 
        result, 1, weight, convergence
    };

    result_table.push_back(new_result);
}

void store_result(vector<int> result, int weight, int convergence) {
    if (!result_table.size()) {
        append_result(result, weight, convergence);
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
            append_result(result, weight, convergence);
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
    FILE *fout;
    vector<bool> last, curr;
    vector<int> tmp_latest;
    int vertex, MWIS_weight = 0;
    char *filename = argv[1];
    int simulation_times = 1000;
    int convergence = 0;

    // Set random seeds.
    srand(time(NULL));

    // Open the output file
    fout = fopen("result.txt","w");

    // Read the input file
    fin.open(filename);
    while (!fin.eof()) {
        fin >> vertex;
        vector<MWIS> mwis(vertex);

        for (int i = 0; i < mwis.size(); ++i) {
            mwis[i].set_weight(fin);
            mwis[i].set_index(i);
        }

        for (int i = 0; i < mwis.size(); ++i) {
            mwis[i].set_path(fin);
            mwis[i].calculate_degree_priority();
            mwis[i].set_map();
        }

        for (int p = 0; p < 10; ++p) {
            double prob = p * 0.1;

            // Initialize
            result_table.clear();

            for (int t = 0; t < simulation_times; ++t) {
                last.resize(vertex, true);
                curr.resize(vertex, false);
                convergence = 0;

                // Initial all the nodes are not in the set
                for (int i = 0; i < mwis.size(); ++i) {
                    mwis[i].set_isMWIS(false);
                }

                int count = 0;
                while (1) {
                    ++convergence;

                    if (!compare_result(last, curr)) {
                        count = 0;
                        for (int i = 0; i < curr.size(); ++i)
                            last[i] = curr[i];
                    } else {
                        if (++count >= 100)
                            break;
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

                    for (int j = 0; j < mwis.size(); ++j) {
                        double k = (rand() % 11) / 10.0;
                        // Receive all msg from own neighbors.
                        if (k <= prob) {
                            mwis[j].recv_msg(true);
                        } else {
                            mwis[j].recv_msg(false);
                        }

                        curr[j] = mwis[j].get_map()[j].isMWIS;
                    }
                }

                MWIS_weight = 0;
                vector<int> tmp;
                for (int i = 0; i < curr.size(); ++i) {
                    if (curr[i]) {
                        tmp.push_back(i);
                        MWIS_weight += mwis[i].get_weight();
                    }
                }

                // Store possible results
                store_result(tmp, MWIS_weight, convergence);
            }

            double avg_weight = 0;
            int avg_convergence = 0;
            printf("Simulation results with probability %.0f%%:\n", prob * 100);
            for (int i = 0; i < result_table.size(); ++i) {    
                printf("MWIS: {");
                for (int j = 0; j < result_table[i].result.size(); ++j) {
                    if (j == 0) {
                        printf("%d", result_table[i].result[j]);
                    } else {
                        printf(", %d", result_table[i].result[j]);
                    }
                }
                double perc = (double)result_table[i].count / (double)simulation_times;
                printf("}, probability: %.1f%%, Total MWIS weight: %d\n", perc * 100, result_table[i].weight);
                avg_weight += (result_table[i].weight * perc);
                avg_convergence += (result_table[i].convergence * perc);
            }
            printf("Average weight: %.2f, Convergence time: %d\n", avg_weight, avg_convergence);
            fprintf(fout, "%.1f %.2f %d\n", prob, avg_weight, avg_convergence);
        }

        fclose(fout);
    }

    // Plotting
    system("python plot_avg_weight.py");
    system("python plot_avg_convergence.py");

    return 0;
}