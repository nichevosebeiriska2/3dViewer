# CMakeList.txt: файл проекта CMake верхнего уровня; проведите глобальную конфигурацию
# и включите сюда подпроекты.
#
cmake_minimum_required (VERSION 3.8)

project (helloworld VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



set(VCPKG_PATH "C:/vcpkg/packages")

list(APPEND CMAKE_PREFIX_PATH ${VCPKG_PATH}"/qtbase_x64-windows")
list(APPEND CMAKE_PREFIX_PATH ${VCPKG_PATH}"/assimp_x64-windows")
list(APPEND CMAKE_PREFIX_PATH ${VCPKG_PATH}"/eigen3_x64-windows")
list(APPEND CMAKE_PREFIX_PATH ${VCPKG_PATH}"/embree_x64-windows")


find_package(Qt6 REQUIRED COMPONENTS Core Widgets OpenGLWidgets)
find_package(assimp CONFIG REQUIRED)
find_package(libigl REQUIRED COMPONENTS core embree)

qt_standard_project_setup()

file(GLOB_RECURSE HEADER_FILES ${CMAKE_SOURCE_DIR}/src/*.h)
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)

include(FetchContent)

FetchContent_Declare(
    libigl
    GIT_REPOSITORY https://github.com/libigl/libigl.git
    GIT_TAG        v2.6.0           # ← Фиксированная версия (важно!)
    GIT_SHALLOW    TRUE             # ← Ускорить загрузку
)

# Сделать libigl доступным
FetchContent_MakeAvailable(libigl)

qt_add_executable(${PROJECT_NAME} main.cpp ${HEADER_FILES} ${SOURCE_FILES})

target_compile_definitions(${PROJECT_NAME} PRIVATE 
    IGL_NO_RAYTRI
    IGL_NO_EMBREE
)


set_target_properties(${PROJECT_NAME} PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
Qt6::Core 
Qt6::Widgets
Qt6::OpenGLWidgets
opengl32.lib
glu32.lib
assimp::assimp
igl::core
#embree
#Eigen3::Eigen
)

target_compile_definitions(${PROJECT_NAME} PRIVATE QT_WIDGETS_LIB)
target_include_directories(${PROJECT_NAME} PRIVATE
  src/global
  src/object_wrapper
  src/glwidget
  src/drawing
  src/node
  src/widgets
  src/modelloader
  src/mainwindow
  src/scene
  src/shaders
  src/WrapperSTL
)



if(WIN32)

  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  
    set(WINDEPLOYQT_PATH "C:/vcpkg/installed/x64-windows/tools/Qt6/bin/windeployqt.debug.bat")
      set(VCPKG_BIN "C:/vcpkg/installed/x64-windows/debug/bin")
      set(OUTPUT_DIR "C:/Users/niche/source/repos/3dViewer/out/build/x64-Debug")
      set(TARGET_DIR "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
      set(VCPKG_BIN_DEBUG, "C:/vcpkg/installed/x64-windows/debug/bin/")

      add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy_if_different
              "${VCPKG_BIN}/harfbuzz.dll"
              "${VCPKG_BIN}/freetyped.dll"
              "${VCPKG_BIN}/libpng16d.dll"
              "${VCPKG_BIN}/zlibd1.dll"
              "${VCPKG_BIN}/icuind78.dll"
              "${VCPKG_BIN}/md4c.dll"
              "${VCPKG_BIN}/double-conversion.dll"
              "${VCPKG_BIN}/pcre2-16d.dll"
              "${VCPKG_BIN}/zstd.dll"
              "${VCPKG_BIN}/bz2d.dll"
              "${VCPKG_BIN}/brotlidec.dll"
              "${VCPKG_BIN}/brotlicommon.dll"
              "${VCPKG_BIN}/assimp-vc143-mtd.dll"
              "${VCPKG_BIN}/poly2tri.dll"
              "${VCPKG_BIN}/minizip.dll"
              "${VCPKG_BIN}/kubazip.dll"
              "${VCPKG_BIN}/pugixml.dll"
              "${TARGET_DIR}/"
          COMMENT "Copying third-party DLLs..."
          VERBATIM
      )
      add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${WINDEPLOYQT_PATH}" 
        $<$<CONFIG:Debug>:--debug> 
        "$<TARGET_FILE:${PROJECT_NAME}>"
        COMMENT "Deploying Qt DLLs..."
    )


  elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  
    set(WINDEPLOYQT_PATH "C:/vcpkg/installed/x64-windows/tools/Qt6/bin/windeployqt.exe")
    set(VCPKG_BIN "C:/vcpkg/installed/x64-windows/bin")
    set(OUTPUT_DIR "C:/work/69-figma_parser/Tools/QtTest/out/build/x64-release")
    set(TARGET_DIR "$<TARGET_FILE_DIR:${PROJECT_NAME}>")

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${VCPKG_BIN}/harfbuzz.dll"
            "${VCPKG_BIN}/freetype.dll"
            "${VCPKG_BIN}/libpng16.dll"
            "${VCPKG_BIN}/zlib1.dll"
            "${VCPKG_BIN}/icuin78.dll"
            "${VCPKG_BIN}/md4c.dll"
            "${VCPKG_BIN}/double-conversion.dll"
            "${VCPKG_BIN}/pcre2-16.dll"
            "${VCPKG_BIN}/zstd.dll"
            "${VCPKG_BIN}/bz2.dll"
            "${VCPKG_BIN}/brotlidec.dll"
            "${VCPKG_BIN}/brotlicommon.dll"
            "${VCPKG_BIN}/assimp-vc143-mt.dll"
            "${VCPKG_BIN}/poly2tri.dll"
            "${VCPKG_BIN}/minizip.dll"
            "${VCPKG_BIN}/kubazip.dll"
            "${VCPKG_BIN}/pugixml.dll"
            "${TARGET_DIR}/"
        COMMENT "Copying third-party DLLs..."
        VERBATIM
    )

    
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${WINDEPLOYQT_PATH}" "$<TARGET_FILE:${PROJECT_NAME}>"
        COMMENT "Deploying Qt DLLs..."
    )
  endif()


endif()