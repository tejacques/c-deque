#ifndef __DEQUE_UTILITY_H__
#define __DEQUE_UTILITY_H__

#ifdef __DEBUG__
#define dbgprintf(...) printf(__VA_ARGS__)
#else
#define dbgprintf(...) 
#endif /* __DEBUG__ */

int power_of_two_greater_than(int x);
#endif /* __DEQUE_UTILITY_H__ */
