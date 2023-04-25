/*============================================================================*/
/* Code.cpp -- Code Fragments                                                 */
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

  p__STRUCT_FROM__ = (struct __STRUCT_FROM__*)aCombinedT->m_aRowCellArrayT;

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

/*----------------------------------------------------------------------------*/
/*-- vExprCheck---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vExprCheck
i8 vExprCheck(Combined_T* aCombinedT) {

  // marker-start

  if (aCombinedT->m_iErrorCode != (i8)EC_NONE) {
    return 0;
  }

  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vFilterBegin ------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vFilterBegin
void vFilterBegin(Combined_T* aCombinedT) {

  char* szStrArea;
  struct __STRUCT_FROM__* p__STRUCT_FROM__;

  __L_LABEL_LOOP__:

  // marker-start

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // FILTER-OP
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

__L_LABEL__:

  if (!aCombinedT->m_iProcessRow) {
    aCombinedT->m_iProcessRow = 1;
    goto __L_LABEL_LOOP__;
  }
  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- iFilterEnd --------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iFilterEnd
i8 iFilterEnd(Combined_T* aCombinedT, bool bFilterResult) {

  // marker-start
  if (bFilterResult && bFilterResult != C_DATUM_NULL) {
    aCombinedT->m_iProcessRow = 1;
  } else {
    aCombinedT->m_iProcessRow = 0;
  }
  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- iGbyAggBegin ------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iGbyAggBegin
i8 iGbyAggBegin(Combined_T* aCombinedT) {

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

  p__STRUCT_FROM__ = (struct __STRUCT_FROM__*)aCombinedT->m_aRowCellArrayT;
  szStrArea = aCombinedT->m_racStrAreaT;

  if (!aCombinedT->m_iProcessRow) {
    aCombinedT->m_iProcessRow = 1;
    goto L_AdvanceNextRow;
  }

  // - - - - - - - - - - - - - -
  // populate the payload key
  aCombinedT->m_aEntIntPL->m_iIntElm0 = 0;
  // marker-end

  return 0;

}

/*----------------------------------------------------------------------------*/
/*-- vGbyAggCreatePayLoad ----------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vGbyAggCreatePayLoad
void vGbyAggCreatePayLoad(Combined_T* aCombinedT) {

  i8 __PAYLOADBYTES__;

    // marker-start
    // - - - - - - - - - - - - - -
    // create the payload

#ifdef WITH_TINYC
    vEnt_payloadCreate(aCombinedT->m_aEntIntPL, __PAYLOADBYTES__);
#endif
    // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vGbyAggFindKey ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vGbyAggFindKey
void vGbyAggFindKey(Combined_T* aCombinedT) {

  void* aEntFound;

  // marker-start
  // - - - - - - - - - - - - - -
  // find key in payload index
  aEntFound = aIndexTree_findEntry(aCombinedT->m_aIndexTreePL, aCombinedT->m_aEntKeyPL);

  if ( !aEntFound ) {
    // marker-end
  }

}

/*----------------------------------------------------------------------------*/
/*-- iGbyAggInsertKey --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iGbyAggInsertKey
i8 iGbyAggInsertKey(Combined_T* aCombinedT) {

  void* aEntInserted;
  void* aEntFound;
  i8 __PAYLOADBYTES__;

  // marker-start

    // - - - - - - - - - - - - - -
    // insert key in the payload
    aEntInserted = aIndexTree_insertEnt(aCombinedT->m_aIndexTreePL, aCombinedT->m_aEntIntPL, &aCombinedT->m_iErrorCode);
    if (aCombinedT->m_iErrorCode != (i8)EC_NONE) {
      return 0;
    }

    if (aEntInserted) {
      aCombinedT->m_iRowsProcessed++;
      aCombinedT->m_aiPayload = (i8 *) ((char *) aEntInserted
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
i8 iGbyAggIntKeyEnd(Combined_T* aCombinedT) {

  void* aEntFound;

  {
  // marker-start

    if (aCombinedT->m_iErrorCode != (i8) EC_NONE) {
      return 0;
    }
  }

  // - - - - - - - - - - - - - -
  // aggregate the payload
  aCombinedT->m_aiPayload = (i8 *) ((char *) aEntFound
                  + sizeof(struct Ent_int)
                  + 8);

  // marker-end

 return 0;

}

/*----------------------------------------------------------------------------*/
/*-- iGbyAggGenKeyEnd --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iGbyAggGenKeyEnd
i8 iGbyAggGenKeyEnd(Combined_T* aCombinedT) {

  void* aEntFound;

  {
  // marker-start

    if (aCombinedT->m_iErrorCode != (i8) EC_NONE) {
      return 0;
    }
  }

  // - - - - - - - - - - - - - -
  // aggregate the payload
  aCombinedT->m_aiPayload = (i8 *) ((char *) aEntFound
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
void vInflateCrwBegin(Combined_T* aCombinedT) {

  i8* paW;
  i8 i_CurrentRow;

  // marker-start


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // INFLATE-CRW
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  struct __STRUCT_TO__* p__STRUCT_TO__;

 L_InflateT:


  paW = &aCombinedT->m_aW[0];

  i_CurrentRow = -1;

  // process the row
  aCombinedT->m_iProcessRow = 1;

  aCombinedT->m_racStrAreaT = aCombinedT->m_racStrAreaTSav;

  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vInflateCrwCol ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vInflateCrwCol
void vInflateCrwCol(Combined_T* aCombinedT) {

  void* aSubReturnLabel;
  u8 uMetaCode;
  u8* auMetaCode;
  u8 uHigh;
  u8 uLow;
  i8 i_Col;
  i8 __COLUMN__;

  // marker-start
  // - - - - - - - - - - - -
  i_Col = __COLUMN__;
  uMetaCode = *auMetaCode++;

  uHigh = uMetaCode >> 4;
  uLow  = uMetaCode & 0x0F;

  aSubReturnLabel = && __L_ReturnToColumn__;
  goto L_InflateAColumn;

  __L_ReturnToColumn__:

  // marker-end
  L_InflateAColumn:
    return;
}

/*----------------------------------------------------------------------------*/
/*-- vInflateCrwReturn -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vInflateCrwReturn
void vInflateCrwReturn(Combined_T* aCombinedT) {

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

  aCombinedT->m_racStrAreaT = acStrArea;
  p__STRUCT_TO__ = (struct __STRUCT_TO__*)aCombinedT->m_aRowCellArrayT;
  szStrArea = aCombinedT->m_racStrAreaT;

  // marker-end

  return;
}

/*----------------------------------------------------------------------------*/
/*-- vProjectBegin -----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vProjectBegin
void vProjectBegin(Combined_T* aCombinedT) {

  struct __STRUCT_FROM__* p__STRUCT_FROM__;

  char* szStrArea;

  __L_LABEL_LOOP__:

  // marker-start



  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // PROJECT-OP
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  struct __STRUCT_TO__* p__STRUCT_TO__;

  __L_LABEL__:

  if (!aCombinedT->m_iProcessRow) {
    aCombinedT->m_iProcessRow = 1;
    goto __L_LABEL_LOOP__;
  }

  aCombinedT->m_racStrAreaP = aCombinedT->m_racStrAreaPSav;
  p__STRUCT_TO__ = (struct __STRUCT_TO__*)aCombinedT->m_aRowCellArrayP;
  szStrArea = aCombinedT->m_racStrAreaP;

  // marker-end

  return;

}

/*----------------------------------------------------------------------------*/
/*-- iProjectEnd -------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iProjectEnd
i8 iProjectEnd(Combined_T* aCombinedT) {

  struct __STRUCT_TO__*   p__STRUCT_TO__;

  char* szStrArea;

  // marker-start

  if (aCombinedT->m_iErrorCode != (i8)EC_NONE) {
    return 0;
  }

  p__STRUCT_TO__ = (struct __STRUCT_TO__*)aCombinedT->m_aRowCellArrayP;
  szStrArea = aCombinedT->m_racStrAreaP;

  // marker-end

  return 0;

}

/*----------------------------------------------------------------------------*/
/*-- vTubBegin ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iTubBegin
i8 iTubBegin(Combined_T* aCombinedT) {

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

  if (!aCombinedT->m_iProcessRow) {
    aCombinedT->m_iProcessRow = 1;
    goto __L_LABEL_LOOP__;
  }

  // allocate a row from the tub
#ifdef WITH_TINYC
  aCombinedT->m_aRowTub = (byte*)aTub_AllotRow((void*)aCombinedT->m_pTub,&aCombinedT->m_racTubStrArea,&aCombinedT->m_iErrorCode);
#endif
  if (aCombinedT->m_iErrorCode != (i8)EC_NONE) {
    return 0;
  }

  p__STRUCT_TO__ = (struct __STRUCT_TO__*)aCombinedT->m_aRowTub;

  // marker-end

  return 0;
}

/*----------------------------------------------------------------------------*/
/*-- iEosTubBegin ------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iEosTubBegin
i8 iEosTubBegin(Combined_T* aCombinedT) {

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

  p__STRUCT_FROM__ = (struct __STRUCT_TO__*)aCombinedT->m_aRowCellArrayAgg;
  szStrArea = aCombinedT->m_racStrAreaAgg;
  if ( aCombinedT->m_iProjectInit == 0 ) {
    // marker-end
  }

  return 0;
}

/*----------------------------------------------------------------------------*/
/*-- iEosTubBody -------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iEosTubBody
i8 iEosTubBody(Combined_T* aCombinedT) {

  struct __STRUCT_TO__* p__STRUCT_TO__;

  {
    // marker-start

    if (aCombinedT->m_iErrorCode != (i8) EC_NONE) {
      return 0;
    }

    aCombinedT->m_iRowsProcessed = 1;
    aCombinedT->m_iProjectInit = 1;
  }

  if (!aCombinedT->m_iRowsProcessed)
    goto L_Return;

  // allocate a row from the tub
#ifdef WITH_TINYC
  aCombinedT->m_aRowTub = (byte*)aTub_AllotRow((void*)aCombinedT->m_pTub,&aCombinedT->m_racTubStrArea,&aCombinedT->m_iErrorCode);
#endif
  if (aCombinedT->m_iErrorCode != (i8) EC_NONE) {
    return 0;
  }

  p__STRUCT_TO__ = (struct __STRUCT_TO__ *) aCombinedT->m_aRowTub;

  // marker-end
  L_Return:
    return 0;
  }

/*----------------------------------------------------------------------------*/
/*-- iEosPayLoadTub ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iEosPayLoadTub
i8 iEosPayLoadTub(Combined_T* aCombinedT) {

  struct __STRUCT_TO__* p__STRUCT_TO__;

  // marker-start
    if (!aCombinedT->m_iRowsProcessed)
      goto L_Return;

  // allocate a row from the tub
#ifdef WITH_TINYC
    aCombinedT->m_aRowTub = (byte*)aTub_AllotRow((void*)aCombinedT->m_pTub,&aCombinedT->m_racTubStrArea,&aCombinedT->m_iErrorCode);
#endif
    if (aCombinedT->m_iErrorCode != (i8) EC_NONE) {
      return 0;
    }

    p__STRUCT_TO__ = (struct __STRUCT_TO__ *) aCombinedT->m_aRowTub;

  // marker-end
  L_Return:
  return 0;
}

/*----------------------------------------------------------------------------*/
/*-- vEosPayLoadTubEnd -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vEosPayLoadTubEnd
void vEosPayLoadTubEnd(Combined_T* aCombinedT) {

  {

  // marker-start
    // - - - - - - - - - - - - - -
    // move to next entry
    aCombinedT->m_aEntIntPL = (struct Ent_int*)aEntBuffer_nextEntry(&aCombinedT->m_aEntBufferPL, aCombinedT->m_aEntIntPL);
  }

  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vEosPayloadTraversal ----------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vEosPayloadTraversal
i8 vEosPayloadTraversal(Combined_T* aCombinedT) {

  i8 iElemSize = 0;

  // marker-start

  struct __STRUCT_TO__* p__STRUCT_TO__;
  struct __STRUCT_TO__* p__STRUCT_FROM__;

  // - - - - - - - - - - - - - -
  // payload traversal
  aCombinedT->m_aEntIntPL = (struct Ent_int*)aIndexTree_begin(aCombinedT->m_aIndexTreePL, &aCombinedT->m_aEntBufferPL);

  while (aCombinedT->m_aEntIntPL) {

    aCombinedT->m_iProcessRow = 1;

    aCombinedT->m_racPayloadArea = aCombinedT->m_racPayloadAreaSav;
    aCombinedT->m_aiPayload = (i8 *) ((char *) aCombinedT->m_aEntIntPL
                                      + sizeof(struct Ent_int)
                                      + 8);

    iElemSize = 0;

    // marker-end
  }

}

/*----------------------------------------------------------------------------*/
/*-- vPayLoadKeyToTub --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vPayLoadKeyToTub
void vPayLoadKeyToTub(Combined_T* aCombinedT) {

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

    p__STRUCT_TO__->__COLUMN_TO__.m_iAsI8 = ((struct Ent_int*)aCombinedT->m_aEntIntPL)->m_iIntElm0 + iElemSize;

  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vTubCol -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vTubCol
void vTubCol(Combined_T* aCombinedT) {

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
void vTubVchCol(Combined_T* aCombinedT) {

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
    szDstStz = aCombinedT->m_racTubStrArea;
    strncpy(szDstStz, szSrcStz, nSrcSize);

    // copy the source to the destination
    amCellAddress = (m8*) &p__STRUCT_TO__->__COLUMN_TO__.m_iOffset;
    iOffset = - nDifference(aCombinedT->m_racTubStrArea,amCellAddress);
    p__STRUCT_TO__->__COLUMN_TO__.m_iOffset = iOffset;

    // update the tub string size
    aCombinedT->m_racTubStrArea += nSrcSize;
#ifdef WITH_TINYC
    vTub_RowStrSize(aCombinedT->m_pTub,nSrcSize);
#endif
  }

  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vPayLoadToTubCol --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vPayLoadToTubCol
void vPayLoadToTubCol(Combined_T* aCombinedT) {

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

    p__STRUCT_TO__->__COLUMN_TO__.m_iAsI8 = aCombinedT->m_aiPayload[__PAYLOAD_INDEX__];

  // marker-end
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
