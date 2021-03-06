#!/bin/bash

#
# This bash script verifies that command line arguments are working right
#


# Include tests utils
. tests/utils.sh


# -------------------- T E S T S ------------------------ #


# Check binary
binary_exists;


# Prints start message
start_message "Command line options";


#
# Verifies -v command line options
#
test_message "Prints program version with -v option";
./${BINDIR}/${BINARY} -v | grep "version" &> ${LIMBO};
assert_command $?;



#
# Verifies --version command line option
#
test_message "Prints program version with --version option";
./${BINDIR}/${BINARY} --version | grep "version" &> ${LIMBO};
assert_command $?;



#
# Verifies -h command line option
#
test_message "Prints program help with -h option";
./${BINDIR}/${BINARY} -h | grep "Description" &> ${LIMBO};
assert_command $?;



#
# Verifies --help command line option
#
test_message "Prints program help with --help option";
./${BINDIR}/${BINARY} --help | grep "Description" &> ${LIMBO};
assert_command $?;



#
# Verifies the use of stdin as the input json
#
test_message "Use stdin as the input json to parse";
echo '{"test": true}' | ${BINDIR}/${BINARY} &> ${LIMBO};
assert_command $?;



#
# Tests no such file or directory
#
test_message "Validate if file does not exists";
./${BINDIR}/${BINARY} "some_invalid_file_name.json" &> ${LIMBO};
assert_command_fail $?;



#
# Tests a valid json input file
#
test_message "Parse a valid json input file";
./${BINDIR}/${BINARY} "tests/samples/ok/input_05.json" &> ${LIMBO};
assert_command $?;



#
# Verifies --no-color option
#
test_message "Should not print colors characteres";
./${BINDIR}/${BINARY} --no-color "tests/samples/ok/input_05.json" \
    | grep "37m" &> ${LIMBO};
assert_command_fail $?;



#
# Verifies --no-color option
#
test_message "Should print colors characteres";
./${BINDIR}/${BINARY} "tests/samples/ok/input_05.json" | grep "37m" &> ${LIMBO};
assert_command $?;



#
# Verifies --tab-stop option
#
test_message "Should print an error when no tabstop is informed";
./${BINDIR}/${BINARY} "tests/samples/ok/input_05.json" --tab-stop 2>&1 >${LIMBO} \
    | grep "requires" &> ${LIMBO};
assert_command $?;



#
# Verifies --tab-stop option with default tab space
#
test_message "Should not have 8 spaces when using default tab space";
./${BINDIR}/${BINARY} "tests/samples/ok/input_05.json" \
    | grep "        " &> ${LIMBO};
assert_command_fail $?;



#
# Verifies --tab-stop been equal 8 spaces
#
test_message "Should print json tabulation using 8 spaces";
./${BINDIR}/${BINARY} "tests/samples/ok/input_05.json" --tab-stop 8 \
    | grep "        " &> ${LIMBO};
assert_command $?;



# Prints tests result
result_message;
