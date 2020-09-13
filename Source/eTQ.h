/*============================================================================*/
/* eTQ.h -- Transform Query Code                                              */
/*============================================================================*/
#pragma once

enum eTQ : u1 // query code
{
  /* */ TQ_NONE=0,
  
  /* */ TQ_FACT_NAME, // push name of top fact
  
  /* */ TQ_FIND_SUBA, // find and push SubA onto dstk
  /* */ TQ_FIND_SUBB, // find and push SubB onto dstk

  /* */ TQ_IS_SUBCLASS_OF,
  
  /* */ TQ_POP_SUBA,  // pop m_pSubA from dstk
  /* */ TQ_POP_SUBB,  // pop m_pSubB from dstk
  
  /* */ TQ_PUSH_SELF, // push m_pSelf onto dstk
  /* */ TQ_PUSH_SUBA, // push m_pSubA onto dstk
  /* */ TQ_PUSH_SUBB, // push m_pSubB onto dstk

  /* */ TQ_CUT,       // cut fact
  /* */ TQ_DELETE,    // delete
  /* */ TQ_PASTE,     // paste
  /* */ TQ_SWAP,      // swap

  /* */ TQ_RETURN,
  
  /* */ TQ_ENUMS,
};

static Stz* aTQ[]
{
  /* */ "TQ_NONE"_Stz,
  
  /* */ "TQ_FACT_NAME"_Stz,

  /* */ "TQ_FIND_SUBA"_Stz,
  /* */ "TQ_FIND_SUBB"_Stz,

  /* */ "TQ_IS_SUBCLASS_OF"_Stz,

  /* */ "TQ_POP_SUBA"_Stz,
  /* */ "TQ_POP_SUBB"_Stz,

  /* */ "TQ_PUSH_SELF"_Stz,
  /* */ "TQ_PUSH_SUBA"_Stz,
  /* */ "TQ_PUSH_SUBB"_Stz,

  /* */ "TQ_CUT"_Stz,
  /* */ "TQ_DELETE"_Stz,
  /* */ "TQ_PASTE"_Stz,
  /* */ "TQ_SWAP"_Stz,

  /* */ "TQ_RETURN"_Stz,
  
  /* */ "TQ_ENUMS"_Stz,
};

//----------------------------------------------------------------------------

struct TqPrm
{
  eTQ m_eParameter;
  i8  m_nArgBytes;
};

/*----------------------------------------------------------------------------*/
/* Description of TQ Prms                                                     */
/*----------------------------------------------------------------------------*/
static TqPrm TqPrms[]
{
  { TQ_NONE,    0 },  // TQ_NONE
  
  { TQ_NONE,    0 },  // TQ_FACT_NAME

  { TQ_NONE,    0 },  // TQ_FIND_SUBA
  { TQ_NONE,    0 },  // TQ_FIND_SUBB

  { TQ_NONE,    0 },  // TQ_IS_SUBCLASS_OF
  
  { TQ_NONE,    0 },  // TQ_POP_SUBA
  { TQ_NONE,    0 },  // TQ_POP_SUBB

  { TQ_NONE,    0 },  // TQ_PUSH_SELF
  { TQ_NONE,    0 },  // TQ_PUSH_SUBA
  { TQ_NONE,    0 },  // TQ_PUSH_SUBB

  { TQ_NONE,    0 },  // TQ_CUT
  { TQ_NONE,    0 },  // TQ_DELETE
  { TQ_NONE,    0 },  // TQ_PASTE
  { TQ_NONE,    0 },  // TQ_SWAP

  { TQ_NONE,    0 },  // TQ_RETURN

  // -- end --
  { TQ_NONE,    0 },  // TO_ENUMS
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
