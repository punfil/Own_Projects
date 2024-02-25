from random import randint

NUMBERS_PATH = "../numbers_generated.txt"
MIN_C_VAL = -9223372036854775808
MAX_C_VAL = 9223372036854775807

def generate_N_numbers(N):
    numbers = [f"{randint(MIN_C_VAL, MAX_C_VAL)}\n" for _ in range(N)]
    with open("../numbers_generated.txt", "w") as f:
        f.writelines(numbers)
