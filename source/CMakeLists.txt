cmake_minimum_required(VERSION 3.14)
project(CG)

# Установите стандарт C++
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Найдите компоненты Qt
find_package(Qt6 REQUIRED COMPONENTS Widgets)


set(UI_FILES mainWindow.ui)
set(SOURCE_FILES main.cpp MainWindow.cpp)
# Укажите исполняемый файл и исходные файлы
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${UI_FILES})

# Ссылки на библиотеки Qt
target_link_libraries(${PROJECT_NAME} Qt6::Widgets)

# Установите пути включения и компиляции
target_include_directories(${PROJECT_NAME} PRIVATE ${Qt6_INCLUDE_DIRS})
