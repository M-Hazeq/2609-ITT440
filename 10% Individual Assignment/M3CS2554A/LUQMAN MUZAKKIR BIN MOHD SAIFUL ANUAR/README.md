# Image Brightness Analyzer: Image Processing with Python
NAME: LUQMAN MUZAKKIR BIN MOHD SAIFUL ANUAR

STUDENT ID: 2025985951

# Introduction
Image processing is a CPU-intensive workload found in medical imaging, photography apps, surveillance and machine learning pipelines. When thousands of images must be analyzed, the way the work is scheduled on the hardware has a major effect on total run time. This project builds an Image Brightness Analyzer that runs every image in a large dataset through a filter pipeline (grayscale conversion, Gaussian blur, edge detection and pixel analysis) and classifies it as **Dark**, **Normal** or **Bright**.

The primary goal is to demonstrate how Sequential, Concurrent (Threading) and Parallel (Multiprocessing) execution use hardware resources differently, especially on multi-core consumer hardware. The program runs all three methods automatically in a single command and prints one combined report.

# System
## Demonstration System

Operating System: Windows 11

Python Version: 3.14

CPU Cores: 6 cores

RAM: 8GB DDR5

## User Manual

### 1. System Requirements
Operating System: Windows 10/11

Language: Python 3.8 or higher

Hardware: Minimum 4GB RAM (8GB+ recommended)

Libraries: `Pillow` (image processing) and the standard Python libraries (`os`, `math`, `time`, `random`, `threading`, `multiprocessing`)

