/*============================================================================*/
/* EntBuffer_position_int.cpp -- position related functions for EntBuffer     */
/*============================================================================*/

#include "Indexing.h"


#define ELM0_OF_KEY(aKey)   ( *(i8*)aKey )

#define ELM0_OF_ENTRY(aEnt_int) ( aEnt_int->m_iIntElm0 )

#define END_TICKET(tNxtEnt) ( C_ENT_END_TICKET == tNxtEnt )

#define GET_ENTRY(tTicket)  ( &aItemArray[tTicket] )

#define NEXT_TICKET(aEnt_int,i_Lvl) ( aEnt_int->m_taNextEntry[i_Lvl] )

#define SCAN_IS_AT_PRIOR_POS(iKeyElm,iNextElm) ( iKeyElm <= iNextElm )

#define STORE_PRIOR_POS(aVec,i_Lvl,aEnt) ( aVec[i_Lvl] = (Ent*)aEnt )

/*----------------------------------------------------------------------------*/
/* find entry given int key                                                   */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::aFindEnt_int(Key* aWithKey_int)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);

  RASSERT(KE_INT == m_eEntKind);
  
  if (nItems() == 0)
    return nullptr;
      
  i8 nLvlLevels = AM::nLvlLevels();
    
  Ent* aSkipRoots = this->aSkipRoots();
  
  Ent_int* aItemArray = (Ent_int*)this->aItemArray();
  
  i8 iWithIntElm0 = ELM0_OF_KEY(aWithKey_int);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Ent_int* aNextEnt_int;
  i8 iNextEnt_IntElm0;

  Ent_int* aScanEnt = (Ent_int*)aSkipRoots;
  for (i4 i_Level = (i4)nLvlLevels - 1; i_Level >= 0; i_Level--) {
    
    for (tET tNextEnt = NEXT_TICKET(aScanEnt,i_Level);
         ! END_TICKET(tNextEnt);
         tNextEnt = NEXT_TICKET(aScanEnt,i_Level)) {

      aNextEnt_int = GET_ENTRY(tNextEnt);
      iNextEnt_IntElm0 = ELM0_OF_ENTRY(aNextEnt_int);
      
      //vTrace("@%lld : %lld v %lld\n"_Stz,i_Level,iKey_IntElm,iNextEnt_IntElm);
            
      if (SCAN_IS_AT_PRIOR_POS(iWithIntElm0,iNextEnt_IntElm0))
        break;
      
      aScanEnt = aNextEnt_int;
    }

  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Ent_int* aFoundEnt = (Ent_int*)GET_ENTRY(aScanEnt->m_taNextEntry[0]);

  if (aFoundEnt) {
    bool b = (iWithIntElm0 == ((Ent_int*)aFoundEnt)->m_iIntElm0);
    if (b)
      return (Ent*)aFoundEnt;
  }
  vTrace("DID NOT FIND %lld\n"_Stz,iWithIntElm0);
  
  NEVER_REACH;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* scan thru skip list and return entry before entry with given key           */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::aFindEntryBefore_int(Key* aGivenKey_int)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  RASSERT(KE_INT == m_eEntKind);

  #ifdef WITH_EB_LAZY_CHAIN
    ASSERT( ! m_aLazyChain );
  #endif
  
  if (nItems() == 0)
    return aSkipRoots();
  
  i8 nLvlLevels = AM::nLvlLevels();
  
  Ent* aSkipRoots = this->aSkipRoots();
  
  Ent_int* aItemArray = (Ent_int*)this->aItemArray();

  i8 iGivenIntElm0 = ELM0_OF_KEY(aGivenKey_int);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Ent_int* aNextEnt_int;
  i8 iNextEnt_IntElm0;

  Ent_int* aScanEnt = (Ent_int*)aSkipRoots;
  for (i4 i_Level = (i4)nLvlLevels - 1; i_Level >= 0; i_Level--) {
    
    for (tET tNextEnt = NEXT_TICKET(aScanEnt,i_Level);
         ! END_TICKET(tNextEnt);
         tNextEnt = NEXT_TICKET(aScanEnt,i_Level)) {

      aNextEnt_int = GET_ENTRY(tNextEnt);
      iNextEnt_IntElm0 = ELM0_OF_ENTRY(aNextEnt_int);
      
      //vTrace("@%lld : %lld v %lld\n"_Stz,i_Level,iKey_IntElm,iNextEnt_IntElm);
            
      if (SCAN_IS_AT_PRIOR_POS(iGivenIntElm0,iNextEnt_IntElm0))
        break;
      
      aScanEnt = aNextEnt_int;
    }

  }

  return (Ent*)aScanEnt;
}

