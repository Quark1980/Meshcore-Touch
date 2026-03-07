/**
 * @file lv_conf.h
 * Configuration file for v9.2.2
 */
#pragma once
#include <stdint.h>

#define LV_COLOR_DEPTH          16
#define LV_USE_OS               LV_OS_FREERTOS // FreeRTOS support on ESP32

/*=========================
 *   MEMORY SETTINGS
 *=========================*/
/*1: use custom malloc/free, 0: use the built-in `lv_malloc()` and `lv_free()`*/
// Let LVGL use its own malloc or the stdlib
#define LV_USE_STDLIB_MALLOC    LV_STDLIB_BUILTIN

/*Size of the memory available for `lv_malloc()` in bytes (>= 2kB)*/
#define LV_MEM_SIZE             (256 * 1024U)

/*====================
 *  HAL SETTINGS
 *====================*/
/*Default display refresh, input device read and animation step period.*/
#define LV_DEF_REFR_PERIOD      10 /*[ms]*/

/*=======================
 * FEATURE CONFIGURATION
 *=======================*/
/*-------------
 * Drawing
 *-----------*/
#define LV_USE_DRAW_SW          1
#define LV_DRAW_SW_COMPLEX      1

// Double Buffering & PSRAM settings
#define LV_USE_DRAW_SW_ASM      LV_DRAW_SW_ASM_NONE
// (PSRAM buffer allocation will be handled manually in main.cpp for `lv_display_set_buffers`)

/*-------------
 * GPU
 *-----------*/
// DMA is handled within TFT_eSPI pushing
#define LV_USE_GPU_STM32_DMA2D  0
#define LV_USE_GPU_SWM341_DMA2D 0

/*-------------
 * Font usage
 *-----------*/
#define LV_FONT_MONTSERRAT_14   1
#define LV_FONT_DEFAULT         &lv_font_montserrat_14

/*=====================
 *  COMPILER SETTINGS
 *====================*/
#define LV_ATTRIBUTE_TICK_INC
#define LV_ATTRIBUTE_TIMER_HANDLER
#define LV_ATTRIBUTE_FLUSH_READY
#define LV_USE_LOG   1
#define LV_LOG_LEVEL LV_LOG_LEVEL_WARN
