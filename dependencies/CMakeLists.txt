# freetype
option(FT_DISABLE_HARFBUZZ "" ON)
add_subdirectory(freetype)

# imgui
file(GLOB IMGUI_SOURCES "imgui/*.cpp")
list(APPEND IMGUI_SOURCES
	"imgui/backends/imgui_impl_dx9.cpp"
	"imgui/backends/imgui_impl_win32.cpp"
	"imgui/misc/freetype/imgui_freetype.cpp"
)
add_library(imgui STATIC ${IMGUI_SOURCES})
target_link_libraries(imgui PRIVATE freetype)
target_include_directories(imgui PUBLIC "imgui")
