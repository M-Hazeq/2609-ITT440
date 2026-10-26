# MUHAMMAD RIEZZLAN HAQIMY BIN MOHD RIZZUAN
# Fundamentals of Computer Graphics and Visualisation

## 1. Introduction
Computer Graphics and Visualisation are core disciplines in computer science focused on generating, manipulating, and rendering visual content, as well as transforming complex abstract data into intuitive visual representations. While computer graphics synthesizes images from geometric models (such as in video games, animations, and simulations), visualisation bridges data science and human perception, enabling users to explore patterns, trends, and anomalies in complex datasets.
 
---
 
## 2. The Computer Graphics Pipeline
The computer graphics pipeline defines the sequence of operations required to convert a 3D geometric description of a scene into a 2D raster image displayed on a screen. The standard pipeline consists of several key stages:

* Modeling / Geometry Specification: Defining objects in a scene using primitives such as points, lines, and triangles (triangle meshes).
* Transformation: Converting coordinates from object space to world space, view/camera space, and ultimately screen space using affine transformations and projection matrices.
* Rasterization: Determining which discrete screen pixels are covered by each geometric primitive (triangle).
* Fragment Shading & Texturing: Computing per-pixel colors, lighting calculations, and applying texture maps.
* Blending & Depth Testing (Z-Buffering): Resolving visibility by ensuring closer objects properly obscure objects behind them.

---

## 3. Principles of Data Visualisation
Visualisation leverages human visual processing capabilities to make complex data comprehensible. Key design principles include:
* Visual Encoding: Mapping data attributes (quantitative, categorical, temporal) to visual channels like position, size, shape, and color.
* Clarity and Simplicity: Minimizing visual clutter and cognitive load to highlight core insights.
* Interactivity: Allowing users to filter, zoom, and rotate visual models to inspect data from multiple perspectives.

---

## 4. Programming Example: 3D Wireframe Transformation and Visualisation
Below is a complete Python programming example using numpy and matplotlib that demonstrates fundamental computer graphics concepts: defining 3D vertices of a cube, applying 3D rotation matrices, performing orthographic projection, and visualising the wireframe in 2D space.

```python
import numpy as np
import plotly.graph_objects as go

# Define 3D vertices for a cube
vertices = np.array([
    [-1, -1, -1],  # 0
    [ 1, -1, -1],  # 1
    [ 1,  1, -1],  # 2
    [-1,  1, -1],  # 3
    [-1, -1,  1],  # 4
    [ 1, -1,  1],  # 5
    [ 1,  1,  1],  # 6
    [-1,  1,  1]   # 7
], dtype=float)

# Define edges connecting the vertices
edges = [
    (0, 1), (1, 2), (2, 3), (3, 0),  # Back face
    (4, 5), (5, 6), (6, 7), (7, 4),  # Front face
    (0, 4), (1, 5), (2, 6), (3, 7)   # Connecting edges
]

def rotation_matrix_y(theta):
    """Generates a 3D rotation matrix around the Y axis."""
    c, s = np.cos(theta), np.sin(theta)
    return np.array([
        [ c, 0, s],
        [ 0, 1, 0],
        [-s, 0, c]
    ])

# Apply rotation (e.g., 45 degrees)
theta = np.radians(45)
rot_matrix = rotation_matrix_y(theta)
rotated_vertices = np.dot(vertices, rot_matrix.T)

# Prepare lines for Plotly 3D plot
x_lines, y_lines, z_lines = [], [], []
for edge in edges:
    p1 = rotated_vertices[edge[0]]
    p2 = rotated_vertices[edge[1]]
    x_lines.extend([p1[0], p2[0], None])
    y_lines.extend([p1[1], p2[1], None])
    z_lines.extend([p1[2], p2[2], None])

# Create the 3D figure
fig = go.Figure(data=[
    go.Scatter3d(
        x=x_lines, y=y_lines, z=z_lines,
        mode='lines',
        line=dict(color='blue', width=5)
    ),
    go.Scatter3d(
        x=rotated_vertices[:, 0],
        y=rotated_vertices[:, 1],
        z=rotated_vertices[:, 2],
        mode='markers',
        marker=dict(size=5, color='red')
    )
])
fig.update_layout(
    title='Interactive 3D Rotation in Google Colab',
    scene=dict(
        xaxis_title='X',
        yaxis_title='Y',
        zaxis_title='Z'
    )
)

fig.show()
```
## Libraries and Tools in Python
Several libraries and frameworks are widely used in Python for computer graphics and data visualisation, including Matplotlib for 2D/3D plotting, NumPy for geometric transformations and matrix calculations, Plotly for interactive web-based graphics, and Open3D for advanced 3D data processing. [This video](https://youtu.be/q2AuFC3Bhvs) shows a demonstration of the 3D rotation and visualisation program.
## Sources

* [Fundamentals of Computer Graphics (PDF) - The Swiss Bay](https://theswissbay.ch/pdf/Gentoomen%20Library/Computer%20Graphics/Fundamentals%20of%20Computer%20Graphics%20--%20Peter%20Shirley.pdf)
* [Graphics Compendium - Chapter 2: The Graphics Pipeline](https://graphicscompendium.com/intro/01-graphics-pipeline)
* [TutorialsPoint - Computer Graphics Useful Resources](https://www.tutorialspoint.com/computer_graphics/computer_graphics_useful_resources.htm)
