# Fundamentals of Computer Graphics and Visualisation

**Course:** Assessment 1 - Individual Assignment  
**Topic:** Fundamentals of Computer Graphics and Visualisation  
**Author:** Muhammad Uzayr Bin Nazari / 2025513977  

---

## 1. Introduction
Computer Graphics (CG) and Visualisation are key fields in computer science that transform abstract data and mathematical models into visual representations. While **Computer Graphics** focuses on rendering geometric shapes, lighting, and transformations, **Data Visualisation** leverages these graphical techniques to communicate complex scalar, vector, or spatial datasets efficiently.

---

## 2. Core Concepts Covered

### 2.1 Discretization & Mesh Grids
Continuous mathematical functions must be converted into discrete data structures for computer rendering. Using NumPy, a 2D spatial grid ($X, Y$) is defined across a domain using `meshgrid`, establishing spatial coordinates for the rendering pipeline.

### 2.2 3D Surface Projection
The continuous scalar field function:
r = \sqrt{X^2 + Y^2} is evaluated at every point on the grid. Matplotlib converts these 3D points (X, Y, Z) into a 2D perspective projection visible on screen.

### 2.3 Color Mapping & Visual Encoding
Visualisation relies heavily on color encoding to convey depth and scalar magnitude. This project uses the `coolwarm` color map:
* **Red Hues:** High intensity values (Z \approx 1.0)
* **Blue Hues:** Low intensity troughs (Z \approx -0.2)

---
## 3. Environment & Technical Specifications

The experiment and demonstration were developed and executed using the following software and environment setup:

| Specification Category | Used Specification / Tool |
| :--- | :--- |
| **Operating System** | Kali Linux (Debian-based Rolling Release) |
| **Execution Environment** | Kali Terminal / Bash Shell (`x86_64`) |
| **Programming Language** | Python 3.11+ |
| **Core Libraries** | `matplotlib` (v3.8+), `numpy` (v1.26+) |
| **Graphics Toolkit** | `mplot3d` (Matplotlib 3D Toolkit) |
| **GUI Backend** | `TkAgg` / `Qt5Agg` (X11 Display Window) |
| **Rendering Backend** | Agg Engine (Anti-Grain Geometry Engine for Anti-Aliased Surface Rendering) |

---

## 4. Python Implementation

The project is implemented using Python, NumPy, and Matplotlib.

```python
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import cm

def generate_3d_terrain():
    # 1. Spatial Coordinates Grid
    x = np.linspace(-5, 5, 100)
    y = np.linspace(-5, 5, 100)
    X, Y = np.meshgrid(x, y)

    # 2. Mathematical Function (Scalar Field)
    R = np.sqrt(X**2 + Y**2)
    Z = np.sin(R) / (R + 1e-5)

    # 3. 3D Plot Setup & Rendering
    fig = plt.figure(figsize=(10, 7))
    ax = fig.add_subplot(111, projection='3d')

    surf = ax.plot_surface(X, Y, Z, cmap=cm.coolwarm, linewidth=0, antialiased=True)
    
    fig.colorbar(surf, ax=ax, shrink=0.5, aspect=10, label='Height / Intensity (Z)')
    ax.set_title("3D Surface Visualisation of Scalar Field Data")
    ax.set_xlabel("X Axis")
    ax.set_ylabel("Y Axis")
    ax.set_zlabel("Z Axis")

    plt.show()

if __name__ == "__main__":
    generate_3d_terrain()
```

---

## 5. Execution Output
<img width="867" height="602" alt="Screenshot 2026-10-25 213800" src="https://github.com/user-attachments/assets/796b8978-f48d-4369-b7d0-02feec0ccb67" />

---

## 6. Video Demonstration
https://youtu.be/-qaLPiB-4BE

---

## 7. Source & References
1. Hearn, D., & Baker, M. P. (2004). Computer Graphics with OpenGL (3rd ed.). Pearson Education / Prentice Hall.

2. Hunter, J. D. (2007). Matplotlib: A 2D graphics environment. Computing in Science & Engineering, 9(3), 90–95. https://doi.org/10.1109/MCSE.2007.55

3. Harris, C. R., Millman, K. J., van der Walt, S. J., et al. (2020). Array programming with NumPy. Nature, 585(7825), 357–362. https://doi.org/10.1038/s41586-020-2649-2

4. Matplotlib Documentation. mplot3d Toolkit: 3D Surface Plots. https://matplotlib.org/stable/tutorials/toolkits/mplot3d.html

5. Offensive Security. Kali Linux Documentation & Technical Specifications. https://www.kali.org/docs

