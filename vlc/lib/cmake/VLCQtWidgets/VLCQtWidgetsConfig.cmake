INCLUDE(CMakeFindDependencyMacro)
FIND_DEPENDENCY(VLCQtCore)

INCLUDE("${CMAKE_CURRENT_LIST_DIR}/VLCQtWidgetsTargets.cmake")

SET_TARGET_PROPERTIES(VLCQt::Widgets PROPERTIES
    INTERFACE_LINK_LIBRARIES VLCQt::Core
)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET(VLCQtWidgetsFramework ${CMAKE_CURRENT_LIST_DIR}/../../VLCQtWidgets.framework)
ELSE()
    GET_TARGET_PROPERTY(VLCQtWidgetsLocation VLCQt::Widgets INTERFACE_INCLUDE_DIRECTORIES)
    STRING(REGEX REPLACE "/include" "" VLCQtWidgetsLocation "${VLCQtWidgetsLocation}")
ENDIF()
