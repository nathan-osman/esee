set(SRC
    exifdialog.cpp
    exifreader.cpp
    main.cpp
)

add_executable(esee WIN32 ${SRC})
set_target_properties(esee PROPERTIES CXX_STANDARD 11)

target_include_directories(esee PRIVATE ${LIBEXIF_INCLUDE_DIRS})
target_link_libraries(esee Qt5::Widgets ${LIBEXIF_LIBRARIES})

install(TARGETS esee RUNTIME DESTINATION bin)
