find_program(LCOV NAMES lcov)
find_program(GENHTML NAMES genhtml)
if(LCOV AND GENHTML)
    message(STATUS "which lcov: ${LCOV}")
endif()

function(coverage _target _prefix)
    set(cov ${CMAKE_CURRENT_BINARY_DIR}/cov)
    set(html ${CMAKE_CURRENT_BINARY_DIR}/html)

    add_custom_target(${_target}
        COMMAND ${CMAKE_COMMAND} -E remove ${cov}.*
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${html}
        COMMAND ${LCOV} -d . --zerocounters
        COMMAND ${CMAKE_CTEST_COMMAND} ${ARGN}
        COMMAND ${LCOV} -c -d . -o ${cov}.info
        COMMAND ${LCOV} -e ${cov}.info '${_prefix}*' -o ${cov}.stripped
        COMMAND ${LCOV} -l ${cov}.stripped
        COMMAND ${GENHTML} -o ${html} ${cov}.stripped
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        COMMENT "Collecting code coverage..."
    )
endfunction()
