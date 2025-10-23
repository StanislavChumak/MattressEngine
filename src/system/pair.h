#ifndef PAIR_H
#define PAIR_H

template <typename T1, typename T2>
struct pair
{
    T1 first;
    T2 second;
};

template <typename T1, typename T2>
pair<T1, T2> make_pair(T1 first, T2 second)
{
    pair<T1, T2> pair;
    pair.first = first;
    pair.second = second;
    return pair;
}

#endif