### 2. Installation Steps
1.  **Install Python**: Download and install the latest version of Python from [python.org](https://www.python.org/).
2.  **Download the Project**:
    *   Clone this repository or download the `image_processor.py` file directly to your computer.
3.  **Setup Environment**: Install the only external dependency:
    ```bash
    python -m pip install pillow
    ```

### 3. How to Run the Program
1.  Open your **Command Prompt (CMD)** or **Terminal**.
2.  Navigate to the folder where you saved the file.
3.  Run the program using the following command:
    ```bash
    python image_processor.py
    ```

# Objectives
1. To develop a functional Image Brightness Analyzer capable of processing a large dataset of images (10,000 or more).

2. To evaluate the performance impact of Python's Global Interpreter Lock (GIL) on multi-threaded execution.

3. To measure the speedup achieved by leveraging a 6-core CPU architecture through multiprocessing.

4. To provide a statistical summary of the image brightness categories (Dark, Normal and Bright) in the dataset.

# Methodology
The system creates a dataset of grayscale images (128 x 128 pixels). Each simulated image is generated from a unique random seed, so every run produces the same dataset. Each record consists of:

1. Image ID: Unique identifier.

2. Source: Random seed for a simulated image, or the file path for an image from the user's own folder.

3. Brightness level: Each simulated image has a random overall brightness (45 to 210) with random pixel noise of +/- 40.

Every image goes through the following pipeline:

1. Grayscale Conversion: Converts the image to a single luminance channel.

2. Gaussian Blur: Smooths the image to reduce noise.

3. Edge Detection: Highlights object boundaries and measures the average edge strength.

4. Pixel Analysis: A pure-Python loop adds up every pixel to compute the average brightness (0 to 255). This is a CPU-bound step that holds the GIL.

5. Classification: The average brightness decides the category.

Because every image is independent of the others, the workload is *embarrassingly parallel* and well suited to comparing the three execution models:

| Model | Python Tool | Expected Behaviour |
| :---- | :---------- | :----------------- |
| Sequential | Simple `for` loop | Uses one core only |
| Concurrent (Threading) | `threading.Thread` | Limited by the GIL for CPU-bound code |
| Parallel (Multiprocessing) | `multiprocessing.Pool` | Uses all available cores |

### The Algorithm
The `classify_image` function labels each image using these thresholds on the average brightness:

  Bright: Brightness > 170.

  Dark: Brightness < 85.

  Normal: All other cases.

```python
def classify_image(brightness):
    if brightness > 170:
        status = "Bright"
    elif brightness < 85:
        status = "Dark"
    else:
        status = "Normal"
    return status
```

The `process_image` function applies the full pipeline to one image:

```python
def process_image(task):
    _, source = task

    # 1. Load / generate + grayscale
    gray = load_gray(source)

    # 2. Gaussian blur (noise reduction)
    blurred = gray.filter(ImageFilter.GaussianBlur(radius=2))

    # 3. Edge detection -> average edge strength
    edges = blurred.filter(ImageFilter.FIND_EDGES)
    edge_strength = ImageStat.Stat(edges).mean[0]

    # 4. Pure-Python pixel analysis (CPU-bound, holds the GIL)
    total = 0
    count = 0
    for px in gray.tobytes():
        total += px
        count += 1
    brightness = total / count

    # 5. Classification
    return classify_image(brightness), brightness, edge_strength
```

The three execution models call the same function:

```python
# Sequential
results = [process_image(t) for t in items]

# Threading: the tasks are split into one block per thread
threads = [threading.Thread(target=worker, args=(start, end)) for start, end in blocks]
for t in threads: t.start()
for t in threads: t.join()

# Parallel (Multiprocessing)
with multiprocessing.Pool(processes=WORKERS) as pool:
    results = pool.map(process_image, items, chunksize=chunk)
```

### Combined Run
Option 1 calls `run_full_comparison()`, which runs the three methods in turn (`[1/3]`, `[2/3]`, `[3/3]`), times each one, and then prints a single report containing the time table, speedup, a time bar chart, the brightness summary and a correctness check.

# Performance Result
The following results were captured by running the analyzer across three different paradigms (replace the values below with your own measured times from the combined report):

| Method     | Time (s) | Speedup |
| :--------- | :------- | :------ |
| Sequential | _88.4075_ s | 1.00x |
| Threading  | _80.7356_ s | _1.10_ x |
| Parallel   | _13.0535_ s | _6.77_ x |

Speedup is calculated as:

```
Speedup = Sequential Time / Method Time
```

Screenshot:
<img width="480" height="197" alt="image" src="https://github.com/user-attachments/assets/b751d7b7-4d30-4610-a63f-43878670cbf7" />


# Image Brightness Summary
The combined report prints the following summary after processing the entire dataset (replace with your own values):

  Bright Images: **_(2,478(24.8%))_**

  Normal Images: **_(5,019(50.2%))_**

  Dark Images: **_(2,503(25%))_**

  Total Processed: **_(10000)_**

  Average Brightness: **_(127.36(0-255))_**

The program also checks that Sequential, Threading and Parallel all produce the same counts ("All methods same result: YES"), which confirms that the parallel versions give correct results.

Screenshot:
<img width="465" height="208" alt="image" src="https://github.com/user-attachments/assets/2b8bf73c-6103-463d-9061-61333501c879" />


# Conclusion
Image analysis is a CPU-bound task. Because of the Python GIL, threads cannot run Python bytecode in parallel, so Threading is expected to give little or no improvement over Sequential execution. Multiprocessing gives each worker its own interpreter and memory space, allowing true parallelism across all CPU cores and a much shorter run time.

This project highlights the importance of choosing the right concurrency model when developing high-performance image and data-processing applications in real-world scenarios.

# YouTube Demonstration
Link: [Demo Video](https://youtu.be/izXepYDVv2I)

# References
1. Python Software Foundation. *multiprocessing: Process-based parallelism*. Python 3 Documentation. https://docs.python.org/3/library/multiprocessing.html

2. Python Software Foundation. *threading: Thread-based parallelism*. Python 3 Documentation. https://docs.python.org/3/library/threading.html

3. Python Software Foundation. *Glossary: global interpreter lock*. Python 3 Documentation. https://docs.python.org/3/glossary.html#term-global-interpreter-lock

4. Pillow contributors. *Pillow (PIL Fork) Documentation: Image, ImageFilter and ImageStat modules*. https://pillow.readthedocs.io/

5. Beazley, D. *Understanding the Python GIL*. PyCon 2010. https://www.dabeaz.com/GIL/

6. Gonzalez, R. C., & Woods, R. E. (2018). *Digital Image Processing* (4th ed.). Pearson.

7. Amdahl, G. M. (1967). Validity of the single processor approach to achieving large scale computing capabilities. *AFIPS Conference Proceedings*, 30, 483-485.
