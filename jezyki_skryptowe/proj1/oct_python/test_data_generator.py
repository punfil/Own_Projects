from random import randint

NUMBERS_PATH = "../numbers_generated.txt"
MIN_C_VAL = -9223372036854775808
MAX_C_VAL = 9223372036854775807

def generate_N_numbers(N):
    uniq_num = set()
    while len(uniq_num) < N:
        uniq_num.add(randint(MIN_C_VAL, MAX_C_VAL))

    numbers = [f"{num}\n" for num in uniq_num]
    with open("../numbers_generated.txt", "w") as f:
        f.writelines(numbers)
