# Maximal Weighted Independent Set Problem

[![License: IEEE](https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg)](http://creativecommons.org/licenses/by-nc-sa/4.0/)

This repository is going to solve the **Maximal Weighted Independent Set (MWIS)** problem by given each vertex's weight. *Notice that this repository is NCTU CS course assignment of "Distributed Algorithnm". If you are taking this course, please do not duplicate from this repository. All rights reserved.*

## Introduction

### Maximum Independent Set (MIS)
In an undirected graph $G = (V, E)$, where $V$ is the vertex set and $E$ is the edge set, $S \subseteq V$ is an independent set if no vertices in $S$ are adjacent to one another. An independent set that has the maximum cardinality is called **maximum independent set**.
If each vertex is associated with a weight (a positive real number), then $S$ is a maximum weighted independent set if it is an independent set that has the maximum total weight among all such sets. Clearly, a maximum independent set is a maximum weighted independent set with uniform weight. Finding either set is known to be NP-hard, for which polynomial-time approximation is NP-hard as well.

### Maximal Weighted Independent Set (MWIS)

There exist many heuristics for these two problems. One well-known greedy approach works by selecting a vertex into the set, removing it and adjacent vertices from the graph, and repeating this process on the remaining graph. The result found by the *greedy approach* can only be a maximal independent set (MIS), an independent set of which no proper superset is also an independent set. If nodes are associated with weights, then the result is a **maximal weighted independent set (MWIS)**.

---
## Solutions

### Centralized programming

Read the input test file and estblish a path map to note each vertex's neighbors. By sorting the priority of each vertex, we can update the latest MWIS set.
* Read the input test file and establish an object called `mwis`.
* Set the path and each vertex's weight by calling function `set_weight_vector` and `set_path_vector`.
* Calculate the degree and the priority by calling function `calculate_degree_priority`.
* Calculate the MWIS set by calling function `calculate_MWIS`.
    * Check each vertex is MWIS or not.
    * If not, push the current vertex into MWIS set first. and then calculate its neighbors.
    * Update the latest degree and priority by calling function `update_degree_priority`.
* Calculate the total weight of vertex in MWIS set by calling `calculate_MWIS_weight`.
* Print the result of MWIS set and it's total weight.

### Distributed programming

Simulate the **mailbox operation**, let each vertex to have own send buffer and reveived buffer to pass the own information to it's neightbors.
* Read the test file and establish each node with an object and store into the vector called `mwis` with type `MWIS`.
* Establosh another two vector called `latest` and `result` to store each run's reault and pre-run's reault.
* Set the path and each vertex's weight by calling function `set_wieght` and `set_path`.
* Set a vector called `map` to store the each vertex's known the current MWIS set.
* Calculate the degree and priority by calling function `calculate_degree_priority`.
* Calculate the MWIS set in several runs.
    * Each run should *compare the current and latest result of MWIS set is same or not*.
        * If it is same, then the program should break the loop.
    * In Each run, there are **two sub-runs** in it.
        * The *first* sub-run is that each vertex should push all send messages to it's own buffer called `send_buff`.
            * The content of message include the information of sender, receiver, the sender's priority, and each vertex known information.
            * All send message in going to be sent to its neighbor.
        * The *second* sub-run is that each vertex should pop out all receive messages from it's own buffer called `recv_buff`.
            * Decide whether it is in the MWIS set or not by all received messages.
            * The received messages should be sorted in each sub-run in priority order due to the lower priority message should not affect the decision with the higher priority message.
* Calculate the total weight of the MWIS set.
* Print the result of MWIS set and it's total weight.

---
## Structure

* Folder `centralized` - program by centralized method
    * `Makefile`
    * `main.cpp` - main function
    * `MWIS.cpp` - implement class for MWIS
    * `MWIS.h` - class for MWIS
    * `header.h` - include all libraries
    * `test1.txt` - input file
    * `test_result1.txt` - reference result for input file
* Folder `distributed` - program by distributed method
    * `Makefile`
    * `main.cpp` - main function
    * `MWIS.cpp` - implement class for MWIS
    * `MWIS.h` - class for MWIS
    * `header.h` - include all libraries
    * `test2.txt` - input file
    * `test_result2.txt` - reference result for input file

---
## Installation

### Centralized

```bash
# Compilation under centralized folder
$ make

# Execution for centralized program
$ ./main test1.txt
```

### Distributed

```bash
# Compilation under distributed folder
$ make

# Exrcution for distributed program
$ ./main test2.txt
```

---
## Results

### Centralized
```bash
$ ./main test1.txt
MWIS: {0, 3, 4, 5, 7, 8}
Total MWIS weight: 259
```

### Distributed

```bash
$ ./main test2.txt
MWIS: {1, 4, 5, 7, 8, 9}
Total MWIS weight: 274
```

---
## TODO

(Update soon.)

---
## Author

* [Yung-Sheng Lu](https://github.com/yungshenglu)

---
> This repository is the NCTU CS course assignment of "Distributed Algorithnm". If you are taking this course, please do not duplicate from this repository. All rights reserved.

[![License: IEEE](https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg)](http://creativecommons.org/licenses/by-nc-sa/4.0/)