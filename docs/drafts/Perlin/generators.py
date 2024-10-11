import random
import math

def simple(net, x_size, y_size):
    for i in range(x_size):
        new = []
        for j in range(y_size):
            # Генерация случайных градиентов, нормируем векторы
            angle = random.uniform(0, 2 * math.pi)  # Случайный угол
            new.append([math.cos(angle), math.sin(angle)])  # Вектор градиента
        net.append(new)

def island(net, x_size, y_size):
    center_x = x_size // 2
    center_y = y_size // 2
    max_distance = math.sqrt(center_x**2 + center_y**2)
    
    for i in range(x_size):
        new = []
        for j in range(y_size):
            # Вычисляем расстояние от центра острова
            distance = math.sqrt((center_x - i) ** 2 + (center_y - j) ** 2)
            # Задаём коэффициент для радиального затухания
            influence = (max_distance - distance) / max_distance
            #influence = math.exp(-0.5 * distance)
        
            
            # Генерация случайных градиентов с нормировкой
            angle = random.uniform(0, 2 * math.pi)
            gradient = [math.cos(angle) * influence, math.sin(angle) * influence]
            print(influence, gradient)
            new.append(gradient)
        net.append(new)
            