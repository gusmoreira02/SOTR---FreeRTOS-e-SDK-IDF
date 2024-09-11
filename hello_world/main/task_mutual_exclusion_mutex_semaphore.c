/*
Arquivo: task_mutual_exclusion.c
Autor: Felipe Viel
Função do arquivo: Cria tasks para printar e utilização exclusão mútua. Baseado no exempo do ESP-IDF
Criado em 17 de novembro de 2020
Modificado em 17 de novembro de 2020
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "freertos/semphr.h"

SemaphoreHandle_t mutual_exclusion_mutex;

void print_name(const char *task_name){
    // Tentar pegar o mutex antes de executar a seção crítica
    if (xSemaphoreTake(mutual_exclusion_mutex, portMAX_DELAY) == pdTRUE) {
        printf("%s\n", task_name);  // Imprimir o nome da tarefa
        xSemaphoreGive(mutual_exclusion_mutex);  // Liberar o mutex
    }
} 

void hello_task2(void *pvParameter){
    const char *my_name = "\nI am Task 2";
    while(1){
        print_name(my_name);  // Chamar a função de impressão
        vTaskDelay(1000 / portTICK_PERIOD_MS);  // Delay de 1 segundo
    }
}
 
void hello_task1(void *pvParameter){
    const char *my_name = "\nI am Task 1";
    while(1){
        print_name(my_name);  // Chamar a função de impressão
        vTaskDelay(1000 / portTICK_PERIOD_MS);  // Delay de 1 segundo
    }
}

void app_main(){
    // Inicializar o NVS (Non-Volatile Storage)
    nvs_flash_init();

    // Criar o mutex para exclusão mútua
    mutual_exclusion_mutex = xSemaphoreCreateMutex();
    if (mutual_exclusion_mutex != NULL) {
       // O mutex foi criado com sucesso
       printf("Mutex was created\n");
    }

    // Criar as tasks e fixá-las em diferentes núcleos
    xTaskCreatePinnedToCore(&hello_task1, "hello_task1", 2048, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(&hello_task2, "hello_task2", 2048, NULL, 1, NULL, 1);

    // A função app_main não deve usar print_name, pois é chamada fora do contexto de tarefa
}
