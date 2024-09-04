#include <vector>
#include <cstdint>
#include <random>

using namespace std;

const int32_t prime = (1 << 25) - 39;

bool isProduct(vector<vector<int64_t>> &A, vector<vector<int64_t>> &B,
               vector<vector<int64_t>> &C, int64_t s);
bool freivalds(vector<vector<int64_t>> &A, vector<vector<int64_t>> &B,
               vector<vector<int64_t>> &C, int64_t s, int k);