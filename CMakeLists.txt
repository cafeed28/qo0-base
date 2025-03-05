cmake_minimum_required(VERSION 3.16)
project(qo0)
add_library(qo0 MODULE)

# common
set_target_properties(qo0 PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)

# include
target_include_directories(qo0 PUBLIC "${CMAKE_SOURCE_DIR}/base")
target_include_directories(qo0 PUBLIC "${CMAKE_SOURCE_DIR}/resources")
target_include_directories(qo0 PUBLIC "${CMAKE_SOURCE_DIR}/dependencies/minhook")

# dependencies
add_subdirectory(dependencies)

# source
target_sources(qo0 PRIVATE

	# dependencies

	# minhook
	"dependencies/minhook/buffer.c"
	"dependencies/minhook/hook.c"
	"dependencies/minhook/trampoline.c"
	"dependencies/minhook/hde/hde32.c"
	"dependencies/minhook/hde/hde64.c"

	# base
	"base/core.cpp"
	"base/features.cpp"
	"base/sdk.cpp"

	# core
	"base/core/config.cpp"
	"base/core/convar.cpp"
	"base/core/entitylistener.cpp"
	"base/core/eventlistener.cpp"
	"base/core/hooks.cpp"
	"base/core/interfaces.cpp"
	"base/core/menu.cpp"
	"base/core/netvar.cpp"
	"base/core/proxies.cpp"

	# features

	# legit
	"base/features/legit/triggerbot.cpp"

	# misc
	"base/features/misc/movement.cpp"

	# rage
	"base/features/rage/antiaim.cpp"

	# visual
	"base/features/visual/chams.cpp"
	"base/features/visual/glow.cpp"
	"base/features/visual/overlay.cpp"
	"base/features/visual/screen.cpp"
	"base/features/visual/world.cpp"
	"base/features/animationcorrection.cpp"
	"base/features/autowall.cpp"
	"base/features/lagcompensation.cpp"
	"base/features/legit.cpp"
	"base/features/misc.cpp"
	"base/features/prediction.cpp"
	"base/features/rage.cpp"
	"base/features/visual.cpp"

	# sdk

	# datatypes
	"base/sdk/datatypes/matrix.cpp"
	"base/sdk/datatypes/qangle.cpp"
	"base/sdk/datatypes/vector.cpp"

	# hash
	"base/sdk/hash/crc32.cpp"
	"base/sdk/hash/md5.cpp"
	"base/sdk/animation.cpp"
	"base/sdk/entity.cpp"
	"base/sdk/studio.cpp"

	# utilities
	"base/utilities/draw.cpp"
	"base/utilities/inputsystem.cpp"
	"base/utilities/log.cpp"
	"base/utilities/memory.cpp"
)

# link
target_link_libraries(qo0 "d3d9.lib" imgui)

# configuration specific
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_compile_definitions(qo0 PRIVATE
		_DEBUG
	)
	set_target_properties(qo0 PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
	target_compile_definitions(qo0 PRIVATE
		NDEBUG
	)
	set_target_properties(qo0 PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded")
endif()

# compiler specific
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	target_compile_definitions(qo0 PRIVATE
		Q_COMPILER_CLANG
	)

	target_compile_options(qo0 PRIVATE
		-fno-rtti
	)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	target_compile_definitions(qo0 PRIVATE
		Q_COMPILER_MSC
	)

	target_compile_options(qo0 PRIVATE
		/MP /GR- /permissive-
	)

	target_link_options(qo0 PRIVATE
		/MANIFEST:NO
	)
endif()

# architecture specific
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	target_compile_definitions(qo0 PRIVATE
		Q_ARCH_X64
	)
else()
	target_compile_definitions(qo0 PRIVATE
		Q_ARCH_X86
	)
endif()
