#!/bin/bash
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

function need_logger
{
    find_option "use-logger" "$@"
}

function need_verbose_output
{
    find_option "verbose" "$@"
}

function need_graphic_dump
{
    find_option "graphic-dump" "$@"
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
-DCMAKE_BUILD_TYPE=Debug \
-DCMAKE_EXPORT_COMPILE_COMMANDS=1 \
-DUSE_LOGGER=1 \
-DVERBOSE_OUTPUT=1 \
-DTREE_2D_DUMP=1 \
-DTREE_3D_DUMP=1"

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

# crete shell script to fast run e2e test (needed for spot work with e2e test)
run_test_script=${build_dir}/run_test
custom_echo "${CONSOLE_COLOR_GREEN}" "${CONSOLE_BIND_FONT}" "Create ${run_test_script}..."

check_that_exists touch || exit 1
check_that_exists echo  || exit 1

touch ${run_test_script} || exit 1

echo "#!/bin/bash"                                                                          >> ${run_test_script}
echo ""                                                                                     >> ${run_test_script}
echo "# DO NOT TRY TO EDIT THIS FILE."                                                      >> ${run_test_script}
echo "# ALL YOUR CORRECTIONS WILL BE LOST."                                                 >> ${run_test_script}
echo ""                                                                                     >> ${run_test_script}
echo "# Automatic generated with:"                                                          >> ${run_test_script}
echo "# '$(realpath $0)'"                                                                   >> ${run_test_script}
echo "# at:"                                                                                >> ${run_test_script}
echo "# $(date)"                                                                            >> ${run_test_script}
echo ""                                                                                     >> ${run_test_script}
echo "set -euo pipefail"                                                                    >> ${run_test_script}
echo ""                                                                                     >> ${run_test_script}
echo "if [ \$# -ne 1 ]; then"                                                               >> ${run_test_script}
echo "    echo \"expect only 1 arg: number of test.\""                                      >> ${run_test_script}
echo "    exit 1"                                                                           >> ${run_test_script}
echo "fi"                                                                                   >> ${run_test_script}
echo ""                                                                                     >> ${run_test_script}
echo "n=\"\$1\""                                                                            >> ${run_test_script}
echo "./run_triangles -v -ifiles ../tests/e2e/dat/\"\$n\".dat ../tests/e2e/ans/\"\$n\".ans" >> ${run_test_script}

check_that_exists chmod     || exit 1
chmod +x ${run_test_script} || exit 1

custom_echo "${CONSOLE_COLOR_GREEN}" "${CONSOLE_BIND_FONT}" "Creating ${run_test_script} completed successfully."
skip_line_in_console


custom_echo "${CONSOLE_COLOR_WHITE}" "${CONSOLE_BIND_FONT}" "To work with the project, execute:"
custom_echo "${CONSOLE_COLOR_WHITE}" "" "cd ${build_dir}/"
