/*============================================================================*/
/* EntBuffer_position_gen.cpp -- position related functions for EntBuffer     */
/*============================================================================*/

#include "BASE.h"

#include "RowSingle.hpp"


#include "EntWrk.hpp"

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"

/*----------------------------------------------------------------------------*/
/* position at a leaf buffer starting from this buffer                        */
/*----------------------------------------------------------------------------*/
EntBuffer* EntBuffer::pFindLeafBuffer_gen(Key* aForKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  RASSERT(KE_GENERAL == m_eEntKind);
  
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
    
#ifdef EB_TRACE_INSERT
        vTrace("?? pFindLeafBuffer_gen"_Stz);
        pEntBuffer->vTraceWhere();
        vTrace("\n"_Stz);
        m_nTabs++;
#endif
    
    iBufferHeight = pEntBuffer->iBufferHeight();
    if (iBufferHeight == 0)
      break;
    
    // get the entry
    //aEnt1 = pEntBuffer->aPositionQuery(xGivenKey);
    //ASSERT(aEnt1);
    
    aEnt = pEntBuffer->aFindEntryBefore_gen(aForKey);
    ASSERT(aEnt);
    Ent* aEntPrev = aEnt;
    
    aEnt = pEntBuffer->pFindEntryPrevTo_gen(aEnt,aForKey);
    if (! aEnt) {
      aForKey->vShow(pIndexDef());
      vTrace("\n"_Stz);
      
      aEntPrev->aKey()->vShow(pIndexDef());
      vTrace("\n"_Stz);
      
      pEntBuffer->vDump();
    }
    ASSERT(aEnt);
    
    // get the index buffer of the entry
    pEntBuffer = EntBuffer::pRefdEntBuffer(aEnt);
    ASSERT(pEntBuffer);
    ASSERT(pEntBuffer->eGetClassNo() == CN_ENTBUFFER);
  }
  
  return pEntBuffer;
}

