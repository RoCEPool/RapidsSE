/*============================================================================*/
/* IndexJoin.cpp -- Code Fragments                                            */
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
/*-- vIndexJoinIntKey --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexJoinIntKey
void vIndexJoinIntKey(Combined_T* aCombinedT) {

  i8 iLeftOuterRow;
  i8 __KEY_KIND__;

  __L_LABEL_LOOP__:

  // marker-start

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // INDEXJOIN-OP
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  struct __STRUCT_TO__ *p__STRUCT_TO__;
  p__STRUCT_TO__ = (struct __STRUCT_TO__ *) aCombinedT->m_aRowCellArrayTJ;

  __L_LABEL__:

  // init indexJoin
  if (aCombinedT->m_iIndexJoinInit == 0) {
    vEntWrk_entKind(aCombinedT->m_aEntWrkJ, 0, __KEY_KIND__);
    iLeftOuterRow = 0;
    aCombinedT->m_iIndexJoinInit = 1;
  }

  if (!aCombinedT->m_iProcessRow) {
    aCombinedT->m_iProcessRow = 1;
    goto __L_LABEL_LOOP__;
  }
// marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vIndexJoinGenKey --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexJoinGenKey
void vIndexJoinGenKey(Combined_T* aCombinedT) {

  i8 iLeftOuterRow;
  i8 __KEY_KIND__;

  __L_LABEL_LOOP__:

  // marker-start

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // INDEXJOIN-OP
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  struct __STRUCT_TO__ *p__STRUCT_TO__;
  p__STRUCT_TO__ = (struct __STRUCT_TO__ *) aCombinedT->m_aRowCellArrayTJ;

  __L_LABEL__:

  // init indexJoin
  if (aCombinedT->m_iIndexJoinInit == 0) {
    vEntWrk_entKind(aCombinedT->m_aEntWrkJ, 0, __KEY_KIND__);
    iLeftOuterRow = 0;
    aCombinedT->m_iIndexJoinInit = 1;
  }
  else {
    vEntWrk_reset(aCombinedT->m_aEntWrkJ);
  }

  if (!aCombinedT->m_iProcessRow) {
    aCombinedT->m_iProcessRow = 1;
    goto __L_LABEL_LOOP__;
  }
// marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vIndexJoinPositionLower -------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexJoinPositionLower
void vIndexJoinPositionLower(Combined_T* aCombinedT) {

  // marker-start

  // position at entry >= join key (lbound)
  aCombinedT->m_aEntIntJ = (struct Ent_int *)aIndexTree_positionGE(aCombinedT->m_aIndexTreeJ, &aCombinedT->m_aEntBufferJ, aCombinedT->m_aEntKeyJ);

  // marker-end

  L_Return:
  return;
}

/*----------------------------------------------------------------------------*/
/*-- vIndexJoinPositionBegin -------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexJoinPositionBegin
void vIndexJoinPositionBegin(Combined_T* aCombinedT) {

  // marker-start

  // cross join - position at beginning of the index
  aCombinedT->m_aEntIntJ =  (struct Ent_int*)aIndexTree_begin(aCombinedT->m_aIndexTreeJ, &aCombinedT->m_aEntBufferJ);

  // marker-end

  L_Return:
  return;
}

/*----------------------------------------------------------------------------*/
/*-- vIndexJoinContinue ------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexJoinContinue
void vIndexJoinContinue(Combined_T* aCombinedT) {

  void *aHeadLineSav;
  void *aHeadLine;
  void *aCrwJ;

  i8 iBufferJ;
  i8 iItemJ;

  static const void *C_ADR_REGION = (void *) 0x204800000000;
  static const i8 C_CRW_HEADBYTES = 32;
  static const i8 C_CRW_FIRSTHEAD = 640;

  __L_LABEL_LOOP__:

  // marker-start

  // did we find the entry ?
  if ( ! aCombinedT->m_aEntIntJ ) {
    aCombinedT->m_iProcessRow = 0;
    goto __L_LABEL_LOOP__;
  }

  // find the upper bound entry
  aCombinedT->m_aEntUpperJ = aIndexTree_positionLE(aCombinedT->m_aIndexTreeJ, aCombinedT->m_aEntKeyJ);

  IndexJoinDeref:
  aCombinedT->m_iProcessRow = 1;

  if (aCombinedT->m_aEntIntJ == aCombinedT->m_aEntUpperJ) {

    aCombinedT->m_iProcessRow = 0;
    goto __L_LABEL_LOOP__;
  }

  aHeadLineSav = aHeadLine;

  // dereference the entry
  iBufferJ = aCombinedT->m_aEntIntJ->Rrf.m_uBuffer;
  iItemJ   = aCombinedT->m_aEntIntJ->Rrf.m_uItem;
  aCrwJ  = ( (char*)C_ADR_REGION + iBufferJ * C_1MB+ C_CRW_FIRSTHEAD+ iItemJ * C_CRW_HEADBYTES );

  // marker-end

  L_Return:
  return;

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
