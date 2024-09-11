#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

// Definição do mutex e da variável compartilhada
SemaphoreHandle_t mutex;
int items_passed = 0;

void conveyor_task(void *pvParameter) {
    const char *task_name = (const char *) pvParameter;
    while (1) {
        // Tentar pegar o mutex
        if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
            items_passed++;
            printf("%s: 1 item passou. Total: %d\n", task_name, items_passed);
            
            // Liberar o mutex
            xSemaphoreGive(mutex);

            // Verificar se 20 itens passaram pelas esteiras
            if (items_passed >= 20) {
                printf("20 itens passaram nas esteiras!\n");
                // Resetar o contador de itens
                items_passed = 0;
            }
        }

        // Simular o delay de processamento da esteira
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main() {
    // Inicializar o mutex
    mutex = xSemaphoreCreateMutex();

    if (mutex != NULL) {
        printf("Mutex criado com sucesso!\n");

        // Criar as três tarefas das esteiras
        xTaskCreate(&conveyor_task, "Esteira 1", 2048, "Esteira 1", 1, NULL);
        xTaskCreate(&conveyor_task, "Esteira 2", 2048, "Esteira 2", 1, NULL);
        xTaskCreate(&conveyor_task, "Esteira 3", 2048, "Esteira 3", 1, NULL);
    }
}
