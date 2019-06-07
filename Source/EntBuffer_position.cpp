/*============================================================================*/
/* EntBuffer_position.cpp -- position related functions for EntBuffer     */
/*============================================================================*/

#include "Data.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* position at an index buffer at given height starting from this buffer      */
/*----------------------------------------------------------------------------*/
EntBuffer* EntBuffer::pPositionH0Buffer(KeyWrk& xGivenKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
#ifdef USE_EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
  
  //if (m_aLazyChain)
  //  vBuildSkipList();
#endif
  
  /*
   
     return the leaf buffer which has the key closest before given key
   
     9?
   
     [1,9]
     [1,3,6]              [9,12]
     [0,1,2][3,4,5][6,7,9][9,10,11][12,13,14]
                      ^
   
   */
  
  
  EntBuffer* pEntBuffer = this;
  
  i8 iBufferHeight;
  Ent* aEnt;
  
  while (true) {
    
    iBufferHeight = pEntBuffer->iBufferHeight();
    if (iBufferHeight == 0)
      break;
    
    // get the entry
    //aEnt1 = pEntBuffer->aPositionQuery(xGivenKey);
    //ASSERT(aEnt1);
    
    aEnt = pEntBuffer->pPositionPriorEntry(xGivenKey);
    ASSERT(aEnt);
    
    aEnt = pEntBuffer->pMoveToPrevEntry(aEnt, xGivenKey, &pEntBuffer);
    ASSERT(aEnt);
    
    // get the index buffer of the entry
    pEntBuffer = EntBuffer::pRefdEntBuffer(aEnt);
    ASSERT(pEntBuffer);
    ASSERT(pEntBuffer->eClass() == CN_ENTBUFFER);
  }
  
  return pEntBuffer;
}

/*----------------------------------------------------------------------------*/
/* scan thru skip list and return entry prior to key                          */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::pPositionPriorEntry(Ent* aFindKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
#ifdef USE_EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (nItems() == 0)
    return aSkipRoots();
  
  Ent* aScanEntry_sl = aSkipRoots();
  
  bool bKeyIsGreaterEqual;
  
  for (i8 i_ScanLevel = gpAttachMap->m_nIndexLevels - 1;
       i_ScanLevel >= 0;
       i_ScanLevel--) {
    
    Ent* aNextEntry_sl = aNextEntry(aScanEntry_sl,i_ScanLevel);
    while (aNextEntry_sl) {
      
      bKeyIsGreaterEqual = Key::bGreaterEqual(aNextEntry_sl, aFindKey);
      
      if (bKeyIsGreaterEqual)
        break;
      
      aScanEntry_sl = aNextEntry_sl;
      aNextEntry_sl = aNextEntry(aNextEntry_sl,i_ScanLevel);
    }
  }
  
  return aScanEntry_sl;
}

/*----------------------------------------------------------------------------*/
/* scan thru skip list and return entry prior to key                          */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::pPositionPriorEntry(KeyWrk& xFindKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
#ifdef USE_EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  if (nItems() == 0)
    return aSkipRoots();
  
  Ent* aScanEntry_sl = aSkipRoots();
  
  bool bKeyIsGreaterEqual;
  
  for (i8 i_ScanLevel = gpAttachMap->m_nIndexLevels - 1;
       i_ScanLevel >= 0;
       i_ScanLevel--) {
    
    Ent* aNextEntry_sl = aNextEntry(aScanEntry_sl,i_ScanLevel);
    while (aNextEntry_sl) {
      
      bKeyIsGreaterEqual = Key::bGreaterEqual(aNextEntry_sl, xFindKey);
      
      if (bKeyIsGreaterEqual)
        break;
      
      aScanEntry_sl = aNextEntry_sl;
      aNextEntry_sl = aNextEntry(aNextEntry_sl,i_ScanLevel);
    }
  }
  
  return aScanEntry_sl;
}

