SET(TARGET_TARGETNAME
    libLRE
)
SET(LIB_NAME
	${TARGET_TARGETNAME}
)
# Output name will be lre.dll/LREd.dll (lre.so/LREd.so on Linux'es)
# A different target name is used to avoid conflicts to lre.exe
SET(LIB_OUTPUT_NAME
	lre
)

#######################################
# Solution definition
#######################################
IF(LRE_BUILD_SHARED_LIBS)
	ADD_DEFINITIONS(-DLRE_LIBRARY)
ELSE(LRE_BUILD_SHARED_LIBS)
	ADD_DEFINITIONS(-DSTATIC_LRE_LIBRARIES)
ENDIF(LRE_BUILD_SHARED_LIBS)

SET(TARGET_SRC
	ArgumentParser.cpp
	Component.cpp
	FileUtils.cpp
	Notify.cpp
	ReplaceEngine.cpp
	Set.cpp
	Settings.cpp
)
SET(TARGET_H
	ArgumentParser.h
	Component.h
	Export.h
	FileUtils.h
	Notify.h
	ReplaceEngine.h
	Set.h
	Settings.h
)
SET(EXTERN_SRC
	extern/FileUtils.h
	extern/FileUtils.cpp
)
SOURCE_GROUP("External" FILES ${EXTERN_SRC})
SOURCE_GROUP("LRE" FILES ${TARGET_H} ${TARGET_SRC})

#######################################
# Build target
#######################################

ADD_LIBRARY(${TARGET_TARGETNAME} 
    ${LRE_USER_DEFINED_DYNAMIC_OR_STATIC}
    ${TARGET_SRC} ${TARGET_H}
    ${EXTERN_SRC}
)

SET_TARGET_PROPERTIES(${TARGET_TARGETNAME} PROPERTIES PROJECT_LABEL "${LABEL_LRE_USER_DEFINED_DYNAMIC_OR_STATIC} Library ${TARGET_TARGETNAME}")
SET_TARGET_PROPERTIES(${TARGET_TARGETNAME} PROPERTIES OUTPUT_NAME ${LIB_OUTPUT_NAME})
  
INCLUDE(ModuleInstall OPTIONAL)
