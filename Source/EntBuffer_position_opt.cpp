/*============================================================================*/
/* EntBuffer_position_opt.cpp -- position related functions for EntBuffer     */
/*============================================================================*/

#include "Indexing.h"

/*----------------------------------------------------------------------------*/
/* compute prior vector for find key, such that next of priorVec[0] is        */
/* specifically the given find entry                                          */
/*----------------------------------------------------------------------------*/
void EntBuffer::vPositionPriorVecEntry_Opt01(Key* aFindKey,
                                             Ent* aPriorVec[],
                                             Ent* aFindEntry)
{
  /*
   
     this optimisation explores bringing Key::bGreaterEqual() into line
   
   */
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  //ASSERT(0 == m_iBufferHeight);
  
#ifdef WITH_EB_LAZY_CHAIN
  ASSERT( ! m_aLazyChain );
#endif
  
  i8 nLvlLevels = AM::nLvlLevels();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Ent* aSkipRoots = this->aSkipRoots();
  for (i8 i_Level = 0; i_Level < nLvlLevels; i_Level++)
    aPriorVec[i_Level] = aSkipRoots;
  
  if (nItems() == 0)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  //i8 iFindLevels = aFindEntry ? (aFindEntry->nOffsets() - 1)
  //                            : (nLvlLevels - 1);

  //bool bIsJuxta;
  Ent* aScanEntry_sl = aSkipRoots;
  
  Ent* aNextEntry_sl;
  i8 iEntSize = sizeof(Ent);

  for (i8 i_ScanLevel = nLvlLevels - 1; i_ScanLevel >= 0; i_ScanLevel--) {
    
    tET tOffset1 = (tET)(aScanEntry_sl->aKey()->nElmSeqSize() + iEntSize +
                         (C_NEO_SIZE * i_ScanLevel));
    tET tOffset2 = (tET)(sizeof(i8) + iEntSize + (C_NEO_SIZE * i_ScanLevel));
    
    tET tNextOffset = * (tET*)((char*)aScanEntry_sl+tOffset1);
    while (C_ENT_END_TICKET != tNextOffset) {
      
      aNextEntry_sl = (Ent*)((char*)aItemArea() + tNextOffset);
      if (aFindEntry == aNextEntry_sl)
        break;
      
      //--------------------------------------------------------------
      Key* aNextKey = aNextEntry_sl->aKey();
      
      i8 iLeftValue = *((i8*)aNextKey->ayElmSeq());
      i8 iRightValue = *((i8*)aFindKey->ayElmSeq());
      
      if (iLeftValue >= iRightValue) {
        if (! aFindEntry || aNextEntry_sl == aFindEntry)
          break;
        
        //if (i_ScanLevel > nLvlLevels - 1 ) //iFindLevels)
        //  break;
      }
      
      aScanEntry_sl = aNextEntry_sl;
      tNextOffset = * (i4*)((char*)aScanEntry_sl+tOffset2);
    }
    aPriorVec[i_ScanLevel] = aScanEntry_sl;
  }
}

