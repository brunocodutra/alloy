if(NOT CLANG_FORMAT)
    message(WARNING "clang-format not found - target 'test.format' will be unavailable.")
    return()
else()
    message(STATUS "clang-format executable: ${CLANG_FORMAT}")
endif()

test_formatting(test.formatting Alloy ${ALLOY_INCLUDE_DIR} EXCLUDE metal)