/*----------------------------------------------------------------------------*/
/* position at a leaf buffer starting from this buffer                        */
/*----------------------------------------------------------------------------*/
EntBuffer* EntBuffer::pFindLeafBuffer_ascGen(Key* aForKey, Ent** ppEntPrior)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);

  //RASSERT(KE_GENERAL == m_eEntKind);

  /*

     return the leaf buffer which has the key closest before given key

     9?

     [1,9]
     [1,3,6]              [9,12]
     [0,1,2][3,4,5][6,7,9][9,10,11][12,13,14]
                      ^

   */


  EntBuffer* pEntBuffer = this;
	Ent *aNextEntry_sl;
	tET tNextEntry;

  //i8 iBufferHeight;
  //Ent* aEnt;

  while (true) {

#ifdef EB_TRACE_INSERT
    vTrace("?? pFindLeafBuffer_gen"_Stz);
        pEntBuffer->vTraceWhere();
        vTrace("\n"_Stz);
        m_nTabs++;
#endif

		//*ppEntPrior = pEntBuffer->aFindEntryBefore_ascGen(aForKey);

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		if (nItems() == 0)
			*ppEntPrior = aSkipRoots();
    else {
			Ent *aScanEntry_sl = aSkipRoots();

			//bool bNextKeyIsGE;

			for (i8 i_ScanLevel = m_nLevels - 1; i_ScanLevel >= 0; i_ScanLevel--) {

				//Ent *aNextEntry_sl = aNextEntry_flat(aScanEntry_sl, i_ScanLevel);
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  - - - - - - -
				tNextEntry = aScanEntry_sl->m_taNextEntry[i_ScanLevel];

				if (C_ENT_END_TICKET == tNextEntry)
					aNextEntry_sl = nullptr;
        else {
					i8 iNextEntryOffset = (i8) tNextEntry * C_ENT_QUANTA;

					aNextEntry_sl = (Ent *) ((char *) this + 624 + iNextEntryOffset);
				}
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  - - - - - - -




				while (aNextEntry_sl) {

					//bNextKeyIsGE = Key::bGreaterEqualAsc(aNextEntry_sl->aKey(), aForKey);
					//eKC eCmp = Key::eCompareAsc(aNextEntry_sl->aKey(), aForKey);
					// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  - - - - - - -
					Key* aKey_l = aNextEntry_sl->aKey();
					i8 nKeyBytes_l = aKey_l->nElmSeqSize();
					i8 nKeyBytes_r = aForKey->nElmSeqSize();

					const i8 * iWord1 = (const i8*)aAdd<Dtm*>(aKey_l,sizeof(nKeyBytes_l));//aKey_l->aElmSeq_dtm();
					const i8 * iWord2 = (const i8*)aAdd<Dtm*>(aForKey,sizeof(nKeyBytes_r));//aForKey->aElmSeq_dtm();

					i8 nCmpBytes = nKeyBytes_l < nKeyBytes_r ? nKeyBytes_l : nKeyBytes_r;

					//i8 iCmp = MemLib::iCompareWords(ayKeyBytes_l,ayKeyBytes_r,nCmpBytes);

					i8 iCmp;
					//const i8 *iWord1 = (const i8*)ayKeyBytes_l;
					//const i8 *iWord2 = (const i8*)ayKeyBytes_r;

					while (nCmpBytes > 0) {
						if (*iWord1++ != *iWord2++) {
							iCmp = iWord1[-1] < iWord2[-1] ? -1 : 1;
							break;
						}
						nCmpBytes -=8;
					}

					if (nCmpBytes == 0)
						iCmp = 0;

          // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  - - - - - - -
//					if (iCmp < 0)
//						return KC_BEFORE;
//
//					if (iCmp == 0) {
//						if (nKeyBytes_l < nKeyBytes_r)
//							return KC_BEFORE;
//
//						if (nKeyBytes_l == nKeyBytes_r)
//							return KC_SAME;
//
//						if (nKeyBytes_l > nKeyBytes_r)
//							return KC_AFTER;
//					}
//
//					return KC_AFTER;


					//if (eCmp == KC_AFTER || eCmp == KC_SAME)
					//	break;

					if ( iCmp > 0 || (iCmp == 0 && nKeyBytes_l >= nKeyBytes_r))
						break;


					aScanEntry_sl = aNextEntry_sl;


					//aNextEntry_sl = aNextEntry_flat(aNextEntry_sl, i_ScanLevel);

					tNextEntry = aNextEntry_sl->m_taNextEntry[i_ScanLevel];

					if (C_ENT_END_TICKET == tNextEntry)
						aNextEntry_sl = nullptr;
					else {
						i8 iNextEntryOffset = (i8) tNextEntry * C_ENT_QUANTA;

						aNextEntry_sl = (Ent *) ((char *) this + 624 + iNextEntryOffset);
					}



				}

			}

			*ppEntPrior = aScanEntry_sl;
		}

    if (pEntBuffer->m_iBufferHeight == 0) {
			break;
		}

    // get the entry
    //aEnt1 = pEntBuffer->aPositionQuery(xGivenKey);
    //ASSERT(aEnt1);

		ASSERT(*ppEntPrior);
    //Ent* aEntPrev = aEnt;
		//*ppEntPrior = aEntPrev;

//    aEnt = pEntBuffer->pFindEntryPrevTo_ascGen(aEnt,aForKey);
//    if (! aEnt) {
//      aForKey->vShow(pIndexDef());
//      vTrace("\n"_Stz);
//
//      aEntPrev->aKey()->vShow(pIndexDef());
//      vTrace("\n"_Stz);
//
//      pEntBuffer->vDump();
//    }
//    ASSERT(aEnt);

    // get the index buffer of the entry
    pEntBuffer = EntBuffer::pRefdEntBuffer(*ppEntPrior);
    ASSERT(pEntBuffer);
    ASSERT(pEntBuffer->eGetClassNo() == CN_ENTBUFFER);
  }

  return pEntBuffer;
}

