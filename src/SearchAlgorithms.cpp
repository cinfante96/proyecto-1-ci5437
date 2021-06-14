#include "SearchAlgorithms.hpp"

vector<int> NODES_PER_HEIGHT;

clock_t START, END;
double MAX_TIME_WITHOUT_PRUN = 1 * 60;
double MAX_TIME_WITH_PRUN = 15 * 60;

// breadth first search without pruning
Node* bfs(Node *root) {
    START = clock();

    queue<Node*> ord;
    ord.push(root);

    while (!ord.empty()) {
        Node *curr = ord.front(); ord.pop();

        if (curr->h >= NODES_PER_HEIGHT.size())
            NODES_PER_HEIGHT.push_back(1);
        else
            ++NODES_PER_HEIGHT[curr->h];

        if (is_goal(curr->state))
            return curr;

        ruleid_iterator_t my_it;
        init_fwd_iter(&my_it, curr->state);
        int rule_id;
        while ((rule_id = next_ruleid(&my_it)) >= 0)
            ord.push(curr->create_succ(rule_id));

        END = clock();
        double current_time = (double)(END - START) / CLOCKS_PER_SEC;
        if (current_time >= MAX_TIME_WITHOUT_PRUN)
            break;
    }
    return nullptr;
}

Node* bfs_with_pruning(Node *root) {
    START = clock();

    queue<Node*> ord;
    ord.push(root);

    set<uint64_t> visited; // cambiar por nuevo set
    visited.insert(hash_state(root->state)); // cambiar por nuevo set

    while (!ord.empty()) {
        Node *curr = ord.front(); ord.pop();

        if (curr->h >= NODES_PER_HEIGHT.size())
            NODES_PER_HEIGHT.push_back(1);
        else
            ++NODES_PER_HEIGHT[curr->h];

        if (is_goal(curr->state))
            return curr;

        ruleid_iterator_t my_it;
        init_fwd_iter(&my_it, curr->state);
        int rule_id;
        while ((rule_id = next_ruleid(&my_it)) >= 0) {
            Node *succ = curr->create_succ(rule_id);

            uint64_t hash = hash_state(succ->state);
            if (visited.find(hash) == visited.end()) { //cambiar por nuevo set
                ord.push(succ);
                visited.insert(hash); //cambiar por nuevo set
            }
            else
                delete succ;
        }

        END = clock();
        double current_time = (double)(END - START) / CLOCKS_PER_SEC;
        if (current_time >= MAX_TIME_WITH_PRUN)
            break;
    }
    return nullptr;
}

void print_nodes_per_height() {
    for (int i = 0; i < NODES_PER_HEIGHT.size(); ++i)
        printf("%d : %d\n", i, NODES_PER_HEIGHT[i]);
}
