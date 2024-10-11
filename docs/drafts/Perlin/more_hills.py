import random
import math
import matplotlib.pyplot as plt
import plotly.graph_objects as go
import generators

def linear_falloff(dist, max_radius):
    return max(0, 1 - dist / max_radius)

def parabolic_falloff(dist, max_radius):
    if dist > max_radius:
        return 0
    return 1 - (dist / max_radius) ** 2

def exponential_falloff(dist, max_radius):
    return math.exp(-0.2 * dist / max_radius)

def cubic_falloff(dist, max_radius):
    if dist > max_radius:
        return 0
    return (1 - (dist / max_radius) ** 3)

def sinusoidal_falloff(dist, max_radius):
    if dist > max_radius:
        return 0
    return math.cos((dist / max_radius) * math.pi / 2)

def gaussian_falloff(distance, max_distance, sigma=0):
    if sigma == 0:
        sigma = 20
    """Гауссовское затухание для плавного перехода высот."""
    return math.exp(-(distance ** 2) / (2 * sigma ** 2))

def hyperbolic_falloff(distance, max_distance, k=5):
    """Гиперболическое затухание с крутыми склонами на краях."""
    return 1 / (1 + (distance / max_distance) ** k)

def parabolic_falloff(distance, max_distance):
    """Параболическое затухание, плавные склоны с резкими краями."""
    return 1 - (distance / max_distance) ** 2

# Комбинированное затухание (Layered Falloff)
def combined_falloff(distance, max_distance):
    """Комбинированное затухание (сочетание гауссовского и гиперболического)."""
    gaussian = gaussian_falloff(distance, max_distance, max_distance / 2)
    hyperbolic = hyperbolic_falloff(distance, max_distance, 3)
    return gaussian * hyperbolic

def generate_peaks(num_peaks, map_size):
    """Генерация нескольких пиков на карте."""
    peaks = []
    for _ in range(num_peaks):
        # Случайная позиция вершины
        x = random.uniform(0, map_size)
        y = random.uniform(0, map_size)
        # Случайная высота и радиус затухания
        height = random.uniform(0.5, 1.0)
        radius = random.uniform(map_size // 8, map_size // 4)
        peaks.append((x, y, height, radius))
    return peaks

def multi_peak_falloff(x, y, peaks):
    """Функция затухания для нескольких пиков."""
    total_height = 0
    for peak_x, peak_y, peak_height, peak_radius in peaks:
        # Вычисляем расстояние до вершины
        dist = math.sqrt((x - peak_x) ** 2 + (y - peak_y) ** 2)
        # Применяем одну из функций затухания
        influence = hyperbolic_falloff(dist, peak_radius)
        total_height += peak_height * influence
    return total_height

# Пример использования
x_size = 100
y_size = 100
num_peaks = 5  # Количество пиков на карте
peaks = generate_peaks(num_peaks, x_size)

# Генерация карты горного массива
perlin = []
for i in range(y_size):
    perlin.append([])
    for j in range(x_size):
        # Рассчитываем высоту на основе нескольких пиков
        height = multi_peak_falloff(j, i, peaks)
        perlin[i].append(height)

# Визуализация карты
plt.imshow(perlin, cmap='gray_r', interpolation='lanczos')
plt.colorbar()  # Добавляем цветовую шкалу
plt.title('Горный массив с несколькими пиками')
plt.xlabel('X')
plt.ylabel('Y')
plt.show()

# График поверхности
fig = go.Figure(data=[go.Surface(z=perlin, colorscale='Greys')])
fig.update_layout(title='Горный массив - График поверхности', autosize=False,
                  scene=dict(
                      xaxis_title='X',
                      yaxis_title='Y',
                      zaxis_title='Height'
                  ),
                  width=800, height=800)
fig.show()
