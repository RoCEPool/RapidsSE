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

  struct __STRUCT_FROM__* p__STRUCT_FROM__;
  RowInfo __ROW_INFO_FROM__;
  RowInfo __ROW_INFO_TO__;

  char* szStrArea;
  i8 nAreaRemaining;

  __L_LABEL_OUTER_LOOP__:

  // marker-start
  struct __STRUCT_TO__* p__STRUCT_TO__;

  __L_LABEL__:

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // FROM
  p__STRUCT_FROM__ = (struct __STRUCT_FROM__*)__ROW_INFO_FROM__.m_aRowCellArray;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // TO
  p__STRUCT_TO__ = (struct __STRUCT_TO__*)__ROW_INFO_TO__.m_aRowCellArray;
  //__ROW_INFO_TO__.m_acStrArea = __ROW_INFO_TO__.m_acStrAreaSav;
  szStrArea = __ROW_INFO_TO__.m_acStrArea;
  nAreaRemaining = __ROW_INFO_TO__.m_nStringAreaSize;

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

  // marker-start
  if ( aDynamicT->m_xRowInfoAT.m_bInitialize ) {
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

    aDynamicT->m_xRowInfoAT.m_bInitialize = false;
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
