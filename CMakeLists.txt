cmake_minimum_required(VERSION 3.13)

# 1. Initialize Pico SDK if available
include(pico_sdk_import.cmake OPTIONAL)

project(Riverbraid-Lite C)
set(CMAKE_C_STANDARD 11)

# 2. Common Include Directories
include_directories(include platform/crypto)

# 3. Common Source Files (The Logic)
set(COMMON_SOURCES 
    src/riverbraid_v1_4.c
    src/integrity.c
    platform/crypto/rb_sha256.c
)

# 4. Bifurcated Build Logic
if(PICO_SDK_PATH)
    message(STATUS "Build Target: Raspberry Pi Pico (Hardware)")
    
    pico_sdk_init()
    
    # We create a placeholder main_pico.c if it doesn't exist to prevent build errors
    add_executable(riverbraid_firmware 
        ${COMMON_SOURCES}
        src/integrity_pico.c
        src/main_pico.c 
    )

    # Link hardware-specific libraries
    target_link_libraries(riverbraid_firmware pico_stdlib pico_unique_id)
    pico_add_extra_outputs(riverbraid_firmware)

else()
    message(STATUS "Build Target: Host System (Audit/Testing)")
    
    add_executable(unit_tests 
        ${COMMON_SOURCES}
        tests/test_vectors.c
    )
endif()
