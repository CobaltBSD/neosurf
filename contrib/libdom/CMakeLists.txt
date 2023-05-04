add_library(dom SHARED
	src/core/string.c
	src/core/node.c
	src/core/attr.c
	src/core/characterdata.c
	src/core/element.c
	src/core/implementation.c
	src/core/text.c
	src/core/comment.c
	src/core/namednodemap.c
	src/core/nodelist.c
	src/core/cdatasection.c
	src/core/document_type.c
	src/core/entity_ref.c
	src/core/pi.c
	src/core/doc_fragment.c
	src/core/document.c 
	src/events/mouse_multi_wheel_event.c
	src/events/mouse_event.c
	src/events/mutation_name_event.c
	src/events/custom_event.c
	src/events/event_listener.c
	src/events/document_event.c
	src/events/ui_event.c
	src/events/keyboard_event.c
	src/events/mutation_event.c
	src/events/text_event.c
	src/events/event.c
	src/events/mouse_wheel_event.c
	src/events/dispatch.c
	src/events/event_target.c
	src/html/html_element.c
	src/html/html_ulist_element.c
	src/html/html_script_element.c
	src/html/html_font_element.c
	src/html/html_canvas_element.c
	src/html/html_menu_element.c
	src/html/html_document.c
	src/html/html_body_element.c
	src/html/html_div_element.c
	src/html/html_hr_element.c
	src/html/html_tablecell_element.c
	src/html/html_directory_element.c
	src/html/html_map_element.c
	src/html/html_frameset_element.c
	src/html/html_pre_element.c
	src/html/html_br_element.c
	src/html/html_option_element.c
	src/html/html_legend_element.c
	src/html/html_base_element.c
	src/html/html_anchor_element.c
	src/html/html_isindex_element.c
	src/html/html_basefont_element.c
	src/html/html_label_element.c
	src/html/html_applet_element.c
	src/html/html_iframe_element.c
	src/html/html_param_element.c
	src/html/html_button_element.c
	src/html/html_input_element.c
	src/html/html_olist_element.c
	src/html/html_select_element.c
	src/html/html_tablecaption_element.c
	src/html/html_mod_element.c
	src/html/html_fieldset_element.c
	src/html/html_object_element.c
	src/html/html_tablecol_element.c
	src/html/html_heading_element.c
	src/html/html_style_element.c
	src/html/html_head_element.c
	src/html/html_quote_element.c
	src/html/html_tablerow_element.c
	src/html/html_options_collection.c
	src/html/html_dlist_element.c
	src/html/html_li_element.c
	src/html/html_meta_element.c
	src/html/html_opt_group_element.c
	src/html/html_form_element.c
	src/html/html_area_element.c
	src/html/html_tablesection_element.c
	src/html/html_text_area_element.c
	src/html/html_frame_element.c
	src/html/html_title_element.c
	src/html/html_table_element.c
	src/html/html_collection.c
	src/html/html_paragraph_element.c
	src/html/html_html_element.c
	src/html/html_link_element.c
	src/html/html_image_element.c
	src/utils/namespace.c
#	src/utils/hashtable.c
	src/utils/character_valid.c
	src/utils/validate.c

	# Build bindings
	bindings/xml/libxml_xmlparser.c
	#bindings/xml/expat_xmlparser.c
)

add_subdirectory(bindings/hubbub)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -W -Wundef -Wpointer-arith -Wcast-align -Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -D_BSD_SOURCE -D_DEFAULT_SOURCE -I${CMAKE_SOURCE_DIR}/contrib/libdom/include/ -I${CMAKE_SOURCE_DIR}/contrib/libdom/src -std=c99 -I${CMAKE_SOURCE_DIR}/contrib/libwapcaplet/include -I${CMAKE_SOURCE_DIR}/contrib/libparserutils/include -Werror -I${CMAKE_SOURCE_DIR}/contrib/libdom/bindings -I/usr/include/libxml2 -I${CMAKE_SOURCE_DIR}/contrib/libhubbub/include")

target_link_libraries(dom wapcaplet parserutils xml2)

install(TARGETS dom DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)