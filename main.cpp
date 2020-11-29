#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <omp.h>

using namespace std;

vector<unsigned long> read_set(const string& path) {
    string line;
    vector<unsigned long> res;
    ifstream in(path);
    if (in.is_open())
    {
        while (getline(in, line)) {
            res.push_back(stoul(line));
        }

    }
    in.close();
    return res;
}

void print_answer(const string& path, const bool& result1, const bool& result2, const bool& result3, const bool& result4) {
    ofstream out;
    out.open(path);
    if (out.is_open())
    {
        out << (result1 ? "+ C = A ∩ B" : "- C = A ∩ B") << endl;
        out << (result2 ? "+ C = A U B" : "- C = A U B") << endl;
        out << (result3 ? "+ C = A \\ B" : "- C = A \\ B") << endl;
        out << (result4 ? "+ C = B \\ A" : "- C = B \\ A") << endl;
    }
    out.close();
}

bool contains(const vector<unsigned long>& set, unsigned long value) {
    return find(set.begin(), set.end(), value) != set.end();
}

vector<unsigned long> intersection_(const vector<unsigned long>& set_1, const vector<unsigned long>& set_2) {
    vector<unsigned long> res;
#pragma omp parallel for
    for (int i = 0; i < set_1.size(); ++i) {
        if (contains(set_2, set_1[i])) {
            res.push_back(set_1[i]);
        }
    }
    return res;
}

vector<unsigned long> union_(const vector<unsigned long>& set_1, const vector<unsigned long>& set_2) {
    vector<unsigned long> res;
#pragma omp parallel for
    for (int i = 0; i < set_1.size(); ++i)
        res.push_back(set_1[i]);


    vector<unsigned long> intersection = intersection_(set_1, set_2);
#pragma omp parallel for
    for (int i = 0; i < set_2.size(); ++i) {
        if (!contains(intersection, set_2[i])) {
            res.push_back(set_2[i]);
        }
    }
    return res;
}

vector<unsigned long> difference_(const vector<unsigned long>& set_1, const vector<unsigned long>& set_2) {
    vector<unsigned long> res;
#pragma omp parallel for
    for (int i = 0; i < set_1.size(); ++i) {
        if (!contains(set_2, set_1[i]))
            res.push_back(set_1[i]);
    }
    return res;
}

bool is_intersection(const vector<unsigned long>& set_1, const vector<unsigned long>& set_2, const vector<unsigned long>& set_3) {
    vector<unsigned long> intersection = intersection_(set_1, set_2);
    if (set_3.size() != intersection.size())
        return false;

    bool res = true;
#pragma omp parallel for
    for (int i = 0; i < intersection.size(); ++i) {
        if (!res) continue;
        if (!contains(set_3, intersection[i])) {
            res = false;
        }
    }
    return res;
}

bool is_union_(const vector<unsigned long>& set_1, const vector<unsigned long>& set_2, const vector<unsigned long>& set_3) {
    vector<unsigned long> un = union_(set_1, set_2);
    if (set_3.size() != un.size())
        return false;

    bool res = true;
#pragma omp parallel for
    for (int i = 0; i < un.size(); ++i) {
        if (!res) continue;
        if (!contains(set_3, un[i])) {
            res = false;
        }
    }
    return res;
}

bool is_difference(const vector<unsigned long>& set_1, const vector<unsigned long>& set_2, const vector<unsigned long>& set_3) {
    vector<unsigned long> difference = difference_(set_1, set_2);
    if (set_3.size() != difference.size())
        return false;

    bool res = true;
#pragma omp parallel for
    for (int i = 0; i < difference.size(); ++i) {
        if (!res) continue;
        if (!contains(set_3, difference[i])) {
            res = false;
        }
    }
    return res;
}

int main(int argc, char* argv[]) {
    vector<unsigned long> set_A = read_set(argv[1]);
    vector<unsigned long> set_B = read_set(argv[2]);
    vector<unsigned long> set_C = read_set(argv[3]);

    bool isIntersection = is_intersection(set_A, set_B, set_C);
    bool isUnion = is_union_(set_A, set_B, set_C);
    bool isDifference1 = is_difference(set_A, set_B, set_C);
    bool isDifference2 = is_difference(set_B, set_A, set_C);

    print_answer(argv[4], isIntersection, isUnion, isDifference1, isDifference2);

    return 0;
}