/*----------------------------------------------------------------------------*/
/* compute prior vector for find key, such that next of priorVec[0] is        */
/* specifically the given find entry                                          */
/*----------------------------------------------------------------------------*/
void EntBuffer::vPositionPriorVecEntry(Ent* aFindKey, Ent* aPriorVec[],
                                         Ent* aFindEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  //ASSERT(0 == m_iBufferHeight);
  
#ifdef USE_EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  for (i8 i_Offset = 0; i_Offset < gpAttachMap->m_nIndexLevels; i_Offset++)
    aPriorVec[i_Offset] = aSkipRoots();
  
  if (nItems() == 0)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  //IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  i8 iFindLevels = aFindEntry ? aFindEntry->nLevels() - 1 : 2;
  
  bool bIsJuxta;
  Ent* aScanEntry_sl = aSkipRoots();
  for (i8 i_ScanLevel = gpAttachMap->m_nIndexLevels - 1;
       i_ScanLevel >= 0;
       i_ScanLevel--) {
    
    i8 iItemAtScanLevel = 0;
    Ent* aNextEntry_sl = aNextEntry(aScanEntry_sl,i_ScanLevel);
    while (aNextEntry_sl) {
      
      if (aFindEntry == aNextEntry_sl)
        break;
      
      Key* aNextKey = _aKey(aNextEntry_sl->aDataArea());
      
      bIsJuxta = Key::bGreaterEqual(aNextEntry_sl, aFindKey);
      
      if (bIsJuxta) {
        if (! aFindEntry || aNextEntry_sl == aFindEntry)
          break;
        
        if (i_ScanLevel > iFindLevels)
          break;
      }
      
      Ent* aPriorEntry_sl = aScanEntry_sl;
      aScanEntry_sl = aNextEntry_sl;
      aNextEntry_sl = aNextEntry(aNextEntry_sl,i_ScanLevel);
      iItemAtScanLevel++;
    }
    aPriorVec[i_ScanLevel] = aScanEntry_sl;
  }
}

/*----------------------------------------------------------------------------*/
/* compute prior vec for find key, such that next of priorVec[0] is the next  */
/* buffer after find keys toBuffer (ie nextBuffer->prevBuffer == toBuffer)    */
/*----------------------------------------------------------------------------*/
void EntBuffer::vPositionPriorVecBuffer(Ent* aInsertingEntry,
                                          Ent* aPriorVec[],
                                          EntBuffer* pRefdChildBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);

  ASSERT(0 != m_iBufferHeight);

#ifdef USE_EB_LAZY_CHAIN
  RASSERT( ! m_aLazyChain );
#endif

  /*

   nb. duplicates are fully qualified by their ref'd buffer


   when this function is called (findKey,toBuffer) is about to be inserted
   into this buffer

   the (findKey,toBuffer) doesnt yet exist in this buffer


   x2?  [a2 y2]           ->  [a2 x2 y2]
        [2]a [2]x [2]y        [2]a [2]x [2]y


   Hn Buffer:
   +--+         +--+              +--+   ~   +--+
   |sr|     L2  |z2|              |0 |   ~   |sr|
   +--+         +--+        +--+  +--+   ~   +--+
   |sr|     L1  |y2|        |z2|  |0 |   ~   |sr|
   +--+         +--+  +--+  +--+  +--+   ~   +--+
   |sr|     L0  |a2|  |y2|  |z2|  |0 |   ~   |y2|
   +--+         +--+  +--+  +--+  +--+   ~   +--+
    pv           sr    a2  ^ y2    z2  ^ ~    pv
                          x2

   */

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  for (i8 i_Level = 0; i_Level < gpAttachMap->m_nIndexLevels; i_Level++)
    aPriorVec[i_Level] = aSkipRoots();

  // first entry
  if (nItems() == 0)
    return;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  i8 i_ScanLevel = gpAttachMap->m_nIndexLevels - 1;
  Ent* aPriorEntry_sl = aSkipRoots();

  for (; i_ScanLevel >= 0; i_ScanLevel--) {

    while (Ent* aNextEntry_sl = aNextEntry(aPriorEntry_sl,i_ScanLevel)) {

      // - key testing

//      if (1 == m_iBufferHeight) {
//        vTrace("-- bGreaterThan ----------\n"_Stz);
//        vShowEntry(aNextEntry_sl);
//        vShowEntry(aInsertingEntry);
//       }
      
      bool bNextKeyIsGreater = Key::bGreaterThan(aNextEntry_sl,aInsertingEntry);
      if (bNextKeyIsGreater) {
        // nextEntry[sl] > aInsertingEntry, insert at this level after prior entry
        aPriorVec[i_ScanLevel] = aPriorEntry_sl;
        // go down one level, same enty
        break;
      }

      bool bKeysAreEqual = Key::bEqualTo(aNextEntry_sl,aInsertingEntry);
      if (! bKeysAreEqual) {
        // haven't found key at this level yet, go right at this level
        aPriorVec[i_ScanLevel] = aNextEntry_sl;//aPriorEntry_sl;
        if (! aNextEntry(aNextEntry_sl,i_ScanLevel) )
          break;
        aPriorEntry_sl = aNextEntry_sl;
        continue;
      }

      // - keys are equal

      if (pRefdChildBuffer != pRefdEntBuffer(aNextEntry_sl)->pPrevBuffer()) {
        // found key but haven't reached buffer, go right at this level
        aPriorVec[i_ScanLevel] = aNextEntry_sl;//aPriorEntry_sl;
        if (! aNextEntry(aNextEntry_sl,i_ScanLevel) )
          break;
        aPriorEntry_sl = aNextEntry_sl;
        continue;
      }

      // reached key & buffer at this level, insert at this level after prior entry
      // go down one level, same entry
      aPriorVec[i_ScanLevel] = aPriorEntry_sl;
      break;

    } // while next entry

  } // for scan levels

}

