import random
import math
import matplotlib.pyplot as plt
import plotly.graph_objects as go
import generators

def fade(f):
    """Функция сглаживания для плавного перехода."""
    #return math.cos(f)
    return 6 * f ** 5 - 15 * f ** 4 + 10 * f ** 3

# Инициализация размеров сетки
x_size = 11
y_size = 11

# Инициализация сетки градиентов
net = []
generators.island(net, x_size, y_size)

# Размер карты шума
perlin = []

def dot_product(grad, offset):
    """Вычисление скалярного произведения градиента и смещения."""
    return grad[0] * offset[0] + grad[1] * offset[1]

def lerp(a, b, t):
    """Линейная интерполяция между a и b с учётом t."""
    return a + t * (b - a)

# Переменные для хранения текущих координат
x_cur = 0
y_cur = 0
count = 0

# Генерация Перлин шума
for i in range(100):
    perlin.append([])
    x_cur = 0  # Сбрасываем x для каждой новой строки
    for j in range(100):
        # Выбор градиентов
        grad1 = net[int(y_cur)][int(x_cur)]          # Левый верхний узел
        grad2 = net[int(y_cur) + 1][int(x_cur)]      # Левый нижний узел
        grad3 = net[int(y_cur)][int(x_cur) + 1]      # Правый верхний узел
        grad4 = net[int(y_cur) + 1][int(x_cur) + 1]  # Правый нижний узел

        # Расчёт смещений
        u = x_cur - int(x_cur)  # Дробная часть по x
        v = y_cur - int(y_cur)  # Дробная часть по y

        # Смещения от узлов к точке (x, y)
        offset1 = (u, v)            # Смещение от градиента grad1
        offset2 = (u, v - 1)        # Смещение от градиента grad2
        offset3 = (u - 1, v)        # Смещение от градиента grad3
        offset4 = (u - 1, v - 1)    # Смещение от градиента grad4

        # Вычисление значений шума от каждого градиента
        noise1 = dot_product(grad1, offset1)  # Шум от лев. верх. узла
        noise2 = dot_product(grad2, offset2)  # Шум от лев. ниж. узла
        noise3 = dot_product(grad3, offset3)  # Шум от прав. верх. узла
        noise4 = dot_product(grad4, offset4)  # Шум от прав. ниж. узла

        # Применение функции сглаживания
        u = fade(u)
        v = fade(v)

        # Интерполяция по оси x
        interpX1 = lerp(noise1, noise3, u)  # Между левым верхним и правым верхним
        interpX2 = lerp(noise2, noise4, u)  # Между левым нижним и правым нижним

        # Интерполяция по оси y
        final_noise = lerp(interpX1, interpX2, v)  # Финальная интерполяция

        # Сохраняем результат в массив
        perlin[count].append(final_noise)
        x_cur += 0.1  # Масштабируем по оси X
    y_cur += 0.1  # Масштабируем по оси Y
    count += 1

# Визуализация карты шума
plt.imshow(perlin, cmap='gray_r', interpolation='lanczos')
plt.colorbar()  # Добавляем цветовую шкалу
plt.title('Шум')
plt.xlabel('X')
plt.ylabel('Y')
plt.show()


fig = go.Figure(data=[go.Surface(z=perlin, colorscale='Greys')])

# Настройка осей и визуализации
fig.update_layout(title='Перлин Шум - График поверхности', autosize=False,
                  scene = dict(
                      xaxis_title='X',
                      yaxis_title='Y',
                      zaxis_title='Height'
                  ),
                  width=800, height=800)

# Отображение графика
fig.show()
