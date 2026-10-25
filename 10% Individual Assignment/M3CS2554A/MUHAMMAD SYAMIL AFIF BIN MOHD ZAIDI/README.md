# MUHAMMAD SYAMIL AFIF BIN MOHD ZAIDI
# Graphic Rendering Performance Simulator
<br>

**NAME:** MUHAMMAD SYAMIL AFIF BIN MOHD ZAIDI<br>
**STUDENT ID:** 2025195645<br>
**CLASS:** CDCS2554A<br>
**TOPIC:** Fundamental Of Computer Graphic And Visualisation<br>
**COURSE CODE:** ITT 440 - Network Programming<br>
**YOUTUBE LINK:** https://youtu.be/Z2Rtt6l2T2M<br>

---

## PROBLEM STATEMENT

In computer graphics and visualization, rendering highly detailed objects such as 3D models with millions of polygons or particle simulations requires immense computational power. Processing graphical data sequentially often creates a processing bottleneck on the CPU, leading to prolonged execution times. 

This project focuses on simulating and evaluating the efficiency of various processing methods (sequential, threading, and multiprocessing) when handling large-scale graphical coordinate calculations. The aim is to identify which approach provides better performance and scalability for rendering high-resolution graphics.

---

## LITERATURE REVIEW

According to *Computer Graphics: Principles and Practice* (Hughes et al., 2013), decoupling computational tasks using parallel processing techniques is essential for achieving efficiency in complex graphic rendering. 

Furthermore, Akenine-Möller et al. (2018) in *Real-Time Rendering* state that modern computational architecture relies on concurrency to distribute workloads evenly across multiple processor cores. This approach significantly accelerates matrix calculations and rasterization processes compared to traditional sequential methods.

---

## SYSTEM DESIGN

The system is designed to automatically generate millions of coordinate data points (vertices/pixels) based on user input to simulate a large-scale graphics workload. The generated data is then processed using three different techniques:

* **Sequential Processing:** Each coordinate is processed and rendered one by one using a single execution thread.
* **Concurrent Processing (Threading):** Multiple threads are used to process the rendering data simultaneously by retrieving tasks from a shared queue.
* **Parallel Processing (Multiprocessing):** Multiple processes are created using a process pool, where each process handles a portion of the computational workload independently across multiple CPU cores.

The system measures the execution time for each processing method using a timer. After processing is complete, the system displays a summarized analytics report, including the number of vertices processed and the total execution time for performance comparison.

---

## SYSTEM REQUIREMENTS

To run the Graphic Rendering Performance Simulator, the system requirements are:

* **Operating System:** Windows, Linux, or macOS
* **Programming Language:** Python 3.8 or above
* **Processor:** Multicore CPU
* **RAM:** Minimum 4GB (larger RAM is recommended as the dataset handled is massive)
* **Storage:** At least 1GB free space
* **Terminal:** Required to run the program

---

## INSTALLATION STEPS

### A. LAUNCH THE SIMULATOR

Navigate to the project folder:
```bash
cd graphic_rendering_sim
```

Run the program:
```bash
python rendering_main.py
```

### B. PROVIDE DESIRED INPUT

When prompted, enter the required values. For example:
```text
Enter number of vertices/pixels to simulate (example: 1000000): 1000000
Enter number of threads: 4
Enter number of processes: 4
```

---

## SAMPLE OUTPUT & ANALYTIC

```text
========== GRAPHIC ANALYTICS REPORT ==========

Vertices distribution by object type:
Terrain (Green) : 199843 vertices (19.98%)
Sky (Blue)      : 199495 vertices (19.95%)
Water (Cyan)    : 199958 vertices (20.00%)
Buildings (Grey): 200565 vertices (20.06%)
Characters (Red): 200139 vertices (20.01%)

Total vertices processed: 1000000

========== PERFORMANCE ==========
Sequential      : 5.0578s
Threading       : 12.7011s
Multiprocessing : 1.7012s
Fastest Method  : Multiprocessing
```

---

## CONCLUSION

In conclusion, this project successfully developed a graphic rendering simulator to process large-scale visual data efficiently. The results validate the computer graphics literature, demonstrating that sequential processing becomes highly inefficient as the dataset size increases due to its one-by-one execution approach. 

Threading provided limited improvements for CPU-intensive rendering operations. On the other hand, multiprocessing achieved the best performance by utilizing multiple CPU cores, enabling true parallel execution and significantly reducing processing time. This highlights the importance of selecting the appropriate processing method based on the nature of the workload to improve efficiency in computer graphics applications.

---

## REFERENCES

1. Scratchapixel. (n.d.). *Foundation of Computer Graphics and Rendering*. This comprehensive online article covers the basic principles of generating 3D images on a 2D screen. Available at: [https://www.scratchapixel.com/](https://www.scratchapixel.com/)
2. Real Python. (n.d.). *Speed Up Your Python Program With Concurrency*. A detailed article explaining how to use threading and multiprocessing to overcome execution bottlenecks in Python. Available at: [https://realpython.com/python-concurrency/](https://realpython.com/python-concurrency/)
3. Python Software Foundation. (n.d.). *multiprocessing — Process-based parallelism*. Official documentation and reading material on how Python handles multiple CPU cores. Available at: [https://docs.python.org/3/library/multiprocessing.html](https://docs.python.org/3/library/multiprocessing.html)
