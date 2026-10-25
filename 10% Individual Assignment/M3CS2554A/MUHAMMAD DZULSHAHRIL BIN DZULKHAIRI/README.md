# MUHAMMAD DZULSHAHRIL BIN DZULKHAIRI

# Fundamentals of Computer Graphics: 2D Geometric Transformations

**NAME:** MUHAMMAD DZULSHAHRIL BIN DZULKHAIRI<br>
**STUDENT ID:** 2025594337<br>
**CLASS:** M3CS2554A<br>
**COURSE CODE:** ITT440 Network Programming<br>
**YOUTUBE LINK:** [Youtube 2D Geometric Transformations](https://youtu.be/ZDvM6zOPJC8?si=wJIf0M9TsA4TqHpf)

<div align="justify">

## INTRODUCTION

Computer graphics is about creating and changing images on a computer. One basic task is changing a shape that is already on the screen, which is called a transformation. Transformations are used to reposition graphics and to change their size or orientation (University of Mosul, 2025). When this happens on a flat plane, it is called a 2D transformation (University of Mosul, 2025).

The basic types are translation, scaling, rotation, reflection and shear (University of Mosul, 2025). This project demonstrates all five:

- **Translation** moves a shape by adding a distance to its x and y coordinates (University of Mosul, 2025; King Fahd University of Petroleum and Minerals, n.d.).
- **Scaling** makes a shape bigger or smaller by multiplying its coordinates by a scale factor (University of Mosul, 2025; King Fahd University of Petroleum and Minerals, n.d.).
- **Rotation** turns a shape by an angle, calculated with sine and cosine (King Fahd University of Petroleum and Minerals, n.d.; Munzner, 2016).
- **Reflection** flips a shape over an axis, like a mirror image (University of Mosul, 2025).
- **Shear** slants a shape sideways so that its corners shift by different amounts (University of Mosul, 2025).

## PROBLEM STATEMENT

A shape on the screen is stored as a list of corner points. To move, resize or turn the shape, the computer must apply a formula to every corner point (Munzner, 2016). Doing this by hand is slow and easy to get wrong, and the result is hard to understand from numbers alone.

Therefore, this project uses Python to apply the five transformations to a triangle, and then draws the original and transformed shapes together so the effect of each transformation can be seen clearly.

## SYSTEM DESIGN

First, the program defines a triangle using the x and y values of its three corners. Next, it applies five functions to the triangle, one for each transformation. Each function takes the corner points and returns the new corner points.

| Transformation | Formula |
|---|---|
| Translation | x' = x + tx, y' = y + ty |
| Scaling | x' = x * sx, y' = y * sy |
| Rotation | x' = x cos(a) - y sin(a), y' = x sin(a) + y cos(a) |
| Reflection (x-axis) | x' = x, y' = -y |
| Shear (x direction) | x' = x + k * y, y' = y |

After that, the program prints the new corner points in the terminal. Finally, it uses Matplotlib's `plot` function (Matplotlib Development Team, n.d.) to draw the original triangle (grey, dashed) and the transformed triangle (coloured) in six small graphs, so each transformation can be compared with the original.

## SYSTEM REQUIREMENTS

- **Operating System:** Windows, Linux or macOS
- **Python Version:** Python 3.8 or above
- **Libraries:** NumPy and Matplotlib
- **Display:** A screen that can open a Matplotlib window

## INSTALLATION STEPS

**A. Install the libraries**

```bash
pip install numpy matplotlib
```

**B. Run the program** (inside the project folder)

```bash
python3 transform_demo.py
```

The program prints the results, saves the graph as `output.png` and opens it in a window.

## SAMPLE OUTPUT

```
===== 2D GEOMETRIC TRANSFORMATIONS =====
Original corners    : [(0.0, 0.0), (4.0, 0.0), (2.0, 3.0)]
Translate (+5, +2)  : [(5.0, 2.0), (9.0, 2.0), (7.0, 5.0)]
Scale (x2)          : [(0.0, 0.0), (8.0, 0.0), (4.0, 6.0)]
Rotate 90 degrees   : [(0.0, 0.0), (0.0, 4.0), (-3.0, 2.0)]
Reflect over x-axis : [(0.0, 0.0), (4.0, 0.0), (2.0, -3.0)]
Shear (k = 1)       : [(0.0, 0.0), (4.0, 0.0), (5.0, 3.0)]
```

<img width="1440" height="960" alt="output" src="https://github.com/user-attachments/assets/66ce43ac-0d0b-4179-8e0a-02abc471ede6" />


## ANALYSIS

| Transformation | What happened to the triangle |
|---|---|
| Translation | Moved 5 to the right and 2 up, with the same size and shape |
| Scaling | Became twice as large, with every corner twice as far from the origin |
| Rotation | Turned 90 degrees around the origin, so (4, 0) became (0, 4) |
| Reflection | Flipped upside down, so the top corner (2, 3) became (2, -3) |
| Shear | Slanted to the right, with only the top corner moving from (2, 3) to (5, 3) |

## DEMONSTRATION VIDEO

Watch the demonstration here: [Youtube 2D Geometric Transformations](https://youtu.be/ZDvM6zOPJC8?si=wJIf0M9TsA4TqHpf)

## CONCLUSION

In conclusion, this project showed how 2D transformations change the position, size and orientation of a shape by applying simple formulas to its corner points. The program demonstrated translation, scaling, rotation, reflection and shear on one triangle, and Matplotlib made the results easy to compare by drawing each one next to the original.

The results also showed that each transformation affects the shape differently: translation, rotation and reflection keep the size and shape, scaling changes the size, and shear changes the shape itself. These basic operations are the foundation for more advanced graphics such as animation and 3D modelling.

## SOURCE CODE

The full source code is in [`transform_demo.py`](transform_demo.py) in this repository.

## REFERENCES

King Fahd University of Petroleum and Minerals. (n.d.). *2D transformations* [Lecture notes, ICS 415 Computer Graphics]. https://faculty.kfupm.edu.sa/ics/mulhem/ics415/05-2dtransformations.pdf

Matplotlib Development Team. (n.d.). *matplotlib.pyplot.plot*. Matplotlib documentation. https://matplotlib.org/stable/api/_as_gen/matplotlib.pyplot.plot.html

Munzner, T. (2016). *Transformations 2* [Lecture slides, CPSC 314 Computer Graphics]. University of British Columbia. https://www.cs.ubc.ca/~tmm/courses/314-16/slides/transforms2.pdf

University of Mosul. (2025). *2D transformations* [Lecture notes]. Department of Computer Science. https://uomosul.edu.iq/computerscience/wp-content/uploads/sites/13/2025/05/2D-Transformations_P1_6.pdf

</div>
