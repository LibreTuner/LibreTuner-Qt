if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CARGO_CMD cargo build)
    set(RUST_TARGET_DIR "target/debug")
else ()
    set(CARGO_CMD cargo build --release)
    set(RUST_TARGET_DIR "target/release")
endif ()

SET(RUST_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
if (WIN32)
  SET(RUST_DLL "${RUST_DIR}/${RUST_TARGET_DIR}/rust.dll")
  SET(RUST_LIB "${RUST_DLL}.lib")
else()
  SET(RUST_DLL "${RUST_DIR}/${RUST_TARGET_DIR}/librust.so")
  SET(RUST_LIB "${RUST_DLL}")
endif()

add_custom_target(rust ALL
    COMMENT "Compiling rust module"
    COMMAND ${CARGO_CMD}
    COMMAND ${CMAKE_COMMAND} -E copy ${RUST_DLL} ${CMAKE_CURRENT_BINARY_DIR}/../gui
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(rust PROPERTIES LOCATION ${RUST_LIB})

add_test(NAME rust_test 
    COMMAND cargo test
    WORKING_DIRECTORY ${RUST_DIR})
