/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <inttypes.h>
#include <stdio.h>
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_heap_trace.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "leak.h"
#include "sdkconfig.h"
static heap_trace_record_t
    trace_record[1000];  // This buffer must be in internal RAM
void app_main(void) {
  ESP_ERROR_CHECK(heap_trace_init_standalone(trace_record, 1000));
  ESP_ERROR_CHECK(heap_trace_start(HEAP_TRACE_LEAKS));
  printf("Hello world!\n");
  leak1();
  leak2();
  ESP_ERROR_CHECK(heap_trace_stop());
  heap_trace_dump();

  /* Print chip information */
  esp_chip_info_t chip_info;
  uint32_t flash_size;
  esp_chip_info(&chip_info);
  printf("This is %s chip with %d CPU core(s), %s%s%s%s, ", CONFIG_IDF_TARGET,
         chip_info.cores,
         (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi/" : "",
         (chip_info.features & CHIP_FEATURE_BT) ? "BT" : "",
         (chip_info.features & CHIP_FEATURE_BLE) ? "BLE" : "",
         (chip_info.features & CHIP_FEATURE_IEEE802154)
             ? ", 802.15.4 (Zigbee/Thread)"
             : "");

  unsigned major_rev = chip_info.revision / 100;
  unsigned minor_rev = chip_info.revision % 100;
  printf("silicon revision v%d.%d, ", major_rev, minor_rev);
  if (esp_flash_get_size(NULL, &flash_size) != ESP_OK) {
    printf("Get flash size failed");
    return;
  }

  printf(
      "%" PRIu32 "MB %s flash\n", flash_size / (uint32_t)(1024 * 1024),
      (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

  printf("Minimum free heap size: %" PRIu32 " bytes\n",
         esp_get_minimum_free_heap_size());
  uint32_t cnt = 0;
  while (1) {
    printf("%lu\n", cnt);
    ESP_ERROR_CHECK(heap_trace_start(HEAP_TRACE_LEAKS));
    leak1();
    leak2();
    ESP_ERROR_CHECK(heap_trace_stop());
    heap_trace_dump();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    cnt++;
  }
}
