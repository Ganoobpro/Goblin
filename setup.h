#ifndef GOBLIN_SETUP_H
#define GOBLIN_SETUP_H

// Type aliases
typedef long long ll;
typedef unsigned long long ull;
typedef long double ld;

// Predicates
#define success            0 ==
#define failure            0 !=
#define fail               0 !=
#define even               0 == 1 &
#define odd                1 == 1 &
#define positive           0 <
#define negative           0 >
#define zero               0 ==
#define empty              NULL ==
#define null               NULL ==

// Ternaries and conditionals.
#define when
#define then        ?
#define other       :
#define otherwise   :
#define otherwhen   :
#define only        : NULL

#define until(...) while (!(__VA_ARGS__))

// Short typing
#define between(a, x, b)    ((a) <= (x) && (x) <= (b))
#define in_range(a, x, b)   ((a) < (x) && (x) < (b))

#define all(x)  (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()

#endif // TEMPLATE_H
