cmake_minimum_required(VERSION 2.8)

project(Sdt_SingleRegion)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})
if (ITKVtkGlue_LOADED)
  find_package(VTK REQUIRED)
  include(${VTK_USE_FILE})
endif()

add_executable(Sdt_SingleRegion MACOSX_BUNDLE Sdt_SingleRegion.cxx)

if( "${ITK_VERSION_MAJOR}" LESS 4 )
  target_link_libraries(test_readpixel20140729 ITKReview ${ITK_LIBRARIES})
else( "${ITK_VERSION_MAJOR}" LESS 4 )
  target_link_libraries(Sdt_SingleRegion ${ITK_LIBRARIES})
endif( "${ITK_VERSION_MAJOR}" LESS 4 )

