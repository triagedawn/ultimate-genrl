#----------------------------------------------------------------
# Generated CMake target import file for configuration "MinSizeRel".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Zycore::Zycore" for configuration "MinSizeRel"
set_property(TARGET Zycore::Zycore APPEND PROPERTY IMPORTED_CONFIGURATIONS MINSIZEREL)
set_target_properties(Zycore::Zycore PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_MINSIZEREL "C"
  IMPORTED_LOCATION_MINSIZEREL "${_IMPORT_PREFIX}/lib/Zycore.lib"
  )

list(APPEND _cmake_import_check_targets Zycore::Zycore )
list(APPEND _cmake_import_check_files_for_Zycore::Zycore "${_IMPORT_PREFIX}/lib/Zycore.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
