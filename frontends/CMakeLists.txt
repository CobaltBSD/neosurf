include_directories("${CMAKE_SOURCE_DIR}/frontends")

if(NEOSURF_BUILD_GTK_FRONTEND)
	add_subdirectory(gtk)
endif()

if(NEOSURF_BUILD_VI_FRONTEND)
	add_subdirectory(visurf)
endif()
