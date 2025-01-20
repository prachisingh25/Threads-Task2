#include <STM32FreeRTOS.h>

#define DEBUG_SERIAL Serial
#define DEBUG_PRINT(x) DEBUG_SERIAL.print(x)
#define DEBUG_PRINTLN(x) DEBUG_SERIAL.println(x)

enum Semaphore {
    PING,
    PONG
};

struct SemaphoreData {
    SemaphoreHandle_t mutex;
    Semaphore value;
};

SemaphoreData semaphore;
SemaphoreHandle_t printMutex;

void printWithTag(const char* tag, const char* message, const char* value = nullptr) {
    if (xSemaphoreTake(printMutex, portMAX_DELAY) == pdTRUE) {
        DEBUG_PRINT("[");
        DEBUG_PRINT(tag);
        DEBUG_PRINT(" ");
        DEBUG_PRINT(xTaskGetTickCount());
        DEBUG_PRINT("ms]: ");
        DEBUG_PRINT(message);
        if (value != nullptr) {
            DEBUG_PRINT(" ");
            DEBUG_PRINT(value);
        }
        DEBUG_PRINTLN("");
        xSemaphoreGive(printMutex);
    }
}

void printSemaphore(const char* tag) {
    if (xSemaphoreTake(printMutex, portMAX_DELAY) == pdTRUE) {
        DEBUG_PRINT("[");
        DEBUG_PRINT(tag);
        DEBUG_PRINT(" ");
        DEBUG_PRINT(xTaskGetTickCount());
        DEBUG_PRINT("ms]: Semaphore State: ");
        DEBUG_PRINTLN(semaphore.value == PING ? "ping" : "pong");
        xSemaphoreGive(printMutex);
    }
}

void thread1(void *pvParameters) {
    while (1) {
        printWithTag("T1", "Trying to acquire mutex");
        
        if (xSemaphoreTake(semaphore.mutex, portMAX_DELAY) == pdTRUE) {
            printWithTag("T1", "Acquired mutex");
            printWithTag("T1", "Semaphore Initial Value:", semaphore.value == PING ? "ping" : "pong");
            
            if (semaphore.value == PING) {
                semaphore.value = PONG;
                printWithTag("T1", "Changed to pong");
            } else {
                printWithTag("T1", "Current value: pong");
            }
            
            printSemaphore("T1");
            xSemaphoreGive(semaphore.mutex);
            printWithTag("T1", "Released mutex");
        }
        
        DEBUG_PRINT("[");
        DEBUG_PRINT(xTaskGetTickCount());
        DEBUG_PRINT("ms]:");
        DEBUG_PRINTLN("Thread 1 waits for 1 second");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void thread2(void *pvParameters) {
    while (1) {
        printWithTag("T2", "Trying to acquire mutex");
        
        if (xSemaphoreTake(semaphore.mutex, portMAX_DELAY) == pdTRUE) {
            printWithTag("T2", "Acquired mutex");
            printWithTag("T2", "Semaphore Initial Value:", semaphore.value == PING ? "ping" : "pong");
            
            if (semaphore.value == PONG) {
                semaphore.value = PING;
                printWithTag("T2", "Changed to ping");
            } else {
                printWithTag("T2", "Current value: ping");
            }
            
            printSemaphore("T2");
            xSemaphoreGive(semaphore.mutex);
            printWithTag("T2", "Released mutex");
        }
        
        DEBUG_PRINT("[");
        DEBUG_PRINT(xTaskGetTickCount());
        DEBUG_PRINT("ms]:");
        DEBUG_PRINTLN("Thread 2 waits for 1 second");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void setup() {
    DEBUG_SERIAL.begin(115200);
    DEBUG_PRINTLN("Starting the Game!!!\n");

    printMutex = xSemaphoreCreateMutex();
    semaphore.mutex = xSemaphoreCreateMutex();
    
    if (printMutex == NULL || semaphore.mutex == NULL) {
        DEBUG_PRINTLN("Failed to create mutexes");
        while (1);
    }

    semaphore.value = PING;

    xTaskCreate(thread1, "Thread1", 256, NULL, 1, NULL);
    xTaskCreate(thread2, "Thread2", 256, NULL, 1, NULL);

    vTaskStartScheduler();
}

void loop() {
    // FreeRTOS tasks handle everything
}