/*----------------------------------------------------------------------------*/
/* find the prev key entry                                                    */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::pFindEntryPrevTo_int(Ent* aGivenEnt, Key* aWithKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);

  if (aGivenEnt == aSkipRoots())
    aGivenEnt = aNextEntry(aGivenEnt,0);
  
  Ent* aEntPrev = aGivenEnt;
  i8 iGivenInt;
  i8 iWithInt;

  iWithInt = *(i8*)aWithKey;

  while (aGivenEnt) {
    
    iGivenInt = aGivenEnt->iIntElm0();
    
    // have we reached the key?
    if (iGivenInt == iWithInt)
      break;
    
    // have we moved beyond ?
    if (iGivenInt > iWithInt)
      return aEntPrev;
    
    aEntPrev = aGivenEnt;
    aGivenEnt = aNextEntry(aGivenEnt,0);
    
    if ( ! aGivenEnt )
      return aEntPrev;
  }
  
  return aEntPrev;
}

/*----------------------------------------------------------------------------*/
/* position at a leaf buffer starting from this buffer                        */
/*----------------------------------------------------------------------------*/
EntBuffer* EntBuffer::pFindLeafBuffer_int(Key* aForKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  RASSERT(KE_INT == m_eEntKind);

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
    Ent* aEntPrev;
  
    while (true) {
      
#ifdef EB_TRACE_INSERT
      vTrace("?? pFindLeafBuffer_int "_Stz);
      pEntBuffer->vTraceWhere();
      vTrace("\n"_Stz);
#endif

      iBufferHeight = pEntBuffer->iBufferHeight();
      if (iBufferHeight == 0)
        break;
      
      // get the entry
      //aEnt1 = pEntBuffer->aPositionQuery(xGivenKey);
      //ASSERT(aEnt1);
      
      aEnt = pEntBuffer->aFindEntryBefore_int(aForKey);
      ASSERT(aEnt);
      aEntPrev = aEnt;

      aEnt = pEntBuffer->pFindEntryPrevTo_int(aEnt,aForKey);
#ifdef WITH_DEBUG_VERSION
      if (! aEnt) {
        aForKey->vShow(pIndexDef());
        vTrace("\n"_Stz);

        aEntPrev->aKey()->vShow(pIndexDef());
        vTrace("\n"_Stz);

        pEntBuffer->vDump();
      }
      ASSERT(aEnt);
#endif
      // get the index buffer of the entry
      pEntBuffer = EntBuffer::pRefdEntBuffer(aEnt);
      ASSERT(pEntBuffer);
      ASSERT(pEntBuffer->eClass() == CN_ENTBUFFER);
    }
    
    return pEntBuffer;
}

