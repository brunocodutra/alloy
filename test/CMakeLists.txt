add_subdirectory(unit)
add_subdirectory(header)
add_subdirectory(linking)
add_subdirectory(deployment)
add_subdirectory(formatting)

add_custom_target(tests DEPENDS
    test.unit
    test.header
    test.linking
    test.deployment
    test.formatting
)
