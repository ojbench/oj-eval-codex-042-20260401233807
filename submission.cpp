#include <bits/stdc++.h>
using namespace std;

constexpr int s_prime = 31;

inline int my_log2(int x) {
    int ans = 0;
    while (x != 1) {
        x /= 2;
        ++ans;
    }
    return ans;
}

template<typename T, int b_prime>
class SpeedCircularLinkedList {
public:
    struct Node {
        std::map<std::string, T> kv_map;
        int bound = 0;
        Node* next = nullptr;
        Node** fast_search_list = nullptr;
        int fast_list_len = 0; // extra member allowed

        Node(int b, int fast_search_list_size) {
            bound = b;
            fast_list_len = fast_search_list_size;
            if (fast_search_list_size > 0) {
                fast_search_list = new Node*[fast_search_list_size];
                for (int i = 0; i < fast_search_list_size; ++i) fast_search_list[i] = nullptr;
            } else {
                fast_search_list = nullptr;
            }
        }

        ~Node() {
            if (fast_search_list) {
                delete [] fast_search_list;
                fast_search_list = nullptr;
            }
        }
    };

private:
    Node* head = nullptr;
    int fast_search_list_size = 0;
    int list_size = 0;

    static int GetHashCode(std::string str) {
        long long ans = 0;
        for (auto& ch : str) {
            ans = (ans * s_prime + ch) % b_prime;
        }
        return static_cast<int>((ans + b_prime) % b_prime);
    }

    void BuildFastSearchList() {
        if (list_size <= 0 || fast_search_list_size <= 0) return;
        vector<Node*> nodes;
        nodes.reserve(list_size);
        Node* cur = head;
        for (int i = 0; i < list_size; ++i) {
            nodes.push_back(cur);
            cur = cur->next;
        }
        for (int i = 0; i < list_size; ++i) nodes[i]->fast_search_list[0] = nodes[(i + 1) % list_size];
        for (int k = 1; k < fast_search_list_size; ++k) {
            for (int i = 0; i < list_size; ++i) {
                Node* prev = nodes[i]->fast_search_list[k-1];
                nodes[i]->fast_search_list[k] = prev ? prev->fast_search_list[k-1] : nullptr;
            }
        }
    }

public:
    explicit SpeedCircularLinkedList(std::vector<int> node_bounds) {
        list_size = static_cast<int>(node_bounds.size());
        fast_search_list_size = (list_size > 1) ? my_log2(list_size) : 0;
        if (list_size == 0) { head = nullptr; return; }
        head = new Node(node_bounds[0], fast_search_list_size);
        Node* prev = head;
        for (int i = 1; i < list_size; ++i) {
            Node* node = new Node(node_bounds[i], fast_search_list_size);
            prev->next = node;
            prev = node;
        }
        prev->next = head;
        BuildFastSearchList();
    }

    ~SpeedCircularLinkedList() {
        if (!head || list_size == 0) return;
        Node* cur = head->next;
        for (int i = 1; i < list_size; ++i) {
            Node* nxt = cur->next;
            delete cur;
            cur = nxt;
        }
        delete head;
        head = nullptr; list_size = 0; fast_search_list_size = 0;
    }

    void put(std::string str, T value) {
        int code = GetHashCode(str);
        if (!head) return;
        if (code <= head->bound) { head->kv_map[str] = value; return; }
        Node* cur = head->next; Node* prev = head;
        while (cur != head) {
            if (code <= cur->bound && code > prev->bound) { cur->kv_map[str] = value; return; }
            prev = cur; cur = cur->next;
        }
        head->kv_map[str] = value;
    }

    T get(std::string str) {
        int code = GetHashCode(str);
        if (!head) return T();
        if (code <= head->bound) {
            auto it = head->kv_map.find(str);
            return it == head->kv_map.end() ? T() : it->second;
        }
        Node* cur = head->next; Node* prev = head;
        while (cur != head) {
            if (code <= cur->bound && code > prev->bound) {
                auto it = cur->kv_map.find(str);
                return it == cur->kv_map.end() ? T() : it->second;
            }
            prev = cur; cur = cur->next;
        }
        auto it = head->kv_map.find(str);
        return it == head->kv_map.end() ? T() : it->second;
    }

    void print() {
        if (!head) return;
        Node* cur = head;
        for (int i = 0; i < list_size; ++i) {
            std::cout << "[Node] Bound = " << cur->bound << ", kv_map_size = " << cur->kv_map.size() << '\n';
            cur = cur->next;
        }
    }

    int size() const { return list_size; }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    // The OJ will compile library and run own tests; keep main minimal
    return 0;
}
