
/*----------------------------------------------------------------------------*/
/*-- Function header Fragments -----------------------------------------------*/
/*----------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>

#include "../src/primitives/BaseTypes_general.h"
#include "../src/base/CodeSynthCommon.h"
#include "../src/base/ClangDynamic.h"

// function-__FUNCTION_CRW__

// marker-start

/*--------------------------------------------------------------------*/
/* flattened function -- CRWBUFFER HANDLER                            */
/*--------------------------------------------------------------------*/
int __FUNCTION_CRW__(struct Dynamic_T* aDynamicT) {

  // marker-end

  return 0;

}

// function-vSetColumns
/*--------------------------------------------------------------------*/
/* vSetColumns --                                                     */
/*--------------------------------------------------------------------*/
int vSetColumns(struct Dynamic_T* aDynamicT) {

  i8 __COLUMNS_IN__;

  // marker-start
  aDynamicT->m_nCols = __COLUMNS_IN__;

  // marker-end
  return 0;

}

// function-vCrwContinue
/*--------------------------------------------------------------------*/
/* vCrwContinue --                                                    */
/*--------------------------------------------------------------------*/
void vCrwContinue(struct Dynamic_T* aDynamicT) {

  // marker-start
  #include "../src/store/vFLAT_Parameters.h"
  #include "../src/store/vFLAT_PreliminaryCrw_v2.h"
  #include "../src/store/Fragments/vFLAT_FunctionLocals.h"

  // marker-end

  L_InflateCrwT:
  L_Return:
    return;
}


//function-__FUNCTION_EOS__

// marker-start

/*--------------------------------------------------------------------*/
/* flattened function -- EOS Handler                                  */
/*--------------------------------------------------------------------*/
int __FUNCTION_EOS__(struct Dynamic_T* aDynamicT) {

  #include "../src/store/Fragments/vFLAT_FunctionLocals.h"

  paW = &aDynamicT->m_aW[0];

  // marker-end

  return 0;
}

//function-__FUNCTION_INDEX__

// marker-start

/*--------------------------------------------------------------------*/
/* flattened function -- INDEXBUFFER HANDLER                          */
/*--------------------------------------------------------------------*/
int __FUNCTION_INDEX__(struct Dynamic_T* aDynamicT) {

  // marker-end

  return 0;

}

// function-vIndexContinue
/*--------------------------------------------------------------------*/
/* vIndexContinue --                                                  */
/*--------------------------------------------------------------------*/
void vIndexContinue(struct Dynamic_T* aDynamicT) {

  // marker-start
  #include "../src/store/vFLAT_Parameters.h"
  #include "../src/store/Fragments/vFLAT_FunctionLocals.h"

  // marker-end

}

//function-__FUNCTION_ROW__

// marker-start

/*--------------------------------------------------------------------*/
/* flattened function -- ROWBUFFER HANDLER                            */
/*--------------------------------------------------------------------*/
int __FUNCTION_ROW__(struct Dynamic_T* aDynamicT)
{
  // marker-end

  return 0;
}

// function-vRowIndexContinue
/*--------------------------------------------------------------------*/
/* vRowIndexContinue --                                               */
/*--------------------------------------------------------------------*/
void vRowIndexContinue(struct Dynamic_T* aDynamicT) {

  // marker-start

  #include "../src/store/Fragments/vFLAT_FunctionLocals.h"

  // marker-end

}

// function-vRowContinue
/*--------------------------------------------------------------------*/
/* vRowContinue --                                                    */
/*--------------------------------------------------------------------*/
void vRowContinue(struct Dynamic_T* aDynamicT) {

  // marker-start

  #include "../src/store/vFLAT_Parameters.h"
  #include "../src/store/Fragments/vFLAT_FunctionLocals.h"

  // marker-end
}

// function-__FUNCTION_STREAMDEF__

// marker-start

/*--------------------------------------------------------------------*/
/* flattened function -- STREAMDEF                                    */
/*--------------------------------------------------------------------*/
int __FUNCTION_STREAMDEF__(struct Dynamic_T* aDynamicT) {

  // marker-end

  return 0;
}
