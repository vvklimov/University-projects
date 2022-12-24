#!/usr/bin/env python3

import collections
my_counter = collections.Counter()


def log_and_count(**arguments):
    def outer(func):
        def wrapper(*args, **kwargs):
            print('called {0} with {1} and {2}'.format(func.__name__, args, kwargs))
            if 'counts' in arguments.keys():
                counter = arguments['counts']
                if 'key' in arguments.keys():
                    key = arguments['key']
                    counter[key] += 1
                else:
                    counter[func.__name__] += 1
            return func(*args, **kwargs)
        return wrapper
    return outer





@log_and_count(key = 'basic functions', counts = my_counter)
def f1(a, b=2):
    return a ** b


@log_and_count(key = 'basic functions', counts = my_counter)
def f2(a, b=3):
    return a ** 2 + b


@log_and_count(counts = my_counter)
def f3(a, b=5):
    return a ** 3 - b


f1(2)
f2(2, b=4)
f1(a=2, b=4)
f2(4)
f2(5)
f3(5)
f3(5,4)
print(my_counter)