/*----------------------------------------------------------------------------*/
/* position at a leaf buffer starting from this buffer                        */
/*----------------------------------------------------------------------------*/
EntBuffer* EntBuffer::pFindLeafBuffer_ascDscGen(Key* aForKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);

  RASSERT(KE_GENERAL == m_eEntKind);

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

#ifdef EB_TRACE_INSERT
    vTrace("?? pFindLeafBuffer_gen"_Stz);
        pEntBuffer->vTraceWhere();
        vTrace("\n"_Stz);
        m_nTabs++;
#endif

    iBufferHeight = pEntBuffer->iBufferHeight();
    if (iBufferHeight == 0)
      break;

    // get the entry
    //aEnt1 = pEntBuffer->aPositionQuery(xGivenKey);
    //ASSERT(aEnt1);

    aEnt = pEntBuffer->aFindEntryBefore_ascDscGen(aForKey);
    ASSERT(aEnt);
    Ent* aEntPrev = aEnt;

    aEnt = pEntBuffer->pFindEntryPrevTo_ascDscGen(aEnt,aForKey);
    if (! aEnt) {
      aForKey->vShow(pIndexDef());
      vTrace("\n"_Stz);

      aEntPrev->aKey()->vShow(pIndexDef());
      vTrace("\n"_Stz);

      pEntBuffer->vDump();
    }
    ASSERT(aEnt);

    // get the index buffer of the entry
    pEntBuffer = EntBuffer::pRefdEntBuffer(aEnt);
    ASSERT(pEntBuffer);
    ASSERT(pEntBuffer->eGetClassNo() == CN_ENTBUFFER);
  }

  return pEntBuffer;
}

