# Copyright daemon_process

INCLUDE = ..\;$(INCLUDE)

FLAGS = /c /MP /GS /TP /analyze- /W4 /wd"4100" /wd"4127" /wd"4244" /wd"4481" /wd"4512" /wd"4701" /wd"4702" /wd"4996" /Gy /Zc:wchar_t /Zi /Gm- /O2 /Ob2 /Zc:inline /fp:precise /D "WIN32" /D "_WINDOWS" /D "NDEBUG" /D "__STDC_CONSTANT_MACROS" /D "__STDC_FORMAT_MACROS" /D "_WIN32" /D "UNICODE" /D "_UNICODE" /D "WINVER=0x0602" /D "_WIN32_WINNT=0x602" /D "NOMINMAX" /D "WIN32_LEAN_AND_MEAN" /D "_HAS_EXCEPTIONS=0" /D "PSAPI_VERSION=1" /D "CEF_USE_SANDBOX" /D "CEF_USE_ATL" /D "_NDEBUG" /D "USING_CEF_SHARED" /errorReport:prompt /GF /WX /Zc:forScope /GR- /Gd /Oy- /MT /EHsc /nologo /Fd:".\temp.pdb"
	
OBJS = app_cpptoc.obj auth_callback_ctocpp.obj base_cpptoc.obj before_download_callback_ctocpp.obj binary_value_ctocpp.obj browser_ctocpp.obj browser_host_ctocpp.obj browser_process_handler_cpptoc.obj callback_ctocpp.obj cef_atomicops_x86_gcc.obj cef_bind_helpers.obj cef_byte_read_handler.obj cef_callback_helpers.obj cef_callback_internal.obj cef_closure_task.obj cef_lock.obj cef_lock_impl.obj cef_logging.obj cef_message_router.obj cef_ref_counted.obj cef_resource_manager.obj cef_stream_resource_handler.obj cef_string16.obj cef_thread_checker_impl.obj cef_thread_collision_warner.obj cef_weak_ptr.obj cef_xml_object.obj cef_zip_archive.obj client_cpptoc.obj command_line_ctocpp.obj completion_callback_cpptoc.obj context_menu_handler_cpptoc.obj context_menu_params_ctocpp.obj cookie_manager_ctocpp.obj cookie_visitor_cpptoc.obj delete_cookies_callback_cpptoc.obj dialog_handler_cpptoc.obj dictionary_value_ctocpp.obj display_handler_cpptoc.obj domdocument_ctocpp.obj domnode_ctocpp.obj domvisitor_cpptoc.obj download_handler_cpptoc.obj download_item_callback_ctocpp.obj download_item_ctocpp.obj drag_data_ctocpp.obj drag_handler_cpptoc.obj end_tracing_callback_cpptoc.obj file_dialog_callback_ctocpp.obj find_handler_cpptoc.obj focus_handler_cpptoc.obj frame_ctocpp.obj geolocation_callback_ctocpp.obj geolocation_handler_cpptoc.obj get_geolocation_callback_cpptoc.obj jsdialog_callback_ctocpp.obj jsdialog_handler_cpptoc.obj keyboard_handler_cpptoc.obj libcef_dll_wrapper.obj libcef_dll_wrapper2.obj life_span_handler_cpptoc.obj list_value_ctocpp.obj load_handler_cpptoc.obj menu_model_ctocpp.obj navigation_entry_ctocpp.obj navigation_entry_visitor_cpptoc.obj pdf_print_callback_cpptoc.obj post_data_ctocpp.obj post_data_element_ctocpp.obj print_dialog_callback_ctocpp.obj print_handler_cpptoc.obj print_job_callback_ctocpp.obj print_settings_ctocpp.obj process_message_ctocpp.obj read_handler_cpptoc.obj render_handler_cpptoc.obj render_process_handler_cpptoc.obj request_callback_ctocpp.obj request_context_ctocpp.obj request_context_handler_cpptoc.obj request_ctocpp.obj request_handler_cpptoc.obj resolve_callback_cpptoc.obj resource_bundle_ctocpp.obj resource_bundle_handler_cpptoc.obj resource_handler_cpptoc.obj response_ctocpp.obj response_filter_cpptoc.obj run_context_menu_callback_ctocpp.obj run_file_dialog_callback_cpptoc.obj scheme_handler_factory_cpptoc.obj scheme_registrar_ctocpp.obj set_cookie_callback_cpptoc.obj sslcert_principal_ctocpp.obj sslinfo_ctocpp.obj stream_reader_ctocpp.obj stream_writer_ctocpp.obj string_visitor_cpptoc.obj task_cpptoc.obj task_runner_ctocpp.obj transfer_util.obj urlrequest_client_cpptoc.obj urlrequest_ctocpp.obj v8accessor_cpptoc.obj v8context_ctocpp.obj v8exception_ctocpp.obj v8handler_cpptoc.obj v8stack_frame_ctocpp.obj v8stack_trace_ctocpp.obj v8value_ctocpp.obj value_ctocpp.obj web_plugin_info_ctocpp.obj web_plugin_info_visitor_cpptoc.obj web_plugin_unstable_callback_cpptoc.obj write_handler_cpptoc.obj xml_reader_ctocpp.obj zip_reader_ctocpp.obj

libcef_dll_wrapper.lib : default $(OBJS)
	lib /nologo *.obj /out:"..\..\build\libcef_dll_wrapper.lib"
	cd ..

{..\libcef_dll\base\}.cc.obj:
	@cl $(FLAGS) $< /Fo:$@

{..\libcef_dll\cpptoc\}.cc.obj:
	@cl $(FLAGS) $< /Fo:$@

{..\libcef_dll\ctocpp\}.cc.obj:
	@cl $(FLAGS) $< /Fo:$@

{..\libcef_dll\wrapper\}.cc.obj:
	@cl $(FLAGS) $< /Fo:$@

{..\libcef_dll\}.cc.obj:
	@cl $(FLAGS) $< /Fo:$@

default:
	@if not exist "out" mkdir out
	cd out

clean:
	del /f /q out\*.*