/*----------------------------------------------------------------------------*/
//void EntBuffer::vPositionPriorVecBuffer(Ent* aFindKey, Ent* aPriorVec[],
//                                          EntBuffer* pToBuffer)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
//
//  ASSERT(0 != m_iBufferHeight);
//
//#ifdef USE_EB_LAZY_CHAIN
//  RASSERT( ! m_aLazyChain );
//#endif
//
//  /*
//
//   nb. duplicates are fully qualified by their ref'd buffer
//
//
//   when this function is called (findKey,toBuffer) is about to be inserted
//   into this buffer
//
//   the (findKey,toBuffer) doesnt yet exist in this buffer
//
//
//   x2?  [a2 y2]           ->  [a2 x2 y2]
//   [2]a [2]x [2]y        [2]a [2]x [2]y
//
//   */
//
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//  for (i8 i_Offset = 0; i_Offset < gpAttachMap->m_nIndexLevels; i_Offset++)
//    aPriorVec[i_Offset] = aSkipRoots();
//
//  if (nItems() == 0)
//    return;
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
//  bool bNextKeyIsGreater;
//  Ent* aScanEntry_sl = aSkipRoots();
//
//  for (i8 i_ScanLevel = gpAttachMap->m_nIndexLevels - 1;
//       i_ScanLevel >= 0;
//       i_ScanLevel--) {
//
//    Ent* aNextEntry_sl = aNextEntry(aScanEntry_sl,i_ScanLevel);
//    while (aNextEntry_sl) {
//
//      //Key* aNextKey = _aKey(aNextEntry_sl->aDataArea());
//
//      //aNextKey->vShow(pIdxColVec); vTrace("\n"_Stz);
//      //aFindKey->vShow(pIdxColVec); vTrace("\n"_Stz);
//
//      //#ifdef USE_EB_SINGLE_COLIDX
//      //      if (m_pIndex->bSingleColumn()) {
//      //        bNextKeyIsGreater = aNextKey->bGreaterThanSingle(aFindKey,m_pIndex);
//      //        RASSERT(bNextKeyIsGreater == aNextKey->bGreaterThan(aFindKey,pIdxColVec));
//      //      }
//      //      else
//      //        bNextKeyIsGreater = aNextKey->bGreaterThan(aFindKey,pIdxColVec);
//      //#else
//      //      bNextKeyIsGreater = aNextKey->bGreaterThan(aFindKey,pIdxColVec);
//      //#endif
//
//      bNextKeyIsGreater = Key::bGreaterThan(aNextEntry_sl, aFindKey);
//
//      if (bNextKeyIsGreater)
//        break;
//
//      if (0 == i_ScanLevel) {
//        //bool bKeysAreEqual = aNextKey->bEqualTo(aFindKey,pIdxColVec);
//        bool bKeysAreEqual = Key::bEqualTo(aNextEntry_sl, aFindKey);
//
//        if (bKeysAreEqual &&
//            (pToBuffer == EntBuffer::pRefdEntBuffer(aNextEntry_sl)->pPrevBuffer()))
//          break;
//      }
//
//      aScanEntry_sl = aNextEntry_sl;
//      aNextEntry_sl = aNextEntry(aNextEntry_sl,i_ScanLevel);
//    }
//    aPriorVec[i_ScanLevel] = aScanEntry_sl;
//  }
//}
//
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
