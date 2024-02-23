/*============================================================================*/
/* Code.cpp -- Code Fragments                                                 */
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
/*-- iGbyAggBegin ------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iGbyAggBegin
i8 iGbyAggBegin(Dynamic_T* aDynamicT) {

  struct __STRUCT_FROM__* p__STRUCT_FROM__;
  RowInfo __ROW_INFO_FROM__;

  char* szStrArea;

  __L_LABEL_LOOP__:

  // marker-start

  __L_LABEL__:

  p__STRUCT_FROM__ = (struct __STRUCT_FROM__*)__ROW_INFO_FROM__.m_aRowCellArray;
  szStrArea = __ROW_INFO_FROM__.m_acStrArea;

  // marker-end

  return 0;
}

/*----------------------------------------------------------------------------*/
/*-- vGbyAggFindKey ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vGbyAggFindKey
void vGbyAggFindKey(Dynamic_T* aDynamicT) {

  void* aEntFound;

  // marker-start
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // find key in payload index
#ifdef WITH_CLANG
  aEntFound = aExtern_IndexTreeFindEntry(aDynamicT->m_xIndexInfoPL.m_aIndexTree, aDynamicT->m_xIndexInfoPL.m_aEntKey);
#endif
  if ( !aEntFound ) {
    // marker-end
  }

}

/*----------------------------------------------------------------------------*/
/*-- vGbyAggFindKeyAsc -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vGbyAggFindKeyAsc
void vGbyAggFindKeyAsc(Dynamic_T* aDynamicT) {

  void *aEntFound;

  // marker-start
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // find key in payload index
#ifdef WITH_CLANG
  aEntFound = aDynamicT->m_fp_aExtern_IndexTreeFindEntryAsc(aDynamicT->m_xIndexInfoPL.m_aIndexTree, aDynamicT->m_xIndexInfoPL.m_aEntKey);
#endif
  if (!aEntFound) {
    // marker-end
  }
}

  /*----------------------------------------------------------------------------*/
/*-- vGbyAggFindKeyAscDsc ------------------------------------------------------*/
/*------------------------------------------------------------------------------*/
// function-vGbyAggFindKeyAscDsc
  void vGbyAggFindKeyAscDsc(Dynamic_T* aDynamicT) {

    void *aEntFound;

    // marker-start
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // find key in payload index
#ifdef WITH_CLANG
    aEntFound = aExtern_IndexTreeFindEntryAscDsc(aDynamicT->m_xIndexInfoPL.m_aIndexTree, aDynamicT->m_xIndexInfoPL.m_aEntKey);
#endif
    if (!aEntFound) {
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

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // insert key in the payload
#ifdef WITH_CLANG
    aEntInserted = aExtern_IndexTreeInsertEnt(
        aDynamicT->m_xIndexInfoPL.m_aIndexTree, aDynamicT->m_xIndexInfoPL.m_aEntInt, &aDynamicT->m_iErrorCode);
#endif

    // check for error
    if (aDynamicT->m_iErrorCode != (i8)EC_NONE) {
      return 0;
    }

    // did we insert the key ?
    if (aEntInserted) {
      aDynamicT->m_iRowsProcessed++;
      aDynamicT->m_aiPayload = (i8 *) ((char *) aEntInserted
          + sizeof(struct Ent_int)
          + 8);
     }

     aEntFound = aEntInserted;

// marker-end

  return 0;

}

/*----------------------------------------------------------------------------*/
/*-- iGbyAggIntKeyEnd --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iGbyAggIntKeyEnd
i8 iGbyAggIntKeyEnd(Dynamic_T* aDynamicT) {

  void* aEntFound;

  {
  // marker-start

    // check for error
    if (aDynamicT->m_iErrorCode != (i8) EC_NONE) {
      return 0;
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // aggregate the payload
  aDynamicT->m_aiPayload = (i8 *) ((char *) aEntFound
                  + sizeof(struct Ent_int)
                  + 8);

  // marker-end

 return 0;

}

/*----------------------------------------------------------------------------*/
/*-- iGbyAggGenKeyEnd --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iGbyAggGenKeyEnd
i8 iGbyAggGenKeyEnd(Dynamic_T* aDynamicT) {

  void* aEntFound;

  {
  // marker-start

    // check for error
    if (aDynamicT->m_iErrorCode != (i8) EC_NONE) {
      return 0;
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // aggregate the payload
  aDynamicT->m_aiPayload = (i8 *) ((char *) aEntFound
                  + sizeof(struct Ent_int)
                  + ((struct Ent_int*)aEntFound)->m_iIntElm0
                  + 8);

  // marker-end

 return 0;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
