cmake_minimum_required(VERSION 3.14)
project(CG)

# Установите стандарт C++
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -march=native -mtune=native -flto")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")
# Найдите компоненты Qt
find_package(Qt6 REQUIRED COMPONENTS Widgets)


set(UI_FILES mainWindow.ui)
set(SOURCE_FILES main.cpp MainWindow.cpp Drawer/QtDrawer.cpp
        Utils/Color.h Utils/SolutionImpl.h
        Camera/Camera.h Camera/Camera.cpp
        Components/BaseComponent.h Components/BaseComponent.cpp
        Components/Concrete/Line.cpp Components/Concrete/Line.h
        Components/Concrete/Polygon.cpp Components/Concrete/Polygon.h
        Objects/BaseObject.cpp Objects/BaseObject.h
        Objects/Concrete/CarcasObject.cpp Objects/Concrete/CarcasObject.h
        Objects/Concrete/PolygonObject.cpp Objects/Concrete/PolygonObject.h
        Manager/Manager.h Manager/Manager.cpp
        DrawAdapter/BaseDrawAdapter.h
        DrawAdapter/Concrete/CarcasDrawAdapter.h DrawAdapter/Concrete/CarcasDrawAdapter.cpp
        DrawAdapter/Concrete/PolygonDrawAdapter.h DrawAdapter/Concrete/PolygonDrawAdapter.cpp
        Scene/Scene.cpp Scene/Scene.h
        Light/Light.h
        )

# Укажите исполняемый файл и исходные файлы
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${UI_FILES})

# Ссылки на библиотеки Qt
target_link_libraries(${PROJECT_NAME} Qt6::Widgets)

# Установите пути включения и компиляции
target_include_directories(${PROJECT_NAME} PRIVATE ${Qt6_INCLUDE_DIRS})
