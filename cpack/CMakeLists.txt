#Specify installation rules
install(TARGETS _${PROJECT_NAME}-${PROJECT_VERSION} DESTINATION lib)

install(FILES ${INC} DESTINATION include)
install(FILES ${INC_UTIL} DESTINATION include/util)
install(FILES ${INC_UTIL_FIO} DESTINATION include/util/fio)
install(FILES ${INC_UTIL_LOGGING} DESTINATION include/util/logging)
install(FILES ${INC_UTIL_PROPERTIES} DESTINATION include/util/properties)

# Set up CPack
set(CPACK_PACKAGE_NAME "lib_${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "library used to help log messages")
set(CPACK_PACKAGE_CONTACT "Salomon Lee")
set(CPACK_PACKAGE_VENDOR "braidedchain")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

# Generate an installer
set(CPACK_GENERATOR "ZIP")  # You can choose other generators like "DEB", "RPM", "NSIS", etc.

include(CPack)