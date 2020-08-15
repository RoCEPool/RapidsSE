/*============================================================================*/
/* EntBuffer_position_gen.cpp -- position related functions for EntBuffer     */
/*============================================================================*/

#include "Indexing.h"

/*----------------------------------------------------------------------------*/
/* position at an index buffer at given height starting from this buffer      */
/*----------------------------------------------------------------------------*/
EntBuffer* EntBuffer::pFindH0Buffer_gen(Key* aWithKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  RASSERT(KE_GENERAL == m_eEntKind);

#ifdef WITH_EB_LAZY_CHAIN
  ASSERT( ! m_aLazyChain );
  
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
    
    aEnt = pEntBuffer->aPosPriorToEntry_gen(aWithKey);
    ASSERT(aEnt);
    Ent* aEntPrev = aEnt;
    
    aEnt = pEntBuffer->pFindPrevKeyEntry_gen(aWithKey,aEnt,&pEntBuffer);
    if (! aEnt ) {
      aWithKey->vShow(m_pIndex->pIndexDef());
      vTrace("\n"_Stz);
      
      aEntPrev->aKey()->vShow(m_pIndex->pIndexDef());
      vTrace("\n"_Stz);
      
      pEntBuffer->vDump();
    }
    ASSERT(aEnt);
    
    // get the index buffer of the entry
    pEntBuffer = EntBuffer::pRefdEntBuffer(aEnt);
    ASSERT(pEntBuffer);
    ASSERT(pEntBuffer->eClass() == CN_ENTBUFFER);
  }
  
  return pEntBuffer;
}

/*----------------------------------------------------------------------------*/
/* find the prev key entry                                                    */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::pFindPrevKeyEntry_gen(Key* aWithKey,
                                      Ent* aGivenEnt,
                                      EntBuffer** ppEntBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);

  EntBuffer* pEntBuffer = *ppEntBuffer;
  ASSERT(pEntBuffer == this);

  if (aGivenEnt == pEntBuffer->aSkipRoots())
    aGivenEnt = pEntBuffer->aNextEntry(aGivenEnt,0);
  
  Ent* aEntPrev = aGivenEnt;
  
  while (aGivenEnt) {
    eKC eCompare = Key::eCompare(aGivenEnt->aKey(),aWithKey);

    // have we reached the key?
    if (eCompare == KC_SAME)
      break;
    
    // have we moved beyond ?
    if (eCompare == KC_AFTER)
      return aEntPrev;
    
    aEntPrev = aGivenEnt;
    aGivenEnt = pEntBuffer->aNextEntry(aGivenEnt,0);
    
    if ( ! aGivenEnt )
      return aEntPrev;
  }
  
  return aEntPrev;
}

/*----------------------------------------------------------------------------*/
/* compute prior vec for find key, such that next of priorVec[0] is the next  */
/* buffer after find keys toBuffer (ie nextBuffer->prevBuffer == toBuffer)    */
/*----------------------------------------------------------------------------*/
void EntBuffer::vPosPriorToBuffer_gen(Key* aWithKey,
                                       Ent* aPriorVec[],
                                       EntBuffer* pRefdChildBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);

  RASSERT(KE_GENERAL == m_eEntKind);

  ASSERT(0 != m_iBufferHeight);

#ifdef WITH_EB_LAZY_CHAIN
  ASSERT( ! m_aLazyChain );
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
  
  
  i8 nLvlLevels = AM::nLvlLevels();


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  for (i8 i_Level = 0; i_Level < nLvlLevels; i_Level++)
    aPriorVec[i_Level] = aSkipRoots();

  // first entry
  if (nItems() == 0)
    return;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  i8 i_ScanLevel = nLvlLevels - 1;
  Ent* aPriorEntry_sl = aSkipRoots();

  for (; i_ScanLevel >= 0; i_ScanLevel--) {

    while (Ent* aNextEntry_sl = aNextEntry(aPriorEntry_sl,i_ScanLevel)) {

      // - key testing

//      if (1 == m_iBufferHeight) {
//        vTrace("-- bGreaterThan ----------\n"_Stz);
//        vShowEntry(aNextEntry_sl);
//        vShowEntry(aInsertingEntry);
//       }
      
      bool bNextKeyIsGreater = Key::bGreaterThan(aNextEntry_sl->aKey(),aWithKey);
      if (bNextKeyIsGreater) {
        // nextEntry[sl] > aInsertingEntry, insert at this level after prior entry
        aPriorVec[i_ScanLevel] = aPriorEntry_sl;
        // go down one level, same enty
        break;
      }

      bool bKeysAreEqual = Key::bEqualTo(aNextEntry_sl->aKey(),aWithKey);
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
/* scan thru skip list and return entry prior to key                          */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::aPosPriorToEntry_gen(Key* aWithKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  RASSERT(KE_GENERAL == m_eEntKind);

#ifdef WITH_EB_LAZY_CHAIN
  ASSERT( ! m_aLazyChain );
#endif
  
  i8 nLvlLevels = AM::nLvlLevels();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (nItems() == 0)
    return aSkipRoots();
  
  Ent* aScanEntry_sl = aSkipRoots();
  
  bool bNextKeyIsGE;
  
  for (i8 i_ScanLevel = nLvlLevels - 1; i_ScanLevel >= 0; i_ScanLevel--) {
    
    Ent* aNextEntry_sl = aNextEntry(aScanEntry_sl,i_ScanLevel);
    while (aNextEntry_sl) {
      
      bNextKeyIsGE = Key::bGreaterEqual(aNextEntry_sl->aKey(),aWithKey);
      if (bNextKeyIsGE)
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
void EntBuffer::vPosPriorToEntry_gen(Key* aWithKey,
                                      Ent* aPriorVec[],
                                      Ent* aToEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  RASSERT(KE_GENERAL == m_eEntKind);

  //ASSERT(0 == m_iBufferHeight);
  
#ifdef WITH_EB_LAZY_CHAIN
  ASSERT( ! m_aLazyChain );
#endif
  
  i8 nLvlLevels = AM::nLvlLevels();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  for (i8 i_Level = 0; i_Level < nLvlLevels; i_Level++)
    aPriorVec[i_Level] = aSkipRoots();
  
  if (nItems() == 0)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 iFindLevels = aToEntry ? (aToEntry->nSkipLevels() - 1)
                              : (nLvlLevels - 1);
  
  bool bIsJuxta;
  Ent* aScanEntry_sl = aSkipRoots();
  for (i8 i_ScanLevel = nLvlLevels - 1; i_ScanLevel >= 0; i_ScanLevel--) {
    
    i8 iItemAtScanLevel = 0;
    Ent* aNextEntry_sl = aNextEntry(aScanEntry_sl,i_ScanLevel);
    while (aNextEntry_sl) {
      
      if (aToEntry == aNextEntry_sl)
        break;
      
      Key* aNextKey = _aKey(aNextEntry_sl->ayElmSeq());
      
      bIsJuxta = Key::bGreaterEqual(aNextEntry_sl->aKey(), aWithKey);
      
      if (bIsJuxta) {
        if (! aToEntry || aNextEntry_sl == aToEntry)
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