/*----------------------------------------------------------------------------*/
/* compute prior vec for find key, such that next of priorVec[0] is the next  */
/* buffer after find keys toBuffer (ie nextBuffer->prevBuffer == toBuffer)    */
/*----------------------------------------------------------------------------*/
void EntBuffer::vPosVecPriorToBuffer_int(EntBuffer* pToRefdBuffer,
                                      Key* aWithKey,
                                      Ent* aPriorVec[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);

  RASSERT(KE_INT == m_eEntKind);
  
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
        
        i8 iNextInt = aNextEntry_sl->iIntElm0();
        i8 iWithInt = *(i8*)aWithKey;

        //bool bNextKeyIsGreater = Key::bGreaterThan(aNextEntry_sl->aKey(),aWithKey);
        bool bNextKeyIsGreater = (iNextInt > iWithInt);
        if (bNextKeyIsGreater) {
          // nextEntry[sl] > aInsertingEntry, insert at this level after prior entry
          aPriorVec[i_ScanLevel] = aPriorEntry_sl;
          // go down one level, same enty
          break;
        }

        //bool bKeysAreEqual = Key::bEqualTo(aNextEntry_sl->aKey(),aWithKey);
        bool bKeysAreEqual = (iNextInt == iWithInt);

        if (! bKeysAreEqual) {
          // haven't found key at this level yet, go right at this level
          aPriorVec[i_ScanLevel] = aNextEntry_sl;//aPriorEntry_sl;
          if (! aNextEntry(aNextEntry_sl,i_ScanLevel) )
            break;
          aPriorEntry_sl = aNextEntry_sl;
          continue;
        }

        // - keys are equal

        if (pToRefdBuffer != pRefdEntBuffer(aNextEntry_sl)->pPrevBuffer()) {
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
/* compute prior vector for given int key                                     */
/*----------------------------------------------------------------------------*/
void EntBuffer::vPosVecPriorToEntry_int(Key* aGivenKey_int,
                                        Ent* aPriorVec[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
    
  RASSERT(KE_INT == m_eEntKind);

#ifdef WITH_EB_LAZY_CHAIN
  ASSERT(! m_aLazyChain);
#endif
  
  i8 nLvlLevels = AM::nLvlLevels();
    
  Ent* aSkipRoots = this->aSkipRoots();
  
  for (i8 i_Level = 0; i_Level < nLvlLevels; i_Level++)
    aPriorVec[i_Level] = aSkipRoots;
  
  // if there are no items then the prior vec is the set of skip roots
  if (nItems() == 0)
    return;
  
  Ent_int* aItemArray = (Ent_int*)this->aItemArray();

  i8 iGivenIntElm0 = ELM0_OF_KEY(aGivenKey_int);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Ent_int* aNextEnt_int;
  i8 iNextEnt_IntElm0;
  
  Ent_int* aScanEnt = (Ent_int*)aSkipRoots;
  for (i8 i_Level = nLvlLevels - 1; i_Level >= 0; i_Level--) {
    
    for (tET tNextEnt = NEXT_TICKET(aScanEnt,i_Level);
         ! END_TICKET(tNextEnt);
         tNextEnt = NEXT_TICKET(aScanEnt,i_Level)) {

      aNextEnt_int = GET_ENTRY(tNextEnt);
      iNextEnt_IntElm0 = ELM0_OF_ENTRY(aNextEnt_int);
            
      if (SCAN_IS_AT_PRIOR_POS(iGivenIntElm0,iNextEnt_IntElm0))
        break;
      
      aScanEnt = aNextEnt_int;
      
    } // next aScanEnt at i_Level

    STORE_PRIOR_POS(aPriorVec,i_Level,aScanEnt);
    
  } // next i_Level
  
}

/*----------------------------------------------------------------------------*/
/* compute prior vector for find key, such that next of priorVec[0] is        */
/* specifically the given find entry                                          */
/*----------------------------------------------------------------------------*/
void EntBuffer::vPosVecPriorToEntry_int(Ent* aToEntry,
                                        Key* aWithKey,
                                        Ent* aPriorVec[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  RASSERT(KE_INT == m_eEntKind);
  
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
  
  Ent* aNextEntry_sl = nullptr;
  
  bool bGreaterEqual;
  Ent* aScanEntry_sl = aSkipRoots();
  for (i8 i_ScanLevel = nLvlLevels - 1; i_ScanLevel >= 0; i_ScanLevel--) {
    
    i8 iItemAtScanLevel = 0;
    aNextEntry_sl = aNextEntry(aScanEntry_sl,i_ScanLevel);
    while (aNextEntry_sl) {
      
      if (aToEntry == aNextEntry_sl)
        break;
      
      Key* aNextKey = _aKey(aNextEntry_sl->aElmSeq_dtm());
      
      i8 iNextInt = aNextEntry_sl->iIntElm0();
      i8 iWithInt = *(i8*)aWithKey;
      
      //bGreaterEqual = Key::bGreaterEqual(aNextEntry_sl->aKey(), aWithKey);
      bGreaterEqual = (iNextInt >= iWithInt);

      if (bGreaterEqual) {
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
  
  RASSERT(aToEntry == aNextEntry_sl)

}


#undef ELM0_OF_KEY
#undef ELM0_OF_ENTRY
#undef END_TICKET
#undef GET_ENTRY
#undef NEXT_TICKET
#undef SCAN_IS_AT_PRIOR_POS
#undef STORE_PRIOR_POS

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
