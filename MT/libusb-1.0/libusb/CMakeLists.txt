project(libusb-1.0)

set(HEADERS
    hotplug.h
    libusb.h
    libusbi.h
    os/poll_windows.h
    os/threads_windows.h
    os/windows_common.h
    os/windows_usb.h
)

set(SOURCES
    core.c
    descriptor.c
    hotplug.c
    io.c
    strerror.c
    sync.c
    os/poll_windows.c
    os/threads_windows.c
    os/windows_usb.c
    libusb-1.0.rc
    libusb-1.0.def
)

include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/os ${PROJECT_SOURCE_DIR}/../msvc)
add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})
module_location(${PROJECT_NAME} ${DNLOAD_INSTALL_DIR})
