cmake_minimum_required( VERSION 2.8 FATAL_ERROR )
set( CMAKE_VERBOSE_MAKEFILE ON )

project( InsectRobotSimulation )

get_filename_component( CINDER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cinder" ABSOLUTE )
get_filename_component( APP_PATH "${CMAKE_CURRENT_SOURCE_DIR}" ABSOLUTE )

include( "${CINDER_PATH}/proj/cmake/modules/cinderMakeApp.cmake" )
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Arduino/libraries/InsectRobot)

set( PROJECT_SOURCES
	${APP_PATH}/src/Convertions.cpp
	${APP_PATH}/src/DebugRenderer.cpp
	${APP_PATH}/src/InsectRobotSimulationApp.cpp
	${APP_PATH}/src/Leg.cpp
	${APP_PATH}/src/Mesh.cpp
	${APP_PATH}/src/MeshPool.cpp
	${APP_PATH}/src/ModelConfigGui.cpp
	${APP_PATH}/src/ModelControlGiu.cpp
	${APP_PATH}/src/ModelRenderer.cpp
	${APP_PATH}/src/Node.cpp
	${APP_PATH}/Arduino/libraries/InsectRobot/Matrix44.cpp
	${APP_PATH}/Arduino/libraries/InsectRobot/Vector3.cpp
)

ci_make_app(
	APP_NAME    "InsectRobotSimulation"
	BLOCKS			${CINDER_PATH}/blocks/Cinder-ImGui
	CINDER_PATH ${CINDER_PATH}
	SOURCES     ${PROJECT_SOURCES}
	INCLUDES    ${APP_PATH}/include
	RESOURCES   ${APP_PATH}/resources/cinder_app_icon.ico
)
