include(SetSourceGroup)

SetSourceGroup(NAME Root
	PREFIX SIMPLE_GAME
	SOURCE_FILES main.cpp
)

SetSourceGroup(NAME Renderer
	PREFIX SIMPLE_GAME
	SOURCE_FILES renderer/renderer.cpp
	             renderer/renderer.h
)

SetSourceGroup(NAME Renderer/Shaders
	PREFIX SIMPLE_GAME
	SOURCE_FILES renderer/shaders/main_vs.glsl
	             renderer/shaders/main_ps.glsl
	             renderer/shaders/sky_vs.glsl
	             renderer/shaders/sky_ps.glsl
)

set(SIMPLE_GAME_SRC 
	${SIMPLE_GAME_ROOT}
	${SIMPLE_GAME_RENDERER}
	${SIMPLE_GAME_RENDERER_SHADERS}
)

configure_file(renderer/shaders/main_vs.glsl main_vs.glsl COPYONLY)
configure_file(renderer/shaders/main_ps.glsl main_ps.glsl COPYONLY)
configure_file(renderer/shaders/sky_vs.glsl sky_vs.glsl COPYONLY)
configure_file(renderer/shaders/sky_ps.glsl sky_ps.glsl COPYONLY)
configure_file(color.bmp color.bmp COPYONLY)

add_executable(SimpleGame ${SIMPLE_GAME_SRC})
target_link_libraries(SimpleGame stb glm glad glfw ${GLFW_LIBRARIES} ${OPENGL_gl_LIBRARY})
target_include_directories(SimpleGame PRIVATE ./)
