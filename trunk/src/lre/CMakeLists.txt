SET(TARGET_TARGETNAME
    libLRE
)
SET(LIB_NAME
	${TARGET_TARGETNAME}
)
# Output name will be LRE.dll/LREd.dll (LRE.so/LREd.so on Linux'es)
# A different target name is used to avoid conflicts to lre.exe
SET(LIB_OUTPUT_NAME
	LRE
)

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
