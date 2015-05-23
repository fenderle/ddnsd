find_path(PCRE_INCLUDE_DIR NAMES pcre.h)
mark_as_advanced(PCRE_INCLUDE_DIR)

find_library(PCRE_LIBRARY NAMES pcre)
mark_as_advanced(PCRE_LIBRARY)

if(PCRE_INCLUDE_DIR)
  foreach(_pcre_version_header pcre.h)
    if(EXISTS "${PCRE_INCLUDE_DIR}/${_pcre_version_header}")
      file(STRINGS "${PCRE_INCLUDE_DIR}/${_pcre_version_header}" pcre_version_str LIMIT_COUNT 2 REGEX "^#define[\t ]+PCRE_(MAJOR|MINOR)[\t ]+[0-9]+")

      string(REGEX REPLACE "^.*_MAJOR[\t ]+([0-9]+).*_MINOR[\t ]+([0-9]+).*" "\\1.\\2" PCRE_VERSION_STRING "${pcre_version_str}")
      unset(pcre_version_str)
      break()
    endif()
  endforeach()
endif()

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PCRE
                                  REQUIRED_VARS PCRE_LIBRARY PCRE_INCLUDE_DIR
                                  VERSION_VAR PCRE_VERSION_STRING)

if(PCRE_FOUND)
  set(PCRE_LIBRARIES ${PCRE_LIBRARY})
  set(PCRE_INCLUDE_DIRS ${PCRE_INCLUDE_DIR})
endif()
