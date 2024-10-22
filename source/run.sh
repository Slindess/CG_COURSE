#!/bin/bash

# Создаем директорию для сборки
mkdir -p build
cd build

# Вызываем cmake для генерации файлов сборки
cmake ..

# Собираем проект с помощью make
make

if [[ $? -eq 0 ]]
then
  ./CG
fi