SET(LIB_NAME LRE)

#######################################
# Library files
#######################################
#FIND_PACKAGE( SomePackage REQUIRED )

#######################################
# Solution definition
#######################################
ADD_DEFINITIONS(-DLRE_LIBRARY)

SET(TARGET_SRC
	ArgumentParser.cpp
	Component.cpp
	FileUtil.cpp
	ReplaceEngine.cpp
	Set.cpp
)
SET(TARGET_H
	ArgumentParser.h
	Component.h
	Export.h
	FileUtil.h
	ReplaceEngine.h
	Set.h
)
SET(EXTERN_SRC
	extern/FileNameUtils.h
	extern/FileNameUtils.cpp
	extern/FileUtils.h
	extern/FileUtils.cpp
)
SOURCE_GROUP("External" FILES ${EXTERN_SRC})
SOURCE_GROUP("LRE" FILES ${TARGET_H} ${TARGET_SRC})

SET(TARGET_TARGETNAME
    ${LIB_NAME}
)

ADD_LIBRARY(${TARGET_TARGETNAME} 
    ${LRE_USER_DEFINED_DYNAMIC_OR_STATIC}
    ${TARGET_SRC} ${TARGET_H}
    ${EXTERN_SRC}
)

LINK_WITH_VARIABLES(
    ${TARGET_TARGETNAME}
#none
)

#LINK_EXTERNAL(${TARGET_TARGETNAME} ${OPENGL_LIBRARIES}) 
 
INCLUDE(ModuleInstall OPTIONAL)

