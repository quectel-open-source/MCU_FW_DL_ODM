project(libusb0)

set(HEADERS
    src/error.h
    src/libusb-win32_version.h
    src/lusb0_usb.h
    src/registry.h
    src/usbi.h
)

set(SOURCES
    src/descriptors.c
    src/error.c
    src/install.c
    src/registry.c
    src/usb.c
    src/windows.c
    libusb0.def
    src/resource.rc
)

remove_definitions(-DUNICODE -D_UNICODE)
add_definitions(-D_WIN32_WINNT=0x0500 -DLOG_APPNAME=\"libusb0\" -DTARGETTYPE=DYNLINK)
include_directories(${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/src/driver)
add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} setupapi.lib)
module_location(${PROJECT_NAME} ${DNLOAD_INSTALL_DIR})
