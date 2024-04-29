/*============================================================================*/
/* Aggreg.cpp -- Code Fragments for aggregate operations                      */
/*============================================================================*/

#include "BASE.h"
#include "ClangDynamic.h"

/*----------------------------------------------------------------------------*/
/*-- Code Fragments ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- vAggregBegin ------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vAggregBegin
void vAggregBegin(Dynamic_T* aDynamicT) {

  i8 CURRENT_DUP_OP;
  i8 PREV_DUP_OP;

  struct __STRUCT_FROM___PREV_OP* p__STRUCT_FROM___PREV_OP;
  RowInfo __ROW_INFO_FROM__[C_MAX_DUP_OPS];
  RowInfo __ROW_INFO_TO__[C_MAX_DUP_OPS];

  char* szStrArea;

  __L_LABEL_OUTER_LOOP__:

  // marker-start
  struct __STRUCT_TO___CURRENT_OP* p__STRUCT_TO___CURRENT_OP;

  __L_LABEL__:

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // FROM
  p__STRUCT_FROM___PREV_OP = (struct __STRUCT_FROM___PREV_OP*)__ROW_INFO_FROM__[PREV_DUP_OP].m_aRowCellArray;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // TO
  p__STRUCT_TO___CURRENT_OP = (struct __STRUCT_TO___CURRENT_OP*)__ROW_INFO_TO__[CURRENT_DUP_OP].m_aRowCellArray;
  szStrArea = __ROW_INFO_TO__[CURRENT_DUP_OP].m_acStrArea;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Process row ?
  if ( ! aDynamicT->m_bProcessRow ) {
    aDynamicT->m_bProcessRow = true;
    goto __L_LABEL_OUTER_LOOP__;
  }

  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vAggregInitBegin --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vAggregInitBegin
void vAggregInitBegin(Dynamic_T* aDynamicT) {

  i8 CURRENT_DUP_OP;

  // marker-start
  if ( aDynamicT->m_xRowInfoAT[CURRENT_DUP_OP].m_bInitialize ) {
  // marker-end
  }

}

/*----------------------------------------------------------------------------*/
/*-- iAggregInitEnd ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iAggregInitEnd
i8 iAggregInitEnd(Dynamic_T* aDynamicT) {

  {
    i8 CURRENT_DUP_OP;
  // marker-start

    aDynamicT->m_xRowInfoAT[CURRENT_DUP_OP].m_bInitialize = false;
  }

  // marker-end

  return 0;

}

/*----------------------------------------------------------------------------*/
/*-- vAggregColsBegin --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vAggregColsBegin
void vAggregColsBegin(Dynamic_T* aDynamicT) {

  // marker-start

  if (aDynamicT->m_bProcessRow) {
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
