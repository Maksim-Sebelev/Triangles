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
        print(f"Usage: python3 {sys.argv[0]}.py <run_traingles_exe> <test.dat> <test.ans>")
        sys.exit(1)
    
    executable, test_dat, ans_dat = sys.argv[1:4]

    try:
        with open(test_dat, 'r') as f:
            result = subprocess.run(
                [executable],
                stdin=f,
                capture_output=True,
                text=True
            )
        
        if result.returncode != 0:
            print(f"Error: Program failed with exit code {result.returncode}")
            print(f"Stderr: {result.stderr}")
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
        print(f"Error running program: {e}")
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

    color_print(Colors.WHITE, "Find triangles:")

    for triangles_num in program_good_triangles:
        if triangles_num in correct_good_triangles:
            color_print(Colors.GREEN, triangles_num, end = ' ')
        else:
            color_print(Colors.RED, triangles_num, end = ' ')

    print('\n')
    color_print(Colors.WHITE, "Not find triangles:")

    for triangles_num in correct_good_triangles:
        if triangles_num in program_good_triangles:
            color_print(Colors.GREEN, triangles_num, end = ' ')
        else:
            color_print(Colors.RED, triangles_num, end = ' ')

    print('\n')
    color_print(Colors.RED, "TEST FAILED", end = '\n')
    return 1

if __name__ == "__main__":
    sys.exit(main())