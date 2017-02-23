/*============================================================================*/
/* Magic.h - includes for Magic namespace                                     */
/*============================================================================*/
#pragma once

#include "Debugging.h"

static const u8 MAGIC = 0x4F4E434947414D5E; // ^MAGICNO

static const u8 C_SCRIBBLE_PATTERN = 0x21454D54494E4921;
static const u8 C_MEMORY_MARKER0   = 0x0706050403020100;
static const u8 C_MEMORY_MARKER1   = 0x0706050403020101;
static const u8 C_MEMORY_MARKER2   = 0x0706050403020102;
static const u8 C_MEMORY_MARKER3   = 0x0706050403020103;
static const u8 C_MEMORY_MARKER4   = 0x0706050403020104;
static const u8 C_MEMORY_MARKER5   = 0x0706050403020105;
static const u8 C_MEMORY_MARKER6   = 0x0706050403020106;
static const u8 C_MEMORY_MARKER7   = 0x0706050403020107;
static const u8 C_MEMORY_MARKER8   = 0x0706050403020108;
static const u8 C_MEMORY_MARKER9   = 0x0706050403020109;
static const u8 C_MEMORY_MARKERA   = 0x070605040302010a;
static const u8 C_MEMORY_MARKERB   = 0x070605040302010b;
static const u8 C_MEMORY_MARKERC   = 0x070605040302010c;
static const u8 C_MEMORY_MARKERD   = 0x070605040302010d;
static const u8 C_MEMORY_MARKERE   = 0x070605040302010e;
static const u8 C_MEMORY_MARKERF   = 0x070605040302010f;



#include "Footer.hpp"
#include "Object.hpp"
#include "Vsp.hpp"





//-----------------------------------------------------------------------------
namespace Magic
{
  void vCheckAssumptions(void);
}

using namespace Magic;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
