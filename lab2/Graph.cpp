//  File: Graph.cpp ------------------------------------------------------------
//  Assignment 2
//  Course: CPSC 526
//  Author: Xi Wang
//  Time: Oct 22, 2016
// ----------------------------------------------------------------------------

#include "Graph.hpp"

u64 Graph::get_node_num() {
    return node_num;
}

bool Graph::get_node(u64 node_id) {
    if (adj_list.count(node_id)) {
        return true;
    }
    return false;
}

bool Graph::get_edge(u64 node_a_id, u64 node_b_id) {
    if (adj_list.count(node_a_id) && adj_list[node_a_id].count(node_b_id)) {
        return true;
    }
    return false;
}

set<u64> Graph::get_neighbors(u64 node_id) {
    return adj_list[node_id];
}

void Graph::add_node(u64 node_id) {
    set<u64> neighbors;
    adj_list[node_id] = neighbors;
    ++node_num;
}

void Graph::add_edge(u64 node_a_id, u64 node_b_id) {
    adj_list[node_a_id].insert(node_b_id);
    adj_list[node_b_id].insert(node_a_id);
}

void Graph::remove_node(u64 node_id) {
    set<u64> neighbors = adj_list[node_id];
    for (u64 ne: neighbors) {
        adj_list[ne].erase(node_id);
    }
    adj_list.erase(node_id);
    --node_num;
}

void Graph::remove_edge(u64 node_a_id, u64 node_b_id) {
    adj_list[node_a_id].erase(node_b_id);
    adj_list[node_b_id].erase(node_a_id);
}

u64 Graph::shortest_path(u64 node_a_id, u64 node_b_id){
    unordered_map<u64, bool> visited;
    unordered_map<u64, u64> dist;

    for(auto adj: adj_list){
        dist[adj.first] = UINT64_MAX;
    }
    dist[node_a_id] = 0;
    priority_queue<pair<u64, u64>, vector<pair<u64, u64>>, greater<pair<int, int>>> pq;
    pq.push(make_pair(0, node_a_id));

    while(!pq.empty()){
        u64 v = pq.top().second;
        pq.pop();
        if(v == node_b_id) return dist[v];
        if(visited[v]) continue;
        visited[v] = true;

        for(u64 u: adj_list[v]){
            if(!visited[u] && dist[u] > dist[v] + 1){
                dist[u] = dist[v] + 1;
                pq.push(make_pair(dist[u], u));
            }
        }
    }
    return 0;
}

void Graph::display() {
    cout << "Graph Display Start -----------------------------------------" << endl;
    map<u64, set<u64>>::iterator it;
    for (it = adj_list.begin(); it != adj_list.end(); ++it) {
        set<u64>::iterator set_it;
        cout << it->first << ": ";
        for (set_it = it->second.begin(); set_it != it->second.end(); ++set_it) {
            cout << *set_it << ' ';
        }
        cout << endl;
    }
    cout << "Graph Display end -------------------------------------------" << endl;

    //cout << serialize() << endl;
}

string Graph::serialize() {
    string res;
    for(auto adj: adj_list){
        res += to_string(adj.first) + "*"; //node end
        for(u64 node: adj.second){
            res += to_string(node) + "*";  
        }
        res += "$"; //list end
    }
    return res += "!"; //graph end
}

void Graph::deserialize(string s) {
    int end = s.find_first_of("!");
    for(int i = 0; i < end;){
        int pos = s.find_first_of("$", i);
        int node_a_end = s.find_first_of("*", i);
        int node_a_id = stoi(s.substr(i, node_a_end-i));
        add_node(node_a_id);
        for(int j = node_a_end + 1; j < pos;){
            int node_b_end = s.find_first_of("*", j);
            int node_b_id = stoi(s.substr(j, node_b_end-j));
            adj_list[node_a_id].insert(node_b_id);
            j = node_b_end + 1;
        }
        i = pos + 1;
    }
}