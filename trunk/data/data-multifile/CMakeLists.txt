SET(DATA_MULTIFILE_FILES
	${DATA_PATH}/data-multifile/commands.lre
	${DATA_PATH}/data-multifile/users.lre
)

INSTALL(
	FILES        ${DATA_MULTIFILE_FILES}
	DESTINATION  ${DATA_INSTALLDIR}/data-multifile
)
