#ifndef HMM_H
#define HMM_H

namespace HMM{
extern void generate_observation_sequence(const stage &);
extern void converge(int);
extern void read_observation_sequence(int, const stage &);
extern int test(int);
};

#endif