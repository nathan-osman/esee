set(SRC
    exifdialog.cpp
    exifreader.cpp
    main.cpp
)

add_executable(esev WIN32 ${SRC})
set_target_properties(esev PROPERTIES CXX_STANDARD 11)

target_include_directories(esev PRIVATE ${LIBEXIF_INCLUDE_DIRS})
target_link_libraries(esev Qt5::Widgets ${LIBEXIF_LIBRARIES})

install(TARGETS esev RUNTIME DESTINATION bin)
