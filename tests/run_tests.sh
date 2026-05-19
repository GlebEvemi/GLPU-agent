#!/bin/bash

# ============================================
# GLPU-Agent Test Runner Script
# ============================================

echo "=========================================="
echo "  GLPU-Agent Unit Tests"
echo "=========================================="
echo ""

CC=gcc
CFLAGS="-Wall -Wextra -std=c99 -g"
INCLUDES="-Iinc"

# Color codes
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Counters
TESTS_RUN=0
TESTS_PASSED=0
TESTS_FAILED=0

# Function to run a test
run_test() {
    local test_name=$1
    local test_file=$2
    local output_file="tests/run_$test_name"
    
    echo -n "Building $test_name... "
    
    if [ "$test_name" == "test_config" ]; then
        $CC $CFLAGS $INCLUDES "tests/$test_file" src/config.c src/cJSON.c -o "$output_file"
    elif [ "$test_name" == "test_api" ]; then
        $CC $CFLAGS $INCLUDES "tests/$test_file" src/cJSON.c -o "$output_file" 2>/dev/null
    fi
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓${NC}"
        echo "Running $test_name..."
        
        if "./$output_file"; then
            echo -e "${GREEN}✓ $test_name PASSED${NC}\n"
            ((TESTS_PASSED++))
        else
            echo -e "${RED}✗ $test_name FAILED${NC}\n"
            ((TESTS_FAILED++))
        fi
    else
        echo -e "${RED}✗${NC}"
        ((TESTS_FAILED++))
    fi
    
    ((TESTS_RUN++))
    
    # Cleanup
    rm -f "$output_file"
}

# Run all tests
run_test "test_config" "test_config.c"
run_test "test_api" "test_api.c"

# Summary
echo "=========================================="
echo "  Test Summary"
echo "=========================================="
echo "Total tests run:  $TESTS_RUN"
echo -e "Passed: ${GREEN}$TESTS_PASSED${NC}"
echo -e "Failed: ${RED}$TESTS_FAILED${NC}"
echo "=========================================="
echo ""

if [ $TESTS_FAILED -eq 0 ]; then
    echo -e "${GREEN}All tests passed!${NC} ✓"
    exit 0
else
    echo -e "${RED}Some tests failed!${NC}"
    exit 1
fi