/*----------------------------------------------------------------------------*/
/* scan thru skip list and return entry prior to key                          */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::aFindEntryBefore_gen(Key* aGivenKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  RASSERT(KE_GENERAL == m_eEntKind);

  IndexDef* pIndexDef = this->pIndexDef();

  i8 nLvlLevels = AM::nLvlLevels();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (nItems() == 0)
    return aSkipRoots();
  
  Ent* aScanEntry_sl = aSkipRoots();
  
  bool bNextKeyIsGE;
  
  for (i8 i_ScanLevel = nLvlLevels - 1; i_ScanLevel >= 0; i_ScanLevel--) {
    
    Ent* aNextEntry_sl = aNextEntry(aScanEntry_sl,i_ScanLevel);
    while (aNextEntry_sl) {
      
      bNextKeyIsGE = Key::bGreaterEqual(pIndexDef,aNextEntry_sl->aKey(),aGivenKey);
      if (bNextKeyIsGE)
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
Ent* EntBuffer::aFindEntryBefore_ascGen(Key* aGivenKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);

  RASSERT(KE_GENERAL == m_eEntKind);

  i8 nLvlLevels = AM::nLvlLevels();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (nItems() == 0)
    return aSkipRoots();

  Ent* aScanEntry_sl = aSkipRoots();

  bool bNextKeyIsGE;

  for (i8 i_ScanLevel = nLvlLevels - 1; i_ScanLevel >= 0; i_ScanLevel--) {

    Ent* aNextEntry_sl = aNextEntry(aScanEntry_sl,i_ScanLevel);
    while (aNextEntry_sl) {

      bNextKeyIsGE = Key::bGreaterEqualAsc(aNextEntry_sl->aKey(),aGivenKey);
      if (bNextKeyIsGE)
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
Ent* EntBuffer::aFindEntryBefore_ascDscGen(Key* aGivenKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);

  RASSERT(KE_GENERAL == m_eEntKind);

  IndexDef* pIndexDef = this->pIndexDef();

  i8 nLvlLevels = AM::nLvlLevels();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (nItems() == 0)
    return aSkipRoots();

  Ent* aScanEntry_sl = aSkipRoots();

  bool bNextKeyIsGE;

  for (i8 i_ScanLevel = nLvlLevels - 1; i_ScanLevel >= 0; i_ScanLevel--) {

    Ent* aNextEntry_sl = aNextEntry(aScanEntry_sl,i_ScanLevel);
    while (aNextEntry_sl) {

      bNextKeyIsGE = Key::bGreaterEqual(pIndexDef,aNextEntry_sl->aKey(),aGivenKey);
      if (bNextKeyIsGE)
        break;

      aScanEntry_sl = aNextEntry_sl;
      aNextEntry_sl = aNextEntry(aNextEntry_sl,i_ScanLevel);
    }

  }

  return aScanEntry_sl;
}

/*----------------------------------------------------------------------------*/
/* find the prev key entry                                                    */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::pFindEntryPrevTo_gen(Ent* aGivenEnt, Key* aWithKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);

  IndexDef* pIndexDef = this->pIndexDef();

  if (aGivenEnt == aSkipRoots())
    aGivenEnt = aNextEntry(aGivenEnt,0);
  
  Ent* aEntPrev = aGivenEnt;
  
  while (aGivenEnt) {
    eKC eCompare = Key::eCompare(pIndexDef,aGivenEnt->aKey(),aWithKey);

    // have we reached the key?
    if (eCompare == KC_SAME)
      break;
    
    // have we moved beyond ?
    if (eCompare == KC_AFTER)
      return aEntPrev;
    
    aEntPrev = aGivenEnt;
    aGivenEnt = aNextEntry(aGivenEnt,0);
    
    if ( ! aGivenEnt )
      return aEntPrev;
  }
  
  return aEntPrev;
}

/*----------------------------------------------------------------------------*/
/* find the prev key entry                                                    */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::pFindEntryPrevTo_ascGen(Ent* aGivenEnt, Key* aWithKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);

  if (aGivenEnt == aSkipRoots())
    aGivenEnt = aNextEntry(aGivenEnt,0);

  Ent* aEntPrev = aGivenEnt;

  while (aGivenEnt) {
    eKC eCompare = Key::eCompareAsc(aGivenEnt->aKey(),aWithKey);

    // have we reached the key?
    if (eCompare == KC_SAME)
      break;

    // have we moved beyond ?
    if (eCompare == KC_AFTER)
      return aEntPrev;

    aEntPrev = aGivenEnt;
    aGivenEnt = aNextEntry(aGivenEnt,0);

    if ( ! aGivenEnt )
      return aEntPrev;
  }

  return aEntPrev;
}

/*----------------------------------------------------------------------------*/
/* find the prev key entry                                                    */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::pFindEntryPrevTo_ascDscGen(Ent* aGivenEnt, Key* aWithKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);

  IndexDef* pIndexDef = this->pIndexDef();

  if (aGivenEnt == aSkipRoots())
    aGivenEnt = aNextEntry(aGivenEnt,0);

  Ent* aEntPrev = aGivenEnt;

  while (aGivenEnt) {
    eKC eCompare = Key::eCompareAscDsc(pIndexDef,aGivenEnt->aKey(),aWithKey);

    // have we reached the key?
    if (eCompare == KC_SAME)
      break;

    // have we moved beyond ?
    if (eCompare == KC_AFTER)
      return aEntPrev;

    aEntPrev = aGivenEnt;
    aGivenEnt = aNextEntry(aGivenEnt,0);

    if ( ! aGivenEnt )
      return aEntPrev;
  }

  return aEntPrev;
}

/*----------------------------------------------------------------------------*/
/* compute prior vec for find key, such that next of priorVec[0] is the next  */
/* buffer after find keys toBuffer (ie nextBuffer->prevBuffer == toBuffer)    */
/*----------------------------------------------------------------------------*/
void EntBuffer::vPosVecPriorToBuffer_gen(EntBuffer* pToRefdBuffer,
                                      Key* aWithKey,
                                      Ent* aPriorVec[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);

  RASSERT(KE_GENERAL == m_eEntKind);

  ASSERT(0 != m_iBufferHeight);
  UNUSED(pToRefdBuffer);
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

  IndexDef* pIndexDef = this->pIndexDef();
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
      
      bool bNextKeyIsGreater = Key::bGreaterThan(pIndexDef,aNextEntry_sl->aKey(),aWithKey);
      if (bNextKeyIsGreater) {
        // nextEntry[sl] > aInsertingEntry, insert at this level after prior entry
        aPriorVec[i_ScanLevel] = aPriorEntry_sl;
        // go down one level, same enty
        break;
      }

      bool bKeysAreEqual = Key::bEqualTo(pIndexDef,aNextEntry_sl->aKey(),aWithKey);
      if (! bKeysAreEqual) {
        // haven't found key at this level yet, go right at this level
        aPriorVec[i_ScanLevel] = aNextEntry_sl;//aPriorEntry_sl;
        if (! aNextEntry(aNextEntry_sl,i_ScanLevel) )
          break;
        aPriorEntry_sl = aNextEntry_sl;
        continue;
      }

      // - keys are equal

			// RAY TO LOOK AT : 18/1/24

//      if (pToRefdBuffer != pRefdEntBuffer(aNextEntry_sl)->pPrevEntBuffer()) {
//        // found key but haven't reached buffer, go right at this level
//        aPriorVec[i_ScanLevel] = aNextEntry_sl;//aPriorEntry_sl;
//        if (! aNextEntry(aNextEntry_sl,i_ScanLevel) )
//          break;
//        aPriorEntry_sl = aNextEntry_sl;
//        continue;
//      }

      // reached key & buffer at this level, insert at this level after prior entry
      // go down one level, same entry
      aPriorVec[i_ScanLevel] = aPriorEntry_sl;
      break;

    } // while next entry

  } // for scan levels

}

