file(GLOB src_dynreg ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
set(src_services ${src_services} ${src_dynreg} PARENT_SCOPE)
file(GLOB src_dynreg_sample ${CMAKE_CURRENT_SOURCE_DIR}/sample/dynreg_sample.c)
add_executable(dynreg_sample ${src_dynreg_sample})
target_link_libraries(dynreg_sample ${libsdk})

if( ${CONFIG_IOT_TEST} STREQUAL "ON")
    file(GLOB src_unit_test ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cc)
    set(src_test ${src_test} ${src_unit_test} PARENT_SCOPE)
endif()
