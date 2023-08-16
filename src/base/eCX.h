/*============================================================================*/
/* eCX.h -- Column Pix Cell Value Kind                                        */
/*============================================================================*/
#pragma once

enum eCX // column pix indicating what the columncell contains
{
  /* 0 */ CX_USERCOL=0,  // user defined column
  /* 1 */ CX_PTNHASH,    // hash of partition columns
  /* 2 */ CX_IDXREF,     // index refence
  /* 3 */ CX_ROWID,      // row id
  /* 4 */ CX_ROWNO,      // row number
  /* 5 */ CX_ENUMS
};

extern Stz* aCX[CX_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
