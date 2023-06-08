if(NEOSURF_BUILD_GTK_FRONTEND)
	add_subdirectory(gtk)
endif()

if(NEOSURF_BUILD_VI_FRONTEND)
	add_subdirectory(visurf)
endif()
