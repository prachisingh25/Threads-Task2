# Ping Pong Semaphore Implementation with STM32 and FreeRTOS

## Overview

This project demonstrates a simple implementation of a semaphore mechanism using FreeRTOS on the STM32 microcontroller. The application alternates between two threads (Thread-1 and Thread-2), which toggle the value of a semaphore between `PING` and `PONG`. The code provides debugging outputs for each critical operation, including timestamps when the mutex is acquired and released.

## Board Used

**Board:** STM32 Nucleo-32 Board (Board-ST-Nucleo-C031C6)

## Features

- **Two Threads (Thread-1 and Thread-2):** Each thread toggles the semaphore value between `PING` and `PONG`. 
  - **Thread-1:** Changes the semaphore from `PING` to `PONG`.
  - **Thread-2:** Changes the semaphore from `PONG` to `PING`.
  
- **Semaphore Management:** Both threads use a mutex to synchronize access to the semaphore and prevent race conditions.
  
- **Timestamps and Debugging:** 
  - The system prints debugging information along with timestamps when the mutex is acquired and released.
  - The timestamp is printed using `xTaskGetTickCount()` to show when each operation occurs in milliseconds.

- **Thread Synchronization:** 
  - Threads use `xSemaphoreTake()` and `xSemaphoreGive()` to acquire and release the mutex, ensuring that the critical section (access to the semaphore value) is properly synchronized.

## Code Walkthrough

- **SemaphoreValue Enum:** Defines two possible values for the semaphore: `PING` and `PONG`.
- **SemaphoreData Struct:** Contains the mutex for synchronizing access to the semaphore and the semaphore value itself.
- **Helper Functions:**
  - `value_to_string`: Converts the `PING`/`PONG` semaphore value to a string for printing.
  - `print_timestamp`: Prints a timestamp for the operations.
  - `print_with_tag`: Prints messages with a timestamp and tag.
  - `print_wait_message`: Prints a message indicating that each thread is waiting for 1 second.
- **Thread Functions:**
  - `thread1`: Toggles the semaphore from `PING` to `PONG`.
  - `thread2`: Toggles the semaphore from `PONG` to `PING`.
- **Setup Function:** Initializes the serial communication, mutexes, and creates two tasks (`thread1` and `thread2`) to run concurrently. The FreeRTOS scheduler is started with `vTaskStartScheduler()`.


Below is an image of the output

![Output](https://github.com/user-attachments/assets/40e6c948-21db-4e95-ad64-4c21a4bd201f)
