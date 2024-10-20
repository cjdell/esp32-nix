#include <Arduino.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <esp_wifi.h>
#include <esp_event.h>
#include <nvs_flash.h>
#include <esp_http_client.h>

#include "lib.h"

#define LED 12

void hello_task(void *pvParameter);
void blinky(void *pvParameter);
void do_web_request();

void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
    }

    pinMode(LED, OUTPUT);

    // WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wm;

    // reset settings - wipe stored credentials for testing
    // these are stored by the esp library
    // wm.resetSettings();

    // Automatically connect using saved credentials,
    // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
    // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
    // then goes into a blocking loop awaiting configuration and will return success result

    bool res;
    // res = wm.autoConnect(); // auto generated AP name from chipid
    // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    res = wm.autoConnect("AutoConnectAP", "password"); // password protected ap

    if (!res)
    {
        Serial.println("Failed to connect");
        // ESP.restart();
    }
    else
    {
        // if you get here you have connected to the WiFi
        Serial.println("connected...yeey :)");
    }

    do_web_request();

    xTaskCreate(&hello_task, "hello_task", 2048, NULL, 5, NULL);
    xTaskCreate(&blinky, "blinky", 2048, NULL, 5, NULL);

    Serial.println("Setup complete");
}

void loop()
{
    Serial.println(foo());
    // Serial.println("Loop");
    delay(2000);

    // digitalWrite(LED, HIGH);
    // delay(2000);
    // digitalWrite(LED, LOW);
    // delay(2000);
}

void hello_task(void *pvParameter)
{
    while (1)
    {
        printf("Hello world!\n");
        delay(1000);
    }
}

void blinky(void *pvParameter)
{
    while (1)
    {
        digitalWrite(LED, HIGH);
        delay(2000);
        digitalWrite(LED, LOW);
        delay(2000);
    }
}

esp_err_t clientEvent(esp_http_client_event_t *evt)
{
    switch (evt->event_id)
    {
    case HTTP_EVENT_ON_DATA:
        printf("%.*s\n", evt->data_len, (char *)evt->data);
        break;
    default:
        break;
    }
    return ESP_OK;
}

void do_web_request(void)
{
    nvs_flash_init();
    esp_event_loop_create_default();

    esp_http_client_config_t clientConfig = {
        .url = "http://google.com",
        // .method = HTTP_METHOD_POST,
        .event_handler = clientEvent};

    esp_http_client_handle_t client = esp_http_client_init(&clientConfig);
    esp_http_client_perform(client);
    esp_http_client_cleanup(client);
}
