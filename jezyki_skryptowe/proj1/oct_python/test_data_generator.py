from random import randint
import sys

NUMBERS_PATH = "../numbers_generated.txt"


def generate_N_numbers(N):
    numbers = [f"{randint(-sys.maxsize, sys.maxsize)}\n" for _ in range(N)]
    with open("../numbers_generated.txt", "w") as f:
        f.writelines(numbers)
