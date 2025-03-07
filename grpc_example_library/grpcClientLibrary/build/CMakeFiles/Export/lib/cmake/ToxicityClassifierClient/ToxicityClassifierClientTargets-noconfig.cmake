#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "ToxicityClassifierClient::ToxicityClassifierClient" for configuration ""
set_property(TARGET ToxicityClassifierClient::ToxicityClassifierClient APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(ToxicityClassifierClient::ToxicityClassifierClient PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libToxicityClassifierClient.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS ToxicityClassifierClient::ToxicityClassifierClient )
list(APPEND _IMPORT_CHECK_FILES_FOR_ToxicityClassifierClient::ToxicityClassifierClient "${_IMPORT_PREFIX}/lib/libToxicityClassifierClient.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
