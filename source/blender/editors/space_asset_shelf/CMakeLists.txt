set(INC
	../include
	../../blenfont
	../../blenkernel
	../../blenlib
	../../gpu
	../../imbuf
	../../makesdna
	../../makesrna
	../../windowmanager
	../../../../intern/guardedalloc
)
 
set(INC_SYS
	${GLEW_INCLUDE_PATH}
)
 
set(SRC
	space_asset_shelf.c
)

blender_add_lib(bf_editor_space_asset_shelf "${SRC}" "${INC}" "${INC_SYS}" "${LIB}")