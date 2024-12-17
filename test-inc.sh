# This function is called to handle errors in the script.
# It prints a "FAIL" message, displays an error message passed as an argument
fail()
{
    local msg="$1"

    echo FAIL
    echo "$msg"
    echo "See the following files:"
    ls -dp $TMP*
    exit 1
}

# length (in number of characters, not bytes) of a UTF-8 string
# Note: the locale must be set to UTF-8
strlen()
{
    [ $# != 1 ] && fail "SYNTAX ERROR in strlen"
    local str="$1"
    (
        eval $OLDLOCALE
        printf "%s" "$str" | wc -m
    )
}

# Display a test
# $1 = test number
# $2 = title
display_test()
{
    [ $# != 2 ] && fail "SYNTAX ERROR in display_test"
    local num="$1" msg="$2"
    local start nbchar nbhyphens

    # echo '\c', though POSIX, is not supported by all Shells
    # POSIX recommends using printf
    # However, printf does not correctly handle Unicode characters
    # so we must use a workaround to maintain alignment of "OK"
    start="Test $num - $msg"
    nbchar=$(strlen "$start")
    nbhyphens=$((80 - 6 - nbchar))
    printf "%s%-${nbhyphens}.${nbhyphens}s " "$start" \
        "...................................................................."
}

# Tests if a file is empty (only tests, does not return an error)
is_empty()
{
    [ $# != 1 ] && fail "SYNTAX ERROR in is_empty"
    [ ! -s $1 ]
}

# Verifies that the error message is sent to the error output
# and not to the standard output
# $1 = name of the log file (without .err or .out)
# $2 (optional) = pattern that should be found on the error output
verify_stderr()
{
    [ $# != 1 -a $# != 2 ] && fail "SYNTAX ERROR in verify_stderr"
    local base="$1" msg=""

    is_empty $base.err \
        && fail "The error message should be on the error output"
    is_empty $base.out \
        || fail "Nothing should be displayed on the standard output"

    if [ $# = 2 ]; then
        grep -q "$2" $base.err \
            || fail "The pattern \"$2\" is not on the error output"
    fi
}

clean_up()
{
    rm -rf $TMP*
}
