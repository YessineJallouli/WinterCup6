#!/usr/bin/env python3
import os
import sys

ERROR_USAGE = 1
ERROR_FILE_OPEN = 2
ERROR_MEMORY = 3
INTERACTION_ERROR = 4
IO_ERROR = 5


def gcd(a, b):
    if a < b:
        return gcd(b, a)
    if b == 0:
        return a
    return gcd(b, a % b)


def lcm(a, b):
    return a * b // gcd(a, b)


non_debug = os.getenv("NDEBUG")
debug_filename = os.getenv("DEBUG")
debug_file = None
test_filename = sys.argv[1]

if non_debug is None and debug_filename is not None:
    debug_file = open(debug_filename, "w")


def is_valid(i, j, n):
    return 0 < i <= n and 0 < j <= n and i != j


def is_permutation(P):
    P_ = [p for p in P]
    P_.sort()
    return P_ == list(range(1, len(P) + 1))


queries = 0
with open(test_filename, "r") as file:
    n = int(file.readline().strip())
    P = list(map(int, file.readline().strip().split(' ')))
    if not is_permutation(P):
        raise RuntimeError("Invalid test data. Array is not a permutation.")
print(n, flush=True)
command = '\0'
while command != '!':
    line = input()
    command, *params = line.strip().split(' ')
    params = list(map(int, params))
    if command == '?':
        queries += 1
        if len(params) != 2:
            raise RuntimeError("Queries should have exactly two parameters.")
        i, j = params
        if not is_valid(i, j, n):
            raise RuntimeError("Invalid query: ? {} {}".format(i,j))
        if debug_file:
            print("Received Input: ? {} {}".format(i,j),file=debug_file,flush=True)
        LCM = lcm(P[i - 1], P[j - 1])
        print(LCM, flush=True)
        if debug_file:
            print("Sent Output: {}".format(LCM),file=debug_file,flush=True)
    elif command == '!':
        if debug_file:
            print("Received Input: ! {}".format(' '.join(map(str,params))),file=debug_file,flush=True)
        if params != P:
            print("""Solution guessed the wrong permutation!
Expected: {}
Found:    {}
""".format(' '.join(map(str, P)),' '.join(map(str, params))), file=sys.stderr, flush=True)
        else:
            print("Solution guessed the correct permutation! Number of queries: {}".format(queries), file=sys.stderr,
                  flush=True)
        break
    else:
        raise RuntimeError("Unknown command {}".format(command))

if debug_file is not None:
    debug_file.close()
