set(ACE_LIBRARY_KIND OBJECT CACHE STRING "Build kind: static library or bunch of .o files. OBJECT allows for more link-time optimizations.")
set(ACE_LIBRARY_KINDS "OBJECT" "STATIC")
set_property(CACHE ACE_LIBRARY_KIND PROPERTY STRINGS ${ACE_LIBRARY_KINDS})
set(ACE_DEBUG OFF CACHE BOOL "Build with ACE-specific debug/safety functionality.")
set(ACE_DEBUG_UAE OFF CACHE BOOL "With ACE_DEBUG enabled, output log to UAE console.")
set(ACE_BOB_WRAP_Y ON CACHE BOOL "Conrols Y-wrapping support in bob manager. Disable for extra performance in simple buffer scenarios.")
set(ACE_USE_ECS_FEATURES OFF CACHE BOOL "Enable ECS feature sets.")
set(ACE_TILEBUFFER_TILE_TYPE UBYTE CACHE STRING "Specify type used for storing tile indices in tileBuffer manager.")
set(ACE_FILE_USE_ONLY_DISK OFF CACHE BOOL "If enabled, only diskFile functions will be available for file access.")
message(STATUS "[ACE] ACE_LIBRARY_KIND: '${ACE_LIBRARY_KIND}'")
message(STATUS "[ACE] ACE_DEBUG: '${ACE_DEBUG}'")
message(STATUS "[ACE] ACE_DEBUG_UAE: '${ACE_DEBUG_UAE}'")
message(STATUS "[ACE] ACE_BOB_WRAP_Y: '${ACE_BOB_WRAP_Y}'")
message(STATUS "[ACE] ACE_USE_ECS_FEATURES: '${ACE_USE_ECS_FEATURES}'")
message(STATUS "[ACE] ACE_TILEBUFFER_TILE_TYPE: '${ACE_TILEBUFFER_TILE_TYPE}'")
message(STATUS "[ACE] ACE_FILE_USE_ONLY_DISK: '${ACE_FILE_USE_ONLY_DISK}'")
