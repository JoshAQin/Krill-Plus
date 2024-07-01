#ifndef TEST_H
#define TEST_H

bool is_equal(int* results, int num_jobs, int num_nodes) {
    // return true;
    int correct[num_nodes] = {0,-1,89,13,13,-1,14,17,11,22,80,51,25,22,89,51,6,22,69,32,113,49,0,37,112,17,2,84,3,6,14,39,51,27,4,22,0,9,14,0,-1,13,97,41,15,0,79,97,85,84,110,22,13,7,39,25,0,17,2,14,12,14,72,92,7,20,13,94,13,51,31,81,79,75,84,39,51,59,54,39,26,0,13,25,45,36,2,38,41,0,106,57,9,85,13,70,39,84,81,38,65,2,2,106,14,76,0,15,22,17,13,75,4,81,51,17,54,125,11,35,25,86,13,48,78,90,15,21};
    bool flag = true;
    for (int j = 0; j < num_jobs; ++j) {
        for (int i = 0; i < num_nodes; ++i){
            int idx = i; // i+j*num_nodes
            if (results[idx] != correct[i]) {
                std::cout << j << " " << i << " " << correct[i] << " " << results[idx] << std::endl;
                flag = false;
            }
        }
    }
    if (flag)
        return true;
    else
        return false;
}

#endif // TEST_H