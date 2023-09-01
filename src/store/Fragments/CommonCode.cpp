/*============================================================================*/
/* CommonCode.cpp -- Common Code Fragments                                    */
/*============================================================================*/

#include "BASE.h"
#include "Dynamic.hpp"
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
/*-- vOperatorBegin ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vOperatorBegin
void vOperatorBegin(Dynamic_T* aDynamicT) {

  struct __STRUCT_FROM__* p__STRUCT_FROM__;
  RowInfo __ROW_INFO_FROM__;
  RowInfo __ROW_INFO_TO__;

  char* szStrArea;

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
  __ROW_INFO_TO__.m_acStrArea = __ROW_INFO_TO__.m_acStrAreaSav;
  szStrArea = __ROW_INFO_TO__.m_acStrArea;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Process row ?
  if (!aDynamicT->m_iProcessRow) {
    aDynamicT->m_iProcessRow = 1;
    goto __L_LABEL_OUTER_LOOP__;
  }

  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vOperatorStub -----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vOperatorStub
void vOperatorStub(Dynamic_T* aDynamicT) {

  // marker-start
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // __OPERATOR_NAME__
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // marker-end
  return;
}

/*----------------------------------------------------------------------------*/
/*-- vExprStack---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vExprStack
i8 vExprStack(Dynamic_T* aDynamicT) {

  i8 __EXPRSTACK__;

  // marker-start
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // populate the key
  __EXPRSTACK__;

  // check for error
  if (aDynamicT->m_iErrorCode != (i8)EC_NONE) {
    return 0;
  }

  // marker-end

  return 0; // ray-alka
}

/*----------------------------------------------------------------------------*/
/*-- vExprCheck---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vExprCheck
i8 vExprCheck(Dynamic_T* aDynamicT) {

  // marker-start

  // check for error
  if (aDynamicT->m_iErrorCode != (i8)EC_NONE) {
    return 0;
  }

  // marker-end

  return 0; // ray-alka
}

/*----------------------------------------------------------------------------*/
/*-- vNullCheck---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vNullCheck
i8 vNullCheck(Dynamic_T* aDynamicT) {

  bool bNullResult;
  i8 __NULLSTACK__;

  __L_LABEL_OUTER_LOOP__:

  // marker-start
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // check for null key
  __NULLSTACK__;

  // check null result
  if (bNullResult) {
    aDynamicT->m_iProcessRow = 0;
    goto __L_LABEL_OUTER_LOOP__;
  }

  // marker-end

  return 0; // ray-alka
}

/*----------------------------------------------------------------------------*/
/*-- vCreatePayLoad ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vCreatePayLoad
void vCreatePayLoad(Dynamic_T* aDynamicT) {

  i8 __PAYLOADBYTES__;

    // marker-start
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // create the payload

#ifdef WITH_CLANG
  vExtern_EntPayloadCreate(aDynamicT->m_xIndexInfoPL.m_aEntInt, __PAYLOADBYTES__);
#endif
    // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- iIndexInsertIntKey ------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iIndexInsertIntKey
i8 iIndexInsertIntKey(Dynamic_T* aDynamicT) {

  void* aEntInserted;
  void* aEntFound;
  i8 __PAYLOADBYTES__;

  // marker-start

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // insert key in the payload
#ifdef WITH_CLANG
    //aEntInserted = aExtern_IndexTreeInsertEnt(
    //    aDynamicT->m_xIndexInfoPL.m_aIndexTree, aDynamicT->m_xIndexInfoPL.m_aEntInt, &aDynamicT->m_iErrorCode);
    aEntInserted = __EXTERN_INSERTENT__();
#endif

    // check for error
    if (aDynamicT->m_iErrorCode != (i8)EC_NONE) {
      return 0;
    }

    // did we insert the key ?
    if (aEntInserted) {
      aDynamicT->m_iRowsProcessed++;
      aDynamicT->m_aiPayload = (i8 * )((char *) aEntInserted
                                       + sizeof(struct Ent_int)
                                       + 8);

// marker-end
    }

  return 0;

}

/*----------------------------------------------------------------------------*/
/*-- iIndexInsertVchKey ------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iIndexInsertVchKey
i8 iIndexInsertVchKey(Dynamic_T* aDynamicT) {

  void* aEntInserted;
  void* aEntFound;
  i8 __PAYLOADBYTES__;

  // marker-start

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // insert key in the payload
#ifdef WITH_CLANG
  //aEntInserted = aExtern_IndexTreeInsertEnt(
  //    aDynamicT->m_xIndexInfoPL.m_aIndexTree, aDynamicT->m_xIndexInfoPL.m_aEntInt, &aDynamicT->m_iErrorCode);
  aEntInserted = __EXTERN_INSERTENT__();
#endif

  // check for error
  if (aDynamicT->m_iErrorCode != (i8)EC_NONE) {
    return 0;
  }

  // did we insert the key ?
  if (aEntInserted) {
    aDynamicT->m_iRowsProcessed++;
    aDynamicT->m_aiPayload = (i8 * )((char *) aEntInserted
                                     + sizeof(struct Ent_int)
                                     + ((struct Ent_int *) aEntInserted)->m_iIntElm0
                                     + 8);

// marker-end
  }

  return 0;

}

/*----------------------------------------------------------------------------*/
/*-- vIndexInsertContinue -----------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexInsertContinue
void vIndexInsertContinue(Dynamic_T* aDynamicT) {

    void* aEntInserted;
    void* aEntFound;

  {
    // marker-start

  }

  aEntFound = aEntInserted;
// marker-end

}

/*----------------------------------------------------------------------------*/
/*-- vCloseBraces ------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vCloseBraces
void vCloseBraces(Dynamic_T* aDynamicT) {

  {
      // marker-start

    }
// marker-end

}

/*----------------------------------------------------------------------------*/
/*-- iSortBegin --------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iSortBegin
i8 iSortBegin(Dynamic_T* aDynamicT) {

  struct __STRUCT_FROM__* p__STRUCT_FROM__;
  char* szStrArea;

  __L_LABEL_LOOP__:

  // marker-start
  __L_LABEL__:

  // marker-end

  return 0;

}

/*----------------------------------------------------------------------------*/
/*-- vTubBegin ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iTubBegin
i8 iTubBegin(Dynamic_T* aDynamicT) {

  RowInfo __ROW_INFO_TO__;
  i8*   paW;

  // marker-start
  __L_LABEL__:

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create the RowPix
  if (__ROW_INFO_TO__.m_bInitialize) {

    struct ColumnInfo xColumnInfo[] = {
        // marker-end
    };
  }

  return 0;
}

/*----------------------------------------------------------------------------*/
/*-- vIndexDefBegin ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexDefBegin
i8 vIndexDefBegin(Dynamic_T* aDynamicT) {

  IndexInfo __INDEX_INFO__;

  // marker-start
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create the indexDef and the indexTree

  if (__INDEX_INFO__.m_bInitialize) {

    struct ColumnInfo xColumnInfo[] = {
        // marker-end
    };
  }

  return 0; // ray-alka

}

/*----------------------------------------------------------------------------*/
/*-- vIndexDefContinue -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexDefContinue
i8 vIndexDefContinue(Dynamic_T* aDynamicT) {

  IndexInfo __INDEX_INFO__;

  ColumnInfo xColumnInfo;
  i8 __INDEX_COLUMNS__;

  {
    // marker-start

#ifdef WITH_CLANG
    bool bPayload = true;
    i8 iKeyKind;

    // create the indexDef
    aDynamicT->m_xIndexInfoPL.m_aIndexDef = aExtern_IndexDefNew(__INDEX_COLUMNS__, (void*)&xColumnInfo);

    // create the indexTree
    aDynamicT->m_xIndexInfoPL.m_aIndexTree = aExtern_IndexTreeNew(aDynamicT->m_xIndexInfoPL.m_aIndexDef, &iKeyKind);

    // do we have a payload ?
    if (__COLUMNS_OUT__ == __INDEX_COLUMNS__)
      bPayload = false;

    // set the entry kind
    vExtern_EntWrkEntKind(aDynamicT->m_xIndexInfoPL.m_aEntWrk, bPayload, iKeyKind);

#endif

    // mark IndexInfo as initialized
    __INDEX_INFO__.m_bInitialize = false;
  }

  // marker-end

  return 0; // ray-alka

}

/*----------------------------------------------------------------------------*/
/*-- vTubRowPix --------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vTubRowPix
void vTubRowPix(Dynamic_T* aDynamicT) {

  RowInfo __ROW_INFO_TO__;
  i8 __POT__;

  {
    // marker-start

#ifdef WITH_CLANG
    void* aRowPix = aExtern_RowPixNew(__COLUMNS_OUT__, &xColumnInfo);
    //vExtern_TubRowPix(aDynamicT->m_pTub, aRowPix);
    __EXTERN_ROWPIX__(__TUB_POT__, aRowPix);
#endif
    __ROW_INFO_TO__.m_bInitialize = false;
    aDynamicT->m_bGeneratePot = __POT__;
  }

  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- iTubContinue ------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iTubContinue
i8 iTubContinue(Dynamic_T* aDynamicT) {

  char* szStrArea;
  ColumnInfo xColumnInfo;

  i8 __COLUMNS_OUT__;
  RowInfo __ROW_INFO_TO__;

  __L_LABEL_LOOP__:

  // marker-start

  struct __STRUCT_TO__* p__STRUCT_TO__;

  // process the row ?
  if (!aDynamicT->m_iProcessRow) {
    aDynamicT->m_iProcessRow = 1;
    goto __L_LABEL_LOOP__;
  }

  // allocate a row from the tub
#ifdef WITH_CLANG
  __ROW_INFO_TO__.m_aRowCellArray = (byte*)__EXTERN_ALLOTROW__();
#endif
  // check for error
  if (aDynamicT->m_iErrorCode != (i8)EC_NONE) {
    return 0;
  }

  p__STRUCT_TO__ = (struct __STRUCT_TO__*)__ROW_INFO_TO__.m_aRowCellArray;

  // marker-end

  return 0;
}

/*----------------------------------------------------------------------------*/
/*-- iEosTubBegin ------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iEosTubBegin
i8 iEosTubBegin(Dynamic_T* aDynamicT) {

  struct __STRUCT_TO__ {
    i8 iDummy;
  };

  char* szStrArea;

  __L_LABEL_LOOP__:

  // marker-start

  struct __STRUCT_TO__* p__STRUCT_TO__;
  struct __STRUCT_TO__* p__STRUCT_FROM__;

  p__STRUCT_FROM__ = (struct __STRUCT_TO__*)aDynamicT->m_xRowInfoAT.m_aRowCellArray;
  szStrArea = aDynamicT->m_xRowInfoAT.m_acStrArea;
  if ( aDynamicT->m_xRowInfoAT.m_bInitialize ) {
    // marker-end
  }

  return 0;
}

/*----------------------------------------------------------------------------*/
/*-- iEosTubBody -------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iEosTubBody
i8 iEosTubBody(Dynamic_T* aDynamicT) {

  struct __STRUCT_TO__* p__STRUCT_TO__;
  RowInfo __ROW_INFO_TO__;

  {
    // marker-start

    // check for error
    if (aDynamicT->m_iErrorCode != (i8) EC_NONE) {
      return 0;
    }

    aDynamicT->m_iRowsProcessed = 1;
    aDynamicT->m_xRowInfoAT.m_bInitialize = false;
  }

  // any rows processed ?
  if (!aDynamicT->m_iRowsProcessed)
    goto L_Return;

  // allocate a row from the tub
#ifdef WITH_CLANG
  aDynamicT->m_xRowInfoTO.m_aRowCellArray = (byte*)__EXTERN_ALLOTROW__();
#endif
  if (aDynamicT->m_iErrorCode != (i8) EC_NONE) {
    return 0;
  }

  p__STRUCT_TO__ = (struct __STRUCT_TO__ *) __ROW_INFO_TO__.m_aRowCellArray;

  // marker-end
  L_Return:
  return 0;
}

/*----------------------------------------------------------------------------*/
/*-- iEosPayLoadTub ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iEosPayLoadTub
i8 iEosPayLoadTub(Dynamic_T* aDynamicT) {

  struct __STRUCT_TO__* p__STRUCT_TO__;
  RowInfo __ROW_INFO_TO__;

  // marker-start
    if (!aDynamicT->m_iRowsProcessed)
      goto L_Return;

  // allocate a row from the tub
#ifdef WITH_CLANG
  //__ROW_INFO_TO__.m_aRowCellArray = (byte*)aExtern_TubAllotRow((void*)aDynamicT->m_pTub,&__ROW_INFO_TO__.m_acStrArea,&aDynamicT->m_iErrorCode);
  __ROW_INFO_TO__.m_aRowCellArray = (byte*)__EXTERN_ALLOTROW__();
#endif
    // check for error
    if (aDynamicT->m_iErrorCode != (i8) EC_NONE) {
      return 0;
    }

    p__STRUCT_TO__ = (struct __STRUCT_TO__ *) __ROW_INFO_TO__.m_aRowCellArray;

  // marker-end
  L_Return:
  return 0;
}

// NOTE: con be combined with iEosPayLoadTub

/*----------------------------------------------------------------------------*/
/*-- vEosPayLoadTubEnd -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vEosPayLoadTubEnd
void vEosPayLoadTubEnd(Dynamic_T* aDynamicT) {

  {

  // marker-start
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // move to next entry
#ifdef WITH_CLANG
    aDynamicT->m_xIndexInfoPL.m_aEntInt = (struct Ent_int*)aExtern_EntBufferNextEntry(
        &aDynamicT->m_xIndexInfoPL.m_aEntBuffer, aDynamicT->m_xIndexInfoPL.m_aEntInt);
#endif
  }

  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vEosPLIntKeyTraversal ---------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vEosPLIntKeyTraversal
i8 vEosPLIntKeyTraversal(Dynamic_T* aDynamicT) {

  i8 iElemSize = 0;

  // marker-start

  struct __STRUCT_TO__* p__STRUCT_TO__;
  struct __STRUCT_FROM__* p__STRUCT_FROM__;

  // - - - - - - - - - - - - - -
  // payload traversal
#ifdef WITH_CLANG
  aDynamicT->m_xIndexInfoPL.m_aEntInt = (struct Ent_int*)
      aExtern_IndexTreeBegin(aDynamicT->m_xIndexInfoPL.m_aIndexTree, &aDynamicT->m_xIndexInfoPL.m_aEntBuffer);
#endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // traverse the index
  while (aDynamicT->m_xIndexInfoPL.m_aEntInt) {

    aDynamicT->m_iProcessRow = 1;

    aDynamicT->m_xRowInfoPT.m_acStrArea = aDynamicT->m_xRowInfoPT.m_acStrAreaSav;
    aDynamicT->m_aiPayload = (i8 *) ((char *) aDynamicT->m_xIndexInfoPL.m_aEntInt
                                      + sizeof(struct Ent_int)
                                      + 8);

    iElemSize = 0;

    // marker-end
  }

  return 0; // ray-alka

}

/*----------------------------------------------------------------------------*/
/*-- vEosPLGenKeyTraversal ---------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vEosPLGenKeyTraversal
i8 vEosPLGenKeyTraversal(Dynamic_T* aDynamicT) {

  i8 iElemSize = 0;

  // marker-start

  struct __STRUCT_TO__* p__STRUCT_TO__;
  struct __STRUCT_FROM__* p__STRUCT_FROM__;

  // - - - - - - - - - - - - - -
  // payload traversal
#ifdef WITH_CLANG
  aDynamicT->m_xIndexInfoPL.m_aEntInt = (struct Ent_int*)aExtern_IndexTreeBegin(
      aDynamicT->m_xIndexInfoPL.m_aIndexTree, &aDynamicT->m_xIndexInfoPL.m_aEntBuffer);
#endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // traverse theindex
  while (aDynamicT->m_xIndexInfoPL.m_aEntInt) {

    aDynamicT->m_iProcessRow = 1;

    aDynamicT->m_xRowInfoPT.m_acStrArea = aDynamicT->m_xRowInfoPT.m_acStrAreaSav;
    aDynamicT->m_aiPayload = (i8 *) ((char *) aDynamicT->m_xIndexInfoPL.m_aEntInt
        + sizeof(struct Ent_int)
        + ((struct Ent_int*)aDynamicT->m_xIndexInfoPL.m_aEntInt)->m_iIntElm0
        + 8);

    iElemSize = 0;

    // marker-end
  }

  return 0; // ray-alka

}

/*----------------------------------------------------------------------------*/
/*-- vPayLoadKeyToTub --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vPayLoadKeyToTub
void vPayLoadKeyToTub(Dynamic_T* aDynamicT) {

  i8 iElemSize;

  struct __STRUCT_FROM__ {
    union Int_T __COLUMN_FROM__;
  };

  struct __STRUCT_FROM__* p__STRUCT_FROM__;

  struct __STRUCT_TO__ {
    union Int_T __COLUMN_TO__;
  };

  struct __STRUCT_TO__* p__STRUCT_TO__;

  // marker-start
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // column __COLUMN_NO__

    p__STRUCT_TO__->__COLUMN_TO__.m_iAsI8 = ((struct Ent_int*)aDynamicT->m_xIndexInfoPL.m_aEntInt)->m_iIntElm0 + iElemSize;

  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vPLIntKeyToTub ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vPLIntKeyToTub
void vPLIntKeyToTub(Dynamic_T* aDynamicT) {

  struct __STRUCT_FROM__ {
    union Vch_T __COLUMN_FROM__;
  };

  struct __STRUCT_FROM__* p__STRUCT_FROM__;

  struct __STRUCT_TO__ {
    union Vch_T __COLUMN_TO__;
  };

  struct __STRUCT_TO__* p__STRUCT_TO__;

  i8 iElemSize;

  // marker-start
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // column __COLUMN_NO__

  p__STRUCT_TO__->__COLUMN_TO__.m_iAsI8 = *(i8*)((char*)(&((struct Ent_int*)aDynamicT->m_xIndexInfoPL.m_aEntInt)->m_iIntElm0) + 8 + iElemSize);
  iElemSize += 8;

  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vPLVchKeyToTub ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vPLVchKeyToTub
void vPLVchKeyToTub(Dynamic_T* aDynamicT) {

  struct __STRUCT_FROM__ {
    union Vch_T __COLUMN_FROM__;
  };

  struct __STRUCT_FROM__* p__STRUCT_FROM__;

  struct __STRUCT_TO__ {
    union Vch_T __COLUMN_TO__;
  };

  struct __STRUCT_TO__* p__STRUCT_TO__;

  char* szSrcStz;
  char* szDstStz;

  void* amCellAddress;

  i8 iElemSize;
  i8 nSrcSize;
  i8 iOffset;

  // marker-start
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // column __COLUMN_NO__

  szSrcStz = ((char*)(&(((struct Ent_int*)aDynamicT->m_xIndexInfoPL.m_aEntInt)->m_iIntElm0)) + 8 + iElemSize);

  if (*(i8*)szSrcStz == C_DATUM_NULL) {
    p__STRUCT_TO__->__COLUMN_TO__.m_iAsI8 = C_DATUM_NULL;
    iElemSize += 8;
  }
  else {
    szDstStz = aDynamicT->m_xRowInfoTO.m_acStrArea;

#ifdef WITH_CLANG
    nSrcSize = iExtern_StrLibWordFlipBack(szDstStz, _C_16KB, szSrcStz); // Note: Alka to fix the C_16KB size
//    for (i8 i_Byte = 0; i_Byte < 16; i_Byte++)
//      printf("%c ", szDstStz[i_Byte]);
//    printf("\n");
#endif
    amCellAddress = (m8*) &p__STRUCT_TO__->__COLUMN_TO__.m_iAsI8;

    iOffset = - nDifference(aDynamicT->m_xRowInfoTO.m_acStrArea,amCellAddress);
    p__STRUCT_TO__->__COLUMN_TO__.m_iAsI8 = iOffset;

    // update the tub string size
    i8 iDstLen = strlen(szDstStz) + 1;
    aDynamicT->m_xRowInfoTO.m_acStrArea += iDstLen;
#ifdef WITH_CLANG
    //vExtern_TubRowStrSize(aDynamicT->m_pTub,iDstLen);
    __EXTERN_ROWSTRSIZE__;
#endif

    iElemSize += nSrcSize;
  }
  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vCopyCol -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vCopyCol
void vCopyCol(Dynamic_T* aDynamicT) {

  struct __STRUCT_FROM__ {
    union Int_T __COLUMN_FROM__;
  };

  struct __STRUCT_FROM__* p__STRUCT_FROM__;

  struct __STRUCT_TO__ {
    union Int_T __COLUMN_TO__;
  };

  struct __STRUCT_TO__* p__STRUCT_TO__;

  // marker-start
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // column __COLUMN_NO__

  p__STRUCT_TO__->__COLUMN_TO__.m_iAsI8 = p__STRUCT_FROM__->__COLUMN_FROM__.m_iAsI8;

  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vCopyNulls --------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vCopyNulls
void vCopyNulls(Dynamic_T* aDynamicT) {

  struct __STRUCT_TO__ {
    union Int_T __COLUMN_TO__;
  };

  struct __STRUCT_TO__* p__STRUCT_TO__;

  // marker-start
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // column __COLUMN_NO__

  p__STRUCT_TO__->__COLUMN_TO__.m_iAsI8 = C_DATUM_NULL;

  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vTubVchCol --------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vTubVchCol
void vTubVchCol(Dynamic_T* aDynamicT) {

  struct __STRUCT_FROM__ {
    union Vch_T __COLUMN_FROM__;
  };

  struct __STRUCT_FROM__* p__STRUCT_FROM__;

  struct __STRUCT_TO__ {
    union Vch_T __COLUMN_TO__;
  };

  struct __STRUCT_TO__* p__STRUCT_TO__;

  char* szSrcStz;
  char* szDstStz;

  i8  nSrcSize;
  i8  iOffset;
  m8* amCellAddress;
  char* __STRING_AREA_TO__;
  RowInfo __ROW_INFO_TO__;

  // marker-start
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // column __COLUMN_NO__

  // get the source string
  szSrcStz = (char*)p__STRUCT_FROM__->__COLUMN_FROM__.m_iAsI8;
  if (szSrcStz == (char*)C_DATUM_NULL) {
    // copy the source to the destination
    p__STRUCT_TO__->__COLUMN_TO__.m_iAsI8 = C_DATUM_NULL;
  }
  else {
    nSrcSize = strlen(szSrcStz) + 1;

    // get the destination string
    szDstStz = __ROW_INFO_TO__.m_acStrArea;
    strncpy(szDstStz, szSrcStz, nSrcSize);

    // copy the source to the destination
    amCellAddress = (m8*) &p__STRUCT_TO__->__COLUMN_TO__.m_iAsI8;
    iOffset = - nDifference(__ROW_INFO_TO__.m_acStrArea,amCellAddress);
    p__STRUCT_TO__->__COLUMN_TO__.m_iAsI8 = iOffset;

    // update the tub string size
    __ROW_INFO_TO__.m_acStrArea += nSrcSize;

#ifdef WITH_CLANG
    //vExtern_TubRowStrSize(aDynamicT->m_pTub,nSrcSize);
    __EXTERN_ROWSTRSIZE__;
#endif
  }

  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vCopyVchCol -------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vCopyVchCol
void vCopyVchCol(Dynamic_T* aDynamicT) {

  struct __STRUCT_FROM__ {
    union Vch_T __COLUMN_FROM__;
  };

  struct __STRUCT_FROM__* p__STRUCT_FROM__;

  struct __STRUCT_TO__ {
    union Vch_T __COLUMN_TO__;
  };

  struct __STRUCT_TO__* p__STRUCT_TO__;

  char* szSrcStz;
  char* szDstStz;

  i8  nSrcSize;
  RowInfo __ROW_INFO_TO__;

  // marker-start
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // column __COLUMN_NO__

  // get the source string
  szSrcStz = (char*)p__STRUCT_FROM__->__COLUMN_FROM__.m_iAsI8;
  if (szSrcStz == (char*)C_DATUM_NULL) {
    // copy the source to the destination
    p__STRUCT_TO__->__COLUMN_TO__.m_iAsI8 = C_DATUM_NULL;
  }
  else {
    nSrcSize = strlen(szSrcStz) + 1;

    // get the destination string
    szDstStz = __ROW_INFO_TO__.m_acStrArea;
    strncpy(szDstStz, szSrcStz, nSrcSize);

    // copy the source to the destination
    p__STRUCT_TO__->__COLUMN_TO__.m_iAsI8 = (i8)szDstStz;

    // update the tub string size
    __ROW_INFO_TO__.m_acStrArea += nSrcSize;
  }

  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vTubRowStr --------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vTubRowStr
void vTubRowStr(Dynamic_T* aDynamicT) {

  {
  // marker-start
#ifdef WITH_CLANG
    //vExtern_TubRowStrSize(aDynamicT->m_pTub,nSrcSize);
    __EXTERN_ROWSTRSIZE__;
#endif
  }
  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vPayLoadToTubCol --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vPayLoadToTubCol
void vPayLoadToTubCol(Dynamic_T* aDynamicT) {

  i8 __INDEX_COLUMN__;

  struct __STRUCT_FROM__ {
    union Int_T __COLUMN_FROM__;
  };

  struct __STRUCT_FROM__* p__STRUCT_FROM__;

  struct __STRUCT_TO__ {
    union Int_T __COLUMN_TO__;
  };

  struct __STRUCT_TO__* p__STRUCT_TO__;

  // marker-start
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // column __COLUMN_NO__

    p__STRUCT_TO__->__COLUMN_TO__.m_iAsI8 = aDynamicT->m_aiPayload[__INDEX_COLUMN__];

  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vPayLoadToTubCol --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vPayLoadToTubVchCol
void vPayLoadToTubVchCol(Dynamic_T* aDynamicT) {

  i8 __INDEX_COLUMN__;
  i8 nSrcSize;

  i8 iOffset;

  char* szSrcStz;
  char* szDstStz;
  m8* amCellAddress;

  struct __STRUCT_FROM__ {
    union Vch_T __COLUMN_FROM__;
  };

  struct __STRUCT_FROM__* p__STRUCT_FROM__;

  struct __STRUCT_TO__ {
    union Vch_T __COLUMN_TO__;
  };

  struct __STRUCT_TO__* p__STRUCT_TO__;

   // marker-start
   // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   // column __COLUMN_NO__

   szSrcStz = (char*)(char*)aDynamicT->m_aiPayload[__INDEX_COLUMN__];
   if (szSrcStz == (char*)C_DATUM_NULL) {
     // copy the source to the destination
     p__STRUCT_TO__->__COLUMN_TO__.m_iAsI8 = C_DATUM_NULL;
   }
   else {
     nSrcSize = strlen(szSrcStz) + 1;
     // get the destination string
     szDstStz = aDynamicT->m_xRowInfoTO.m_acStrArea;
     strncpy(szDstStz, szSrcStz, sizeof(aDynamicT->m_xRowInfoTO.m_cStringArea));

     // copy the source to the destination
     amCellAddress = (m8*) &p__STRUCT_TO__->__COLUMN_TO__.m_iAsI8;
     iOffset = - nDifference(aDynamicT->m_xRowInfoTO.m_acStrArea,amCellAddress);
     p__STRUCT_TO__->__COLUMN_TO__.m_iAsI8 = iOffset;

     // update the tub string size
     aDynamicT->m_xRowInfoTO.m_acStrArea += nSrcSize;
#ifdef WITH_CLANG
     //vExtern_TubRowStrSize(aDynamicT->m_pTub,nSrcSize);
     __EXTERN_ROWSTRSIZE__;
#endif
  }

  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vPopulateIntPL ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vPopulateIntPL
void vPopulateIntPL(Dynamic_T* aDynamicT) {

  i8 __INDEX_COLUMN__;
  struct __STRUCT_FROM__ {
    union Int_T __COLUMN_FROM__;
  };

  struct __STRUCT_FROM__* p__STRUCT_FROM__;

  char* szSrcStz;
  char* szDstStz;

  i8 nSrcSize;

  // marker-start
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // column __COLUMN_NO__

  aDynamicT->m_aiPayload[__INDEX_COLUMN__] = p__STRUCT_FROM__->__COLUMN_FROM__.m_iAsI8;

  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vPopulateVchPL ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vPopulateVchPL
void vPopulateVchPL(Dynamic_T* aDynamicT) {

  i8 __INDEX_COLUMN__ = 0;
  struct __STRUCT_FROM__ {
    union Vch_T __COLUMN_FROM__;
  };

  struct __STRUCT_FROM__* p__STRUCT_FROM__;

  char* szSrcStz;
  char* szDstStz;

  i8 nSrcSize;

  // marker-start
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // column __COLUMN_NO__

  szSrcStz = (char*)p__STRUCT_FROM__->__COLUMN_FROM__.m_iAsI8;
  if ((i8)szSrcStz == C_DATUM_NULL) {
    aDynamicT->m_aiPayload[__INDEX_COLUMN__] = C_DATUM_NULL;
  }
  else {
    nSrcSize = strlen(szSrcStz);
#ifdef WITH_CLANG
    aDynamicT->m_aiPayload[__INDEX_COLUMN__] = (i8)aExtern_IndexTreeAllotPayStr(
        aDynamicT->m_xIndexInfoPL.m_aIndexTree, szSrcStz);
#endif

    szDstStz = (char*)aDynamicT->m_aiPayload[__INDEX_COLUMN__];
    strncpy(szDstStz, szSrcStz, 1024); // todo: discuss with Alka
  }
  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- iStreamDefBegin ---------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iStreamDefBegin
void vStreamDefBegin(Dynamic_T* aDynamicT) {

  // marker-start

  struct ColumnInfo xColumnInfo[] = {
      // marker-end
  };


}
/*----------------------------------------------------------------------------*/
/*-- vStreamDefEnd -----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vStreamDefEnd
void vStreamDefEnd(Dynamic_T* aDynamicT) {

  ColumnInfo xColumnInfo;
  i8 __COLUMNS_OUT__;

  // marker-start
#ifdef WITH_CLANG
  aDynamicT->m_aStreamDefOut = aExtern_StreamDefNew(__COLUMNS_OUT__, (void*)&xColumnInfo);
#endif

  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- iResetPLKey -------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iResetPLKey
i8 iResetPLKey(Dynamic_T* aDynamicT) {

  __L_LABEL_LOOP__:

  // marker-start

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // process row ?
  if ( ! aDynamicT->m_iProcessRow ) {
    aDynamicT->m_iProcessRow = 1;
    goto __L_LABEL_LOOP__;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // populate the payload key
  aDynamicT->m_xIndexInfoPL.m_aEntInt->m_iIntElm0 = 0;

  // marker-end

  return 0;

}

/*----------------------------------------------------------------------------*/
/*-- vEosLimit ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vEosLimit
void vEosLimit(Dynamic_T* aDynamicT) {

  i8 __LIMIT_ROWS__ = 0;
  i8*   paW;

  // marker-start
  L_Limit:

  if (aDynamicT->m_iLimitRows >= __LIMIT_ROWS__) {
    goto L_Return;
  }

  aDynamicT->m_iLimitRows++;

  // marker-end

  L_Return:
  if (paW != &aDynamicT->m_aW[0])
    NEVER_REACH;
  return;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
