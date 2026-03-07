#pragma once

#include <plugin.h>

struct tStoredLine {
    CVector   vecInf;
    CVector   vecSup;
    uint32_t  color1;
    uint32_t  color2;
};

uint32_t DbgLineColour = 0x0000FFFF; // r = 0, g = 0, b = 255, a = 255

static constexpr uint32_t MAX_NUM_STORED_LINES = 1024;

static uint16_t NumScriptDebugLines;
static tStoredLine aStoredLines[MAX_NUM_STORED_LINES];