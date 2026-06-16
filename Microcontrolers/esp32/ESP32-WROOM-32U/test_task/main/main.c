#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

#include "esp_system.h"
#include "esp_log.h"

QueueHandle_t queue;
SemaphoreHandle_t semaphore;

static const char *TAG_1 = "TASK 1";
static const char *TAG_2 = "TASK 2";
static const char *TAG_INIT = "TASK INIT";

void task1(void *pvParameters) {
    int value = 0;
    while (1) {
        ESP_LOGI(TAG_1, "Task 1 executando no core %d", xPortGetCoreID());
        xQueueSend(queue, &value, portMAX_DELAY);
        value++;
        ESP_LOGI(TAG_1, "Send: %d", value);
        vTaskDelay(pdMS_TO_TICKS(1000));

        if (xSemaphoreTake(semaphore, portMAX_DELAY)) {
            ESP_LOGI(TAG_1, "Semáforo pega");
        }
    }
}

void task2(void *pvParameters) {
    int received;
    while (1) {
        ESP_LOGI(TAG_2, "Task 2 executando no core %d", xPortGetCoreID());
        if (xQueueReceive(queue, &received, portMAX_DELAY)) {
            ESP_LOGI(TAG_2, "Valor recebido na fila: %d", received);
        }
        vTaskDelay(pdMS_TO_TICKS(2000));
        xSemaphoreGive(semaphore);
    }
}


void app_main(void)
{
    ESP_LOGI(TAG_INIT, "ESP32-WROOM-32U rodando com ESP-IDF");
    ESP_LOGI(TAG_INIT, "Número de cores: %d", portNUM_PROCESSORS);
    queue = xQueueCreate(1, sizeof(int));

    semaphore = xSemaphoreCreateBinary();

    xTaskCreatePinnedToCore(task1, "SEND", 2048, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(task2, "RECEIVE", 2048, NULL, 1, NULL, 1);
}
