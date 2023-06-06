/*============================================================================*/
/* Aggreg.cpp -- Code Fragments for aggregate operations                      */
/*============================================================================*/

#include "BASE.h"
#include "Combined.hpp"
#include "IndexDef.hpp"
#include "CodeSynthCommon.h"
#include "ClangStuff.h"
#include "ClangDynamic.h"
#include "Pay.hpp"
#include "Key.hpp"
#include "Ent.hpp"
#include "Ent_int.h"

/*----------------------------------------------------------------------------*/
/*-- Code Fragments ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- vAggregBegin -----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iAggregBegin
i8 iAggregBegin(Dynamic_T* aDynamicT) {

  struct __STRUCT_FROM__* p__STRUCT_FROM__;
  RowInfo __ROW_INFO_FROM__;

  char* szStrArea;

  __L_LABEL_LOOP__:

  // marker-start

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // AGGREG-OP
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  struct __STRUCT_TO__* p__STRUCT_TO__;

  __L_LABEL__:

  printf("AGGREG-OP\n");

  p__STRUCT_FROM__ = (struct __STRUCT_FROM__*)__ROW_INFO_FROM__.m_aRowCellArray;

  p__STRUCT_TO__ = (struct __STRUCT_TO__*)aDynamicT->m_xRowInfoAT.m_aRowCellArray;
  szStrArea = aDynamicT->m_xRowInfoAT.m_acStrArea;

  // marker-end

  return 0;

}

/*----------------------------------------------------------------------------*/
/*-- vAggregInit -------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vAggregInitBegin
void vAggregInitBegin(Dynamic_T* aDynamicT) {

  // marker-start
  if ( aDynamicT->m_iAggregInit == 1 ) {
  // marker-end
  }

}

/*----------------------------------------------------------------------------*/
/*-- iAggregInitEnd ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iAggregInitEnd
i8 iAggregInitEnd(Dynamic_T* aDynamicT) {

  {
  // marker-start

    if (aDynamicT->m_iErrorCode != (i8)EC_NONE) {
      return 0;
    }

    aDynamicT->m_iAggregInit = 0;
  }

  // marker-end

  return 0;

}

/*----------------------------------------------------------------------------*/
/*-- vAggregColsBegin --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vAggregColsBegin
i8 vAggregColsBegin(Dynamic_T* aDynamicT) {

  // marker-start

  if (aDynamicT->m_iProcessRow) {
    aDynamicT->m_iRowsProcessed++;
    // marker-end
  }

}

/*----------------------------------------------------------------------------*/
/*-- iAggregColsEnd ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iAggregColsEnd
i8 iAggregColsEnd(Dynamic_T* aDynamicT) {

  {
  // marker-start

    if (aDynamicT->m_iErrorCode != (i8) EC_NONE) {
      return 0;
    }

  }
   // marker-end


  return 0;

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