/*----------------------------------------------------------------------------*/
/* compute prior vector for find key, such that next of priorVec[0] is        */
/* specifically the given find entry                                          */
/*----------------------------------------------------------------------------*/
void EntBuffer::vPositionPriorVecEntry_Alka01(Key* aFindKey,
                                              Ent* aPriorVec[],
                                              Ent* aFindEntry)
{
  /*
   
     this optimisation explores unrolling the scan loop
   
   */
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  //ASSERT(0 == m_iBufferHeight);
  
#ifdef WITH_EB_LAZY_CHAIN
  ASSERT( ! m_aLazyChain );
#endif
  
  i8 nLvlLevels = AM::nLvlLevels();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Ent* aSkipRoots = this->aSkipRoots();
  for (i8 i_Level = 0; i_Level < nLvlLevels; i_Level++)
    aPriorVec[i_Level] = aSkipRoots;
  
  if (nItems() == 0)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  //i8 iFindLevels = aFindEntry ? (aFindEntry->nOffsets() - 1)
  //                            : (nLvlLevels - 1);

  //bool bIsJuxta;
  Ent* aScanEntry_sl = aSkipRoots;
  
  Ent* aNextEntry_sl;
  i8 iEntSize = sizeof(Ent);
  
  //  for (i8 i_ScanLevel = nLvlLevels - 1;
  //       i_ScanLevel >= 0;
  //       i_ScanLevel--) {
  
  // FLATTENED OUT for loop
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // SCAN LEVEL 2
  i8 i_ScanLevel = 2;
  
  tET tOffset1 = (tET)(aScanEntry_sl->aKey()->nElmSeqSize() + iEntSize +
                        (C_NEO_SIZE * i_ScanLevel));
  tET tOffset2 = (tET)(sizeof(i8) + iEntSize + (C_NEO_SIZE * i_ScanLevel));
  
  tET tNextOffset = * (tET*)((char*)aScanEntry_sl+tOffset1);
  while (C_ENT_END_TICKET != tNextOffset) {
    
    aNextEntry_sl = (Ent*)((char*)aItemArea() + tNextOffset);
    if (aFindEntry == aNextEntry_sl)
      break;
    
    //--------------------------------------------------------------
    Key* aNextKey = aNextEntry_sl->aKey();
    
    i8 iLeftValue = *((i8*)aNextKey->ayElmSeq());
    i8 iRightValue = *((i8*)aFindKey->ayElmSeq());
    
    if (iLeftValue >= iRightValue) {
      if (! aFindEntry || aNextEntry_sl == aFindEntry)
        break;
      
      //if (2 > nLvlLevels - 1 ) //iFindLevels)
      //  break;
    }
    
    aScanEntry_sl = aNextEntry_sl;
    tNextOffset = * (i4*)((char*)aScanEntry_sl+tOffset2);
  }
  aPriorVec[i_ScanLevel] = aScanEntry_sl;
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // SCAN LEVEL 1
  i_ScanLevel = 1;
  
  tOffset1 = aScanEntry_sl->aKey()->nElmSeqSize() + iEntSize + C_NEO_SIZE;
  tOffset2 = sizeof(i8) + iEntSize + C_NEO_SIZE;
  
  tNextOffset = * (i4*)((char*)aScanEntry_sl+tOffset1);
  while (C_ENT_END_TICKET != tNextOffset) {
    
    aNextEntry_sl = (Ent*)((char*)aItemArea() + tNextOffset);
    if (aFindEntry == aNextEntry_sl)
      break;
    
    //--------------------------------------------------------------
    Key* aNextKey = aNextEntry_sl->aKey();
    
    i8 iLeftValue = *((i8*)aNextKey->ayElmSeq());
    i8 iRightValue = *((i8*)aFindKey->ayElmSeq());
    
    if (iLeftValue >= iRightValue) {
      if (! aFindEntry || aNextEntry_sl == aFindEntry)
        break;
      
    }
    
    aScanEntry_sl = aNextEntry_sl;
    tNextOffset = * (i4*)((char*)aScanEntry_sl+tOffset2);
  }
  aPriorVec[i_ScanLevel] = aScanEntry_sl;
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // SCAN LEVEL 0
  i_ScanLevel = 0;
  
  tOffset1 = aScanEntry_sl->aKey()->nElmSeqSize() + iEntSize;
  tOffset2 = sizeof(i8) + iEntSize;
  
  tNextOffset = * (i4*)((char*)aScanEntry_sl+tOffset1);
  while (C_ENT_END_TICKET != tNextOffset) {
    
    aNextEntry_sl = (Ent*)((char*)aItemArea() + tNextOffset);
    if (aFindEntry == aNextEntry_sl)
      break;
    
    //--------------------------------------------------------------
    Key* aNextKey = aNextEntry_sl->aKey();
    
    i8 iLeftValue = *((i8*)aNextKey->ayElmSeq());
    i8 iRightValue = *((i8*)aFindKey->ayElmSeq());
    
    if (iLeftValue >= iRightValue) {
      if (! aFindEntry || aNextEntry_sl == aFindEntry)
        break;
    }
    
    aScanEntry_sl = aNextEntry_sl;
    tNextOffset = * (i4*)((char*)aScanEntry_sl+tOffset2);
  }
  aPriorVec[i_ScanLevel] = aScanEntry_sl;
  
  
  //}
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* compute prior vector for find key, such that next of priorVec[0] is        */
/* specifically the given find entry                                          */
/*----------------------------------------------------------------------------*/
void EntBuffer::vPositionPriorVecEntry_Ray01(Key* aFindKey,
                                                Ent* aPriorVec[],
                                                Ent* aFindEntry)
{
  /*
   
     this optimisation explores bringing Key::bGreaterEqual() into line
   
   */
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  //ASSERT(0 == m_iBufferHeight);
  
#ifdef WITH_EB_LAZY_CHAIN
  ASSERT( ! m_aLazyChain );
#endif
  
  i8 nLvlLevels = AM::nLvlLevels();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Ent* aSkipRoots = this->aSkipRoots();
  for (i8 i_Level = 0; i_Level < nLvlLevels; i_Level++)
    aPriorVec[i_Level] = aSkipRoots;
  
  if (nItems() == 0)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  //i8 iFindLevels = aFindEntry ? (aFindEntry->nOffsets() - 1)
  //                            : (nLvlLevels - 1);

  //bool bIsJuxta;
  Ent* aScanEntry_sl = aSkipRoots;
  
  Ent* aNextEntry_sl;
  i8 iEntSize = sizeof(Ent);

  for (i8 i_ScanLevel = nLvlLevels - 1; i_ScanLevel >= 0; i_ScanLevel--) {
    
    tET tOffset1 = (tET)(aScanEntry_sl->aKey()->nElmSeqSize() + iEntSize +
                           (C_NEO_SIZE * i_ScanLevel));
    tET tOffset2 = (tET)(sizeof(i8) + iEntSize + (C_NEO_SIZE * i_ScanLevel));
    
    tET tNextOffset = * (tET*)((char*)aScanEntry_sl+tOffset1);
    while (C_ENT_END_TICKET != tNextOffset) {
      
      aNextEntry_sl = (Ent*)((char*)aItemArea() + tNextOffset);
      if (aFindEntry == aNextEntry_sl)
        break;
      
      //--------------------------------------------------------------
      Key* aNextKey = aNextEntry_sl->aKey();
      
      i8 iLeftValue = *((i8*)aNextKey->ayElmSeq());
      i8 iRightValue = *((i8*)aFindKey->ayElmSeq());
      
      if (iLeftValue >= iRightValue) {
        if (! aFindEntry || aNextEntry_sl == aFindEntry)
          break;
        
        //if (i_ScanLevel > nLvlLevels - 1 ) //iFindLevels)
        //  break;
      }
      
      aScanEntry_sl = aNextEntry_sl;
      tNextOffset = * (i4*)((char*)aScanEntry_sl+tOffset2);
    }
    aPriorVec[i_ScanLevel] = aScanEntry_sl;
  }
}

/*----------------------------------------------------------------------------*/
/* compute prior vector for find key, such that next of priorVec[0] is        */
/* specifically the given find entry                                          */
/*----------------------------------------------------------------------------*/
void EntBuffer::vPositionPriorVecEntry_Ray02(Key* aFindKey,
                                             Ent* aPriorVec[])
{
  /*
   
     this optimisation explores bringing Key::bGreaterEqual() into line
   
   */
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
    
#ifdef WITH_EB_LAZY_CHAIN
  ASSERT( ! m_aLazyChain );
#endif
  
  i8 nLvlLevels = AM::nLvlLevels();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Ent* aSkipRoots = this->aSkipRoots();
  for (i8 i_Level = 0; i_Level < nLvlLevels; i_Level++)
    aPriorVec[i_Level] = aSkipRoots;
  
  if (nItems() == 0)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Ent* aScanEntry_sl = aSkipRoots;
  
  Ent* aNextEntry_sl;
  i8 iEntSize = sizeof(Ent);

  for (i8 i_ScanLevel = nLvlLevels - 1; i_ScanLevel >= 0; i_ScanLevel--) {
    
    tET tOffset1 = (tET)(aScanEntry_sl->aKey()->nElmSeqSize() + iEntSize +
                          (C_NEO_SIZE * i_ScanLevel));
    tET tOffset2 = (tET)(sizeof(i8) + iEntSize + (C_NEO_SIZE * i_ScanLevel));
    
    tET tNextOffset = * (tET*)((char*)aScanEntry_sl+tOffset1);
    while (C_ENT_END_TICKET != tNextOffset) {
      
      aNextEntry_sl = (Ent*)((char*)aItemArea() + tNextOffset);
      
      //--------------------------------------------------------------
      Key* aNextKey = aNextEntry_sl->aKey();
      
      i8 iLeftValue = *((i8*)aNextKey->ayElmSeq());
      i8 iRightValue = *((i8*)aFindKey->ayElmSeq());
      
      if (iLeftValue >= iRightValue)
        break;
              
      aScanEntry_sl = aNextEntry_sl;
      tNextOffset = * (i4*)((char*)aScanEntry_sl+tOffset2);
    }
    aPriorVec[i_ScanLevel] = aScanEntry_sl;
  }
  
  
}

/*----------------------------------------------------------------------------*/
/* compute prior vector for find key, such that next of priorVec[0] is        */
/* specifically the given find entry                                          */
/*----------------------------------------------------------------------------*/
void EntBuffer::vPositionPriorVecEntry_Ray03(Key* aFindKey,
                                             Ent* aPriorVec[])
{
  /*
   
     this optimisation explores bringing Key::bGreaterEqual() into line
   
   */
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
    
#ifdef WITH_EB_LAZY_CHAIN
  ASSERT( ! m_aLazyChain );
#endif
  
  i8 nLvlLevels = AM::nLvlLevels();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Ent* aSkipRoots = this->aSkipRoots();
  for (i8 i_Level = 0; i_Level < nLvlLevels; i_Level++)
    aPriorVec[i_Level] = aSkipRoots;
  
  if (nItems() == 0)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Ent* aScanEntry_sl = aSkipRoots;
  
  Ent* aNextEntry_sl;
  i8 iEntSize = sizeof(Ent);

  for (i8 i_ScanLevel = nLvlLevels - 1; i_ScanLevel >= 0; i_ScanLevel--) {
    
    tET tOffset1 = (tET)(aScanEntry_sl->aKey()->nElmSeqSize() + iEntSize +
                          (C_NEO_SIZE * i_ScanLevel));
    tET tOffset2 = (tET)(sizeof(i8) + iEntSize + (C_NEO_SIZE * i_ScanLevel));
    
    tET tNextOffset = * (tET*)((char*)aScanEntry_sl+tOffset1);
    while (C_ENT_END_TICKET != tNextOffset) {
      
      aNextEntry_sl = (Ent*)((char*)aItemArea() + tNextOffset);
      
      Ent_flat* aEnt_flat = (Ent_flat*)aNextEntry_sl;
      
      //--------------------------------------------------------------
      Key* aNextKey = aNextEntry_sl->aKey();
      
      i8 iLeftValue = *((i8*)aNextKey->ayElmSeq());
      i8 iRightValue = *((i8*)aFindKey->ayElmSeq());
      
      ASSERT(aEnt_flat->m_iIntKey == iLeftValue);
      
      if (iLeftValue >= iRightValue)
        break;
              
      aScanEntry_sl = aNextEntry_sl;
      tNextOffset = * (i4*)((char*)aScanEntry_sl+tOffset2);
    }
    aPriorVec[i_ScanLevel] = aScanEntry_sl;
  }
  
  
}

/*----------------------------------------------------------------------------*/
/* compute prior vector for given key                                         */
/*----------------------------------------------------------------------------*/

#define ELM_OF_KEY(aKey)     ( aKey->iGetIntElm() )

#define ELM_OF_ENTRY(aEnt)   ( aEnt->aKey()->iGetIntElm() )

#define NEXT_TICKET(aEnt,i_Lvl)  ( aEnt->tNextEntry(i_Lvl) )

#define ENTRY(tTicket)       ( aGetItem(tTicket) )

#define END_TICKET(tNxtEnt)  ( C_ENT_END_TICKET == tNxtEnt )

#define AT_PRIOR_POS(iPrior,iNext) ( iPrior <= iNext )

void EntBuffer::vPositionPriorVecEntry_Ray04(Key* aPriorKey,
                                             Ent* aPriorVec[])
{
  /*
   
     this optimisation explores bringing Key::bGreaterEqual() into line
   
   */
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
    
#ifdef WITH_EB_LAZY_CHAIN
  ASSERT(! m_aLazyChain);
#endif
  
  i8 nLvlLevels = AM::nLvlLevels();
    
  Ent* aSkipRoots = this->aSkipRoots();
  for (i8 i_Level = 0; i_Level < nLvlLevels; i_Level++)
    aPriorVec[i_Level] = aSkipRoots;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // if there are no items then the prior vec is the set of skip roots
  if (nItems() == 0)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 iPriorElm = ELM_OF_KEY(aPriorKey);

  Ent* aScanEnt = aSkipRoots;
  for (i8 i_Level = nLvlLevels - 1; i_Level >= 0; i_Level--) {
 
    for (tET tNextEnt = NEXT_TICKET(aScanEnt,i_Level);
         ! END_TICKET(tNextEnt);
         tNextEnt = NEXT_TICKET(aScanEnt,i_Level)) {

      Ent* aNextEnt = ENTRY(tNextEnt);
      i8 iNextElm = ELM_OF_ENTRY(aNextEnt);
            
      if (AT_PRIOR_POS(iPriorElm,iNextElm))
        break;
              
      aScanEnt = aNextEnt;
    }
    
    aPriorVec[i_Level] = aScanEnt;
  }
  
}

#undef ELM_OF_KEY
#undef ELM_OF_ENTRY
#undef NEXT_TICKET
#undef NEXT_ENTRY
#undef END_TICKET
#undef PRIOR

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
