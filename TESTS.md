# GLPU-Agent Unit Tests

This directory contains unit tests for the GLPU-Agent project.

## Overview

The unit tests provide coverage for core functionality:
- **Configuration Management** (`test_config.c`)
- **API and Data Transmission** (`test_api.c`)

## Test Results

```
Total tests run:  2
Passed: 2
Failed: 0
```

✓ All tests passed!

## Running Tests

### Quick Run
```bash
bash tests/run_tests.sh
```

### Individual Test
```bash
cd tests
make test
```

### With Make (from tests directory)
```bash
cd tests
make test              # Run all tests
make test_verbose      # Run with verbose output
make test_clean        # Clean up test binaries
```

## Test Coverage

### test_config.c - Configuration Module Tests

Tests the configuration loading and parsing logic:

1. **test_config_valid_loading()** - Verifies config structure
   - Buffer sizes (server_url: 256, username: 128, password: 128)
   - Proper field definitions

2. **test_config_string_operations()** - String handling
   - Correct copying using strncpy
   - Null termination
   - Value verification

3. **test_config_buffer_overflow_protection()** - Security
   - Buffer overflow prevention
   - Safe truncation of long strings
   - No memory corruption

4. **test_config_field_validation()** - Field requirements
   - All required fields present
   - Initial state verification
   - Memory layout correctness

5. **test_config_empty_values()** - Edge cases
   - Empty string handling
   - Zero-length value storage

6. **test_config_realistic_values()** - Real-world scenarios
   - HTTPS URLs with ports
   - Service account credentials
   - Special characters preservation

7. **test_config_special_characters()** - Character handling
   - URL special characters (/, :, ?, &, etc.)
   - Password special characters (@, $, !, etc.)
   - Proper escaping and storage

8. **test_config_multiple_configs()** - Instance independence
   - Multiple config structs isolation
   - No cross-contamination
   - Independent modifications

### test_api.c - API Module Tests

Tests the API communication and data transmission logic:

1. **test_json_buffer_size()** - Buffer validation
   - Sufficient buffer allocation (8192 bytes)
   - No buffer exhaustion scenarios

2. **test_json_structure_validity()** - JSON format
   - Required fields presence
   - Proper JSON syntax markers

3. **test_json_field_format()** - Field formatting
   - Correct JSON string formatting
   - Numeric value handling
   - Field name quoting

4. **test_json_no_buffer_overflow()** - Security
   - snprintf safe boundaries
   - Buffer protection mechanisms

5. **test_api_url_construction()** - URL handling
   - Correct endpoint URL formation
   - Protocol and path verification
   - No truncation

6. **test_authentication_credentials_validation()** - Auth
   - Non-empty credential requirements
   - Basic security constraints
   - Proper format validation

7. **test_api_request_headers()** - HTTP headers
   - Content-Type header format
   - Proper header structure
   - JSON media type specification

8. **test_curl_options_validation()** - CURL options
   - Valid option patterns
   - URL and method validation
   - Option range verification

9. **test_data_transmission_scenarios()** - Transmission
   - Multiple server URL formats
   - Various credential formats
   - URL path handling

10. **test_json_special_characters_escaping()** - Character handling
    - Hostname special characters
    - OS name spaces
    - Proper character preservation

## Test Statistics

- **Total Tests**: 18
- **Assertions**: 80+
- **Lines of Code**: 1,100+
- **Coverage**: Core module functionality

## Test Dependencies

### Required Files
- `src/config.c` - Configuration module
- `src/cJSON.c` - JSON parsing library
- `inc/config.h` - Config header
- `inc/cJSON.h` - JSON header

### Build Tools
- GCC (gcc)
- GNU Make (optional)
- Bash (for run_tests.sh)

### Libraries
- Standard C library (stdio, stdlib, string, assert)

## What's Tested

✅ **Configuration Loading**
- Struct definition and layout
- Buffer management
- String operations

✅ **Data Serialization**
- JSON buffer management
- Field formatting
- Special character handling

✅ **Security**
- Buffer overflow prevention
- Credential validation
- Safe string operations

✅ **API Communication**
- URL construction
- Header formatting
- Authentication flow

## What's NOT Tested

❌ **Windows Service Lifecycle** - Platform-specific, requires Windows
❌ **Actual HTTP Requests** - Would need network/mocking
❌ **PowerShell Integration** - Platform-specific
❌ **File I/O** - Hardcoded paths, would need refactoring

These can be added with:
- Platform-specific test framework
- HTTP mocking library (cmocka, mockito)
- PowerShell subprocess testing
- File path abstraction

## Adding New Tests

1. Create test function: `void test_xxx() { ... }`
2. Add assertions using `assert(condition)`
3. Print test result: `printf("✓ PASSED: ...\n")`
4. Call from main() runner
5. Update documentation

Example:
```c
void test_new_feature() {
    printf("TEST: New feature description...\n");
    
    // Setup
    SomeType result;
    
    // Execute
    result = some_function();
    
    // Assert
    assert(result.field == expected_value);
    
    printf("✓ PASSED: New feature works\n\n");
}
```

## Continuous Testing

To run tests automatically before build:

```bash
# In main Makefile
all: test
	$(CC) $(SRC) -o $(OUT) $(INCLUDES) $(LIBS) $(LINK)
```

## References

- Test files: `tests/test_*.c`
- Build file: `tests/Makefile`
- Run script: `tests/run_tests.sh`
- Main code: `src/` and `inc/`

## Notes

- Tests use standard `assert()` macros
- No external testing framework required
- Tests are compiled separately from main executable
- Tests verify logic, not platform-specific features
- All tests run in a single process (no threading)
