#ifndef HFCOINCLOOP
#define HFCOINCLOOP

// #define HFCOINC_PAIRS(X)                                \
//   X(2,4)  X(2,5)  X(2,6)  X(2,7)  X(2,8)  X(2,9)        \
//   X(3,4)  X(3,5)  X(3,6)  X(3,7)  X(3,8)  X(3,9)

#define PASTE(a,b) a##b
#define XPASTE(a,b) PASTE(a,b)

#define STRPASTE(a,b) #a #b
#define XSTRPASTE(a,b) STRPASTE(a,b)

#define STRPASTE3(a,b,c) #a #b #c
#define XSTRPASTE3(a,b) STRPASTE3(a,b)

#define STR(x) #x
#define XSTR(x) STR(x)

#define HFFPATH(a, b) pphfCoincFilterPF##a##Th##b

#define HFCOINC(EXPAND)                         \
  EXPAND(HFFPATH(2, 4))                         \
  EXPAND(HFFPATH(2, 5))                         \
  EXPAND(HFFPATH(2, 6))                         \
  EXPAND(HFFPATH(2, 7))                         \
  EXPAND(HFFPATH(2, 8))                         \
  EXPAND(HFFPATH(2, 9))                         \
  EXPAND(HFFPATH(3, 4))                         \
  EXPAND(HFFPATH(3, 5))                         \
  EXPAND(HFFPATH(3, 6))                         \
  EXPAND(HFFPATH(3, 7))                         \

#define HFCOINC3D(EXPAND, t, p)                 \
  EXPAND(HFFPATH(2, 4), t, p)                   \
  EXPAND(HFFPATH(2, 5), t, p)                   \
  EXPAND(HFFPATH(2, 6), t, p)                   \
  EXPAND(HFFPATH(2, 7), t, p)                   \
  EXPAND(HFFPATH(2, 8), t, p)                   \
  EXPAND(HFFPATH(2, 9), t, p)                   \
  EXPAND(HFFPATH(3, 4), t, p)                   \
  EXPAND(HFFPATH(3, 5), t, p)                   \
  EXPAND(HFFPATH(3, 6), t, p)                   \
  EXPAND(HFFPATH(3, 7), t, p)                   \

#endif
