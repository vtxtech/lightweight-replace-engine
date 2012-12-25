# INSTALL and SOURCE_GROUP commands for module installation

# Required Vars:
# ${LIB_NAME}
# ${TARGET_H}

SET(INSTALL_INCDIR include)
SET(INSTALL_BINDIR bin)
IF(WIN32)
    SET(INSTALL_LIBDIR bin)
    SET(INSTALL_ARCHIVEDIR lib)
ELSE(WIN32)
    SET(INSTALL_LIBDIR lib${LIB_POSTFIX})
    SET(INSTALL_ARCHIVEDIR lib${LIB_POSTFIX})
ENDIF(WIN32)

# set up project groups (mainly used by visual studio)
SET(HEADERS_GROUP "Header Files")

SOURCE_GROUP(
    ${HEADERS_GROUP}
    FILES ${TARGET_H}
)


IF(MSVC)
	SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES PREFIX "../")
ENDIF(MSVC)


IF(LIB_NAME_LABEL)
    SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES PROJECT_LABEL "${LIB_NAME_LABEL}")
ENDIF(LIB_NAME_LABEL)


INSTALL(
    TARGETS ${LIB_NAME}
    RUNTIME DESTINATION ${INSTALL_BINDIR}
    LIBRARY DESTINATION ${INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${INSTALL_ARCHIVEDIR}
)
# FIXME: Do not run for OS X framework
INSTALL(
    FILES       ${TARGET_H}
    DESTINATION ${INSTALL_INCDIR}/${LIB_NAME}
)