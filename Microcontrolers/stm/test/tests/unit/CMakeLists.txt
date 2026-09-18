cmake_minimum_required(VERSION 3.16)
project(ethernet_unit_tests C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
enable_testing()
set(CMAKE_C_STANDARD 11)
set(LAYERS ${CMAKE_CURRENT_LIST_DIR}/../../Common/Src/ethernet)
add_executable(test_layers test_layers.c ${LAYERS}/physical.c ${LAYERS}/data.c ${LAYERS}/network.c ${LAYERS}/transport.c)
target_include_directories(test_layers PRIVATE mocks ${CMAKE_CURRENT_LIST_DIR}/../../Common/Inc/ethernet)
target_compile_options(test_layers PRIVATE -Wall -Wextra -Werror -UNDEBUG)
foreach(case data_init data_mac phy_ids phy_zero_ids phy_states phy_wait phy_monitor network_static network_validation network_dhcp network_callback network_ready network_wait ping_success ping_timeout ping_filter ping_errors ping_wait transport_validation transport_connect transport_failures transport_io)
    add_test(NAME ${case} COMMAND test_layers ${case})
endforeach()
foreach(mode 4 5 6)
    add_test(NAME ping_filter_${mode} COMMAND test_layers ping_filter)
    set_tests_properties(ping_filter_${mode} PROPERTIES ENVIRONMENT "PACKET_MODE=${mode}")
endforeach()
set_tests_properties(phy_monitor phy_wait network_wait ping_timeout ping_wait PROPERTIES TIMEOUT 5)

add_executable(test_mqtt_connection test_mqtt_connection.c
    ../../Common/Src/mqtt/mqtt_client.c
    ../../Common/Src/mqtt/mqtt_tls.c)
target_include_directories(test_mqtt_connection PRIVATE
    mqtt_mocks ../../Common/Inc/mqtt ../../Common/Inc/logger)
target_compile_options(test_mqtt_connection PRIVATE -Wall -Wextra -Werror -UNDEBUG)
add_test(NAME mqtt_connection COMMAND test_mqtt_connection)

add_executable(test_logger test_logger.c
    ../../Common/Src/logger/logger.c ../../Common/Src/logger/ring_buffer.c)
target_include_directories(test_logger PRIVATE logger_mocks ../../Common/Inc/logger)
target_compile_definitions(test_logger PRIVATE CORE_CM7)
target_compile_options(test_logger PRIVATE -Wall -Wextra -Werror -UNDEBUG)
add_test(NAME logger_bounded_output COMMAND test_logger)
set_tests_properties(logger_bounded_output PROPERTIES TIMEOUT 2)
