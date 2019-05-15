# module Physx 4.1

message(STATUS "Finding PhysX libraries")

find_path(PhysX_INCLUDE_DIR PxPhysicsAPI.h
  PATH_SUFFIXES include Include
  PATHS
  ${PHYSX_HOME}
  $ENV{PHYSX_HOME}
  ${CMAKE_SOURCE_DIR}/../PhysX/physx/
)
message(STATUS "Physx Include Path: " ${PhysX_INCLUDE_DIR})

if(MSVC)
	if (CMAKE_SIZEOF_VOID_P EQUAL 8) 
		set(PHYSX_BIN_ARCH_PREFIX "win.x86_64")
		set(PHYSX_LIB_POSTFIX "_64")
	else (CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(PHYSX_BIN_ARCH_PREFIX "win.x86_32")
	endif (CMAKE_SIZEOF_VOID_P EQUAL 8)
	
	message (STATUS ${PHYSX_BIN_ARCH_PREFIX})

	if(MSVC_VERSION EQUAL 1800)
		set(PHYSX_BIN_COMPILER_POSTFIX "vc120.mt")
	elseif(MSVC_VERSION EQUAL 1900)
		set(PHYSX_BIN_COMPILER_POSTFIX "vc140.mt")
	elseif(MSVC_VERSION GREATER 1900)
		set(PHYSX_BIN_COMPILER_POSTFIX "vc141.mt")
	else()
		return()
	endif(MSVC_VERSION EQUAL 1800)	
	set(PHYSX_LIB_CONFIG ${PHYSX_BIN_ARCH_PREFIX}.${PHYSX_BIN_COMPILER_POSTFIX})

	message(STATUS ${PHYSX_LIB_CONFIG})
endif (MSVC)

set(PHYSX_BIN_RELEASE_PATH ${PhysX_INCLUDE_DIR}/../bin/${PHYSX_LIB_CONFIG}/release/)
find_library(Physx_LIBRARY_RELEASE PhysX${PHYSX_LIB_POSTFIX}
	PATHS
	${PHYSX_BIN_RELEASE_PATH}
)
message(STATUS "Physx Release Lib " ${Physx_LIBRARY_RELEASE})
SET(PhysX_LIBRARIES
  optimized ${Physx_LIBRARY_RELEASE}
)

set(PHYSX_BIN_DEBUG_PATH ${PhysX_INCLUDE_DIR}/../bin/${PHYSX_LIB_CONFIG}/debug/)
find_library(Physx_LIBRARY_DEBUG PhysX${PHYSX_LIB_POSTFIX}
	PATHS
	${PHYSX_BIN_DEBUG_PATH}
)
message(STATUS "Physx Debug Lib " ${Physx_LIBRARY_DEBUG})
set(PhysX_LIBRARIES
  ${PhysX_LIBRARIES} debug ${Physx_LIBRARY_DEBUG}
)

foreach(component ${Physx_FIND_COMPONENTS})
	message(STATUS "Component " ${component})
	set(COMPONENT_LIBRARY_NAME PhysX${component}${PHYSX_LIB_POSTFIX} PhysX${component}_static${PHYSX_LIB_POSTFIX})

	find_library(Physx_${component}_LIBRARY_DEBUG ${COMPONENT_LIBRARY_NAME}
		PATHS
		${PHYSX_BIN_DEBUG_PATH}
	)
	message(STATUS "Physx ${component} Debug Lib"  ${Physx_${component}_LIBRARY_DEBUG})
	if (Physx_${component}_LIBRARY_DEBUG)
		set(PhysX_LIBRARIES
		  ${PhysX_LIBRARIES}
		  debug "${Physx_${component}_LIBRARY_DEBUG}"
		)
	endif()

	find_library(Physx_${component}_LIBRARY_RELEASE ${COMPONENT_LIBRARY_NAME}
		PATHS
		${PHYSX_BIN_RELEASE_PATH}
	)
	message(STATUS "Physx ${component} Release Lib"  ${Physx_${component}_LIBRARY_RELEASE})
	if (Physx_${component}_LIBRARY_RELEASE)
		set(PhysX_LIBRARIES
		  ${PhysX_LIBRARIES}
		  optimized "${Physx_${component}_LIBRARY_RELEASE}"
		)
	endif()
endforeach()

set(PxSHARED_INCLUDE_DIR ${PhysX_INCLUDE_DIR}/../../pxshared/include/)
message(STATUS "PxShared Include Path: " ${PxSHARED_INCLUDE_DIR})
set(PhysX_INCLUDE_DIR ${PhysX_INCLUDE_DIR} ${PxSHARED_INCLUDE_DIR})