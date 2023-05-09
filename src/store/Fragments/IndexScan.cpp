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

  // marker-end

  L_Return:
  return;

}

/*----------------------------------------------------------------------------*/
/*-- vIndexPositionBegin -----------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexPositionBegin
void vIndexPositionBegin(Dynamic_T* aDynamicT) {

  // marker-start

#ifdef WITH_TINYC
  // position at beginning of the index
  aDynamicT->__INDEX_INFO__.m_aEntInt =(struct Ent_int*)aIndexTree_begin(
        aDynamicT->__INDEX_INFO__.m_aIndexTree, &aDynamicT->__INDEX_INFO__.m_aEntBuffer);
#endif

  if ( ! aDynamicT->m_xIndexInfoT.m_aEntInt )
    goto L_Return;

  // marker-end

  L_Return:
  return;

}


/*----------------------------------------------------------------------------*/
/*-- vIndexPositionLower -----------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexPositionLower
void vIndexPositionLower(Dynamic_T* aDynamicT) {

  // marker-start

#ifdef WITH_TINYC
  // position at lower bound
  aDynamicT->__INDEX_INFO__.m_aEntInt =  (struct Ent_int *)__INDEX_POSITION_LOWER__(
      aDynamicT->__INDEX_INFO__.m_aIndexTree, &aDynamicT->__INDEX_INFO__.m_aEntBuffer, aDynamicT->__INDEX_INFO__.m_aEntKey);
#endif

  if ( ! aDynamicT->__INDEX_INFO__.m_aEntInt )
    goto L_Return;

  // marker-end

  L_Return:
  return;

}

/*----------------------------------------------------------------------------*/
/*-- vIndexPositionUpper -----------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexPositionUpper
void vIndexPositionUpper(Dynamic_T* aDynamicT) {

  // marker-start

#ifdef WITH_TINYC
  // position at upper bound
  aDynamicT->__INDEX_INFO__.m_aEntUpper =  (struct Ent_int *)__INDEX_POSITION_UPPER__(
      aDynamicT->__INDEX_INFO__.m_aIndexTree, aDynamicT->__INDEX_INFO__.m_aEntKey);
#endif

  // reached upper bound ?
  if (aDynamicT->__INDEX_INFO__.m_aEntInt == aDynamicT->__INDEX_INFO__.m_aEntUpper) {
    goto L_Return;
  }

  // process the row
  aDynamicT->m_iProcessRow = 1;

  // marker-end

  L_Return:
  return;

}

/*----------------------------------------------------------------------------*/
/*-- vIndexCheckUpper --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexCheckUpper
void vIndexCheckUpper(Dynamic_T* aDynamicT) {

  // marker-start

  L_IndexScanDeref:

  // reached upper bound ?
  if (aDynamicT->__INDEX_INFO__.m_aEntInt == aDynamicT->__INDEX_INFO__.m_aEntUpper) {
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
  aDynamicT->__INDEX_INFO__.m_aEntInt = (struct Ent_int*)aEntBuffer_nextEntry(
      &aDynamicT->__INDEX_INFO__.m_aEntBuffer, aDynamicT->__INDEX_INFO__.m_aEntInt);
  goto L_IndexScanDeref;

  // marker-end

  __L_LABEL_NEXT__:
  L_IndexScanDeref:

  return;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
