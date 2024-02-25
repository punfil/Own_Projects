import logging
from random import randint
import os
import pytest
import subprocess
import concurrent.futures
from test_data_generator import generate_N_numbers, NUMBERS_PATH

from my_oct import my_oct

LOGGER = logging.getLogger()

@pytest.fixture(params=[True, False])
def generate_single_number(request):
    if (request.param):
        return randint(0, 1000000)

    return randint(-1000000, 1000000)


@pytest.fixture()
def load_generated_numbers():
    numbers = []

    if not os.path.exists(NUMBERS_PATH):
        generate_N_numbers(1000000)

    with open(NUMBERS_PATH, "r") as file:
        for line in file:
            numbers.append(int(line))

    yield numbers


def get_c_result(dec_num: int):
    script_dir = os.path.dirname(os.path.abspath(__file__))
    return subprocess.run(f"{script_dir}/../oct_c/build/OctTester {str(dec_num)}", capture_output=True, encoding="utf-8", shell=True, check=True).stdout


def compare_octals(number):
    python_octal = oct(number)
    c_octal = get_c_result(number)
    if python_octal not in c_octal:
        raise ValueError(f"Improper value {c_octal} from {number}\n")


def test_python_single_number(generate_single_number):
    LOGGER.log(logging.INFO, "Generated number= %d", generate_single_number)
    oct_num = oct(generate_single_number)
    my_oct_num = my_oct(generate_single_number)

    assert oct_num == my_oct_num


def test_c_single_number(generate_single_number):
    LOGGER.log(logging.INFO, "Generated number= %d", generate_single_number)
    oct_num = oct(generate_single_number)

    assert oct_num in get_c_result(generate_single_number)


def test_python_generated_numbers(load_generated_numbers):
    assert load_generated_numbers

    for number in load_generated_numbers:
        if oct(number) != my_oct(number):
            raise ValueError("Improper value %d from %d", my_oct(number), number)

def test_c_generated_numbers(load_generated_numbers):
    assert load_generated_numbers

    with concurrent.futures.ThreadPoolExecutor() as executor:
        executor.map(compare_octals, load_generated_numbers)
