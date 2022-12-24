# ukol za 2 body
def first_odd_or_even(numbers):
    """Returns 0 if there is the same number of even numbers and odd numbers
       in the input list of ints, or there are only odd or only even numbers.
       Returns the first odd number in the input list if the list has more even
       numbers.
       Returns the first even number in the input list if the list has more odd
       numbers.

    >>> first_odd_or_even([2,4,2,3,6])
    3
    >>> first_odd_or_even([3,5,4])
    4
    >>> first_odd_or_even([2,4,3,5])
    0
    >>> first_odd_or_even([2,4])
    0
    >>> first_odd_or_even([3])
    0
    """
    first_even = 0
    first_odd = 0
    odd_cnt = 0
    even_cnt = 0
    for number in reversed(numbers):
        if number % 2 != 0:
            odd_cnt += 1
            first_odd = number
        else:
            first_even = number
            even_cnt += 1
    if odd_cnt == even_cnt or even_cnt == 0 or odd_cnt == 0:
        res = 0
    if odd_cnt > even_cnt:
        res = first_even
    if odd_cnt < even_cnt:
        res = first_odd
    return res


# ukol za 3 body
def to_pilot_alpha(word):
    """Returns a list of pilot alpha codes corresponding to the input word

    >>> to_pilot_alpha('Smrz')
    ['Sierra', 'Mike', 'Romeo', 'Zulu']
    """

    pilot_alpha = ['Alfa', 'Bravo', 'Charlie', 'Delta', 'Echo', 'Foxtrot',
        'Golf', 'Hotel', 'India', 'Juliett', 'Kilo', 'Lima', 'Mike',
        'November', 'Oscar', 'Papa', 'Quebec', 'Romeo', 'Sierra', 'Tango',
        'Uniform', 'Victor', 'Whiskey', 'Xray', 'Yankee', 'Zulu']

    pilot_alpha_list = []
    word = word.upper()
    for symbol in word:
        for name in pilot_alpha:
            for symbol_name in name:
                if symbol == symbol_name:
                    pilot_alpha_list.append(name)
    return pilot_alpha_list


if __name__ == "__main__":
    import doctest
    doctest.testmod()