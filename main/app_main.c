#include <stdio.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "mqtt_client.h"
#include "driver/gpio.h"
#include <inttypes.h>
#include "esp_crt_bundle.h"

static const char *TAG = "SIMPLE_MQTT";

#define GPIO_BUTTON 33
#define GPIO_LED 26

#define TOPIC_SUB "esp32/led_input"
#define TOPIC_PUB "esp32/button_output"

esp_mqtt_client_handle_t client;
int is_led_on = 0;

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;

    if (event_id == MQTT_EVENT_CONNECTED)
    {
        ESP_LOGI(TAG, "Connected to Broker!");
        esp_mqtt_client_subscribe(client, TOPIC_SUB, 0);
    }
    else if (event_id == MQTT_EVENT_DATA)
    {
        if (strncmp(event->data, "1", event->data_len) == 0)
        {
            gpio_set_level(GPIO_LED, 1);
        }
        else
        {
            gpio_set_level(GPIO_LED, 0);
        }
    }
}

void app_main(void)
{
    nvs_flash_init();
    esp_netif_init();
    esp_event_loop_create_default();
    example_connect();

    gpio_reset_pin(GPIO_LED);
    gpio_set_direction(GPIO_LED, GPIO_MODE_OUTPUT);

    gpio_reset_pin(GPIO_BUTTON);
    gpio_set_direction(GPIO_BUTTON, GPIO_MODE_INPUT);
    gpio_set_pull_mode(GPIO_BUTTON, GPIO_PULLUP_ONLY);

    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = CONFIG_BROKER_URL,
        .broker.verification.crt_bundle_attach = esp_crt_bundle_attach,
        .credentials.username = "pedrohugo",
        .credentials.authentication.password = "Pe12345678",
        .session.protocol_ver = MQTT_PROTOCOL_V_3_1_1,
    };
    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);

    int last_button_state = 1;

    while (1)
    {
        int current_button_state = gpio_get_level(GPIO_BUTTON);

        if (current_button_state != last_button_state)
        {

            vTaskDelay(50 / portTICK_PERIOD_MS);

            current_button_state = gpio_get_level(GPIO_BUTTON);

            if (current_button_state == 0)
            {
                esp_mqtt_client_publish(client, TOPIC_PUB, "1", 0, 1, 0);
                ESP_LOGI(TAG, "Sent: 1");
            }
            else
            {
                esp_mqtt_client_publish(client, TOPIC_PUB, "0", 0, 1, 0);
                ESP_LOGI(TAG, "Sent: 0");
            }

            last_button_state = current_button_state;
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}