#ifndef API_H
#define API_H

// rule: W_T = X
#define W_ROW 1024
#define W_COL 1024
#define X_ROW 1024
#define X_COL 1024

// const int32_t p = (1 << 25) - 39;
const int l = 8;  // Number of fractional bits double precision
                  // const int32_t p = (1<< 24) - 3;
                  // Prime number for finite field (2^25 - 39) (1
                  // << 25) - 39  - overflow > 19937
#endif