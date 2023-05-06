/*============================================================================*/
/* Code.cpp -- Code Fragments                                                 */
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
/*-- vExprCheck---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vExprCheck
i8 vExprCheck(Dynamic_T* aDynamicT) {

  // marker-start

  if (aDynamicT->m_iErrorCode != (i8)EC_NONE) {
    return 0;
  }

  // marker-end
}


/*----------------------------------------------------------------------------*/
/*-- iGbyAggBegin ------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iGbyAggBegin
i8 iGbyAggBegin(Dynamic_T* aDynamicT) {

  struct __STRUCT_FROM__* p__STRUCT_FROM__;
  char* szStrArea;

  __L_LABEL_LOOP__:

  L_AdvanceNextRow:

  // marker-start

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // AGGREG-OP
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  struct __STRUCT_TO__* p__STRUCT_TO__;

  __L_LABEL__:

  p__STRUCT_FROM__ = (struct __STRUCT_FROM__*)aDynamicT->m_xRowInfoIT.m_aRowCellArray;
  szStrArea = aDynamicT->m_xRowInfoIT.m_racStrArea;

  if (!aDynamicT->m_iProcessRow) {
    aDynamicT->m_iProcessRow = 1;
    goto L_AdvanceNextRow;
  }

  // - - - - - - - - - - - - - -
  // populate the payload key
  aDynamicT->m_xIndexInfoPL.m_aEntInt->m_iIntElm0 = 0;
  // marker-end

  return 0;

}

/*----------------------------------------------------------------------------*/
/*-- vGbyAggCreatePayLoad ----------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vGbyAggCreatePayLoad
void vGbyAggCreatePayLoad(Dynamic_T* aDynamicT) {

  i8 __PAYLOADBYTES__;

    // marker-start
    // - - - - - - - - - - - - - -
    // create the payload

#ifdef WITH_TINYC
    vEnt_payloadCreate(aDynamicT->m_xIndexInfoPL.m_aEntInt, __PAYLOADBYTES__);
#endif
    // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vGbyAggFindKey ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vGbyAggFindKey
void vGbyAggFindKey(Dynamic_T* aDynamicT) {

  void* aEntFound;

  // marker-start
  // - - - - - - - - - - - - - -
  // find key in payload index
  aEntFound = aIndexTree_findEntry(aDynamicT->m_xIndexInfoPL.m_aIndexTree, aDynamicT->m_xIndexInfoPL.m_aEntKey);

  if ( !aEntFound ) {
    // marker-end
  }

}

/*----------------------------------------------------------------------------*/
/*-- iGbyAggInsertIntKey -----------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iGbyAggInsertIntKey
i8 iGbyAggInsertIntKey(Dynamic_T* aDynamicT) {

  void* aEntInserted;
  void* aEntFound;
  i8 __PAYLOADBYTES__;

  // marker-start

    // - - - - - - - - - - - - - -
    // insert key in the payload
    aEntInserted = aIndexTree_insertEnt(
        aDynamicT->m_xIndexInfoPL.m_aIndexTree, aDynamicT->m_xIndexInfoPL.m_aEntInt, &aDynamicT->m_iErrorCode);
    if (aDynamicT->m_iErrorCode != (i8)EC_NONE) {
      return 0;
    }

    if (aEntInserted) {
      aDynamicT->m_iRowsProcessed++;
      aDynamicT->m_aiPayload = (i8 *) ((char *) aEntInserted
          + sizeof(struct Ent_int)
          + __PAYLOADBYTES__);
     }

     aEntFound = aEntInserted;

// marker-end


}

/*----------------------------------------------------------------------------*/
/*-- iGbyAggIntKeyEnd --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iGbyAggIntKeyEnd
i8 iGbyAggIntKeyEnd(Dynamic_T* aDynamicT) {

  void* aEntFound;

  {
  // marker-start

    if (aDynamicT->m_iErrorCode != (i8) EC_NONE) {
      return 0;
    }
  }

  // - - - - - - - - - - - - - -
  // aggregate the payload
  aDynamicT->m_aiPayload = (i8 *) ((char *) aEntFound
                  + sizeof(struct Ent_int)
                  + 8);

  // marker-end

 return 0;

}

/*----------------------------------------------------------------------------*/
/*-- iGbyAggInsertVchKey -----------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iGbyAggInsertVchKey
i8 iGbyAggInsertVchKey(Dynamic_T* aDynamicT) {

  void* aEntInserted;
  void* aEntFound;
  i8 __PAYLOADBYTES__;

  // marker-start

  // - - - - - - - - - - - - - -
  // insert key in the payload
  aEntInserted = aIndexTree_insertEnt(
      aDynamicT->m_xIndexInfoPL.m_aIndexTree, aDynamicT->m_xIndexInfoPL.m_aEntInt, &aDynamicT->m_iErrorCode);
  if (aDynamicT->m_iErrorCode != (i8)EC_NONE) {
    return 0;
  }

  if (aEntInserted) {
    aDynamicT->m_iRowsProcessed++;
    aDynamicT->m_aiPayload = (i8 *) ((char *) aEntInserted
        + sizeof(struct Ent_int)
        + ((struct Ent_int*)aEntInserted)->m_iIntElm0
        + __PAYLOADBYTES__);
  }

  aEntFound = aEntInserted;

// marker-end


}
/*----------------------------------------------------------------------------*/
/*-- iGbyAggGenKeyEnd --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iGbyAggGenKeyEnd
i8 iGbyAggGenKeyEnd(Dynamic_T* aDynamicT) {

  void* aEntFound;

  {
  // marker-start

    if (aDynamicT->m_iErrorCode != (i8) EC_NONE) {
      return 0;
    }
  }

  // - - - - - - - - - - - - - -
  // aggregate the payload
  aDynamicT->m_aiPayload = (i8 *) ((char *) aEntFound
                  + sizeof(struct Ent_int)
                  + ((struct Ent_int*)aEntFound)->m_iIntElm0
                  + 8);

  // marker-end

 return 0;

}

/*----------------------------------------------------------------------------*/
/*-- vInflateCrwBegin --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vInflateCrwBegin
void vInflateCrwBegin(Dynamic_T* aDynamicT) {

  i8* paW;
  i8 i_CurrentRow;

  // marker-start


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // INFLATE-CRW
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  struct __STRUCT_TO__* p__STRUCT_TO__;

  __L_LABEL__:

  paW = &aDynamicT->m_aW[0];

  i_CurrentRow = -1;

  // process the row
  aDynamicT->m_iProcessRow = 1;

  aDynamicT->m_xRowInfoIT.m_racStrArea = aDynamicT->m_xRowInfoIT.m_racStrAreaSav;

  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vInflateCrwCol ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vInflateCrwCol
void vInflateCrwCol(Dynamic_T* aDynamicT) {

  void* aSubReturnLabel;
  u8 uMetaCode;
  u8* auMetaCode;
  u8 uHigh;
  u8 uLow;
  i8 i_Col;
  i8 __COLUMN_NO__;

  // marker-start
  // - - - - - - - - - - - -
  i_Col = __COLUMN_NO__;
  uMetaCode = *auMetaCode++;

  uHigh = uMetaCode >> 4;
  uLow  = uMetaCode & 0x0F;

  aSubReturnLabel = && __L_RETURN_TO_LABEL__;
  goto L_InflateAColumn;

  __L_RETURN_TO_LABEL__:

  // marker-end
  L_InflateAColumn:
  return;
}

/*----------------------------------------------------------------------------*/
/*-- vInflateCrwReturn -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vInflateCrwReturn
void vInflateCrwReturn(Dynamic_T* aDynamicT) {

  char* szStrArea;
  char* acStrArea;
  void* aSubReturnLabel;
  struct __STRUCT_TO__* p__STRUCT_TO__;

  // marker-start
  aSubReturnLabel = && L_InflateCrwRet;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // INFLATE-CRW-RET
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  L_InflateCrwRet:

  aDynamicT->m_xRowInfoIT.m_racStrArea = acStrArea;
  p__STRUCT_TO__ = (struct __STRUCT_TO__*)aDynamicT->m_xRowInfoIT.m_aRowCellArray;
  szStrArea = aDynamicT->m_xRowInfoIT.m_racStrArea;

  // marker-end

  return;
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

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // SORT-OP
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  __L_LABEL__:

  //p__STRUCT_FROM__ = (struct __STRUCT_FROM__*)aDynamicT->m_xRowInfoIT.m_aRowCellArray;
  //szStrArea = aDynamicT->m_racStrAreaT;

  if (!aDynamicT->m_iProcessRow) {
    aDynamicT->m_iProcessRow = 1;
    goto __L_LABEL_LOOP__;
  }

  // - - - - - - - - - - - - - -
  // populate the payload key
  aDynamicT->m_xIndexInfoPL.m_aEntInt->m_iIntElm0 = 0;
  // marker-end

  return 0;

}

/*----------------------------------------------------------------------------*/
/*-- vTubBegin ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iTubBegin
i8 iTubBegin(Dynamic_T* aDynamicT) {

  struct __STRUCT_TO__ {
    i8 iDummy;
  };

  char* szStrArea;

  __L_LABEL_LOOP__:

  // marker-start


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // TUB
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  struct __STRUCT_TO__* p__STRUCT_TO__;

  __L_LABEL__:

  if (!aDynamicT->m_iProcessRow) {
    aDynamicT->m_iProcessRow = 1;
    goto __L_LABEL_LOOP__;
  }

  // allocate a row from the tub
#ifdef WITH_TINYC
  aDynamicT->m_xRowInfoTO.m_aRowSingle = (byte*)aTub_AllotRow((void*)aDynamicT->m_pTub,&aDynamicT->m_xRowInfoTO.m_racStrArea,&aDynamicT->m_iErrorCode);
#endif
  if (aDynamicT->m_iErrorCode != (i8)EC_NONE) {
    return 0;
  }

  p__STRUCT_TO__ = (struct __STRUCT_TO__*)aDynamicT->m_xRowInfoTO.m_aRowSingle;

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


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // TUB
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  struct __STRUCT_TO__* p__STRUCT_TO__;
  struct __STRUCT_TO__* p__STRUCT_FROM__;

  __L_LABEL__:

  p__STRUCT_FROM__ = (struct __STRUCT_TO__*)aDynamicT->m_xRowInfoAT.m_aRowCellArray;
  szStrArea = aDynamicT->m_xRowInfoAT.m_racStrArea;
  if ( aDynamicT->m_iProjectInit == 0 ) {
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

  {
    // marker-start

    if (aDynamicT->m_iErrorCode != (i8) EC_NONE) {
      return 0;
    }

    aDynamicT->m_iRowsProcessed = 1;
    aDynamicT->m_iProjectInit = 1;
  }

  if (!aDynamicT->m_iRowsProcessed)
    goto L_Return;

  // allocate a row from the tub
#ifdef WITH_TINYC
  aDynamicT->m_xRowInfoTO.m_aRowSingle = (byte*)aTub_AllotRow((void*)aDynamicT->m_pTub,&aDynamicT->m_xRowInfoTO.m_racStrArea,&aDynamicT->m_iErrorCode);
#endif
  if (aDynamicT->m_iErrorCode != (i8) EC_NONE) {
    return 0;
  }

  p__STRUCT_TO__ = (struct __STRUCT_TO__ *) aDynamicT->m_xRowInfoTO.m_aRowSingle;

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

  // marker-start
    if (!aDynamicT->m_iRowsProcessed)
      goto L_Return;

  // allocate a row from the tub
#ifdef WITH_TINYC
    aDynamicT->m_xRowInfoTO.m_aRowSingle = (byte*)aTub_AllotRow((void*)aDynamicT->m_pTub,&aDynamicT->m_xRowInfoTO.m_racStrArea,&aDynamicT->m_iErrorCode);
#endif
    if (aDynamicT->m_iErrorCode != (i8) EC_NONE) {
      return 0;
    }

    p__STRUCT_TO__ = (struct __STRUCT_TO__ *) aDynamicT->m_xRowInfoTO.m_aRowSingle;

  // marker-end
  L_Return:
  return 0;
}

/*----------------------------------------------------------------------------*/
/*-- vEosPayLoadTubEnd -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vEosPayLoadTubEnd
void vEosPayLoadTubEnd(Dynamic_T* aDynamicT) {

  {

  // marker-start
    // - - - - - - - - - - - - - -
    // move to next entry
    aDynamicT->m_xIndexInfoPL.m_aEntInt = (struct Ent_int*)aEntBuffer_nextEntry(
        &aDynamicT->m_xIndexInfoPL.m_aEntBuffer, aDynamicT->m_xIndexInfoPL.m_aEntInt);
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
  struct __STRUCT_TO__* p__STRUCT_FROM__;

  // - - - - - - - - - - - - - -
  // payload traversal
  aDynamicT->m_xIndexInfoPL.m_aEntInt = (struct Ent_int*)
      aIndexTree_begin(aDynamicT->m_xIndexInfoPL.m_aIndexTree, &aDynamicT->m_xIndexInfoPL.m_aEntBuffer);

  while (aDynamicT->m_xIndexInfoPL.m_aEntInt) {

    aDynamicT->m_iProcessRow = 1;

    aDynamicT->m_xRowInfoPT.m_racStrArea = aDynamicT->m_xRowInfoPT.m_racStrAreaSav;
    aDynamicT->m_aiPayload = (i8 *) ((char *) aDynamicT->m_xIndexInfoPL.m_aEntInt
                                      + sizeof(struct Ent_int)
                                      + 8);

    iElemSize = 0;

    // marker-end
  }

}

/*----------------------------------------------------------------------------*/
/*-- vEosPLGenKeyTraversal ---------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vEosPLGenKeyTraversal
i8 vEosPLGenKeyTraversal(Dynamic_T* aDynamicT) {

  i8 iElemSize = 0;

  // marker-start

  struct __STRUCT_TO__* p__STRUCT_TO__;
  struct __STRUCT_TO__* p__STRUCT_FROM__;

  // - - - - - - - - - - - - - -
  // payload traversal
  aDynamicT->m_xIndexInfoPL.m_aEntInt = (struct Ent_int*)aIndexTree_begin(
      aDynamicT->m_xIndexInfoPL.m_aIndexTree, &aDynamicT->m_xIndexInfoPL.m_aEntBuffer);

  while (aDynamicT->m_xIndexInfoPL.m_aEntInt) {

    aDynamicT->m_iProcessRow = 1;

    aDynamicT->m_xRowInfoPT.m_racStrArea = aDynamicT->m_xRowInfoPT.m_racStrAreaSav;
    aDynamicT->m_aiPayload = (i8 *) ((char *) aDynamicT->m_xIndexInfoPL.m_aEntInt
        + sizeof(struct Ent_int)
        + ((struct Ent_int*)aDynamicT->m_xIndexInfoPL.m_aEntInt)->m_iIntElm0
        + 8);

    iElemSize = 0;

    // marker-end
  }

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

  if ((i8)szSrcStz == C_DATUM_NULL) {
    p__STRUCT_TO__->__COLUMN_TO__.m_iAsI8 = C_DATUM_NULL;
    iElemSize += 8;
  }
  else {
    szDstStz = aDynamicT->m_xRowInfoTO.m_racStrArea;

#ifdef WITH_TINYC
    nSrcSize = StrLib_iWordFlipBack(szDstStz,szSrcStz);
    //for (i8 i_Byte = 0; i_Byte < 16; i_Byte++)
    //  printf("%c ", szDstStz[i_Byte]);
    //printf("\n");
#endif
    amCellAddress = (m8*) &p__STRUCT_TO__->__COLUMN_TO__.m_iOffset;

    iOffset = - nDifference(aDynamicT->m_xRowInfoTO.m_racStrArea,amCellAddress);
    p__STRUCT_TO__->__COLUMN_TO__.m_iOffset = iOffset;

    // update the tub string size
    i8 iDstLen = strlen(szDstStz) + 1;
    aDynamicT->m_xRowInfoTO.m_racStrArea += iDstLen;
#ifdef WITH_TINYC
    vTub_RowStrSize(aDynamicT->m_pTub,iDstLen);
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
  char* __STRING_AREA__;

  // marker-start
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // column __COLUMN_NO__

  // get the source string
  szSrcStz = (char*)p__STRUCT_FROM__->__COLUMN_FROM__.m_iOffset;
  if (szSrcStz == (char*)C_DATUM_NULL) {
    // copy the source to the destination
    p__STRUCT_TO__->__COLUMN_TO__.m_iOffset = C_DATUM_NULL;
  }
  else {
    nSrcSize = strlen(szSrcStz) + 1;

    // get the destination string
    szDstStz = __STRING_AREA__;
    strncpy(szDstStz, szSrcStz, nSrcSize);

    // copy the source to the destination
    amCellAddress = (m8*) &p__STRUCT_TO__->__COLUMN_TO__.m_iOffset;
    iOffset = - nDifference(__STRING_AREA__,amCellAddress);
    p__STRUCT_TO__->__COLUMN_TO__.m_iOffset = iOffset;

    // update the tub string size
    __STRING_AREA__ += nSrcSize;
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
  char* __STRING_AREA__;

  // marker-start
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // column __COLUMN_NO__

  // get the source string
  szSrcStz = (char*)p__STRUCT_FROM__->__COLUMN_FROM__.m_iOffset;
  if (szSrcStz == (char*)C_DATUM_NULL) {
    // copy the source to the destination
    p__STRUCT_TO__->__COLUMN_TO__.m_iOffset = C_DATUM_NULL;
  }
  else {
    nSrcSize = strlen(szSrcStz) + 1;

    // get the destination string
    szDstStz = __STRING_AREA__;
    strncpy(szDstStz, szSrcStz, nSrcSize);

    // copy the source to the destination
    p__STRUCT_TO__->__COLUMN_TO__.m_iOffset = (i8)szDstStz;

    // update the tub string size
    __STRING_AREA__ += nSrcSize;
  }

  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vIndexDereference -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexDereference
void vIndexDereference(Dynamic_T* aDynamicT) {

#include "vFLAT_Parameters_v2.h"

  i8 iIndexBuffer;
  i8 iIndexItem;
  i8 i_BaseOffsetDataT;
  char* aIndexCrw;

  static const void* C_ADR_REGION = (void*)0x204800000000;
  static const i8 C_CRW_HEADBYTES = 32;
  static const i8 C_CRW_FIRSTHEAD = 640;

  L_InflateAColumn:

  // marker-start

  // dereference the entry
  iIndexBuffer = aDynamicT->m_xIndexInfoT.m_aEntInt->Rrf.m_uBuffer;
  iIndexItem   = aDynamicT->m_xIndexInfoT.m_aEntInt->Rrf.m_uItem;
  aIndexCrw    = ( (char*)C_ADR_REGION + iIndexBuffer * C_1MB + C_CRW_FIRSTHEAD+ iIndexItem * C_CRW_HEADBYTES );

  // inflate the derefenced entry
  aHeadLine = (struct HeadLine_T *)aIndexCrw;
  auMetaCode = META_AREA(aHeadLine);
  ayDstByte = (byte*)aDynamicT->m_xRowInfoIT.m_aRowCellArray;
  acStrArea = (char*)aDynamicT->m_xRowInfoIT.m_racStrArea;
  racStrArea = (char*)aDynamicT->m_xRowInfoIT.m_racStrArea;
  eStrInflateRule = SI_STRAREA_ADR;
  i_BaseOffsetDataT = C_1B  + C_8B  + 4;
  ayDataByte = ADDR_ADD(byte*,aHeadLine,i_BaseOffsetDataT);

  // marker-end

  __L_LABEL_NEXT__:

  L_Return:
  return;

}

/*----------------------------------------------------------------------------*/
/*-- vTubRowStr --------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vTubRowStr
void vTubRowStr(Dynamic_T* aDynamicT) {

  // marker-start
#ifdef WITH_TINYC
  vTub_RowStrSize(aDynamicT->m_pTub,nSrcSize);
#endif

  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vPayLoadToTubCol --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vPayLoadToTubCol
void vPayLoadToTubCol(Dynamic_T* aDynamicT) {

  i8 __PAYLOAD_INDEX__;

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

    p__STRUCT_TO__->__COLUMN_TO__.m_iAsI8 = aDynamicT->m_aiPayload[__PAYLOAD_INDEX__];

  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vPayLoadToTubCol --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vPayLoadToTubVchCol
void vPayLoadToTubVchCol(Dynamic_T* aDynamicT) {

  i8 __PAYLOAD_INDEX__;
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

   szSrcStz = (char*)(char*)aDynamicT->m_aiPayload[__PAYLOAD_INDEX__];
   if (szSrcStz == (char*)C_DATUM_NULL) {
     // copy the source to the destination
     p__STRUCT_TO__->__COLUMN_TO__.m_iOffset = C_DATUM_NULL;
   }
   else {
     nSrcSize = strlen(szSrcStz) + 1;

     // get the destination string
     szDstStz = aDynamicT->m_xRowInfoTO.m_racStrArea;
     strncpy(szDstStz, szSrcStz, nSrcSize);

     // copy the source to the destination
     amCellAddress = (m8*) &p__STRUCT_TO__->__COLUMN_TO__.m_iOffset;
     iOffset = - nDifference(aDynamicT->m_xRowInfoTO.m_racStrArea,amCellAddress);
     p__STRUCT_TO__->__COLUMN_TO__.m_iOffset = iOffset;

     // update the tub string size
     aDynamicT->m_xRowInfoTO.m_racStrArea += nSrcSize;
#ifdef WITH_TINYC
     vTub_RowStrSize(aDynamicT->m_pTub,nSrcSize);
#endif
  }

  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vPopulateIntPL ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vPopulateIntPL
void vPopulateIntPL(Dynamic_T* aDynamicT) {

  i8 __PAYLOAD_INDEX__;
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

  aDynamicT->m_aiPayload[__PAYLOAD_INDEX__] = p__STRUCT_FROM__->__COLUMN_FROM__.m_iAsI8;

  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vPopulateVchPL ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vPopulateVchPL
void vPopulateVchPL(Dynamic_T* aDynamicT) {

  i8 __PAYLOAD_INDEX__;
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
    aDynamicT->m_aiPayload[__PAYLOAD_INDEX__] = C_DATUM_NULL;
  }
  else {
    nSrcSize = strlen(szSrcStz);
    aDynamicT->m_aiPayload[__PAYLOAD_INDEX__] = (i8)aIndexTree_allotPayStr(
        aDynamicT->m_xIndexInfoPL.m_aIndexTree, szSrcStz);
    szDstStz = (char*)aDynamicT->m_aiPayload[__PAYLOAD_INDEX__];
    strncpy(szDstStz, szSrcStz, nSrcSize);
  }
  // marker-end
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
