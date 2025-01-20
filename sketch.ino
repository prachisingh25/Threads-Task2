#include "STM32FreeRTOS.h"
#include <string.h>

#define DEBUG_SERIAL Serial
#define DEBUG_PRINT(x) DEBUG_SERIAL.print(x)
#define DEBUG_PRINTLN(x) DEBUG_SERIAL.println(x)

typedef enum {
    PING,
    PONG
} SemaphoreValue;

typedef struct {
    SemaphoreHandle_t mutex;
    SemaphoreValue value;
} SemaphoreData;

static SemaphoreData semaphore;
static SemaphoreHandle_t printMutex;

static const char* value_to_string(SemaphoreValue value) {
    return value == PING ? "ping" : "pong";
}

static void print_timestamp(const char* tag) {
    uint32_t time = xTaskGetTickCount();
    DEBUG_PRINT("[");
    DEBUG_PRINT(time);
    DEBUG_PRINT("ms]: ");
    DEBUG_PRINT(tag);
    DEBUG_PRINT(" ");
    
    
    
}

static void print_with_tag(const char* tag, const char* message, const char* value) {
    if (xSemaphoreTake(printMutex, portMAX_DELAY) == pdTRUE) {
        print_timestamp(tag);
        DEBUG_PRINT(message);
        if (value != NULL) {
            DEBUG_PRINT(" ");
            DEBUG_PRINT(value);
        }
        DEBUG_PRINTLN("");
        xSemaphoreGive(printMutex);
    }
}

static void print_wait_message(const char* thread_num) {
    if (xSemaphoreTake(printMutex, portMAX_DELAY) == pdTRUE) {
        print_timestamp(thread_num);
        DEBUG_PRINT("Waiting for 1 second");
        DEBUG_PRINTLN("");
        xSemaphoreGive(printMutex);
    }
}

static void thread1(void *pvParameters) {
    const char* tag = "Thread-1";
    
    while (1) {
        print_with_tag(tag, "Trying to acquire mutex", NULL);
        
        if (xSemaphoreTake(semaphore.mutex, portMAX_DELAY) == pdTRUE) {
            print_with_tag(tag, "Acquired mutex", NULL);
            print_with_tag(tag, "Initial semaphore value:", value_to_string(semaphore.value));
            
            SemaphoreValue oldValue = semaphore.value;
            if (semaphore.value == PING) {
                semaphore.value = PONG;
            }
            
            
            print_with_tag(tag, "New value:", value_to_string(semaphore.value));
            
            print_with_tag(tag, "Released mutex", NULL);
            xSemaphoreGive(semaphore.mutex);
        }
        
        print_wait_message(tag);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void thread2(void *pvParameters) {
    const char* tag = "Thread-2";
    
    while (1) {
        print_with_tag(tag, "Trying to acquire mutex", NULL);
        
        if (xSemaphoreTake(semaphore.mutex, portMAX_DELAY) == pdTRUE) {
            print_with_tag(tag, "Acquired mutex", NULL);
            print_with_tag(tag, "Initial semaphore value:", value_to_string(semaphore.value));
            
            SemaphoreValue oldValue = semaphore.value;
            if (semaphore.value == PONG) {
                semaphore.value = PING;
            }
            
            
            print_with_tag(tag, "New value:", value_to_string(semaphore.value));
            
            print_with_tag(tag, "Released mutex", NULL);
            xSemaphoreGive(semaphore.mutex);
        }
        
        print_wait_message(tag);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void setup(void) {
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

void loop(void) {
    // FreeRTOS tasks handle everything
}