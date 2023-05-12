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

  __L_LABEL_OUTER_LOOP__:

  // marker-start

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // INDEXJOIN-OP
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  __L_LABEL__:

  p__STRUCT_FROM__ = (struct __STRUCT_FROM__ *) aDynamicT->m_xRowInfoIT.m_aRowCellArray;

  struct __STRUCT_TO__ *p__STRUCT_TO__;
  p__STRUCT_TO__ = (struct __STRUCT_TO__ *) aDynamicT->m_xRowInfoTJ.m_aRowCellArray;

  // init indexJoin
  if (aDynamicT->m_iIndexJoinInit == 0) {
    vEntWrk_entKind(aDynamicT->m_xIndexInfoJ.m_aEntWrk, 0, __KEY_KIND__);
    iLeftOuterRow = 0;
    aDynamicT->m_iIndexJoinInit = 1;
  }

  if (!aDynamicT->m_iProcessRow) {
    aDynamicT->m_iProcessRow = 1;
    goto __L_LABEL_OUTER_LOOP__;
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

  __L_LABEL_OUTER_LOOP__:

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
    goto __L_LABEL_OUTER_LOOP__;
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

  __L_LABEL_OUTER_LOOP__:
  __L_LABEL_LOOP__:

  // marker-start

  printf("M1\n");
  // did we find the entry ?
  if ( ! aDynamicT->m_xIndexInfoJ.m_aEntInt ) {
    aDynamicT->m_iProcessRow = 0;
    goto __L_LABEL_OUTER_LOOP__;
  }

  // find the upper bound entry
  aDynamicT->m_xIndexInfoJ.m_aEntUpper = aIndexTree_positionLE(
      aDynamicT->m_xIndexInfoJ.m_aIndexTree, aDynamicT->m_xIndexInfoJ.m_aEntKey);

  __L_LABEL_INDEX_DEREF__:
  aDynamicT->m_iProcessRow = 1;

  printf("M5\n");
  if (aDynamicT->m_xIndexInfoJ.m_aEntInt == aDynamicT->m_xIndexInfoJ.m_aEntUpper) {

    aDynamicT->m_iProcessRow = 0;
    printf("M6\n");
    goto __L_LABEL_OUTER_LOOP__;
  }
  printf("M7\n");
  aHeadLineSav = aHeadLine;

  // dereference the entry
//  iBufferJ = aDynamicT->m_xIndexInfoJ.m_aEntInt->Rrf.m_uBuffer;
//  iItemJ   = aDynamicT->m_xIndexInfoJ.m_aEntInt->Rrf.m_uItem;
//  aCrwJ  = ( (char*)C_ADR_REGION + iBufferJ * C_1MB + C_CRW_FIRSTHEAD+ iItemJ * C_CRW_HEADBYTES );
  printf("M2\n");
  // marker-end

  L_Return:
  return;

}

/*----------------------------------------------------------------------------*/
/*-- vIndexJoinContinueLO0 ---------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexJoinContinueLO0
void vIndexJoinContinueLO0(Dynamic_T* aDynamicT) {

  i8 iLeftOuterRow;
  // marker-start

  printf("M1\n");
  // did we find the entry ?
  if ( ! aDynamicT->m_xIndexInfoJ.m_aEntInt ) {

    // marker-end
  }
}

/*----------------------------------------------------------------------------*/
/*-- vIndexJoinContinueLO1 ---------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexJoinContinueLO1
void vIndexJoinContinueLO1(Dynamic_T* aDynamicT) {

  i8 iLeftOuterRow;
  // marker-start

    // did we join any rows with current left row ?
    if (aDynamicT->m_iJoinedRows == 0) {

      aDynamicT->m_iProcessRow = 1;
      iLeftOuterRow = 1;

      // marker-end

  }
}

/*----------------------------------------------------------------------------*/
/*-- vIndexJoinContinueLO2 ---------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexJoinContinueLO2
void vIndexJoinContinueLO2(Dynamic_T* aDynamicT) {

  __L_LABEL_OUTER_LOOP__:

  {
    if (true) {
      // marker-start
      goto __L_LABEL_NEXT__;
    } else {
      aDynamicT->m_iProcessRow = 0;
      goto __L_LABEL_OUTER_LOOP__;
    }
  }
    // marker-end
  __L_LABEL_NEXT__:
  return;
}

/*----------------------------------------------------------------------------*/
/*-- vIndexJoinContinueLO3 ---------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexJoinContinueLO3
void vIndexJoinContinueLO3(Dynamic_T* aDynamicT) {

  __L_LABEL_OUTER_LOOP__:
  __L_LABEL_LOOP__:

  // marker-start

  // find the upper bound entry
  aDynamicT->m_xIndexInfoJ.m_aEntUpper = aIndexTree_positionLE(
      aDynamicT->m_xIndexInfoJ.m_aIndexTree, aDynamicT->m_xIndexInfoJ.m_aEntKey);

  __L_LABEL_INDEX_DEREF__:
  aDynamicT->m_iProcessRow = 1;

  if (aDynamicT->m_xIndexInfoJ.m_aEntInt == aDynamicT->m_xIndexInfoJ.m_aEntUpper) {

    // marker-end
  }
}

/*----------------------------------------------------------------------------*/
/*-- vIndexJoinContinueLO4 ---------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexJoinContinueLO4
void vIndexJoinContinueLO4(Dynamic_T* aDynamicT) {

  void *aHeadLineSav;
  void *aHeadLine;
  void *aCrwJ;

  i8 iBufferJ;
  i8 iItemJ;
  i8 iLeftOuterRow;

  static const void *C_ADR_REGION = (void *) 0x204800000000;
  static const i8 C_CRW_HEADBYTES = 32;
  static const i8 C_CRW_FIRSTHEAD = 640;

  // marker-start

  //aHeadLineSav = aHeadLine;

  // dereference the entry
  iBufferJ = aDynamicT->m_xIndexInfoJ.m_aEntInt->Rrf.m_uBuffer;
  iItemJ = aDynamicT->m_xIndexInfoJ.m_aEntInt->Rrf.m_uItem;
  aCrwJ = ((char *) C_ADR_REGION + iBufferJ * C_1MB + C_CRW_FIRSTHEAD + iItemJ * C_CRW_HEADBYTES);

  // marker-end

  __L_LABEL_NEXT__:

  L_Return:
  return;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
