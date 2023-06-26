
/*----------------------------------------------------------------------------*/
/*-- Function header Fragments -----------------------------------------------*/
/*----------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>

#include "../src/primitives/BaseTypes_tcc.h"
#include "../src/base/CodeSynthCommon.h"
#include "../src/base/ClangDynamic.h"

// function-__FUNCTION_CRW__

// marker-start

/*--------------------------------------------------------------------*/
/* flattened function -- CRWBUFFER HANDLER                            */
/*--------------------------------------------------------------------*/
int __FUNCTION_CRW__(struct Dynamic_T* aDynamicT) {

  // marker-end

  return 0; // ray-alka

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
  return 0; // ray-alka

}

// function-vCrwContinue
/*--------------------------------------------------------------------*/
/* vCrwContinue --                                                    */
/*--------------------------------------------------------------------*/
int vCrwContinue(struct Dynamic_T* aDynamicT) {

  // marker-start
  #include "../src/store/vFLAT_Parameters_v2.h"
  #include "../src/store/vFLAT_PreliminaryCrw_v2.h"
  #include "../src/store/Fragments/vFLAT_FunctionLocals.h"

  // marker-end

  L_InflateCrwT:
  L_Return:
    return 0;
}


//function-__FUNCTION_EOS__

// marker-start

/*--------------------------------------------------------------------*/
/* flattened function -- EOS Handler                                  */
/*--------------------------------------------------------------------*/
int __FUNCTION_EOS__(struct Dynamic_T* aDynamicT) {

  #include "../src/store/Fragments/vFLAT_FunctionLocals.h"

  // marker-end

  return 0; // ray-alka

}

//function-__FUNCTION_INDEX__

// marker-start

/*--------------------------------------------------------------------*/
/* flattened function -- INDEXBUFFER HANDLER                          */
/*--------------------------------------------------------------------*/
int __FUNCTION_INDEX__(struct Dynamic_T* aDynamicT) {

  // marker-end

  return 0; // ray-alka

}

// function-vIndexContinue
/*--------------------------------------------------------------------*/
/* vIndexContinue --                                                  */
/*--------------------------------------------------------------------*/
int vIndexContinue(struct Dynamic_T* aDynamicT) {

  // marker-start
  #include "../src/store/vFLAT_Parameters_v2.h"
  #include "../src/store/Fragments/vFLAT_FunctionLocals.h"

  // marker-end

  return 0; // ray-alka

}

//function-__FUNCTION_ROW__

// marker-start

/*--------------------------------------------------------------------*/
/* flattened function -- ROWBUFFER HANDLER                            */
/*--------------------------------------------------------------------*/
int __FUNCTION_ROW__(struct Dynamic_T* aDynamicT)
{
  // marker-end

  return 0; // ray-alka
}

// function-vRowIndexContinue
/*--------------------------------------------------------------------*/
/* vRowIndexContinue --                                               */
/*--------------------------------------------------------------------*/
int vRowIndexContinue(struct Dynamic_T* aDynamicT) {

  // marker-start

  #include "../src/store/Fragments/vFLAT_FunctionLocals.h"

  // marker-end

  return 0;
}

// function-vRowContinue
/*--------------------------------------------------------------------*/
/* vRowContinue --                                                    */
/*--------------------------------------------------------------------*/
int vRowContinue(struct Dynamic_T* aDynamicT) {

  // marker-start

  #include "../src/store/vFLAT_Parameters_v2.h"
  #include "../src/store/Fragments/vFLAT_FunctionLocals.h"

  // marker-end

  return 0;
}

// function-__FUNCTION_STREAMDEF__

// marker-start

/*--------------------------------------------------------------------*/
/* flattened function -- STREAMDEF                                    */
/*--------------------------------------------------------------------*/
int __FUNCTION_STREAMDEF__(struct Dynamic_T* aDynamicT) {

  // marker-end

  return 0; // ray-alka

}
