# Profiler Library

This is a simple profiling utility class in C++ that allows you to measure the execution time of code blocks in microseconds.

## How to Include the Profiler in Your Project

You can include the `Profiler` class in your project in two ways:

### 1. If the Profiler is in the Same Directory:
Simply include the header file in your source code:
```cpp
#include "profiler.h"
```

### If the Profiler is in a Different Directory:
To include the profiler.h from another directory, you need to specify the relative path in the #include directive. For example, if the profiler.h file is in the libs/profiler directory relative to your source code, include it like this:

```cpp
#include "../libs/profiler/profiler.h"
```

In this example, the profiler is located one level up from the current directory in the libs/profiler folder.

## Example Usage
Here’s how you can use the Profiler class in your project:
```cpp
#include <iostream>
#include "../libs/profiler/profiler.h"

int main() {
    // Create a Profiler object
    Profiler profiler;

    // Start the profiler
    profiler.start();

    // Code you want to profile
    for (int i = 0; i < 1000000; ++i) {
        // Simulated work
        int x = i * i;
    }

    // Stop the profiler
    profiler.stop();

    // Output the average time of the profiled code block
    std::cout << "Average time: " << profiler.average_time() << " microseconds" << std::endl;

    // Optionally reset the profiler for a new session
    profiler.reset();

    return 0;
}
```

## Explanation:
Start Profiling:

### 1. Start Profiling:
Use the start() method before the code block you want to measure.
Stop Profiling:

### 2. Stop Profiling:
Use the stop() method after the code block you want to measure. This will record the time elapsed.
Get the Average Time:

### 3. Get the Average Time:
Use the average_time() method to get the average time in microseconds for all the measurements.

### 4. Reset:
The reset() method will clear the accumulated time and count, allowing for a new profiling session.

## Methods in Profiler Class
`start()`: Starts the timer for the code block to be profiled.
`stop()`: Stops the timer and adds the elapsed time to the total time.
`average_time()`: Returns the average time in microseconds for the profiled code block. If no profiling has been done, it returns 0.0.
`reset()`: Resets the accumulated time and count, allowing for a new profiling session.
