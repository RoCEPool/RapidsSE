/*============================================================================*/
/* IndexScan.cpp -- Code Fragments                                            */
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
/*-- vIndexScanBegin ---------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexScanBegin
void vIndexScanBegin(Combined_T* aCombinedT) {

  i8 i_CurrentRow;
  i8*   paW;

  __L_LABEL_LOOP__:

  // marker-start



  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // INDEXSCAN-OP
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  struct __STRUCT_TO__* p__STRUCT_TO__;
  p__STRUCT_TO__ = (struct __STRUCT_TO__*)aCombinedT->m_xRowInfoT.m_aRowCellArray;

  __L_LABEL__:

  paW = &aCombinedT->m_aW[0];

  i_CurrentRow = -1;

  // position at beginning of the index
  aCombinedT->m_aEntIntT =  (struct Ent_int*)aIndexTree_begin(aCombinedT->m_aIndexTreeT, &aCombinedT->m_aEntBufferT);

  if ( ! aCombinedT->m_aEntIntT )
    goto L_Return;

  // populate EntKeyT with upper bound value
  // marker-end

  L_Return:
  return;

}

/*----------------------------------------------------------------------------*/
/*-- vIndexUpperCheck ------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexUpperCheck
void vIndexUpperCheck(Combined_T* aCombinedT) {

  // marker-start

  /// position at upper bound
  aCombinedT->m_aEntUpperT =  (struct Ent_int *)aIndexTree_positionLT(aCombinedT->m_aIndexTreeT, aCombinedT->m_aEntKeyT);

  L_IndexScanDeref:

  // reached upper bound ?
  if (aCombinedT->m_aEntIntT == aCombinedT->m_aEntUpperT) {
    goto L_Return;
  }

  // process the row
  aCombinedT->m_iProcessRow = 1;

  // marker-end

  L_Return:
  return;

}

/*----------------------------------------------------------------------------*/
/*-- vIndexScanReturn --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexScanReturn
void vIndexScanReturn(Combined_T* aCombinedT) {

  void* aHeadLine;
  void* aHeadLineSav;
  void* aSubReturnLabel;

  // marker-start
  goto L__IndexScanRet;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // INFLATE-CRW-RET
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  L__IndexScanRet:

  aHeadLine = aHeadLineSav;

  goto __L_LABEL_NEXT__;

  __L_LABEL_LOOP__:

  // move to next entry
  aCombinedT->m_aEntIntT = (struct Ent_int*)aEntBuffer_nextEntry(&aCombinedT->m_aEntBufferT, aCombinedT->m_aEntIntT);
  goto L_IndexScanDeref;

  // marker-end

  __L_LABEL_NEXT__:
  L_IndexScanDeref:

  return;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
