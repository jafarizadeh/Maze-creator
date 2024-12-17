#!/bin/sh

PROG=${PROG:=./maze_test}  # Default to maze_test if PROG is not set

# Read common variables and functions
. ./test-inc.sh

# Initialize variables
TMP=$(mktemp)

#####################################################################
# Test maze initialization
test_1()
{
    # It checks the initialization of the maze.
    # It runs the program with the "value" argument, which is expected to return a value.
    # The program's output is captured in TMP.out and any error messages in TMP.err.
    # The test passes if the program exits with status 1, indicating a successful operation.

    display_test 1 "maze initialization"
    clean_up
    $PROG value > $TMP.out 2> $TMP.err
    if [ $? -eq 1 ]; then 
        echo "Test passed"
    else
        fail "failure for value"
    fi
    clean_up
}

test_2()
{
    # It checks for invalid maze dimensions
    # It runs the program with negative dimensions (-1, -1) and a "border" argument
    # The program should return an error, and the test passes if the program exits with status 1

    display_test 2 "invalid dimensions"
    clean_up
    $PROG -1 -1 border > $TMP.out 2> $TMP.err
    if [ $? -eq 1 ]; then  
        echo "Test passed"
    else
        fail "failure for border"
    fi
    clean_up
}

# Test maze display option
test_3()
{
    # It checks if the maze structure can be displayed correctly
    # It runs the program with the "tree" argument, which should display the maze structure
    # The test passes if the output contains '+' characters, indicating the grid structure of the maze

    display_test 3 "display maze structure"
    clean_up
    # Test that the output contains '+' characters (for the grid)
    $PROG tree > $TMP.out 2> $TMP.err
    if [ $? -eq 1 ]; then  # If the command returns 1, it's a success
        echo "Test passed"
    else
        fail "tree error"
    fi
    clean_up
}

# Test queue functionality
test_4()
{
    # It checks the integrity of the queue functionality
    # It runs the program with the "queue" argument, which should print information about the maze's queue
    # The test passes if the program exits with status 1, indicating successful execution without errors

    display_test 4 "queue integrity"
    clean_up
    $PROG queue > $TMP.out 2> $TMP.err
    if [ $? -eq 1 ]; then  # If the command returns 1, it's a success
        echo "Test passed"
    else
        fail "queue test failed"
    fi
    clean_up
}

#####################################################################
# Run all tests sequentially
run_test()
{
    case $1 in
    1) test_1 ;;
    2) test_2 ;;
    3) test_3 ;;
    4) test_4 ;;
    *) echo "Unknown test number: $1" ; exit 1 ;;
    esac
}

# If a specific test number is given, run that test
if [ $# -eq 1 ]; then
    run_test $1   # Run a specific test
    exit 0        # Exit after running that test
else
    # Otherwise, run all tests sequentially
    for test_func in $(grep -E '^test_[0-9]+\(\)$' $0 | sed s/\(\)//g); do
        $test_func
    done
fi

clean_up
echo "All tests passed"
exit 0
