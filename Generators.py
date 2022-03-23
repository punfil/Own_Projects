# Generators
# Xn -> seed, M -> modulus
# When c!=0 -> Mixed Generator, otherwise multiplicative

def InfiniteLinearGenerator(a: int, Xn: int, c: int, M: int):
    seed = Xn
    while True:
        number = (a * seed + c) % M
        yield number
        seed = number


# Shifting generator
def LFSR(beginning, taps, length):
    state = (1 << beginning) | 1
    returning = 0
    while True:
        for i in range(length):
            returning = (returning | state & 1) << 1
            bit = state
            for nb in taps:
                bit ^= (state >> nb)
            state = (state >> 1) | (bit << beginning)
        yield returning
        returning = 0


# Infinite not linear
def InfiniteNotLinearGenerator(a: int, Xn: int, c: int, M: int):
    seed = Xn
    while True:
        denominator = (a * (seed + Xn) + c)
        if denominator != 0:
            yield (1 / denominator) % M
        else:
            yield 0
        seed += 1


print("I am generating 6 numbers using a linear generator")
Linear = InfiniteLinearGenerator(69069, 5, 0, pow(2, 31) - 1)
[print(next(Linear)) for i in range(6)]
print("I am generating 6 6 binary long number using a LSFR")
Shifting = LFSR(5, [1, 2, 7], 15)
[print(next(Shifting)) for i in range(6)]
print("I am generating 6 numbers using a notlinear generator")
NotLinear = InfiniteNotLinearGenerator(69069, 5, 0, pow(2, 31) - 1)
[print(next(NotLinear)) for i in range(6)]
