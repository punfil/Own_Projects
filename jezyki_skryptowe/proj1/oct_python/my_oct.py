def my_oct(dec_num: int) -> str:
    '''
    Convert decimal number to octal representation.
    '''
    if not dec_num:
        return "0o0"

    prefix = "-0o" if dec_num < 0 else "0o"

    dec_num = abs(dec_num)
    oct_num_rev = ""
    while dec_num:
        oct_num_rev += str(dec_num % 8)
        dec_num //= 8

    return prefix + oct_num_rev[::-1]
