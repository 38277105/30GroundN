INCLUDE("${CMAKE_CURRENT_LIST_DIR}/VLCQtCoreTargets.cmake")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET(VLCQtCoreFramework ${CMAKE_CURRENT_LIST_DIR}/../../VLCQtCore.framework)
ELSE()
    GET_TARGET_PROPERTY(VLCQtCoreLocation VLCQt::Core INTERFACE_INCLUDE_DIRECTORIES)
    STRING(REGEX REPLACE "/include" "" VLCQtCoreLocation "${VLCQtCoreLocation}")
ENDIF()
