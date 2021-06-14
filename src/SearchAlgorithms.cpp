#include "SearchAlgorithms.hpp"

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
        if (current_time >= MAX_TIME)
            break;
    }
    return nullptr;
}