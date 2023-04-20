/*============================================================================*/
/* IndexTree_CS2.cpp -- Index Tree                                            */
/*============================================================================*/

#include "BASE.h"

#include "IndexDef.hpp"

#include "Pay.hpp"
#include "Key.hpp"
#include "Ent.hpp"

#include "RowCoder.hpp"
#include "HeadLine.hpp"
#include "TailLine.hpp"

#include "Crw.hpp"
#include "CrwPacker.hpp"
#include "CrwBuffer.hpp"
#include "RowInflator.hpp"

#include "EntBufSorter.hpp"

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "CrvBuffer.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"


/*----------------------------------------------------------------------------*/
/* find an entry in the indexTree                                             */
/*----------------------------------------------------------------------------*/
Ent* IndexTree::aFindEntry(IndexTree* pIndexTree, Key* aKey)
{

#if 0

  return aFindEntry_CS2_v1(pIndexTree,aKey);

#else

  EntBuffer* pEntBuffer = nullptr;
  eKC eCmp;

  //pIndexTree->vDump(false);

  IndexDef* pIndexDef = pIndexTree->pIndexDef();

  Ent* aEnt = pIndexTree->aPositionPriorEntry(aKey, &pEntBuffer);
  if ( !aEnt )
    return nullptr;

  aEnt = EntBuffer::pMoveToNextEntry(aEnt, &pEntBuffer);
  if ( !aEnt )
    return nullptr;

  Key* aEntKey = aEnt->aKey();

  if (KE_GENERAL == aEnt->eKind()) {
    eCmp = Key::eCompare(pIndexDef, aEntKey,aKey);
  }
  else {
    i8 iEntVal = *(i8 *) aEntKey;
    i8 iWrkVal = *(i8 *) aKey;

    if (iEntVal < iWrkVal)
      eCmp = KC_BEFORE;
    else if (iEntVal == iWrkVal)
      eCmp = KC_SAME;
    else
      eCmp = KC_AFTER;
  }

  if (eCmp == KC_SAME)
    return aEnt;
  else
    return nullptr;

#endif
}

