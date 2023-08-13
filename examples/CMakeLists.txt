set(exampleModemManagerQt_SRCS
   main.cpp
)

add_executable(exampleModemManagerQt ${exampleModemManagerQt_SRCS})
target_link_libraries(exampleModemManagerQt
    KF5::ModemManagerQt
    Qt${QT_MAJOR_VERSION}::Core
)
