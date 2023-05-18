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
  RowInfo __ROW_INFO_FROM__;

  char* szStrArea;

  __L_LABEL_LOOP__:

  // marker-start

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // GBYAGG-OP
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  struct __STRUCT_TO__* p__STRUCT_TO__;

  __L_LABEL__:

  printf("GBYAGG-OP\n");

  p__STRUCT_FROM__ = (struct __STRUCT_FROM__*)__ROW_INFO_FROM__.m_aRowCellArray;
  szStrArea = __ROW_INFO_FROM__.m_acStrArea;

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

  RowInfo __ROW_INFO_TO__;

  i8* paW;
  i8 i_CurrentRow;
  i8 __NUM_COLUMNS__;

  // marker-start


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // INFLATE-CRW
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  struct __STRUCT_TO__* p__STRUCT_TO__;

  __L_LABEL__:

  printf("INFLATE-CRW\n");

  paW = &aDynamicT->m_aW[0];

  i_CurrentRow = -1;

  aDynamicT->m_nCols = __NUM_COLUMNS__;

  // process the row
  aDynamicT->m_iProcessRow = 1;

  __ROW_INFO_TO__.m_acStrArea  = __ROW_INFO_TO__.m_acStrAreaSav;

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

  aDynamicT->m_xRowInfoIT.m_acStrArea = acStrArea;
  p__STRUCT_TO__ = (struct __STRUCT_TO__*)aDynamicT->m_xRowInfoIT.m_aRowCellArray;
  szStrArea = aDynamicT->m_xRowInfoIT.m_acStrArea;

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

  printf("SORT-OP\n");

  //p__STRUCT_FROM__ = (struct __STRUCT_FROM__*)aDynamicT->m_xRowInfoIT.m_aRowCellArray;
  //szStrArea = aDynamicT->m_acStrAreaT;

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

  printf("TUB\n");

  if (aDynamicT->m_xRowInfoTO.m_iInitialize) {
    struct ColumnInfo xColumnInfo[] = {
        {0, "n_nationkey", 6, 19,  0, 0, 1},
        {1, "n_name",      11, 25, 0, 0, 0},
        {1, "n_regionkey", 6, 19, 0, 0, 0},
        {1, "n_comment",   11, 152, 0, 0, 0}
    };
#ifdef WITH_TINYC
    void* aRowPix = aRowPix_new(4, &xColumnInfo);
    vTub_RowPix(aDynamicT->m_pTub, aRowPix);
#endif
    aDynamicT->m_xRowInfoTO.m_iInitialize = 0;
  }

  if (!aDynamicT->m_iProcessRow) {
    aDynamicT->m_iProcessRow = 1;
    goto __L_LABEL_LOOP__;
  }

  // allocate a row from the tub
#ifdef WITH_TINYC
  aDynamicT->m_xRowInfoTO.m_aRowCellArray = (byte*)aTub_AllotRow((void*)aDynamicT->m_pTub,&aDynamicT->m_xRowInfoTO.m_acStrArea,&aDynamicT->m_iErrorCode);
#endif
  if (aDynamicT->m_iErrorCode != (i8)EC_NONE) {
    return 0;
  }

  p__STRUCT_TO__ = (struct __STRUCT_TO__*)aDynamicT->m_xRowInfoTO.m_aRowCellArray;

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

  printf("TUB\n");

  p__STRUCT_FROM__ = (struct __STRUCT_TO__*)aDynamicT->m_xRowInfoAT.m_aRowCellArray;
  szStrArea = aDynamicT->m_xRowInfoAT.m_acStrArea;
  if ( aDynamicT->m_iAggregInit == 0 ) {
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
    aDynamicT->m_iAggregInit = 1;
  }

  if (!aDynamicT->m_iRowsProcessed)
    goto L_Return;

  // allocate a row from the tub
#ifdef WITH_TINYC
  aDynamicT->m_xRowInfoTO.m_aRowCellArray = (byte*)aTub_AllotRow((void*)aDynamicT->m_pTub,&aDynamicT->m_xRowInfoTO.m_acStrArea,&aDynamicT->m_iErrorCode);
#endif
  if (aDynamicT->m_iErrorCode != (i8) EC_NONE) {
    return 0;
  }

  p__STRUCT_TO__ = (struct __STRUCT_TO__ *) aDynamicT->m_xRowInfoTO.m_aRowCellArray;

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
    aDynamicT->m_xRowInfoTO.m_aRowCellArray = (byte*)aTub_AllotRow((void*)aDynamicT->m_pTub,&aDynamicT->m_xRowInfoTO.m_acStrArea,&aDynamicT->m_iErrorCode);
