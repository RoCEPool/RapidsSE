/*============================================================================*/
/* eTQ.cpp -- Transform Query Code                                            */
/*============================================================================*/
#include "BASE.h"
#include "eTQ.h"

Stz* aTQ[TQ_ENUMS]
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
  
  /* */ "TQ_SCHEDULE"_Stz,

  /* */ "TQ_CUT"_Stz,
  /* */ "TQ_DELETE"_Stz,
  /* */ "TQ_PASTE"_Stz,
  /* */ "TQ_SWAP"_Stz,

  
  /* */ "TQ_ACCESS_METHODS"_Stz,      //
  /* */ "TQ_ADD_ASM_HANDLERS"_Stz,    //
  /* */ "TQ_ADD_ASM_SUBROUTINES"_Stz, //
  /* */ "TQ_ADD_OPERATOR"_Stz,        //
  /* */ "TQ_ADD_OUTPUT_STAGE"_Stz,    //
  /* */ "TQ_ADD_PIPELINE"_Stz,        //
  /* */ "TQ_ADD_START_STAGE"_Stz,     //
  /* */ "TQ_FROM_RESOLUTION"_Stz,     //
  /* */ "TQ_IMPORT_TABLES"_Stz,       //
  /* */ "TQ_INITIAL_PLAN"_Stz,        //
  /* */ "TQ_OUTPUT_STREAMDEF"_Stz,    //
  /* */ "TQ_PROJECT_RESOLUTION"_Stz,  //
  /* */ "TQ_WRITE_EOASM"_Stz,         //
  /* */ "TQ_WRITE_PIPELINE"_Stz,      //

  /* */ "TQ_RETURN"_Stz,
  
//  /* */ "TQ_ENUMS"_Stz,
};


/*----------------------------------------------------------------------------*/
/* Description of TQ Prms                                                     */
/*----------------------------------------------------------------------------*/
TqPrm TqPrms[TQ_ENUMS]
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
  
  { TQ_NONE,    0 },  // TQ_SCHEDULE

  { TQ_NONE,    0 },  // TQ_CUT
  { TQ_NONE,    0 },  // TQ_DELETE
  { TQ_NONE,    0 },  // TQ_PASTE
  { TQ_NONE,    0 },  // TQ_SWAP

  
  { TQ_NONE,    0 },  // TQ_ACCESS_METHODS
  { TQ_NONE,    0 },  // TQ_ADD_ASM_HANDLERS
  { TQ_NONE,    0 },  // TQ_ADD_ASM_SUBROUTINES
  { TQ_NONE,    0 },  // TQ_ADD_OPERATOR
  { TQ_NONE,    0 },  // TQ_ADD_OUTPUT_STAGE
  { TQ_NONE,    0 },  // TQ_ADD_PIPELINE
  { TQ_NONE,    0 },  // TQ_ADD_START_STAGE
  { TQ_NONE,    0 },  // TQ_FROM_RESOLUTION
  { TQ_NONE,    0 },  // TQ_IMPORT_TABLES
  { TQ_NONE,    0 },  // TQ_INITIAL_PLAN
  { TQ_NONE,    0 },  // TQ_OUTPUT_STREAMDEF
  { TQ_NONE,    0 },  // TQ_PROJECT_RESOLUTION
  { TQ_NONE,    0 },  // TQ_WRITE_EOASM
  { TQ_NONE,    0 },  // TQ_WRITE_PIPELINE
  

  { TQ_NONE,    0 },  // TQ_RETURN

  // -- end --
//  { TQ_NONE,    0 },  // TO_ENUMS
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
