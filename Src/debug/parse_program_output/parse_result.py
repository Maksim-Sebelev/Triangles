#!/usr/bin/python3

import sys
import subprocess

class Colors:
    RED       = '\033[91m'
    GREEN     = '\033[92m'
    YELLOW    = '\033[93m'
    BLUE      = '\033[94m'
    MAGENTA   = '\033[95m'
    CYAN      = '\033[96m'
    WHITE     = '\033[97m'
    BOLD      = '\033[1m'
    UNDERLINE = '\033[4m'
    RESET     = '\033[0m'

def color_print(color, arg, **kwargs):
    print(f"{color}{arg}{Colors.RESET}", **kwargs)

def main():
    if len(sys.argv) != 4:
        print(f"Usage: {sys.argv[0]}.py <run_traingles_exe> <test.dat> <test.ans>")
        sys.exit(1)

    executable, test_dat, ans_dat = sys.argv[1:4]
    n_triangles = 0

    try:
        with open(test_dat, 'r') as f:
            first_line = f.readline().strip()
            n_triangles = int(first_line)

            f.seek(0)

            result = subprocess.run(
                [executable],
                stdin=f,
                capture_output=True,
                text=True
            )

        if result.returncode != 0:
            color_print(Colors.RED, f"Error: Program failed with exit code {result.returncode}")
            print(f"stderr: {result.stderr}")
            print(f"stdin: ${result.stdout}")
            sys.exit(1)

        program_good_triangles = []
        for line in result.stdout.strip().split():
            try:
                num = int(line)
                if num >= 0:
                    program_good_triangles.append(num)
            except ValueError:
                continue

    except Exception as e:
        color_print(Colors.RED, f"Error running program: {e}")
        sys.exit(1)

    try:
        with open(ans_dat, 'r') as f:
            content = f.read().strip()

        correct_good_triangles = []
        for token in content.split():
            try:
                num = int(token)
                if num >= 0:
                    correct_good_triangles.append(num)
            except ValueError:
                continue

    except Exception as e:
        print(f"Error reading answer file: {e}")
        sys.exit(1)

    if program_good_triangles == correct_good_triangles:
        color_print(Colors.GREEN, "TEST PASSED", end = '\n')
        return 0

    errors_quantity = 0

    color_print(Colors.WHITE, "Found triangles:")

    if len(program_good_triangles) == 0:
        if len(correct_good_triangles) == 0:
            color_print(Colors.GREEN, "No triagles was found")
        else:
            color_print(Colors.RED, "No triagles was found")
    else:
        for triangles_num in program_good_triangles:
            if triangles_num in correct_good_triangles:
                color_print(Colors.GREEN, triangles_num, end = ' ')
            else:
                color_print(Colors.RED, triangles_num, end = ' ')
                errors_quantity += 1

    print('\n')
    color_print(Colors.WHITE, "Not found triangles:")


    for triangles_num in range(0, n_triangles):
        if triangles_num in program_good_triangles:
            continue

        if triangles_num in correct_good_triangles:
            color_print(Colors.RED, triangles_num, end = ' ')
            errors_quantity += 1

        else:
            color_print(Colors.GREEN, triangles_num, end = ' ')

    color_print(Colors.WHITE, "\n\nINFO:", end = '\n\n')

    color_print(Colors.WHITE, f"[found triangles]: {len(program_good_triangles)}/{len(correct_good_triangles)}", end = "\n\n")

    percent_of_errors = (errors_quantity / n_triangles) * 100
    percent_of_accuracy = 100 - percent_of_errors 

    color_print(Colors.GREEN, f"[percentage of accuracy]: {round(percent_of_accuracy, 2)}%")
    color_print(Colors.RED  , f"[percentage of errors  ]: {round(percent_of_errors  , 2)}%")

    color_print(Colors.RED, "\n\nTEST FAILED", end = "\n")

    return 1

if __name__ == "__main__":
    sys.exit(main())
