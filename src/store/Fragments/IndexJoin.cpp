/*============================================================================*/
/* IndexJoin.cpp -- Code Fragments                                            */
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
/*-- vIndexJoinIntKey --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexJoinIntKey
void vIndexJoinIntKey(Dynamic_T* aDynamicT) {

  i8 iLeftOuterRow;
  i8 __KEY_KIND__;

  struct __STRUCT_FROM__* p__STRUCT_FROM__;

  __L_LABEL_LOOP__:

  // marker-start

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // INDEXJOIN-OP
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  //struct __STRUCT_FROM__ *p__STRUCT_FROM__;
  p__STRUCT_FROM__ = (struct __STRUCT_FROM__ *) aDynamicT->m_xRowInfoIT.m_aRowCellArray;

  struct __STRUCT_TO__ *p__STRUCT_TO__;
  p__STRUCT_TO__ = (struct __STRUCT_TO__ *) aDynamicT->m_xRowInfoTJ.m_aRowCellArray;

  __L_LABEL__:

  // init indexJoin
  if (aDynamicT->m_iIndexJoinInit == 0) {
    vEntWrk_entKind(aDynamicT->m_xIndexInfoJ.m_aEntWrk, 0, __KEY_KIND__);
    iLeftOuterRow = 0;
    aDynamicT->m_iIndexJoinInit = 1;
  }

  if (!aDynamicT->m_iProcessRow) {
    aDynamicT->m_iProcessRow = 1;
    goto __L_LABEL_LOOP__;
  }

// marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vIndexJoinGenKey --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexJoinGenKey
void vIndexJoinGenKey(Dynamic_T* aDynamicT) {

  i8 iLeftOuterRow;
  i8 __KEY_KIND__;

  L_AdvanceNextRow:

  // marker-start

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // INDEXJOIN-OP
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  struct __STRUCT_TO__ *p__STRUCT_TO__;
  p__STRUCT_TO__ = (struct __STRUCT_TO__ *) aDynamicT->m_xRowInfoTJ.m_aRowCellArray;

  __L_LABEL__:

  // init indexJoin
  if (aDynamicT->m_iIndexJoinInit == 0) {
    vEntWrk_entKind(aDynamicT->m_xIndexInfoJ.m_aEntWrk, 0, __KEY_KIND__);
    iLeftOuterRow = 0;
    aDynamicT->m_iIndexJoinInit = 1;
  }
  else {
    vEntWrk_reset(aDynamicT->m_xIndexInfoJ.m_aEntWrk);
  }

  if (!aDynamicT->m_iProcessRow) {
    aDynamicT->m_iProcessRow = 1;
    goto L_AdvanceNextRow;
  }
// marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vIndexJoinContinue ------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexJoinContinue
void vIndexJoinContinue(Dynamic_T* aDynamicT) {

  void *aHeadLineSav;
  void *aHeadLine;
  void *aCrwJ;

  i8 iBufferJ;
  i8 iItemJ;

  static const void *C_ADR_REGION = (void *) 0x204800000000;
  static const i8 C_CRW_HEADBYTES = 32;
  static const i8 C_CRW_FIRSTHEAD = 640;

  L_AdvanceNextRow:

  // marker-start

  printf("M1\n");
  // did we find the entry ?
  if ( ! aDynamicT->m_xIndexInfoJ.m_aEntInt ) {
    aDynamicT->m_iProcessRow = 0;
    goto L_AdvanceNextRow;
  }

  // find the upper bound entry
  aDynamicT->m_xIndexInfoJ.m_aEntUpper = aIndexTree_positionLE(
      aDynamicT->m_xIndexInfoJ.m_aIndexTree, aDynamicT->m_xIndexInfoJ.m_aEntKey);

  L_IndexJoinDeref:
  aDynamicT->m_iProcessRow = 1;

  printf("M5\n");
  if (aDynamicT->m_xIndexInfoJ.m_aEntInt == aDynamicT->m_xIndexInfoJ.m_aEntUpper) {

    aDynamicT->m_iProcessRow = 0;
    printf("M6\n");
    goto L_AdvanceNextRow;              //
  }
  printf("M7\n");
  aHeadLineSav = aHeadLine;

  // dereference the entry
  iBufferJ = aDynamicT->m_xIndexInfoJ.m_aEntInt->Rrf.m_uBuffer;
  iItemJ   = aDynamicT->m_xIndexInfoJ.m_aEntInt->Rrf.m_uItem;
  aCrwJ  = ( (char*)C_ADR_REGION + iBufferJ * C_1MB + C_CRW_FIRSTHEAD+ iItemJ * C_CRW_HEADBYTES );
  printf("M2\n");
  // marker-end

  L_Return:
  return;

}

/*----------------------------------------------------------------------------*/
/*-- vIndexJoinReturn --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexJoinReturn
void vIndexJoinReturn(Dynamic_T* aDynamicT) {

  void* aHeadLine;
  void* aHeadLineSav;
  void* aSubReturnLabel;

  // marker-start

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // INFLATE-CRW-RET
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  L__IndexScanRet:
  
  aHeadLine = aHeadLineSav;
  aDynamicT->m_iJoinedRows++;
  goto __L_LABEL_NEXT__;

  __L_LABEL_LOOP__:

   aDynamicT->m_xRowInfoTJ.m_acStrArea = aDynamicT->m_xRowInfoTJ.m_acStrAreaSav;

   // move to next entry
   aDynamicT->m_xIndexInfoJ.m_aEntInt = (struct Ent_int*)aEntBuffer_nextEntry(
       &aDynamicT->m_xIndexInfoJ.m_aEntBuffer, aDynamicT->m_xIndexInfoJ.m_aEntInt);
   goto L_IndexJoinDeref;
   
  // marker-end

  __L_LABEL_NEXT__:
  L_IndexJoinDeref:

  return;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
