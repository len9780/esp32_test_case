#include "leak.h"
#include <stdio.h>
#include <stdlib.h>
#include "esp_heap_caps.h"

void leak1() {
  printf("leak1\n");
  heap_caps_malloc(50, 50);
  heap_caps_malloc(50, 50);
  heap_caps_malloc(50, 50);
}
void leak2() {
  printf("leak2\n");
  heap_caps_malloc(50, 50);
  heap_caps_malloc(50, 50);
  heap_caps_malloc(50, 50);
}
