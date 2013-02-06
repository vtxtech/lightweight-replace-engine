SET(DATA_PATH ${CMAKE_SOURCE_DIR}/data)

SET(DATA_FILES
	${DATA_PATH}/doc1.csv.in
	${DATA_PATH}/doc_with_errors1.csv.in2
	${DATA_PATH}/doc_with_errors2.csv.in2
	${DATA_PATH}/doc_with_errors3.csv.in2
	${DATA_PATH}/test.csv
)

SET(DATA_INSTALLDIR "${LRE_SHARE_INSTALLDIR_DATA}")

# Setup install
INSTALL(
	FILES        ${DATA_FILES}
	DESTINATION  ${DATA_INSTALLDIR}
)

ADD_SUBDIRECTORY(sub)
ADD_SUBDIRECTORY(data-multifile)
ADD_SUBDIRECTORY(data-singlefile)
