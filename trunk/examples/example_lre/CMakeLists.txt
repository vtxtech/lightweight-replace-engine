# LRE Example Application
SET(TARGET_TARGETNAME
    example_lre
)

SET(TARGET_SRC
    main.cpp
    )
    
SET(TARGET_H
#	none
    )
    
ADD_EXECUTABLE(${TARGET_TARGETNAME} ${TARGET_SRC} ${TARGET_H})


LINK_INTERNAL(${TARGET_TARGETNAME} LRE) 

