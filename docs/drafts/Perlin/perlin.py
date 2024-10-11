import numpy as np
import matplotlib.pyplot as plt
import plotly.graph_objects as go

def perlin(x, y, seed=0):
    np.random.seed(seed)
    ptable = np.arange(256, dtype=int)
    print(ptable)
    np.random.shuffle(ptable)
    ptable = np.stack([ptable, ptable]).flatten()
    xi, yi = x.astype(int), y.astype(int)
    xg, yg = x - xi, y - yi
    xf, yf = fade(xg), fade(yg)

    n00 = gradient(ptable[ptable[xi] + yi], xg, yg)
    n01 = gradient(ptable[ptable[xi] + yi + 1], xg, yg - 1)
    n11 = gradient(ptable[ptable[xi + 1] + yi + 1], xg - 1, yg - 1)
    n10 = gradient(ptable[ptable[xi + 1] + yi], xg - 1, yg)

    x1 = lerp(n00, n10, xf)
    x2 = lerp(n01, n11, xf)
    return lerp(x1, x2, yf)

def lerp(a, b, x):
    return a + x * (b - a)

def fade(f):
    return 6 * f ** 5 - 15 * f ** 4 + 10 * f ** 3
    #return f**2 / np.e ** f

def gradient(c, x, y):
    vectors = np.array([[0, 1], [0, -1], [1, 0], [-1, 0]])
    gradient_co = vectors[c % 4]
    return gradient_co[:, :, 0] * x + gradient_co[:, :, 1] * y

# Generate coordinates for the Perlin function
lin_array = np.linspace(1, 10, 500, endpoint=False)
x, y = np.meshgrid(lin_array, lin_array)

# Compute Perlin noise
z = perlin(x, y, seed=2)

# Normalize z values to range [0, 1]
"""
z_min = z.min()
z_max = z.max()
z_normalized = (z - z_min) / (z_max - z_min)
"""
z_normalized = z

# 2D plot using Matplotlib
plt.imshow(z, origin='upper', cmap='gray_r')
plt.colorbar()
plt.title('2D Perlin Noise')
plt.xlabel('X Axis')
plt.ylabel('Y Axis')
plt.gca().invert_yaxis()  # Изменение направления оси Y
plt.show()

# Create a 3D plot using Plotly with a black-to-white gradient
fig = go.Figure(data=[go.Surface(z=z_normalized, x=x, y=y, colorscale='Greys')])

# Update layout for better visualization
fig.update_layout(title='3D Perlin Noise (Black to White Gradient)',
                  scene=dict(xaxis_title='X Axis',
                             yaxis_title='Y Axis',
                             zaxis_title='Z Axis'),
                  autosize=True)

# Show the plot
fig.show()
