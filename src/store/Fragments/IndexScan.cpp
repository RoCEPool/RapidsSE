/*============================================================================*/
/* IndexScan.cpp -- Code Fragments                                            */
/*============================================================================*/

#include "BASE.h"
#include "Dynamic.hpp"
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
void vIndexScanBegin(Dynamic_T* aDynamicT) {

  i8 i_CurrentRow;
  i8*   paW;

  __L_LABEL_LOOP__:

  // marker-start



  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // INDEXSCAN-OP
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  struct __STRUCT_TO__* p__STRUCT_TO__;
  p__STRUCT_TO__ = (struct __STRUCT_TO__*)aDynamicT->m_xRowInfoIT.m_aRowCellArray;

  __L_LABEL__:

  paW = &aDynamicT->m_aW[0];

  i_CurrentRow = -1;

  // position at beginning of the index
  aDynamicT->m_aEntIntT =  (struct Ent_int*)aIndexTree_begin(aDynamicT->m_aIndexTreeT, &aDynamicT->m_aEntBufferT);

  if ( ! aDynamicT->m_aEntIntT )
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
void vIndexUpperCheck(Dynamic_T* aDynamicT) {

  // marker-start

  /// position at upper bound
  aDynamicT->m_aEntUpperT =  (struct Ent_int *)aIndexTree_positionLT(aDynamicT->m_aIndexTreeT, aDynamicT->m_aEntKeyT);

  L_IndexScanDeref:

  // reached upper bound ?
  if (aDynamicT->m_aEntIntT == aDynamicT->m_aEntUpperT) {
    goto L_Return;
  }

  // process the row
  aDynamicT->m_iProcessRow = 1;

  // marker-end

  L_Return:
  return;

}

/*----------------------------------------------------------------------------*/
/*-- vIndexScanReturn --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexScanReturn
void vIndexScanReturn(Dynamic_T* aDynamicT) {

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
  aDynamicT->m_aEntIntT = (struct Ent_int*)aEntBuffer_nextEntry(&aDynamicT->m_aEntBufferT, aDynamicT->m_aEntIntT);
  goto L_IndexScanDeref;

  // marker-end

  __L_LABEL_NEXT__:
  L_IndexScanDeref:

  return;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
