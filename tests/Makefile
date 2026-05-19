# ============================================
# GLPU-Agent Test Build Configuration
# ============================================

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

# Test source files
TEST_SRC = tests/test_config.c src/config.c src/cJSON.c
TEST_OUT = tests/run_tests

# Include paths
INCLUDES = -Iinc

# Test target
test: $(TEST_OUT)
	@echo "Running tests..."
	@./$(TEST_OUT)

$(TEST_OUT): $(TEST_SRC)
	@echo "Building tests..."
	@$(CC) $(CFLAGS) $(INCLUDES) $(TEST_SRC) -o $(TEST_OUT)
	@echo "✓ Test build complete"

test_clean:
	@rm -f $(TEST_OUT)
	@echo "Test build cleaned"

# Run tests with verbose output
test_verbose: $(TEST_OUT)
	@echo "Running tests (verbose)..."
	./$(TEST_OUT)

# Main build (existing)
all:
	@echo "For main build, use 'make' in project root"

.PHONY: test test_clean test_verbose all