#endif
    if (aDynamicT->m_iErrorCode != (i8) EC_NONE) {
      return 0;
    }

    p__STRUCT_TO__ = (struct __STRUCT_TO__ *) aDynamicT->m_xRowInfoTO.m_aRowCellArray;

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
  struct __STRUCT_FROM__* p__STRUCT_FROM__;

  // - - - - - - - - - - - - - -
  // payload traversal
  aDynamicT->m_xIndexInfoPL.m_aEntInt = (struct Ent_int*)
      aIndexTree_begin(aDynamicT->m_xIndexInfoPL.m_aIndexTree, &aDynamicT->m_xIndexInfoPL.m_aEntBuffer);

  while (aDynamicT->m_xIndexInfoPL.m_aEntInt) {

    aDynamicT->m_iProcessRow = 1;

    aDynamicT->m_xRowInfoPT.m_acStrArea = aDynamicT->m_xRowInfoPT.m_acStrAreaSav;
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
  struct __STRUCT_FROM__* p__STRUCT_FROM__;

  // - - - - - - - - - - - - - -
  // payload traversal
  aDynamicT->m_xIndexInfoPL.m_aEntInt = (struct Ent_int*)aIndexTree_begin(
      aDynamicT->m_xIndexInfoPL.m_aIndexTree, &aDynamicT->m_xIndexInfoPL.m_aEntBuffer);

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
    szDstStz = aDynamicT->m_xRowInfoTO.m_acStrArea;

#ifdef WITH_TINYC
    nSrcSize = StrLib_iWordFlipBack(szDstStz,szSrcStz);
    //for (i8 i_Byte = 0; i_Byte < 16; i_Byte++)
    //  printf("%c ", szDstStz[i_Byte]);
    //printf("\n");
#endif
    amCellAddress = (m8*) &p__STRUCT_TO__->__COLUMN_TO__.m_iOffset;

    iOffset = - nDifference(aDynamicT->m_xRowInfoTO.m_acStrArea,amCellAddress);
    p__STRUCT_TO__->__COLUMN_TO__.m_iOffset = iOffset;

    // update the tub string size
    i8 iDstLen = strlen(szDstStz) + 1;
    aDynamicT->m_xRowInfoTO.m_acStrArea += iDstLen;
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
  szSrcStz = (char*)p__STRUCT_FROM__->__COLUMN_FROM__.m_iOffset;
  if (szSrcStz == (char*)C_DATUM_NULL) {
    // copy the source to the destination
    p__STRUCT_TO__->__COLUMN_TO__.m_iOffset = C_DATUM_NULL;
  }
  else {
    nSrcSize = strlen(szSrcStz) + 1;

    // get the destination string
    szDstStz = __ROW_INFO_TO__.m_acStrArea;
    strncpy(szDstStz, szSrcStz, nSrcSize);

    // copy the source to the destination
    amCellAddress = (m8*) &p__STRUCT_TO__->__COLUMN_TO__.m_iOffset;
    iOffset = - nDifference(__ROW_INFO_TO__.m_acStrArea,amCellAddress);
    p__STRUCT_TO__->__COLUMN_TO__.m_iOffset = iOffset;

    // update the tub string size
    __ROW_INFO_TO__.m_acStrArea += nSrcSize;

#ifdef WITH_TINYC
    vTub_RowStrSize(aDynamicT->m_pTub,nSrcSize);
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
  szSrcStz = (char*)p__STRUCT_FROM__->__COLUMN_FROM__.m_iOffset;
  if (szSrcStz == (char*)C_DATUM_NULL) {
    // copy the source to the destination
    p__STRUCT_TO__->__COLUMN_TO__.m_iOffset = C_DATUM_NULL;
  }
  else {
    nSrcSize = strlen(szSrcStz) + 1;

    // get the destination string
    szDstStz = __ROW_INFO_TO__.m_acStrArea;
    strncpy(szDstStz, szSrcStz, nSrcSize);

    // copy the source to the destination
    p__STRUCT_TO__->__COLUMN_TO__.m_iOffset = (i8)szDstStz;

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
#ifdef WITH_TINYC
    vTub_RowStrSize(aDynamicT->m_pTub,nSrcSize);
#endif
  }
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
     szDstStz = aDynamicT->m_xRowInfoTO.m_acStrArea;
     strncpy(szDstStz, szSrcStz, nSrcSize);

     // copy the source to the destination
     amCellAddress = (m8*) &p__STRUCT_TO__->__COLUMN_TO__.m_iOffset;
     iOffset = - nDifference(aDynamicT->m_xRowInfoTO.m_acStrArea,amCellAddress);
     p__STRUCT_TO__->__COLUMN_TO__.m_iOffset = iOffset;

     // update the tub string size
     aDynamicT->m_xRowInfoTO.m_acStrArea += nSrcSize;
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
