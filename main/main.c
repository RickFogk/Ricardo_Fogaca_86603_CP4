#include <stdio.h>
#include <stdlib.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "esp_mac.h"

// Declaração dos semáforos binários para sincronização
SemaphoreHandle_t xSemaforo1 = NULL;
SemaphoreHandle_t xSemaforo2 = NULL;
SemaphoreHandle_t xSemaforo3 = NULL;

// Nome do aluno - altere para seu nome
#define NOME_ALUNO "Ricardo Fogaca"

// Tarefa 1 - Primeira a executar
void Tarefa1(void *pvParameters)
{
    while (1) 
    {
        // Aguarda o semáforo 1 (na primeira vez já está liberado)
        if(xSemaphoreTake(xSemaforo1, portMAX_DELAY) == pdTRUE) 
        {
            // Executa a tarefa
            printf("[Tarefa 1] Executou - %s\n", NOME_ALUNO);
            
            // Aguarda 1 segundo
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            
            // Libera o semáforo 2 para a próxima tarefa
            xSemaphoreGive(xSemaforo2);
        }
    }
}

// Tarefa 2 - Segunda a executar
void Tarefa2(void *pvParameters)
{
    while (1) 
    {
        // Aguarda o semáforo 2 ser liberado pela Tarefa 1
        if(xSemaphoreTake(xSemaforo2, portMAX_DELAY) == pdTRUE) 
        {
            // Executa a tarefa
            printf("[Tarefa 2] Executou - %s\n", NOME_ALUNO);
            
            // Aguarda 1 segundo
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            
            // Libera o semáforo 3 para a próxima tarefa
            xSemaphoreGive(xSemaforo3);
        }
    }
}

// Tarefa 3 - Terceira a executar
void Tarefa3(void *pvParameters)
{
    while (1) 
    {
        // Aguarda o semáforo 3 ser liberado pela Tarefa 2
        if(xSemaphoreTake(xSemaforo3, portMAX_DELAY) == pdTRUE) 
        {
            // Executa a tarefa
            printf("[Tarefa 3] Executou - %s\n", NOME_ALUNO);
            
            // Aguarda 1 segundo
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            
            // Libera o semáforo 1 para reiniciar o ciclo
            xSemaphoreGive(xSemaforo1);
        }
    }
}

void app_main(void)
{
    printf("===========================================\n");
    printf("CP 1 - Semaforos com FreeRTOS\n");
    printf("Aluno: %s\n", NOME_ALUNO);  // ← CORREÇÃO AQUI!
    printf("===========================================\n\n");
    
    // Criação dos semáforos binários
    xSemaforo1 = xSemaphoreCreateBinary();
    xSemaforo2 = xSemaphoreCreateBinary();
    xSemaforo3 = xSemaphoreCreateBinary();
    
    // Verifica se os semáforos foram criados com sucesso
    if (xSemaforo1 == NULL || xSemaforo2 == NULL || xSemaforo3 == NULL) 
    {
        printf("Erro: Falha ao criar os semaforos!\n");
        return;
    }
    
    // Libera apenas o primeiro semáforo para iniciar o ciclo
    xSemaphoreGive(xSemaforo1);
    
    // Criação da Tarefa 1
    xTaskCreate(
        Tarefa1,                // Função da tarefa
        "Tarefa_1",            // Nome da tarefa
        2048,                  // Tamanho da pilha
        NULL,                  // Parâmetros
        1,                     // Prioridade
        NULL                   // Handle
    );
    
    // Criação da Tarefa 2
    xTaskCreate(
        Tarefa2,                // Função da tarefa
        "Tarefa_2",            // Nome da tarefa
        2048,                  // Tamanho da pilha
        NULL,                  // Parâmetros
        1,                     // Prioridade
        NULL                   // Handle
    );
    
    // Criação da Tarefa 3
    xTaskCreate(
        Tarefa3,                // Função da tarefa
        "Tarefa_3",            // Nome da tarefa
        2048,                  // Tamanho da pilha
        NULL,                  // Parâmetros
        1,                     // Prioridade
        NULL                   // Handle
    );
    
    printf("Sistema iniciado! As tarefas executarao intercaladamente...\n\n");
}