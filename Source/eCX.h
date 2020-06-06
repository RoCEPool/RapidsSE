/*============================================================================*/
/* eCX.h -- Column Pix Cell Value Kind                                        */
/*============================================================================*/
#pragma once

enum eCX // column pic indicating what the cell contains
{
  /* 0 */ CX_USER=0,  // user defined column
  /* 1 */ CX_HASH,    // hash of partition columns
  /* 2 */ CX_IDXREF,  // index refence
  /* 3 */ CX_ROWID,   // row id
  /* 4 */ CX_ROWNO,   // row number
};

static Stz* aCX[]
{
  /* 0 */ "CX_USER"_Stz,
  /* 1 */ "CX_HASH"_Stz,
  /* 2 */ "CX_IDXREF"_Stz,
  /* 3 */ "CX_ROWID"_Stz,
  /* 4 */ "CX_ROWNO"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
