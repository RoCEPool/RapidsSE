/*============================================================================*/
/* Aggreg.cpp -- Code Fragments for aggregate operations                      */
/*============================================================================*/

#include "BASE.h"
#include "Combined.hpp"
#include "IndexDef.hpp"
#include "TccStuff.h"
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
i8 iAggregBegin(Combined_T* aCombinedT) {

  struct __STRUCT_FROM__* p__STRUCT_FROM__;
  char* szStrArea;

  __L_LABEL_LOOP__:

  // marker-start

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // AGGREG-OP
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  struct __STRUCT_TO__* p__STRUCT_TO__;

  __L_LABEL__:

  p__STRUCT_FROM__ = (struct __STRUCT_FROM__*)aCombinedT->m_xRowInfoT.m_aRowCellArray;

  p__STRUCT_TO__ = (struct __STRUCT_TO__*)aCombinedT->m_aRowCellArrayAgg;
  szStrArea = aCombinedT->m_racStrAreaAgg;

  // marker-end

  return 0;

}

/*----------------------------------------------------------------------------*/
/*-- vAggregInit -------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vAggregInitBegin
void vAggregInitBegin(Combined_T* aCombinedT) {

  // marker-start
  if ( aCombinedT->m_iProjectInit == 0 ) {
  // marker-end
  }

}

/*----------------------------------------------------------------------------*/
/*-- iAggregInitEnd ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iAggregInitEnd
i8 iAggregInitEnd(Combined_T* aCombinedT) {

  {
  // marker-start

    if (aCombinedT->m_iErrorCode != (i8)EC_NONE) {
      return 0;
    }

    aCombinedT->m_iProjectInit = 1;
  }

  // marker-end

  return 0;

}

/*----------------------------------------------------------------------------*/
/*-- vAggregColsBegin --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vAggregColsBegin
i8 vAggregColsBegin(Combined_T* aCombinedT) {

  // marker-start

  if (aCombinedT->m_iProcessRow) {
    aCombinedT->m_iRowsProcessed++;
    // marker-end
  }

}

/*----------------------------------------------------------------------------*/
/*-- iAggregColsEnd ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iAggregColsEnd
i8 iAggregColsEnd(Combined_T* aCombinedT) {

  {
  // marker-start

    if (aCombinedT->m_iErrorCode != (i8) EC_NONE) {
      return 0;
    }

  }
   // marker-end


  return 0;

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
