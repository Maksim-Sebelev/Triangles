set -euo pipefail

# Console colors
CONSOLE_COLOR_GREEN='\x1b[32m'
CONSOLE_COLOR_WHITE='\e[0;37m'
CONSOLE_COLOR_YELLOW='\e[0;33m'
CONSOLE_COLOR_RED='\x1b[31m'
CONSOLE_BIND_FONT='\e[1m'
RESET_CONSOLE_OUTPUT_SETTINGS='\e[0m'

function custom_echo
{
    local color=$1 font=$2 msg=$3

    echo -e "${color}${font}${msg}${RESET_CONSOLE_OUTPUT_SETTINGS}"
}

function custom_echo_err
{
    local color=$1 font=$2 msg=$3

    echo -e "${color}${font}${msg}${RESET_CONSOLE_OUTPUT_SETTINGS}" >&2
}

function skip_line_in_console
{
    echo
}

function check_that_exists
{
    local program=$1

    if ! command -v "${program}" >/dev/null 2>&1; then
        custom_echo_err "${CONSOLE_COLOR_RED}" "${CONSOLE_BIND_FONT}" "'${program}' not found."
        custom_echo_err "" "" "This script requires '${program}' to be installed."
        return 1
    fi

    return 0
}

function find_option
{
    local needed_option="$1"
    shift

    for option in "$@"
    do
        if [ "${option}" == "${needed_option}" ]; then
            return 0
        fi
    done

    return 1
}

function need_3d_dump
{
    find_option "3d-dump" "$@"
}

build_dir="build"
source_dir="Src"

# clean build directory if exists and not empty
if [ -d "${build_dir}" ] && [ -n "$(ls -A "${build_dir}" 2>/dev/null)" ]; then
    custom_echo "${CONSOLE_COLOR_GREEN}" "${CONSOLE_BIND_FONT}" "Cleaning directory: ${build_dir}/"
    custom_echo "${CONSOLE_COLOR_WHITE}" "" "rm -rf ${build_dir}/"
    skip_line_in_console
    rm -rf "${build_dir}/"
fi

# check dependencies
custom_echo "${CONSOLE_COLOR_GREEN}" "${CONSOLE_BIND_FONT}" "Checking dependencies..."

check_that_exists cmake || exit 1
custom_echo "${CONSOLE_COLOR_GREEN}" "${CONSOLE_BIND_FONT}" "cmake   was found"

check_that_exists clang++ || exit 1
custom_echo "${CONSOLE_COLOR_GREEN}" "${CONSOLE_BIND_FONT}" "clang++ was found"

check_that_exists ninja || exit 1
custom_echo "${CONSOLE_COLOR_GREEN}" "${CONSOLE_BIND_FONT}" "ninja   was found"

custom_echo "${CONSOLE_COLOR_GREEN}" "${CONSOLE_BIND_FONT}" "All dependencies are found, continue to work..."
skip_line_in_console

# generating build system with optional logger
custom_echo "${CONSOLE_COLOR_GREEN}" "${CONSOLE_BIND_FONT}" "Generating build system..."

cmake_command="cmake \
-G Ninja \
-S ${source_dir} \
-B ${build_dir} \
-DCMAKE_CXX_COMPILER=clang++ \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_EXPORT_COMPILE_COMMANDS=1"

if need_3d_dump "$@"; then
    cmake_command="${cmake_command} -DTREE_3D_DUMP=1"
fi

custom_echo "${CONSOLE_COLOR_WHITE}" "" "${cmake_command}"
eval ${cmake_command}

custom_echo "${CONSOLE_COLOR_GREEN}" "${CONSOLE_BIND_FONT}" "Generating build system completed successfully."
skip_line_in_console

# building projects
custom_echo "${CONSOLE_COLOR_GREEN}" "${CONSOLE_BIND_FONT}" "Building project..."
custom_echo "${CONSOLE_COLOR_WHITE}" "" "cmake --build ${build_dir}/"
cmake --build "${build_dir}/"

custom_echo "${CONSOLE_COLOR_GREEN}" "${CONSOLE_BIND_FONT}" "Building project completed successfully."
skip_line_in_console

custom_echo "${CONSOLE_COLOR_WHITE}" "${CONSOLE_BIND_FONT}" "To work with the project, execute:"
custom_echo "${CONSOLE_COLOR_WHITE}" "" "cd ${build_dir}/"
