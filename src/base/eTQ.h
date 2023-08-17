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

  /* */ TQ_SCHEDULE,  // schedule

  /* */ TQ_CUT,       // cut fact
  /* */ TQ_DELETE,    // delete
  /* */ TQ_PASTE,     // paste
  /* */ TQ_SWAP,      // swap

  
  /* */ TQ_ACCESS_METHODS,      //
  /* */ TQ_ADD_ASM_HANDLERS,    //
  /* */ TQ_ADD_ASM_SUBROUTINES, //
  /* */ TQ_ADD_OPERATOR,        //
  /* */ TQ_ADD_OUTPUT_STAGE,    //
  /* */ TQ_ADD_PIPELINE,        //
  /* */ TQ_ADD_START_STAGE,     //
  /* */ TQ_FROM_RESOLUTION,     //
  /* */ TQ_IMPORT_TABLES,       //
  /* */ TQ_INITIAL_PLAN,        //
  /* */ TQ_OUTPUT_STREAMDEF,    //
  /* */ TQ_PROJECT_RESOLUTION,  //
  /* */ TQ_WRITE_EOASM,         //
  /* */ TQ_WRITE_PIPELINE,      //

  

  /* */ TQ_RETURN,
  
  /* */ TQ_ENUMS,
};

extern Stz* aTQ[TQ_ENUMS];

//----------------------------------------------------------------------------

struct TqPrm
{
  eTQ m_eParameter;
  i8  m_nArgBytes;
};

/*----------------------------------------------------------------------------*/
/* Description of TQ Prms                                                     */
/*----------------------------------------------------------------------------*/
extern TqPrm TqPrms[TQ_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
