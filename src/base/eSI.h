/*============================================================================*/
/* eSI.h - String Inflate                                                     */
/*============================================================================*/
#pragma once

#include "CodeSynthCommon.h"

// ALERT: If this enum changes, make the change in LogicCompiler (line 765)
//enum eSI // string inflate
//{
//  /*  0 */ SI_NONE=0,
//  /*  3 */ SI_CELLARRAY_ADR, // put address into given cell (no inflate per se)
//  /*  4 */ SI_CELLARRAY_ANT, // inflate into cell(s), with size before
//  /*  5 */ SI_CELLARRAY_PST, // inflate into cell(s), with size after
//  /*  6 */ SI_KEY,           // inflate into cell(s), with key semantics
//  /*  1 */ SI_STRAREA_ADR,   // inflate into str area, put address into cell
//  /*  2 */ SI_STRAREA_OFS,   // inflate into str area, put offset into cell
//  /*  7 */ SI_ENUMS
//};

extern Stz* aSI[SI_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
