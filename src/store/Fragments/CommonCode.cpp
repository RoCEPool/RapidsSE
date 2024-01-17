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
#include "KeyHsh.hpp"
#include "Ent.hpp"
#include "Ent_int.h"

/*----------------------------------------------------------------------------*/
/*-- Code Fragments ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- vAssignHash -------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vAssignHash
void vAssignHash(Dynamic_T* aDynamicT) {

	struct __STRUCT_FROM__ {
			union Int_T __COLUMN_FROM__;
	};

	struct __STRUCT_FROM__* p__STRUCT_FROM__;

	i8 iMixVal;
	i8 iKeyHash;

	// marker-start
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// column __COLUMN_NO__

	iMixVal = p__STRUCT_FROM__->__COLUMN_FROM__.m_iAsI8;

#ifdef WITH_CLANG
  vExtern_RunningHash(&iKeyHash, iMixVal);
#endif
	// marker-end
}

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
  if ( ! aDynamicT->m_bProcessRow ) {
    aDynamicT->m_bProcessRow = true;
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
    aDynamicT->m_bProcessRow = false;
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

	char* szStrArea;
	struct __STRUCT_FROM__* p__STRUCT_FROM__;
	RowInfo __ROW_INFO_FROM__;

  // marker-start
  __L_LABEL__:

	p__STRUCT_FROM__ = (struct __STRUCT_FROM__*)__ROW_INFO_FROM__.m_aRowCellArray;
	szStrArea = __ROW_INFO_FROM__.m_acStrArea;

  // marker-end

  return 0;

}

/*----------------------------------------------------------------------------*/
/*-- iPotTubBegin ------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iPotTubBegin
i8 iPotTubBegin(Dynamic_T* aDynamicT) {

  RowInfo __ROW_INFO_TO__;
  i8*   paW;

  // marker-start
  __L_LABEL__:

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Initialize
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
/*-- vPotTubRowPix -----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vPotTubRowPix
void vPotTubRowPix(Dynamic_T* aDynamicT) {

  RowInfo __ROW_INFO_TO__;
  i8 __POT__;

  {
    // marker-start

#ifdef WITH_CLANG
    void* aRowPix = aExtern_RowPixNew(__COLUMNS_OUT__, &xColumnInfo);
    __EXTERN_ROWPIX__(__TUB_POT__, aRowPix);
#endif
    __ROW_INFO_TO__.m_bInitialize = false;
    aDynamicT->m_bGeneratePot = __POT__;
  }

  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- iPotTubContinue ---------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iPotTubContinue
i8 iPotTubContinue(Dynamic_T* aDynamicT) {

  char* szStrArea;
  ColumnInfo xColumnInfo;

  i8 __COLUMNS_OUT__;
  RowInfo __ROW_INFO_TO__;

  __L_LABEL_LOOP__:

  // marker-start

  struct __STRUCT_TO__* p__STRUCT_TO__;

  // process the row ?
  if ( ! aDynamicT->m_bProcessRow ) {
    aDynamicT->m_bProcessRow = true;
    goto __L_LABEL_LOOP__;
  }

  // allocate a row from the tub
#ifdef WITH_CLANG
  __ROW_INFO_TO__.m_aRowCellArray = (byte*)__EXTERN_ALLOTROW__();
	__ROW_INFO_TO__.m_nStringAreaSize = sizeof(__ROW_INFO_TO__.m_cStringArea);
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
/*-- iPotTubContinue_pds -----------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iPotTubContinue_pds
i8 iPotTubContinue_pds(Dynamic_T* aDynamicT) {

	char* szStrArea;
	ColumnInfo xColumnInfo;

	i8 __COLUMNS_OUT__;
	RowInfo __ROW_INFO_TO__;

	__L_LABEL_LOOP__:

	// marker-start

	struct __STRUCT_TO__* p__STRUCT_TO__;

	// process the row ?
	if ( ! aDynamicT->m_bProcessRow ) {
		aDynamicT->m_bProcessRow = true;
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
/*-- iEosPotTubBegin ---------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iEosPotTubBegin
i8 iEosPotTubBegin(Dynamic_T* aDynamicT) {

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
/*-- iEosPotTubBody ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iEosPotTubBody
i8 iEosPotTubBody(Dynamic_T* aDynamicT) {

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
	__ROW_INFO_TO__.m_nStringAreaSize = sizeof(__ROW_INFO_TO__.m_cStringArea);
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
/*-- iEosPotTubBody_pds ------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iEosPotTubBody_pds
i8 iEosPotTubBody_pds(Dynamic_T* aDynamicT) {

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
/*-- iEosPLPotTub ------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iEosPLPotTub
i8 iEosPLPotTub(Dynamic_T* aDynamicT) {

  struct __STRUCT_TO__* p__STRUCT_TO__;
  RowInfo __ROW_INFO_TO__;

  // marker-start
	  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // allocate a row from the tub/pot
#ifdef WITH_CLANG
    __ROW_INFO_TO__.m_aRowCellArray = (byte*)__EXTERN_ALLOTROW__();
	  __ROW_INFO_TO__.m_nStringAreaSize = sizeof(__ROW_INFO_TO__.m_cStringArea);
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

/*----------------------------------------------------------------------------*/
/*-- iEosPLPotTub_pds --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iEosPLPotTub_pds
i8 iEosPLPotTub_pds(Dynamic_T* aDynamicT) {

  struct __STRUCT_TO__* p__STRUCT_TO__;
  RowInfo __ROW_INFO_TO__;

  // marker-start
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allocate a row from the tub/pot
#ifdef WITH_CLANG
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
/*-- vEosPLPotTubEnd ---------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vEosPLPotTubEnd
void vEosPLPotTubEnd(Dynamic_T* aDynamicT) {

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
/*-- vHashBegin --------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vHashBegin
void vHashBegin(Dynamic_T* aDynamicT) {

	i8 iKeyHash;

	// marker-start
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Calculate the Partition Key Hash
	iKeyHash = 0;

	// marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vEosPLIntKeyTraversal ---------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vEosPLIntKeyTraversal
void vEosPLIntKeyTraversal(Dynamic_T* aDynamicT) {

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

	if (!aDynamicT->m_iRowsProcessed)
		goto L_Return;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // traverse the index
  while (aDynamicT->m_xIndexInfoPL.m_aEntInt) {

    aDynamicT->m_bProcessRow = true;

    aDynamicT->m_xRowInfoPT.m_acStrArea = aDynamicT->m_xRowInfoPT.m_acStrAreaSav;
    aDynamicT->m_aiPayload = (i8 *) ((char *) aDynamicT->m_xIndexInfoPL.m_aEntInt
                                      + sizeof(struct Ent_int)
                                      + 8);

    iElemSize = 0;

    // marker-end
  }

	L_Return:
	  return;
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

    aDynamicT->m_bProcessRow = true;

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
/*-- vPLToHashBegin ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vPLToHashBegin
void vPLToHashBegin(Dynamic_T* aDynamicT) {

	i8 iKeySize;
	i8 iKeyHash;

	// marker-start
	  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	  // Calculate the Partition Key Hash
	  iKeySize = 0;
	  iKeyHash = 0;

	// marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vPLIntKeyToHash ---------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vPLIntKeyToHash
void vPLIntKeyToHash(Dynamic_T* aDynamicT) {

	i8 iMixVal;
	i8 iKeySize;
	i8 iKeyHash;

	struct __STRUCT_FROM__ {
			union Vch_T __COLUMN_FROM__;
	};

	struct __STRUCT_FROM__* p__STRUCT_FROM__;

	struct __STRUCT_TO__ {
			union Vch_T __COLUMN_TO__;
	};

	struct __STRUCT_TO__* p__STRUCT_TO__;

	// marker-start
	iMixVal = *(i8*)((char*)(&((struct Ent_int*)aDynamicT->m_xIndexInfoPL.m_aEntInt)->m_iIntElm0) + 8 + iKeySize);
#ifdef WITH_CLANG
	vExtern_RunningHash(&iKeyHash, iMixVal);
#endif

	// marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vPLSkipVchKey -----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vPLSkipVchKey
void vPLSkipVchKey(Dynamic_T* aDynamicT) {

	struct __STRUCT_FROM__ {
			union Vch_T __COLUMN_FROM__;
	};

	struct __STRUCT_FROM__* p__STRUCT_FROM__;

	struct __STRUCT_TO__ {
			union Vch_T __COLUMN_TO__;
	};

	struct __STRUCT_TO__* p__STRUCT_TO__;

	char* szSrcStz;

	i8 iKeySize;
	i8 nSrcSize;

	// marker-start
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		// skipping key column __COLUMN_NO__
		szSrcStz = ((char*)(&(((struct Ent_int*)aDynamicT->m_xIndexInfoPL.m_aEntInt)->m_iIntElm0)) + 8 + iKeySize);

		if (*(i8*)szSrcStz == C_DATUM_NULL) {
			iKeySize += 8;
		}
		else {

#ifdef WITH_CLANG
		nSrcSize = iExtern_StrLibWordFlipSize(szSrcStz);
#endif

		iKeySize += nSrcSize;
	}
	// marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vPLSkipIntKey -----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vPLSkipIntKey
void vPLSkipIntKey(Dynamic_T* aDynamicT) {

	struct __STRUCT_FROM__ {
			union Vch_T __COLUMN_FROM__;
	};

	struct __STRUCT_FROM__* p__STRUCT_FROM__;

	struct __STRUCT_TO__ {
			union Vch_T __COLUMN_TO__;
	};

	struct __STRUCT_TO__* p__STRUCT_TO__;

	char* szSrcStz;

	i8 iKeySize;
	i8 nSrcSize;

	// marker-start
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		// skipping key column __COLUMN_NO__
		iKeySize += 8;

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
		char* m_acStrArea;
		i8    m_nStringAreaSize;
  };

  struct __STRUCT_TO__* p__STRUCT_TO__;

	RowInfo __ROW_INFO_TO__;

  char* szSrcStz;
  char* szDstStz;
  char* azStringDyn;

  void* amCellAddress;

  i8 iElemSize;
  i8 nSrcSize;
  i8 iOffset;
  i8 nMaxStrAreaLen;

  // marker-start
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // column __COLUMN_NO__

  szSrcStz = ((char*)(&(((struct Ent_int*)aDynamicT->m_xIndexInfoPL.m_aEntInt)->m_iIntElm0)) + 8 + iElemSize);

  if (*(i8*)szSrcStz == C_DATUM_NULL) {
    p__STRUCT_TO__->__COLUMN_TO__.m_iAsI8 = C_DATUM_NULL;
    iElemSize += 8;
  }
  else {
		szDstStz = (char*)(azStringDyn + sizeof(i8));

		i8 iDstBufLen = __ROW_INFO_TO__.m_nStringAreaSize;

#ifdef WITH_CLANG
    nSrcSize = iExtern_StrLibWordFlipBack(szDstStz, nMaxStrAreaLen, szSrcStz);

//    for (i8 i_Byte = 0; i_Byte < 16; i_Byte++)
//      printf("%c ", szDstStz[i_Byte]);
//    printf("\n");
#endif

    // save the string offset
    amCellAddress = (m8*) &p__STRUCT_TO__->__COLUMN_TO__.m_iAsI8;
    iOffset = - nDifference(azStringDyn, amCellAddress);
    p__STRUCT_TO__->__COLUMN_TO__.m_iAsI8 = iOffset;

    // Store the len of string copied
    i8 iDstLen = strlen(szDstStz) + 1;
    i8* pnArea = (i8*) azStringDyn;
    *pnArea = iDstLen;

    iDstLen += sizeof(i8);

    // update the tub string size
    __ROW_INFO_TO__.m_nStringAreaSize -= iDstLen;
    __ROW_INFO_TO__.m_acStrArea += iDstLen;

    // Add SrcSize to ElemSize since payload keys are on 8 byte boundries
    // SrcSize is rounded up to an 8 byte value
    iElemSize += nSrcSize;

#ifdef WITH_CLANG
    // Below function uses nSrcSize to update Pds
    // So replace nSrcSize with iDstLen
    nSrcSize = iDstLen;

    // Take string size bytes from Head area of Pds
    __EXTERN_ROWSTRSIZE__;
#endif
    // update string address to point to the next string area in the pds
    azStringDyn = azStringDyn + nSrcSize;
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
/*-- vPotTubVchCol -----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vPotTubVchCol
void vPotTubVchCol(Dynamic_T* aDynamicT) {

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

  char* azStringDyn;

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
    // get the destination string

    szDstStz = (char*)(azStringDyn + sizeof(i8));

		nSrcSize = strlen(szSrcStz) + 1;
		char *d = szDstStz;
		const char *s = (char*)szSrcStz;

		i8 iBytesToCopy = nSrcSize - 1;
    while (iBytesToCopy--)
			*d++ = *s++;

    *d = '\0';

    // save the string offset
    amCellAddress = (m8*) &p__STRUCT_TO__->__COLUMN_TO__.m_iAsI8;
    iOffset = - nDifference(azStringDyn, amCellAddress);
    p__STRUCT_TO__->__COLUMN_TO__.m_iAsI8 = iOffset;

    // Update string header with size of string
    i8* pnArea = (i8*) azStringDyn;
    *pnArea = nSrcSize;

    // Add header size to total string length
    nSrcSize += sizeof(i8);

    __ROW_INFO_TO__.m_acStrArea += nSrcSize;

#ifdef WITH_CLANG
    __EXTERN_ROWSTRSIZE__;
#endif

    // update string address to point to the next string area in the pds
    azStringDyn = azStringDyn + nSrcSize;
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

		char *d = szDstStz;
		const char *s = (char*)szSrcStz;

		i8 iBytesToCopy = nSrcSize - 1;
		while (iBytesToCopy--)
			*d++ = *s++;

		*d = '\0';

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
    __EXTERN_ROWSTRSIZE__;
#endif
  }
  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vPayLoadToHash ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vPayLoadToHash
void vPayLoadToHash(Dynamic_T* aDynamicT) {

	i8 __INDEX_COLUMN__;

	i8 iMixVal;
	i8 iKeyHash;

	// marker-start
	  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	  // column __COLUMN_NO__

	  iMixVal = aDynamicT->m_aiPayload[__INDEX_COLUMN__];
#ifdef WITH_CLANG
	  vExtern_RunningHash(&iKeyHash, iMixVal);
#endif

	// marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vPayLoadToTubCol --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vPayLoadToTubCol
void vPayLoadToTubCol(Dynamic_T* aDynamicT) {

  i8 __INDEX_COLUMN__;

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
  char* azStringDyn;
  m8* amCellAddress;

	RowInfo __ROW_INFO_TO__;

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
     szDstStz = (char*)(azStringDyn + sizeof(i8));

		 char *d = szDstStz;
		 const char *s = (char*)szSrcStz;

		 i8 iBytesToCopy = nSrcSize - 1;
		 while (iBytesToCopy--)
			 *d++ = *s++;

		 *d = '\0';

     // save the string offset
     amCellAddress = (m8*) &p__STRUCT_TO__->__COLUMN_TO__.m_iAsI8;
     iOffset = - nDifference(azStringDyn, amCellAddress);
     p__STRUCT_TO__->__COLUMN_TO__.m_iAsI8 = iOffset;

     // Update string header with size of string
     i8* pnArea = (i8*) azStringDyn;
     *pnArea = nSrcSize;

     // Add header size to total string length
     nSrcSize += sizeof(i8);

     // update the tub string size
     __ROW_INFO_TO__.m_acStrArea += nSrcSize;

#ifdef WITH_CLANG
     // Take string size bytes from Head area of Pds
     __EXTERN_ROWSTRSIZE__;
#endif

    // update string address to point to the next string area in the pds
     azStringDyn = azStringDyn + nSrcSize;
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
    nSrcSize = strlen(szSrcStz) + 1;
#ifdef WITH_CLANG
    aDynamicT->m_aiPayload[__INDEX_COLUMN__] = (i8)aExtern_IndexTreeAllotPayStr(
        aDynamicT->m_xIndexInfoPL.m_aIndexTree, szSrcStz);
#endif

		// get the destination string
    szDstStz = (char*)aDynamicT->m_aiPayload[__INDEX_COLUMN__];

		char *d = szDstStz;
		const char *s = (char*)szSrcStz;

		i8 iBytesToCopy = nSrcSize - 1;
		while (iBytesToCopy--)
			*d++ = *s++;

		*d = '\0';
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
  if ( ! aDynamicT->m_bProcessRow ) {
    aDynamicT->m_bProcessRow = true;
    goto __L_LABEL_LOOP__;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // populate the payload key
  aDynamicT->m_xIndexInfoPL.m_aEntInt->m_iIntElm0 = 0;

  // marker-end

  return 0;

}

/*----------------------------------------------------------------------------*/
/*-- vEosPotTubLimit ---------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vEosPotTubLimit
void vEosPotTubLimit(Dynamic_T* aDynamicT) {

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
