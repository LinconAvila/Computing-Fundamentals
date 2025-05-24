# Operational Systems

This repository contains a collection of programs demonstrating various concepts in operating systems, including process management, thread synchronization, and parallel computing. The programs are written in C and C++ and cover a range of topics from basic process creation to matrix operations using threads.

---

## Programs Overview

### 1. Process Management

- **collatz_sequence_processes.c**: Demonstrates process creation using `fork()` and the Collatz sequence calculation.
- **extern_process_executing.c**: Shows how to execute an external program using `fork()` and `execl()`.
- **process_array_multiplication.c**: Multiplies two arrays using processes, showcasing process creation and synchronization.
- **process_parity_array_ops.c**: Performs array operations based on the parity of process IDs, demonstrating conditional process behavior.

### 2. Thread Synchronization and Parallel Computing

- **matrix_sum_threads.c**: Sums two matrices using threads, with workload distribution and synchronization.
- **matrix_product.c**: Multiplies two matrices using threads, demonstrating parallel computation.
- **sum_matrices.c**: Another implementation of matrix summation using threads.
- **transpose_matrices.c**: Transposes a matrix using threads, showcasing parallel data transformation.
- **vector_product.c**: Multiplies two vectors using threads, illustrating parallel element-wise operations.

### 3. File and Text Processing

- **text_analyser.c**: Analyzes text files in a directory using threads, counting words, vowels, and consonants.
- **image_converter.cpp**: Converts a color image to grayscale using threads, demonstrating parallel image processing with OpenCV.

---

## Compilation and Execution

### C Programs

To compile and run the C programs:
```bash
gcc program_name.c -o output_name -lpthread
./output_name
```

### C++ Program (Image Converter)

To compile and run the C++ program:
```bash
g++ image_converter.cpp -o image_converter -lpthread -lopencv_core -lopencv_highgui -lopencv_imgproc
./image_converter
```

---

## Features

- **Process Management**: Demonstrates `fork()`, `waitpid()`, and `execl()`.
- **Thread Synchronization**: Uses `pthread_create()`, `pthread_join()`, and workload balancing.
- **Parallel Computing**: Implements matrix and vector operations using threads for efficiency.
- **Error Handling**: Includes robust input validation and error handling.
- **Performance Measurement**: Measures execution time for parallel tasks.

---

## Usage Examples

- **Matrix Summation**:  
  Run `matrix_sum_threads.c`, input matrix dimensions, and observe the parallel summation.

- **Text Analysis**:  
  Run `text_analyser.c`, provide a directory path, and analyze text files for word and character statistics.

- **Image Conversion**:  
  Run `image_converter.cpp`, input an image path, and convert it to grayscale using multiple threads.

---

## Dependencies

- **C Programs**: Standard C libraries (`stdio.h`, `stdlib.h`, `pthread.h`, etc.).
- **C++ Program**: OpenCV library for image processing.

---

## Notes

- Ensure all dependencies are installed before compiling.
- Adjust the number of threads or processes based on your system's capabilities for optimal performance.

---

This repository serves as a practical guide for understanding operating system concepts through hands-on programming. Feel free to explore, modify, and extend the programs for further learning.