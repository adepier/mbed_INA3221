 
set(INA3221_SOURCES INA3221.cpp) 
add_library(mbed_INA3221 STATIC ${INA3221_SOURCES})
target_link_libraries( mbed_INA3221 mbed-os)

set(INA3221_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR} )
target_include_directories(mbed_INA3221 PUBLIC ${INA3221_HEADER_DIR})