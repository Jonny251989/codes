set(target telemetry_client)

add_executable(${target}
    main.cpp
    cli_interface.cpp
)

target_link_libraries(${target}
    common
)