/*----------------------------------------------------------------------------*/
/* compute prior vector for find key, such that next of priorVec[0] is        */
/* specifically the given find entry                                          */
/*----------------------------------------------------------------------------*/
void EntBuffer::vPosVecPriorToEntry_gen(Ent* aToEntry,
                                     Key* aWithKey,
                                     Ent* aPriorVec[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  RASSERT(KE_GENERAL == m_eEntKind);

  //ASSERT(0 == m_iBufferHeight);
  
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
  IndexDef* pIndexDef = this->pIndexDef();

  for (i8 i_ScanLevel = nLvlLevels - 1; i_ScanLevel >= 0; i_ScanLevel--) {
    
//    i8 iItemAtScanLevel = 0;
    Ent* aNextEntry_sl = aNextEntry(aScanEntry_sl,i_ScanLevel);
    while (aNextEntry_sl) {
      
      if (aToEntry == aNextEntry_sl)
        break;
      
      Key* aNextKey = _aKey(aNextEntry_sl->aElmSeq_dtm());
      UNUSED(aNextKey);

      bIsJuxta = Key::bGreaterEqual(pIndexDef, aNextEntry_sl->aKey(), aWithKey);

      if (bIsJuxta) {
        if (! aToEntry || aNextEntry_sl == aToEntry)
          break;
        
        if (i_ScanLevel > iFindLevels)
          break;
      }
      
      Ent* aPriorEntry_sl = aScanEntry_sl;
      UNUSED(aPriorEntry_sl);
      
      aScanEntry_sl = aNextEntry_sl;
      aNextEntry_sl = aNextEntry(aNextEntry_sl,i_ScanLevel);
//      iItemAtScanLevel++;
    }
    aPriorVec[i_ScanLevel] = aScanEntry_sl;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