/*----------------------------------------------------------------------------*/
/* find an entry in the indexTree                                         CS2 */
/*----------------------------------------------------------------------------*/
Ent* IndexTree::aFindEntry_CS2_v1(IndexTree* pIndexTree, Key* aKey)
{
  EntBuffer* pEntBuffer = nullptr;
  eKC eCmp;

  //pIndexTree->vDump(false);

  ///////////////////////////////////////////////////////////////////////////

  IndexDef* pIndexDef = pIndexTree->pIndexDef();

  ///////////////////////////////////////////////////////////////////////////

  Ent* aEnt = pIndexTree->aPositionPriorEntry(aKey, &pEntBuffer);
  if ( !aEnt )
    return nullptr;

  ///////////////////////////////////////////////////////////////////////////
  /// FLATTENED:

//  aEnt = EntBuffer::pMoveToNextEntry(aEnt, &pEntBuffer);
//  if ( !aEnt )
//    return nullptr;

  /*-----------------------------------------------------------------------*/

  // move to next L0 entry (across buffers if necessary)

  if (! (aEnt = pEntBuffer->aNextEntry(aEnt,0)) ) {
    // - no more entries in buffer

    // move to the next buffer
    if (! (pEntBuffer = pEntBuffer->pNextEntBuffer()) )
      return nullptr;

    // move to first entry in that buffer
    if (! (aEnt = pEntBuffer->aNextEntry(pEntBuffer->aSkipRoots(),0)) )
      return nullptr;
  }

  ///////////////////////////////////////////////////////////////////////////

  Key* aEntKey = aEnt->aKey();

  if (KE_GENERAL == aEnt->eKind()) {
    eCmp = Key::eCompare(pIndexDef, aEntKey,aKey);
  }
  else {
    i8 iEntVal = *(i8 *) aEntKey;
    i8 iWrkVal = *(i8 *) aKey;

    if (iEntVal < iWrkVal)
      eCmp = KC_BEFORE;
    else if (iEntVal == iWrkVal)
      eCmp = KC_SAME;
    else
      eCmp = KC_AFTER;
  }

  if (eCmp == KC_SAME)
    return aEnt;
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find an entry in the indexTree                                         CS2 */
/*----------------------------------------------------------------------------*/
Ent* IndexTree::aFindEntry_CS2_v2(IndexTree* pIndexTree, Key* aKey)
{

  ///////////////////////////////////////////////////////////////////////////
  /// MOVED INTO IF BELOW

  //IndexDef* pIndexDef = pIndexTree->pIndexDef();

  ///////////////////////////////////////////////////////////////////////////

  EntBuffer* pEntBuffer = nullptr;
  Ent* aEnt = pIndexTree->aPositionPriorEntry(aKey, &pEntBuffer);
  if (! aEnt)
    return nullptr;

  ///////////////////////////////////////////////////////////////////////////

  // move to next L0 entry (across buffers if necessary)

  if (! (aEnt = pEntBuffer->aNextEntry(aEnt,0)) ) {
    // - no more entries in buffer

    // move to the next buffer
    if (! (pEntBuffer = pEntBuffer->pNextEntBuffer()) )
      return nullptr;

    // move to first entry in that buffer
    if (! (aEnt = pEntBuffer->aNextEntry(pEntBuffer->aSkipRoots(),0)) )
      return nullptr;
  }

  ///////////////////////////////////////////////////////////////////////////

  Key* aEntKey = aEnt->aKey();

  eKC eCmp;

  if (KE_GENERAL == aEnt->eKind()) {
    IndexDef* pIndexDef = pIndexTree->pIndexDef();
    eCmp = Key::eCompare(pIndexDef, aEntKey,aKey);
  }
  else {
    i8 iEntVal = *(i8 *) aEntKey;
    i8 iWrkVal = *(i8 *) aKey;

    if (iEntVal < iWrkVal)
      eCmp = KC_BEFORE;
    else if (iEntVal == iWrkVal)
      eCmp = KC_SAME;
    else
      eCmp = KC_AFTER;
  }

  if (eCmp == KC_SAME)
    return aEnt;
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find an entry in the indexTree                                         CS2 */
/*----------------------------------------------------------------------------*/
Ent* IndexTree::aFindEntry_CS2_v3(IndexTree* pIndexTree, Key* aKey)
{

  ///////////////////////////////////////////////////////////////////////////

  EntBuffer* pEntBuffer = nullptr;
  Ent* aEnt = pIndexTree->aPositionPriorEntry(aKey, &pEntBuffer);
  if (! aEnt)
    return nullptr;

  ///////////////////////////////////////////////////////////////////////////

  // move to next L0 entry (across buffers if necessary)

  if (! (aEnt = pEntBuffer->aNextEntry(aEnt,0)) ) {
    // - no more entries in buffer

    // move to the next buffer
    if (! (pEntBuffer = pEntBuffer->pNextEntBuffer()) )
      return nullptr;

    // move to first entry in that buffer
    if (! (aEnt = pEntBuffer->aNextEntry(pEntBuffer->aSkipRoots(),0)) )
      return nullptr;
  }

  ///////////////////////////////////////////////////////////////////////////
  /// TIDY THIS UP

  Key* aEntKey = aEnt->aKey();

  if (KE_GENERAL == aEnt->eKind()) {
    IndexDef* pIndexDef = pIndexTree->pIndexDef();
    if (KC_SAME == Key::eCompare(pIndexDef, aEntKey,aKey))
      return aEnt;
    else
      return nullptr;
  }

  i8 iEntVal = *(i8 *) aEntKey;
  i8 iWrkVal = *(i8 *) aKey;

  if (iEntVal < iWrkVal)
    return nullptr; // eCmp = KC_BEFORE;
  else if (iEntVal == iWrkVal)
    return aEnt;    // eCmp = KC_SAME;
  else
    return nullptr; // eCmp = KC_AFTER;
}

/*----------------------------------------------------------------------------*/
/* find an INT entry in the indexTree                                     CS2 */
/*----------------------------------------------------------------------------*/
Ent* IndexTree::aFindEntry_CS2_v4(IndexTree* pIndexTree, Key* aKey)
{

  ///////////////////////////////////////////////////////////////////////////

  EntBuffer* pEntBuffer = nullptr;
  Ent* aEnt = pIndexTree->aPositionPriorEntry(aKey, &pEntBuffer);
  if (! aEnt)
    return nullptr;

  ///////////////////////////////////////////////////////////////////////////

  // move to next L0 entry (across buffers if necessary)

  if (! (aEnt = pEntBuffer->aNextEntry(aEnt,0)) ) {
    // - no more entries in buffer

    // move to the next buffer
    if (! (pEntBuffer = pEntBuffer->pNextEntBuffer()) )
      return nullptr;

    // move to first entry in that buffer
    if (! (aEnt = pEntBuffer->aNextEntry(pEntBuffer->aSkipRoots(),0)) )
      return nullptr;
  }

  ///////////////////////////////////////////////////////////////////////////
  /// TIDY THIS UP (3)

  // general case
//  if (KE_GENERAL == aEnt->eKind()) {
//    IndexDef* pIndexDef = pIndexTree->pIndexDef();
//    if (KC_SAME == Key::eCompare(pIndexDef, aEntKey,aKey))
//      return aEnt;
//    else
//      return nullptr;
//  }

  // integer case
  Key* aEntKey = aEnt->aKey();
  ASSERT(KE_INT == aEnt->eKind());

  i8 iEntVal = *(i8 *) aEntKey;
  i8 iWrkVal = *(i8 *) aKey;

  if (iEntVal == iWrkVal)
    return aEnt;
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find an INT entry in the indexTree                                     CS2 */
/*----------------------------------------------------------------------------*/
Ent* IndexTree::aFindEntry_CS2_v5(IndexTree* pIndexTree, Key* aKey)
{

  ///////////////////////////////////////////////////////////////////////////

  // position on entry prior to given key

  EntBuffer *pEntBuffer = nullptr;
  Ent *aEnt = pIndexTree->aPositionPriorEntry(aKey, &pEntBuffer);
  if (!aEnt)
    return nullptr;

  ///////////////////////////////////////////////////////////////////////////

  // move to next L0 entry (across buffers if necessary)

  tET tNextEntry = aEnt->m_taNextEntry[0];
  if (C_ENT_END_TICKET != tNextEntry) {
    // + more entries in buffer

    i8 iNextEntryOffset = (i8) tNextEntry * C_ENT_QUANTA;
    aEnt = (Ent *) ((char *) pEntBuffer + 624 + iNextEntryOffset);
  }
  else {
    // - no more entries in buffer

    // move to the next buffer
    if (! (pEntBuffer = pEntBuffer->pNextEntBuffer()) )
      return nullptr;

    // move to first entry in that buffer
    if (! (aEnt = pEntBuffer->aNextEntry(pEntBuffer->aSkipRoots(),0)) )
      return nullptr;
  }

  ///////////////////////////////////////////////////////////////////////////
  /// FLATTEN Ent::aKey()

//  Key* aEntKey = aEnt->aKey();
  Key* aEntKey = &aEnt->m_xKey;
  ASSERT(KE_INT == aEnt->eKind());

  i8 iEntVal = *(i8 *) aEntKey;
  i8 iWrkVal = *(i8 *) aKey;

  if (iEntVal == iWrkVal)
    return aEnt;
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find an INT entry in the indexTree                                     CS2 */
/*----------------------------------------------------------------------------*/
Ent* IndexTree::aFindEntry_CS2_v6(IndexTree* pIndexTree, Key* aKey)
{

  ///////////////////////////////////////////////////////////////////////////
  /// FLATTEN IndexTree::aPositionPriorEntry()

  // position on entry prior to given key

//  EntBuffer *pEntBuffer = nullptr;
//  Ent *aEnt = pIndexTree->aPositionPriorEntry(aKey, &pEntBuffer);
//  if (!aEnt)
//    return nullptr;

  /*-----------------------------------------------------------------------*/

  // root buffer ?
  if (! pIndexTree->m_pRootEntBuffer)
    return nullptr;

  // position at prior entry in H0 buffer
  EntBuffer* pEntBuffer = pIndexTree->m_pRootEntBuffer->pFindLeafBuffer(aKey);
  Ent* aEnt = pEntBuffer->aFindEntryBefore(aKey);
  if (! aEnt)
    return nullptr;


  ///////////////////////////////////////////////////////////////////////////

  // move to next L0 entry (across buffers if necessary)

  tET tNextEntry = aEnt->m_taNextEntry[0];
  if (C_ENT_END_TICKET != tNextEntry) {
    // + more entries in buffer

    i8 iNextEntryOffset = (i8) tNextEntry * C_ENT_QUANTA;
    aEnt = (Ent *) ((char *) pEntBuffer + 624 + iNextEntryOffset);
  }
  else {
    // - no more entries in buffer

    // move to the next buffer
    if (! (pEntBuffer = pEntBuffer->pNextEntBuffer()) )
      return nullptr;

    // move to first entry in that buffer
    if (! (aEnt = pEntBuffer->aNextEntry(pEntBuffer->aSkipRoots(),0)) )
      return nullptr;
  }

  ///////////////////////////////////////////////////////////////////////////

  Key* aEntKey = &aEnt->m_xKey;
  ASSERT(KE_INT == aEnt->eKind());

  i8 iEntVal = *(i8 *) aEntKey;
  i8 iWrkVal = *(i8 *) aKey;

  if (iEntVal == iWrkVal)
    return aEnt;
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find an INT entry in the indexTree                                     CS2 */
/*----------------------------------------------------------------------------*/
Ent* IndexTree::aFindEntry_CS2_v7(IndexTree* pIndexTree, Key* aKey)
{

  ///////////////////////////////////////////////////////////////////////////

  // find H0 which would contain key

  // root buffer ?
  if (! pIndexTree->m_pRootEntBuffer)
    return nullptr;

  // position at prior entry in H0 buffer
  EntBuffer* pEntBuffer = pIndexTree->m_pRootEntBuffer->pFindLeafBuffer(aKey);

  ///////////////////////////////////////////////////////////////////////////
  /// FLATTEN EntBuffer::aFindEntryBefore_int()

  // position on entry prior to given key

//  Ent* aEnt = pEntBuffer->aFindEntryBefore(aKey);
//  if (! aEnt)
//    return nullptr;

  i8 nLvlLevels = AM::nLvlLevels();

  Ent* aEnt = nullptr;

  if (pEntBuffer->nItems() == 0) {
    aEnt = pEntBuffer->aSkipRoots();
    goto L_NEXT_L0_ENTRY;
  }

  // have to declare these because of goto
  Ent_int* aItemArray;
  Ent* aSkipRoots;
  i8 iGivenIntElm0;
  aSkipRoots = pEntBuffer->aSkipRoots();
  aItemArray = (Ent_int*)pEntBuffer->aItemArray();
  iGivenIntElm0 = ELM0_OF_KEY(aKey);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Ent_int* aNextEnt_int;
  i8 iNextEnt_IntElm0;
  Ent_int* aScanEnt;

  aScanEnt = (Ent_int*)aSkipRoots;
  for (i4 i_Level = (i4)nLvlLevels - 1; i_Level >= 0; i_Level--) {

    for (tET tNextEnt = NEXT_TICKET(aScanEnt,i_Level);
         ! END_TICKET(tNextEnt);
         tNextEnt = NEXT_TICKET(aScanEnt,i_Level)) {

      aNextEnt_int = GET_ENTRY(tNextEnt);
      iNextEnt_IntElm0 = ELM0_OF_ENTRY(aNextEnt_int);

      //RASSERT( ! pIndexDef()->bHasDescending() );
      if (SCAN_IS_AT_PRIOR_POS(iGivenIntElm0,iNextEnt_IntElm0))
        break;

      aScanEnt = aNextEnt_int;
    }

  }

  aEnt = (Ent*)aScanEnt;

  ///////////////////////////////////////////////////////////////////////////

  L_NEXT_L0_ENTRY:

  // move to next L0 entry (across buffers if necessary)

  tET tNextEntry = aEnt->m_taNextEntry[0];
  if (C_ENT_END_TICKET != tNextEntry) {
    // + more entries in buffer

    i8 iNextEntryOffset = (i8) tNextEntry * C_ENT_QUANTA;
    aEnt = (Ent *) ((char *) pEntBuffer + 624 + iNextEntryOffset);
  }
  else {
    // - no more entries in buffer

    // move to the next buffer
    if (! (pEntBuffer = pEntBuffer->pNextEntBuffer()) )
      return nullptr;

    // move to first entry in that buffer
    if (! (aEnt = pEntBuffer->aNextEntry(pEntBuffer->aSkipRoots(),0)) )
      return nullptr;
  }

  ///////////////////////////////////////////////////////////////////////////

  Key* aEntKey = &aEnt->m_xKey;
  ASSERT(KE_INT == aEnt->eKind());

  i8 iEntVal = *(i8 *) aEntKey;
  i8 iWrkVal = *(i8 *) aKey;

  if (iEntVal == iWrkVal)
    return aEnt;
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find an INT entry in the indexTree                                     CS2 */
/*----------------------------------------------------------------------------*/
Ent* IndexTree::aFindEntry_CS2_v8(IndexTree* pIndexTree, Key* aKey)
{

  ///////////////////////////////////////////////////////////////////////////

  // find H0 which would contain key

  // root buffer ?
  if (! pIndexTree->m_pRootEntBuffer)
    return nullptr;

  // position at prior entry in H0 buffer
  EntBuffer* pEntBuffer = pIndexTree->m_pRootEntBuffer->pFindLeafBuffer(aKey);

  ///////////////////////////////////////////////////////////////////////////
  /// FLATTEN EntBuffer::aSkipRoots()

  // position on entry prior to given key

  i8 nLvlLevels = AM::nLvlLevels();

  Ent* aEnt = nullptr;

  if (pEntBuffer->nItems() == 0) {
    aEnt = & pEntBuffer->m_xSkipRoots;
    goto L_NEXT_L0_ENTRY;
  }

  // have to declare these because of goto
  Ent_int* aItemArray;
  Ent* aSkipRoots;
  i8 iGivenIntElm0;
  aSkipRoots = & pEntBuffer->m_xSkipRoots;
  aItemArray = (Ent_int*)pEntBuffer->aItemArray();
  iGivenIntElm0 = ELM0_OF_KEY(aKey);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Ent_int* aNextEnt_int;
  i8 iNextEnt_IntElm0;
  Ent_int* aScanEnt;

  aScanEnt = (Ent_int*)aSkipRoots;
  for (i4 i_Level = (i4)nLvlLevels - 1; i_Level >= 0; i_Level--) {
    for (tET tNextEnt = NEXT_TICKET(aScanEnt,i_Level);
         ! END_TICKET(tNextEnt);
         tNextEnt = NEXT_TICKET(aScanEnt,i_Level)) {

      aNextEnt_int = GET_ENTRY(tNextEnt);
      iNextEnt_IntElm0 = ELM0_OF_ENTRY(aNextEnt_int);

      //RASSERT( ! pIndexDef()->bHasDescending() );
      if (SCAN_IS_AT_PRIOR_POS(iGivenIntElm0,iNextEnt_IntElm0))
        break;

      aScanEnt = aNextEnt_int;
    }
  }
  aEnt = (Ent*)aScanEnt;

  ///////////////////////////////////////////////////////////////////////////

  L_NEXT_L0_ENTRY:

  // move to next L0 entry (across buffers if necessary)

  tET tNextEntry = aEnt->m_taNextEntry[0];
  if (C_ENT_END_TICKET != tNextEntry) {
    // + more entries in buffer

    i8 iNextEntryOffset = (i8) tNextEntry * C_ENT_QUANTA;
    aEnt = (Ent *) ((char *) pEntBuffer + 624 + iNextEntryOffset);
  }
  else {
    // - no more entries in buffer

    // move to the next buffer
    if (! (pEntBuffer = pEntBuffer->pNextEntBuffer()) )
      return nullptr;

    // move to first entry in that buffer
    if (! (aEnt = pEntBuffer->aNextEntry(&pEntBuffer->m_xSkipRoots,0)) )
      return nullptr;
  }

  ///////////////////////////////////////////////////////////////////////////

  Key* aEntKey = &aEnt->m_xKey;
  ASSERT(KE_INT == aEnt->eKind());

  i8 iEntVal = *(i8 *) aEntKey;
  i8 iWrkVal = *(i8 *) aKey;

  if (iEntVal == iWrkVal)
    return aEnt;
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find an INT entry in the indexTree                                     CS2 */
/*----------------------------------------------------------------------------*/
Ent* IndexTree::aFindEntry_CS2_v9(IndexTree* pIndexTree, Key* aKey)
{

  ///////////////////////////////////////////////////////////////////////////

  // find H0 which would contain key

  // root buffer ?
  if (! pIndexTree->m_pRootEntBuffer)
    return nullptr;

  // position at prior entry in H0 buffer
  EntBuffer* pEntBuffer = pIndexTree->m_pRootEntBuffer->pFindLeafBuffer(aKey);

  ///////////////////////////////////////////////////////////////////////////
  /// FLATTEN MiniHeap_t<ITEM>::nItems() ; aItemArray()

  // position on entry prior to given key

  i8 nLvlLevels = AM::nLvlLevels();

  Ent* aEnt = nullptr;

  if (0 == pEntBuffer->m_nItems) {
    aEnt = & pEntBuffer->m_xSkipRoots;
    goto L_NEXT_L0_ENTRY;
  }

  // declare these separately because of goto
  Ent_int* aItemArray;
  Ent* aSkipRoots;
  i8 iGivenIntElm0;
  Ent_int* aNextEnt_int;
  i8 iNextEnt_IntElm0;
  Ent_int* aScanEnt;

  aSkipRoots = & pEntBuffer->m_xSkipRoots;
  aItemArray = (Ent_int*)pEntBuffer->m_xWsp.ayArea();
  iGivenIntElm0 = ELM0_OF_KEY(aKey);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  aScanEnt = (Ent_int*)aSkipRoots;
  for (i4 i_Level = (i4)nLvlLevels - 1; i_Level >= 0; i_Level--) {
    for (tET tNextEnt = NEXT_TICKET(aScanEnt,i_Level);
         ! END_TICKET(tNextEnt);
         tNextEnt = NEXT_TICKET(aScanEnt,i_Level)) {

      aNextEnt_int = GET_ENTRY(tNextEnt);
      iNextEnt_IntElm0 = ELM0_OF_ENTRY(aNextEnt_int);

      //RASSERT( ! pIndexDef()->bHasDescending() );
      if (SCAN_IS_AT_PRIOR_POS(iGivenIntElm0,iNextEnt_IntElm0))
        break;

      aScanEnt = aNextEnt_int;
    }
  }
  aEnt = (Ent*)aScanEnt;

  ///////////////////////////////////////////////////////////////////////////

  L_NEXT_L0_ENTRY:

  // move to next L0 entry (across buffers if necessary)

  tET tNextEntry = aEnt->m_taNextEntry[0];
  if (C_ENT_END_TICKET != tNextEntry) {
    // + more entries in buffer

    i8 iNextEntryOffset = (i8) tNextEntry * C_ENT_QUANTA;
    aEnt = (Ent *) ((char *) pEntBuffer + 624 + iNextEntryOffset);
  }
  else {
    // - no more entries in buffer

    // move to the next buffer
    if (! (pEntBuffer = pEntBuffer->pNextEntBuffer()) )
      return nullptr;

    // move to first entry in that buffer
    if (! (aEnt = pEntBuffer->aNextEntry(&pEntBuffer->m_xSkipRoots,0)) )
      return nullptr;
  }

  ///////////////////////////////////////////////////////////////////////////

  Key* aEntKey = &aEnt->m_xKey;
  ASSERT(KE_INT == aEnt->eKind());

  i8 iEntVal = *(i8 *) aEntKey;
  i8 iWrkVal = *(i8 *) aKey;

  if (iEntVal == iWrkVal)
    return aEnt;
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find an INT entry in the indexTree                                     CS2 */
/*----------------------------------------------------------------------------*/
Ent* IndexTree::aFindEntry_CS2_v10(IndexTree* pIndexTree, Key* aKey)
{

  ///////////////////////////////////////////////////////////////////////////

  // find H0 which would contain key

  // root buffer ?
  if (! pIndexTree->m_pRootEntBuffer)
    return nullptr;

  // position at prior entry in H0 buffer
  EntBuffer* pEntBuffer = pIndexTree->m_pRootEntBuffer->pFindLeafBuffer(aKey);

  ///////////////////////////////////////////////////////////////////////////
  /// FLATTEN m_xWsp.ayArea()

  // position on entry prior to given key

  i8 nLvlLevels = AM::nLvlLevels();

  Ent* aEnt = nullptr;

  if (0 == pEntBuffer->m_nItems) {
    aEnt = & pEntBuffer->m_xSkipRoots;
    goto L_NEXT_L0_ENTRY;
  }

  // declare these separately because of goto
  Ent_int* aItemArray;
  Ent* aSkipRoots;
  i8 iGivenIntElm0;
  Ent_int* aNextEnt_int;
  i8 iNextEnt_IntElm0;
  Ent_int* aScanEnt;

  aSkipRoots = & pEntBuffer->m_xSkipRoots;
  aItemArray = (Ent_int*)pEntBuffer->m_xWsp.m_yaByteArea;
  iGivenIntElm0 = ELM0_OF_KEY(aKey);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  aScanEnt = (Ent_int*)aSkipRoots;
  for (i4 i_Level = (i4)nLvlLevels - 1; i_Level >= 0; i_Level--) {
    for (tET tNextEnt = NEXT_TICKET(aScanEnt,i_Level);
         ! END_TICKET(tNextEnt);
         tNextEnt = NEXT_TICKET(aScanEnt,i_Level)) {

      aNextEnt_int = GET_ENTRY(tNextEnt);
      iNextEnt_IntElm0 = ELM0_OF_ENTRY(aNextEnt_int);

      //RASSERT( ! pIndexDef()->bHasDescending() );
      if (SCAN_IS_AT_PRIOR_POS(iGivenIntElm0,iNextEnt_IntElm0))
        break;

      aScanEnt = aNextEnt_int;
    }
  }
  aEnt = (Ent*)aScanEnt;

  ///////////////////////////////////////////////////////////////////////////

  L_NEXT_L0_ENTRY:

  // move to next L0 entry (across buffers if necessary)

  tET tNextEntry = aEnt->m_taNextEntry[0];
  if (C_ENT_END_TICKET != tNextEntry) {
    // + more entries in buffer

    i8 iNextEntryOffset = (i8) tNextEntry * C_ENT_QUANTA;
    aEnt = (Ent *) ((char *) pEntBuffer + 624 + iNextEntryOffset);
  }
  else {
    // - no more entries in buffer

    // move to the next buffer
    if (! (pEntBuffer = pEntBuffer->pNextEntBuffer()) )
      return nullptr;

    // move to first entry in that buffer
    if (! (aEnt = pEntBuffer->aNextEntry(&pEntBuffer->m_xSkipRoots,0)) )
      return nullptr;
  }

  ///////////////////////////////////////////////////////////////////////////

  Key* aEntKey = &aEnt->m_xKey;
  ASSERT(KE_INT == aEnt->eKind());

  i8 iEntVal = *(i8 *) aEntKey;
  i8 iWrkVal = *(i8 *) aKey;

  if (iEntVal == iWrkVal)
    return aEnt;
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find an INT entry in the indexTree                                     CS2 */
/*----------------------------------------------------------------------------*/
Ent* IndexTree::aFindEntry_CS2_v11(IndexTree* pIndexTree, Key* aKey)
{

  ///////////////////////////////////////////////////////////////////////////

  // find H0 which would contain key

  // root buffer ?
  if (! pIndexTree->m_pRootEntBuffer)
    return nullptr;

  // position at prior entry in H0 buffer
  EntBuffer* pEntBuffer = pIndexTree->m_pRootEntBuffer->pFindLeafBuffer(aKey);

  ///////////////////////////////////////////////////////////////////////////
  /// REMOVED call to AM::nLvlLevels()

  // position on entry prior to given key in the found H0 leaf buffer

  //i8 nLvlLevels = AM::nLvlLevels();

  Ent* aEnt = nullptr;

  if (0 == pEntBuffer->m_nItems) {
    aEnt = & pEntBuffer->m_xSkipRoots;
    goto L_NEXT_L0_ENTRY;
  }

  // declare these separately because of goto
  Ent_int* aItemArray;
  Ent* aSkipRoots;
  i8 iGivenIntElm0;
  Ent_int* aNextEnt_int;
  i8 iNextEnt_IntElm0;
  Ent_int* aScanEnt;

  aSkipRoots = & pEntBuffer->m_xSkipRoots;
  aItemArray = (Ent_int*)pEntBuffer->m_xWsp.m_yaByteArea;
  iGivenIntElm0 = ELM0_OF_KEY(aKey);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  aScanEnt = (Ent_int*)aSkipRoots;
  for (i4 i_Level = 2; i_Level >= 0; i_Level--) {
    for (tET tNextEnt = NEXT_TICKET(aScanEnt,i_Level);
         ! END_TICKET(tNextEnt);
         tNextEnt = NEXT_TICKET(aScanEnt,i_Level)) {

      aNextEnt_int = GET_ENTRY(tNextEnt);
      iNextEnt_IntElm0 = ELM0_OF_ENTRY(aNextEnt_int);

      //RASSERT( ! pIndexDef()->bHasDescending() );
      if (SCAN_IS_AT_PRIOR_POS(iGivenIntElm0,iNextEnt_IntElm0))
        break;

      aScanEnt = aNextEnt_int;
    }
  }
  aEnt = (Ent*)aScanEnt;

  ///////////////////////////////////////////////////////////////////////////

  L_NEXT_L0_ENTRY:

  // move to next L0 entry (across buffers if necessary)

  tET tNextEntry = aEnt->m_taNextEntry[0];
  if (C_ENT_END_TICKET != tNextEntry) {
    // + more entries in buffer

    i8 iNextEntryOffset = (i8) tNextEntry * C_ENT_QUANTA;
    aEnt = (Ent *) ((char *) pEntBuffer + 624 + iNextEntryOffset);
  }
  else {
    // - no more entries in buffer

    // move to the next buffer
    if (! (pEntBuffer = pEntBuffer->pNextEntBuffer()) )
      return nullptr;

    // move to first entry in that buffer
    if (! (aEnt = pEntBuffer->aNextEntry(&pEntBuffer->m_xSkipRoots,0)) )
      return nullptr;
  }

  ///////////////////////////////////////////////////////////////////////////

  Key* aEntKey = &aEnt->m_xKey;
  ASSERT(KE_INT == aEnt->eKind());

  i8 iEntVal = *(i8 *) aEntKey;
  i8 iWrkVal = *(i8 *) aKey;

  if (iEntVal == iWrkVal)
    return aEnt;
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find an INT entry in the indexTree                                     CS2 */
/*----------------------------------------------------------------------------*/
Ent* IndexTree::aFindEntry_CS2_v12(IndexTree* pIndexTree, Key* aKey)
{

  //pIndexTree->vDump(false);

  ///////////////////////////////////////////////////////////////////////////

  // find H0 which would contain key

  // root buffer ?
  if (! pIndexTree->m_pRootEntBuffer)
    return nullptr;

  // position at prior entry in H0 buffer
  EntBuffer* pEntBuffer = pIndexTree->m_pRootEntBuffer->pFindLeafBuffer(aKey);

  ///////////////////////////////////////////////////////////////////////////

  // position on entry prior to given key in the found H0 leaf buffer

  Ent* aEnt = nullptr;

  if (0 == pEntBuffer->m_nItems) {
    aEnt = & pEntBuffer->m_xSkipRoots;
    goto L_NEXT_L0_ENTRY;
  }

  // declare these separately because of goto
  Ent_int* aItemArray;
  Ent* aSkipRoots;
  i8 iGivenIntElm0;
  Ent_int* aNextEnt_int;
  i8 iNextEnt_IntElm0;
  Ent_int* aScanEnt;

  aSkipRoots = & pEntBuffer->m_xSkipRoots;
  aItemArray = (Ent_int*)pEntBuffer->m_xWsp.m_yaByteArea;
  iGivenIntElm0 = ELM0_OF_KEY(aKey);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  aScanEnt = (Ent_int*)aSkipRoots;
  for (i4 i_Level = 2; i_Level >= 0; i_Level--) {
    for (tET tNextEnt = NEXT_TICKET(aScanEnt,i_Level);
         ! END_TICKET(tNextEnt);
         tNextEnt = NEXT_TICKET(aScanEnt,i_Level)) {

      aNextEnt_int = GET_ENTRY(tNextEnt);
      iNextEnt_IntElm0 = ELM0_OF_ENTRY(aNextEnt_int);

      //RASSERT( ! pIndexDef()->bHasDescending() );
      if (SCAN_IS_AT_PRIOR_POS(iGivenIntElm0,iNextEnt_IntElm0))
        break;

      aScanEnt = aNextEnt_int;
    }
  }
  aEnt = (Ent*)aScanEnt;

  ///////////////////////////////////////////////////////////////////////////
  /// FLATTENING pEntBuffer->aNextEntry(

  L_NEXT_L0_ENTRY:

  // move to next L0 entry (across buffers if necessary)

  tET tNextEntry = aEnt->m_taNextEntry[0];
  if (C_ENT_END_TICKET != tNextEntry) {
    // + more entries in buffer

    i8 iNextEntryOffset = (i8) tNextEntry * C_ENT_QUANTA;
    aEnt = (Ent *) ((char *) pEntBuffer + 624 + iNextEntryOffset);
  }
  else {
    // - no more entries in buffer

    // move to the next buffer
    if (! (pEntBuffer = pEntBuffer->pNextEntBuffer()) )
      return nullptr;

    // move to first entry in that buffer
//    if (! (aEnt = pEntBuffer->aNextEntry(&pEntBuffer->m_xSkipRoots,0)) )
//      return nullptr;

    tNextEntry = pEntBuffer->m_xSkipRoots.m_taNextEntry[0];
    if (C_ENT_END_TICKET == tNextEntry)
      return nullptr;

    i8 iNextEntryOffset = (i8) tNextEntry * C_ENT_QUANTA;
    aEnt = (Ent *) ((char *) pEntBuffer + 624 + iNextEntryOffset);
  }

  ///////////////////////////////////////////////////////////////////////////

  Key* aEntKey = &aEnt->m_xKey;
  ASSERT(KE_INT == aEnt->eKind());

  i8 iEntVal = *(i8 *) aEntKey;
  i8 iWrkVal = *(i8 *) aKey;

  if (iEntVal == iWrkVal)
    return aEnt;
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find an INT entry in the indexTree                                     CS2 */
/*----------------------------------------------------------------------------*/
Ent* IndexTree::aFindEntry_CS2_v13(IndexTree* pIndexTree, Key* aKey)
{

  //pIndexTree->vDump(false);

  ///////////////////////////////////////////////////////////////////////////

  // find H0 which would contain key

  // root buffer ?
  if (! pIndexTree->m_pRootEntBuffer)
    return nullptr;

  // position at prior entry in H0 buffer
  EntBuffer* pEntBuffer = pIndexTree->m_pRootEntBuffer->pFindLeafBuffer(aKey);

  ///////////////////////////////////////////////////////////////////////////

  // position on entry prior to given key in the found H0 leaf buffer

  Ent* aEnt = nullptr;

  if (0 == pEntBuffer->m_nItems) {
    aEnt = & pEntBuffer->m_xSkipRoots;
    goto L_NEXT_L0_ENTRY;
  }

  // declare these separately because of goto
  Ent_int* aItemArray;
  Ent* aSkipRoots;
  i8 iGivenIntElm0;
  Ent_int* aNextEnt_int;
  i8 iNextEnt_IntElm0;
  Ent_int* aScanEnt;

  aSkipRoots = & pEntBuffer->m_xSkipRoots;
  aItemArray = (Ent_int*)pEntBuffer->m_xWsp.m_yaByteArea;
  iGivenIntElm0 = ELM0_OF_KEY(aKey);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  aScanEnt = (Ent_int*)aSkipRoots;
  for (i4 i_Level = 2; i_Level >= 0; i_Level--) {
    for (tET tNextEnt = NEXT_TICKET(aScanEnt,i_Level);
         ! END_TICKET(tNextEnt);
         tNextEnt = NEXT_TICKET(aScanEnt,i_Level)) {

      aNextEnt_int = GET_ENTRY(tNextEnt);
      iNextEnt_IntElm0 = ELM0_OF_ENTRY(aNextEnt_int);

      //RASSERT( ! pIndexDef()->bHasDescending() );
      if (SCAN_IS_AT_PRIOR_POS(iGivenIntElm0,iNextEnt_IntElm0))
        break;

      aScanEnt = aNextEnt_int;
    }
  }
  aEnt = (Ent*)aScanEnt;

  ///////////////////////////////////////////////////////////////////////////
  /// USE OF END_TICKET()
  
  L_NEXT_L0_ENTRY:

  // move to next L0 entry (across buffers if necessary)

  tET tNextEntry = aEnt->m_taNextEntry[0];
  if (! END_TICKET(tNextEntry)) {
    // + more entries in buffer

    i8 iNextEntryOffset = (i8) tNextEntry * C_ENT_QUANTA;
    aEnt = (Ent *) ((char *) pEntBuffer + 624 + iNextEntryOffset);
  }
  else {
    // - no more entries in buffer

    // move to the next buffer
    if (! (pEntBuffer = pEntBuffer->pNextEntBuffer()) )
      return nullptr;

    // move to first entry in that buffer

    tNextEntry = pEntBuffer->m_xSkipRoots.m_taNextEntry[0];
    if (END_TICKET(tNextEntry))
      return nullptr;

    i8 iNextEntryOffset = (i8) tNextEntry * C_ENT_QUANTA;
    aEnt = (Ent *) ((char *) pEntBuffer + 624 + iNextEntryOffset);
  }

  ///////////////////////////////////////////////////////////////////////////

  Key* aEntKey = &aEnt->m_xKey;
  ASSERT(KE_INT == aEnt->eKind());

  i8 iEntVal = *(i8 *) aEntKey;
  i8 iWrkVal = *(i8 *) aKey;

  if (iEntVal == iWrkVal)
    return aEnt;
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find an INT entry in the indexTree                                     CS2 */
/*----------------------------------------------------------------------------*/
Ent* IndexTree::aFindEntry_CS2_v14(IndexTree* pIndexTree, Key* aKey)
{

  //pIndexTree->vDump(false);

  ///////////////////////////////////////////////////////////////////////////

  // find H0 which would contain key

  // root buffer ?
  if (! pIndexTree->m_pRootEntBuffer)
    return nullptr;

  ///////////////////////////////////////////////////////////////////////////
  /// FLATTENIN pFindLeafBuffer_int(

  // position at H0 buffer

  Ent* aEnt;
  EntBuffer* pEntBuffer;

#if 0
  pEntBuffer = pIndexTree->m_pRootEntBuffer->pFindLeafBuffer_int(aKey);

  /*-----------------------------------------------------------------------*/
#else

  pEntBuffer = pIndexTree->m_pRootEntBuffer;

  while (true) {

    if (0 == pEntBuffer->m_iBufferHeight)
      break;

    aEnt = pEntBuffer->aFindEntryBefore_int(aKey);
    ASSERT(aEnt);

    aEnt = pEntBuffer->pFindEntryPrevTo_int(aEnt,aKey);
    ASSERT(aEnt);

    // get the index buffer of the entry
    pEntBuffer = EntBuffer::pRefdEntBuffer(aEnt);
    ASSERT(pEntBuffer);
    ASSERT(pEntBuffer->eGetClassNo() == CN_ENTBUFFER);
  }

#endif

  ///////////////////////////////////////////////////////////////////////////

  // position on entry prior to given key in the found H0 leaf buffer


  if (0 == pEntBuffer->m_nItems) {
    aEnt = & pEntBuffer->m_xSkipRoots;
    goto L_NEXT_L0_ENTRY;
  }

  // declare these separately because of goto
  Ent_int* aItemArray;
  Ent* aSkipRoots;
  i8 iGivenIntElm0;
  Ent_int* aNextEnt_int;
  i8 iNextEnt_IntElm0;
  Ent_int* aScanEnt;

  aSkipRoots = & pEntBuffer->m_xSkipRoots;
  aItemArray = (Ent_int*)pEntBuffer->m_xWsp.m_yaByteArea;
  iGivenIntElm0 = ELM0_OF_KEY(aKey);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  aScanEnt = (Ent_int*)aSkipRoots;
  for (i4 i_Level = 2; i_Level >= 0; i_Level--) {
    for (tET tNextEnt = NEXT_TICKET(aScanEnt,i_Level);
         ! END_TICKET(tNextEnt);
         tNextEnt = NEXT_TICKET(aScanEnt,i_Level)) {

      aNextEnt_int = GET_ENTRY(tNextEnt);
      iNextEnt_IntElm0 = ELM0_OF_ENTRY(aNextEnt_int);

      //RASSERT( ! pIndexDef()->bHasDescending() );
      if (SCAN_IS_AT_PRIOR_POS(iGivenIntElm0,iNextEnt_IntElm0))
        break;

      aScanEnt = aNextEnt_int;
    }
  }
  aEnt = (Ent*)aScanEnt;

  ///////////////////////////////////////////////////////////////////////////

  L_NEXT_L0_ENTRY:

  // move to next L0 entry (across buffers if necessary)

  tET tNextEntry = aEnt->m_taNextEntry[0];
  if (! END_TICKET(tNextEntry)) {
    // + more entries in buffer

    i8 iNextEntryOffset = (i8) tNextEntry * C_ENT_QUANTA;
    aEnt = (Ent *) ((char *) pEntBuffer + 624 + iNextEntryOffset);
  }
  else {
    // - no more entries in buffer

    // move to the next buffer
    if (! (pEntBuffer = pEntBuffer->pNextEntBuffer()) )
      return nullptr;

    // move to first entry in that buffer

    tNextEntry = pEntBuffer->m_xSkipRoots.m_taNextEntry[0];
    if (END_TICKET(tNextEntry))
      return nullptr;

    i8 iNextEntryOffset = (i8) tNextEntry * C_ENT_QUANTA;
    aEnt = (Ent *) ((char *) pEntBuffer + 624 + iNextEntryOffset);
  }

  ///////////////////////////////////////////////////////////////////////////

  Key* aEntKey = &aEnt->m_xKey;
  ASSERT(KE_INT == aEnt->eKind());

  i8 iEntVal = *(i8 *) aEntKey;
  i8 iWrkVal = *(i8 *) aKey;

  if (iEntVal == iWrkVal)
    return aEnt;
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find an INT entry in the indexTree                                     CS2 */
/*----------------------------------------------------------------------------*/
Ent* IndexTree::aFindEntry_CS2_v15(IndexTree* pIndexTree, Key* aKey)
{

  //pIndexTree->vDump(false);

  ///////////////////////////////////////////////////////////////////////////

  // find H0 which would contain key

  // root buffer ?
  if (! pIndexTree->m_pRootEntBuffer)
    return nullptr;

  ///////////////////////////////////////////////////////////////////////////
  /// FLATTENING EntBuffer::pRefdEntBuffer(aEnt);

  // position at H0 buffer

  Ent* aEnt;
  EntBuffer* pEntBuffer;

  pEntBuffer = pIndexTree->m_pRootEntBuffer;

  while (true) {

    if (0 == pEntBuffer->m_iBufferHeight)
      break;

    aEnt = pEntBuffer->aFindEntryBefore_int(aKey);
    ASSERT(aEnt);

    aEnt = pEntBuffer->pFindEntryPrevTo_int(aEnt,aKey);
    ASSERT(aEnt);

    /*-----------------------------------------------------------------------*/

    i8 i_PoolBuffer = aEnt->m_xRef.m_uBuffer;

    HeapBuffer* pHeapBuffer = _pHeapBuffer(FileMap::pToPoolBuffer(i_PoolBuffer));

    pEntBuffer = _pEntBuffer(pHeapBuffer->aFirstChunk());

    ASSERT(pEntBuffer);
    ASSERT(pEntBuffer->eGetClassNo() == CN_ENTBUFFER);
  }

  ///////////////////////////////////////////////////////////////////////////

  // position on entry prior to given key in the found H0 leaf buffer


  if (0 == pEntBuffer->m_nItems) {
    aEnt = & pEntBuffer->m_xSkipRoots;
    goto L_NEXT_L0_ENTRY;
  }

  // declare these separately because of goto
  Ent_int* aItemArray;
  Ent* aSkipRoots;
  i8 iGivenIntElm0;
  Ent_int* aNextEnt_int;
  i8 iNextEnt_IntElm0;
  Ent_int* aScanEnt;

  aSkipRoots = & pEntBuffer->m_xSkipRoots;
  aItemArray = (Ent_int*)pEntBuffer->m_xWsp.m_yaByteArea;
  iGivenIntElm0 = ELM0_OF_KEY(aKey);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  aScanEnt = (Ent_int*)aSkipRoots;
  for (i4 i_Level = 2; i_Level >= 0; i_Level--) {
    for (tET tNextEnt = NEXT_TICKET(aScanEnt,i_Level);
         ! END_TICKET(tNextEnt);
         tNextEnt = NEXT_TICKET(aScanEnt,i_Level)) {

      aNextEnt_int = GET_ENTRY(tNextEnt);
      iNextEnt_IntElm0 = ELM0_OF_ENTRY(aNextEnt_int);

      //RASSERT( ! pIndexDef()->bHasDescending() );
      if (SCAN_IS_AT_PRIOR_POS(iGivenIntElm0,iNextEnt_IntElm0))
        break;

      aScanEnt = aNextEnt_int;
    }
  }
  aEnt = (Ent*)aScanEnt;

  ///////////////////////////////////////////////////////////////////////////

  L_NEXT_L0_ENTRY:

  // move to next L0 entry (across buffers if necessary)

  tET tNextEntry = aEnt->m_taNextEntry[0];
  if (! END_TICKET(tNextEntry)) {
    // + more entries in buffer

    i8 iNextEntryOffset = (i8) tNextEntry * C_ENT_QUANTA;
    aEnt = (Ent *) ((char *) pEntBuffer + 624 + iNextEntryOffset);
  }
  else {
    // - no more entries in buffer

    // move to the next buffer
    if (! (pEntBuffer = pEntBuffer->pNextEntBuffer()) )
      return nullptr;

    // move to first entry in that buffer

    tNextEntry = pEntBuffer->m_xSkipRoots.m_taNextEntry[0];
    if (END_TICKET(tNextEntry))
      return nullptr;

    i8 iNextEntryOffset = (i8) tNextEntry * C_ENT_QUANTA;
    aEnt = (Ent *) ((char *) pEntBuffer + 624 + iNextEntryOffset);
  }

  ///////////////////////////////////////////////////////////////////////////

  Key* aEntKey = &aEnt->m_xKey;
  ASSERT(KE_INT == aEnt->eKind());

  i8 iEntVal = *(i8 *) aEntKey;
  i8 iWrkVal = *(i8 *) aKey;

  if (iEntVal == iWrkVal)
    return aEnt;
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find an INT entry in the indexTree                                     CS2 */
/*----------------------------------------------------------------------------*/
Ent* IndexTree::aFindEntry_CS2_v16(IndexTree* pIndexTree, Key* aKey)
{

  //pIndexTree->vDump(false);

  ///////////////////////////////////////////////////////////////////////////

  // find H0 which would contain key

  // root buffer ?
  if (! pIndexTree->m_pRootEntBuffer)
    return nullptr;

  ///////////////////////////////////////////////////////////////////////////
  /// FLATTENING pHeapBuffer->aFirstChunk()

  // position at H0 buffer

  Ent* aEnt;
  EntBuffer* pEntBuffer;

  pEntBuffer = pIndexTree->m_pRootEntBuffer;

  while (true) {

    if (0 == pEntBuffer->m_iBufferHeight)
      break;

    aEnt = pEntBuffer->aFindEntryBefore_int(aKey);
    ASSERT(aEnt);

    aEnt = pEntBuffer->pFindEntryPrevTo_int(aEnt,aKey);
    ASSERT(aEnt);

    /*-----------------------------------------------------------------------*/

    i8 i_PoolBuffer = aEnt->m_xRef.m_uBuffer;

    HeapBuffer* pHeapBuffer = _pHeapBuffer(FileMap::pToPoolBuffer(i_PoolBuffer));

//    pEntBuffer = _pEntBuffer(pHeapBuffer->aFirstChunk());

    pEntBuffer = (EntBuffer *)aAdd<FreeChunk*>(pHeapBuffer,sizeof(HeapBuffer));

    ASSERT(pEntBuffer);
    ASSERT(pEntBuffer->eGetClassNo() == CN_ENTBUFFER);
  }

  ///////////////////////////////////////////////////////////////////////////

  // position on entry prior to given key in the found H0 leaf buffer


  if (0 == pEntBuffer->m_nItems) {
    aEnt = & pEntBuffer->m_xSkipRoots;
    goto L_NEXT_L0_ENTRY;
  }

  // declare these separately because of goto
  Ent_int* aItemArray;
  Ent* aSkipRoots;
  i8 iGivenIntElm0;
  Ent_int* aNextEnt_int;
  i8 iNextEnt_IntElm0;
  Ent_int* aScanEnt;

  aSkipRoots = & pEntBuffer->m_xSkipRoots;
  aItemArray = (Ent_int*)pEntBuffer->m_xWsp.m_yaByteArea;
  iGivenIntElm0 = ELM0_OF_KEY(aKey);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  aScanEnt = (Ent_int*)aSkipRoots;
  for (i4 i_Level = 2; i_Level >= 0; i_Level--) {
    for (tET tNextEnt = NEXT_TICKET(aScanEnt,i_Level);
         ! END_TICKET(tNextEnt);
         tNextEnt = NEXT_TICKET(aScanEnt,i_Level)) {

      aNextEnt_int = GET_ENTRY(tNextEnt);
      iNextEnt_IntElm0 = ELM0_OF_ENTRY(aNextEnt_int);

      //RASSERT( ! pIndexDef()->bHasDescending() );
      if (SCAN_IS_AT_PRIOR_POS(iGivenIntElm0,iNextEnt_IntElm0))
        break;

      aScanEnt = aNextEnt_int;
    }
  }
  aEnt = (Ent*)aScanEnt;

  ///////////////////////////////////////////////////////////////////////////

  L_NEXT_L0_ENTRY:

  // move to next L0 entry (across buffers if necessary)

  tET tNextEntry = aEnt->m_taNextEntry[0];
  if (! END_TICKET(tNextEntry)) {
    // + more entries in buffer

    i8 iNextEntryOffset = (i8) tNextEntry * C_ENT_QUANTA;
    aEnt = (Ent *) ((char *) pEntBuffer + 624 + iNextEntryOffset);
  }
  else {
    // - no more entries in buffer

    // move to the next buffer
    if (! (pEntBuffer = pEntBuffer->pNextEntBuffer()) )
      return nullptr;

    // move to first entry in that buffer

    tNextEntry = pEntBuffer->m_xSkipRoots.m_taNextEntry[0];
    if (END_TICKET(tNextEntry))
      return nullptr;

    i8 iNextEntryOffset = (i8) tNextEntry * C_ENT_QUANTA;
    aEnt = (Ent *) ((char *) pEntBuffer + 624 + iNextEntryOffset);
  }

  ///////////////////////////////////////////////////////////////////////////

  Key* aEntKey = &aEnt->m_xKey;
  ASSERT(KE_INT == aEnt->eKind());

  i8 iEntVal = *(i8 *) aEntKey;
  i8 iWrkVal = *(i8 *) aKey;

  if (iEntVal == iWrkVal)
    return aEnt;
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find an INT entry in the indexTree                                     CS2 */
/*----------------------------------------------------------------------------*/
Ent* IndexTree::aFindEntry_CS2_v17(IndexTree* pIndexTree, Key* aKey)
{

  //pIndexTree->vDump(false);

  ///////////////////////////////////////////////////////////////////////////

  // find H0 which would contain key

  // root buffer ?
  if (! pIndexTree->m_pRootEntBuffer)
    return nullptr;

  ///////////////////////////////////////////////////////////////////////////
  /// TIDY UP casting

  // position at H0 buffer

  Ent* aEnt;
  EntBuffer* pEntBuffer;

  pEntBuffer = pIndexTree->m_pRootEntBuffer;

  while (pEntBuffer->m_iBufferHeight) {

    aEnt = pEntBuffer->aFindEntryBefore_int(aKey);
    ASSERT(aEnt);

    aEnt = pEntBuffer->pFindEntryPrevTo_int(aEnt,aKey);
    ASSERT(aEnt);

    i8 i_BuffOffset = C_SIZE_POOL_BUFFER * (i8)aEnt->m_xRef.m_uBuffer;
    HeapBuffer* pHeapBuffer = aAdd<HeapBuffer*>(gpBufferPool->m_pBase, i_BuffOffset);
    pEntBuffer = aAdd<EntBuffer*>(pHeapBuffer,sizeof(HeapBuffer));

    ASSERT(pEntBuffer);
    ASSERT(pEntBuffer->eGetClassNo() == CN_ENTBUFFER);
  }

  ///////////////////////////////////////////////////////////////////////////

  // position on entry prior to given key in the found H0 leaf buffer

  if (0 == pEntBuffer->m_nItems) {
    aEnt = & pEntBuffer->m_xSkipRoots;
    goto L_NEXT_L0_ENTRY;
  }

  // declare these separately because of goto
  Ent_int* aItemArray;
  Ent* aSkipRoots;
  i8 iGivenIntElm0;
  Ent_int* aNextEnt_int;
  i8 iNextEnt_IntElm0;
  Ent_int* aScanEnt;

  aSkipRoots = & pEntBuffer->m_xSkipRoots;
  aItemArray = (Ent_int*)pEntBuffer->m_xWsp.m_yaByteArea;
  iGivenIntElm0 = ELM0_OF_KEY(aKey);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  aScanEnt = (Ent_int*)aSkipRoots;
  for (i4 i_Level = 2; i_Level >= 0; i_Level--) {
    for (tET tNextEnt = NEXT_TICKET(aScanEnt,i_Level);
         ! END_TICKET(tNextEnt);
         tNextEnt = NEXT_TICKET(aScanEnt,i_Level)) {

      aNextEnt_int = GET_ENTRY(tNextEnt);
      iNextEnt_IntElm0 = ELM0_OF_ENTRY(aNextEnt_int);

      //RASSERT( ! pIndexDef()->bHasDescending() );
      if (SCAN_IS_AT_PRIOR_POS(iGivenIntElm0,iNextEnt_IntElm0))
        break;

      aScanEnt = aNextEnt_int;
    }
  }
  aEnt = (Ent*)aScanEnt;

  ///////////////////////////////////////////////////////////////////////////

  L_NEXT_L0_ENTRY:

  // move to next L0 entry (across buffers if necessary)

  tET tNextEntry = aEnt->m_taNextEntry[0];
  if (! END_TICKET(tNextEntry)) {
    // + more entries in buffer

    i8 iNextEntryOffset = (i8) tNextEntry * C_ENT_QUANTA;
    aEnt = (Ent *) ((char *) pEntBuffer + 624 + iNextEntryOffset);
  }
  else {
    // - no more entries in buffer

    // move to the next buffer
    if (! (pEntBuffer = pEntBuffer->pNextEntBuffer()) )
      return nullptr;

    // move to first entry in that buffer

    tNextEntry = pEntBuffer->m_xSkipRoots.m_taNextEntry[0];
    if (END_TICKET(tNextEntry))
      return nullptr;

    i8 iNextEntryOffset = (i8) tNextEntry * C_ENT_QUANTA;
    aEnt = (Ent *) ((char *) pEntBuffer + 624 + iNextEntryOffset);
  }

  ///////////////////////////////////////////////////////////////////////////

  Key* aEntKey = &aEnt->m_xKey;
  ASSERT(KE_INT == aEnt->eKind());

  i8 iEntVal = *(i8 *) aEntKey;
  i8 iWrkVal = *(i8 *) aKey;

  if (iEntVal == iWrkVal)
    return aEnt;
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find an INT entry in the indexTree                                     CS2 */
/*----------------------------------------------------------------------------*/
Ent* IndexTree::aFindEntry_CS2_v18(IndexTree* pIndexTree, Key* aKey)
{

  //pIndexTree->vDump(false);

  ///////////////////////////////////////////////////////////////////////////

  // find H0 which would contain key

  // root buffer ?
  if (! pIndexTree->m_pRootEntBuffer)
    return nullptr;

  ///////////////////////////////////////////////////////////////////////////
  ///

  // position at H0 buffer

  Ent* aEnt;
  EntBuffer* pEntBuffer;

  pEntBuffer = pIndexTree->m_pRootEntBuffer;

  while (pEntBuffer->m_iBufferHeight) {

    aEnt = pEntBuffer->aFindEntryBefore_int(aKey);
    ASSERT(aEnt);

    /*-----------------------------------------------------------------------*/

#if 0
    aEnt = pEntBuffer->pFindEntryPrevTo_int(aEnt,aKey);
    ASSERT(aEnt);
#else

    Ent* aGivenEnt = aEnt;
    if (aGivenEnt == pEntBuffer->aSkipRoots())
      aGivenEnt = pEntBuffer->aNextEntry(aGivenEnt,0);

    Ent* aEntPrev = aGivenEnt;
    i8 iGivenInt;
    i8 iWithInt;

    iWithInt = *(i8*)aKey;

    while (aGivenEnt) {

      iGivenInt = aGivenEnt->iIntElm0();

      // have we reached the key?
      if (iGivenInt == iWithInt)
        break;

      // have we moved beyond ?
      if (iGivenInt > iWithInt)
        return aEntPrev;

      aEntPrev = aGivenEnt;
      aGivenEnt = pEntBuffer->aNextEntry(aGivenEnt,0);

      if ( ! aGivenEnt )
        return aEntPrev;
    }

    aEnt = aEntPrev;

#endif

    /*-----------------------------------------------------------------------*/

    i8 i_BuffOffset = C_SIZE_POOL_BUFFER * (i8)aEnt->m_xRef.m_uBuffer;
    HeapBuffer* pHeapBuffer = aAdd<HeapBuffer*>(gpBufferPool->m_pBase, i_BuffOffset);
    pEntBuffer = aAdd<EntBuffer*>(pHeapBuffer,sizeof(HeapBuffer));

    ASSERT(pEntBuffer);
    ASSERT(pEntBuffer->eGetClassNo() == CN_ENTBUFFER);
  }

  ///////////////////////////////////////////////////////////////////////////

  // position on entry prior to given key in the found H0 leaf buffer

  if (0 == pEntBuffer->m_nItems) {
    aEnt = & pEntBuffer->m_xSkipRoots;
    goto L_NEXT_L0_ENTRY;
  }

  // declare these separately because of goto
  Ent_int* aItemArray;
  Ent* aSkipRoots;
  i8 iGivenIntElm0;
  Ent_int* aNextEnt_int;
  i8 iNextEnt_IntElm0;
  Ent_int* aScanEnt;

  aSkipRoots = & pEntBuffer->m_xSkipRoots;
  aItemArray = (Ent_int*)pEntBuffer->m_xWsp.m_yaByteArea;
  iGivenIntElm0 = ELM0_OF_KEY(aKey);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  aScanEnt = (Ent_int*)aSkipRoots;
  for (i4 i_Level = 2; i_Level >= 0; i_Level--) {
    for (tET tNextEnt = NEXT_TICKET(aScanEnt,i_Level);
         ! END_TICKET(tNextEnt);
         tNextEnt = NEXT_TICKET(aScanEnt,i_Level)) {

      aNextEnt_int = GET_ENTRY(tNextEnt);
      iNextEnt_IntElm0 = ELM0_OF_ENTRY(aNextEnt_int);

      //RASSERT( ! pIndexDef()->bHasDescending() );
      if (SCAN_IS_AT_PRIOR_POS(iGivenIntElm0,iNextEnt_IntElm0))
        break;

      aScanEnt = aNextEnt_int;
    }
  }
  aEnt = (Ent*)aScanEnt;

  ///////////////////////////////////////////////////////////////////////////

  L_NEXT_L0_ENTRY:

  // move to next L0 entry (across buffers if necessary)

  tET tNextEntry = aEnt->m_taNextEntry[0];
  if (! END_TICKET(tNextEntry)) {
    // + more entries in buffer

    i8 iNextEntryOffset = (i8) tNextEntry * C_ENT_QUANTA;
    aEnt = (Ent *) ((char *) pEntBuffer + 624 + iNextEntryOffset);
  }
  else {
    // - no more entries in buffer

    // move to the next buffer
    if (! (pEntBuffer = pEntBuffer->pNextEntBuffer()) )
      return nullptr;

    // move to first entry in that buffer

    tNextEntry = pEntBuffer->m_xSkipRoots.m_taNextEntry[0];
    if (END_TICKET(tNextEntry))
      return nullptr;

    i8 iNextEntryOffset = (i8) tNextEntry * C_ENT_QUANTA;
    aEnt = (Ent *) ((char *) pEntBuffer + 624 + iNextEntryOffset);
  }

  ///////////////////////////////////////////////////////////////////////////

  Key* aEntKey = &aEnt->m_xKey;
  ASSERT(KE_INT == aEnt->eKind());

  i8 iEntVal = *(i8 *) aEntKey;
  i8 iWrkVal = *(i8 *) aKey;

  if (iEntVal == iWrkVal)
    return aEnt;
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find an INT entry in the indexTree                                     CS2 */
/*----------------------------------------------------------------------------*/
Ent* IndexTree::aFindEntry_CS2_v19(IndexTree* pIndexTree, Key* aKey_find)
{
  _static_

  //pIndexTree->vDump(false);

  ///////////////////////////////////////////////////////////////////////////

  // find H0 which would contain key

  // root buffer ?
  if (! pIndexTree->m_pRootEntBuffer)
    return nullptr;

  ///////////////////////////////////////////////////////////////////////////
  ///

  i8 iKey_find_int = *(i8*)aKey_find;

  // position at H0 buffer

  Ent* aEnt_Hn;
  EntBuffer* pEntBuffer_Hn;

  pEntBuffer_Hn = pIndexTree->m_pRootEntBuffer;

  while (pEntBuffer_Hn->m_iBufferHeight) {

    aEnt_Hn = pEntBuffer_Hn->aFindEntryBefore_int(aKey_find);
    ASSERT(aEnt_Hn);

    /*-----------------------------------------------------------------------*/

    Ent* aEnt_Hn_test;
    if (aEnt_Hn == & pEntBuffer_Hn->m_xSkipRoots)
      aEnt_Hn = aEnt_Hn_test = pEntBuffer_Hn->aNextEntry(aEnt_Hn, 0);
    else
      aEnt_Hn_test = aEnt_Hn;

    while (aEnt_Hn_test) {

      // have we reached the key?
      i8 iKey_test_int = aEnt_Hn_test->iIntElm0();
      if (iKey_test_int == iKey_find_int)
        break;

      // have we moved beyond ?
      if (iKey_test_int > iKey_find_int)
        break;

      // move to next ent
      aEnt_Hn = aEnt_Hn_test;
      aEnt_Hn_test = pEntBuffer_Hn->aNextEntry(aEnt_Hn_test, 0);
    }

    /*-----------------------------------------------------------------------*/

    i8 i_BuffOffset = C_SIZE_POOL_BUFFER * (i8)aEnt_Hn->m_xRef.m_uBuffer;
    HeapBuffer* pHeapBuffer = aAdd<HeapBuffer*>(gpBufferPool->m_pBase, i_BuffOffset);
    pEntBuffer_Hn = aAdd<EntBuffer*>(pHeapBuffer, sizeof(HeapBuffer));

    ASSERT(pEntBuffer_Hn);
    ASSERT(pEntBuffer_Hn->eGetClassNo() == CN_ENTBUFFER);
  }

  EntBuffer* pEntBuffer_H0 = pEntBuffer_Hn;

  ///////////////////////////////////////////////////////////////////////////

  // position on entry prior to given key in the found H0 leaf buffer

  Ent* aEnt_H0;

  if (0 == pEntBuffer_H0->m_nItems) {
    aEnt_H0 = & pEntBuffer_H0->m_xSkipRoots;
    goto L_NEXT_L0_ENTRY;
  }

  // declare these separately because of goto
  Ent_int* aItemArray;
  Ent* aSkipRoots;
  i8 iGivenIntElm0;
  Ent_int* aNextEnt_int;
  i8 iNextEnt_IntElm0;
  Ent_int* aEnt_scan;

  aSkipRoots = & pEntBuffer_H0->m_xSkipRoots;
  aItemArray = (Ent_int*)pEntBuffer_H0->m_xWsp.m_yaByteArea;
  iGivenIntElm0 = ELM0_OF_KEY(aKey_find);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  aEnt_scan = (Ent_int*)aSkipRoots;
  for (i4 i_Level = 2; i_Level >= 0; i_Level--) {
    for (tET tNextEnt = NEXT_TICKET(aEnt_scan, i_Level);
         ! END_TICKET(tNextEnt);
         tNextEnt = NEXT_TICKET(aEnt_scan, i_Level)) {

      aNextEnt_int = GET_ENTRY(tNextEnt);
      iNextEnt_IntElm0 = ELM0_OF_ENTRY(aNextEnt_int);

      //RASSERT( ! pIndexDef()->bHasDescending() );
      if (SCAN_IS_AT_PRIOR_POS(iGivenIntElm0,iNextEnt_IntElm0))
        break;

      aEnt_scan = aNextEnt_int;
    }
  }
  aEnt_H0 = (Ent*)aEnt_scan;

  ///////////////////////////////////////////////////////////////////////////

  L_NEXT_L0_ENTRY:

  // move to next L0 entry (across buffers if necessary)

  tET tNextEntry = aEnt_H0->m_taNextEntry[0];
  if (! END_TICKET(tNextEntry)) {
    // + more entries in buffer

    i8 iNextEntryOffset = (i8) tNextEntry * C_ENT_QUANTA;
    aEnt_H0 = (Ent *) ((char *) pEntBuffer_H0 + 624 + iNextEntryOffset);
  }
  else {
    // - no more entries in buffer

    // move to the next buffer
    if (! (pEntBuffer_H0 = pEntBuffer_H0->pNextEntBuffer()) )
      return nullptr;

    // move to first entry in that buffer

    tNextEntry = pEntBuffer_H0->m_xSkipRoots.m_taNextEntry[0];
    if (END_TICKET(tNextEntry))
      return nullptr;

    i8 iNextEntryOffset = (i8) tNextEntry * C_ENT_QUANTA;
    aEnt_H0 = (Ent *) ((char *) pEntBuffer_H0 + 624 + iNextEntryOffset);
  }

  ///////////////////////////////////////////////////////////////////////////

  Key* aEntKey = &aEnt_H0->m_xKey;
  ASSERT(KE_INT == aEnt_H0->eKind());

  i8 iEntVal = *(i8 *) aEntKey;
  i8 iWrkVal = *(i8 *) aKey_find;

  if (iEntVal == iWrkVal)
    return aEnt_H0;
  else
    return nullptr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
