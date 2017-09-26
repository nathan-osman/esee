set(SRC
    main.cpp
)

add_executable(esev WIN32 ${SRC})
set_target_properties(esev PROPERTIES CXX_STANDARD 11)

target_link_libraries(esev Qt5::Multimedia Qt5::Widgets)

install(TARGETS esev RUNTIME DESTINATION bin)
