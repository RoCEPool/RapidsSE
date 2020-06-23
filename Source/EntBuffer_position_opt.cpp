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
    
    tEO tOffset1 = (tEO)(aScanEntry_sl->aKey()->nElmSeqSize() + iEntSize +
                         (C_NEO_SIZE * i_ScanLevel));
    tEO tOffset2 = (tEO)(sizeof(i8) + iEntSize + (C_NEO_SIZE * i_ScanLevel));
    
    tEO tNextOffset = * (tEO*)((char*)aScanEntry_sl+tOffset1);
    while (C_OFS_NIL_i4 != tNextOffset) {
      
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
  
  tEO tOffset1 = (tEO)(aScanEntry_sl->aKey()->nElmSeqSize() + iEntSize +
                        (C_NEO_SIZE * i_ScanLevel));
  tEO tOffset2 = (tEO)(sizeof(i8) + iEntSize + (C_NEO_SIZE * i_ScanLevel));
  
  tEO tNextOffset = * (tEO*)((char*)aScanEntry_sl+tOffset1);
  while (C_OFS_NIL_i4 != tNextOffset) {
    
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
  while (C_OFS_NIL_i4 != tNextOffset) {
    
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
  while (C_OFS_NIL_i4 != tNextOffset) {
    
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
    
    tEO tOffset1 = (tEO)(aScanEntry_sl->aKey()->nElmSeqSize() + iEntSize +
                           (C_NEO_SIZE * i_ScanLevel));
    tEO tOffset2 = (tEO)(sizeof(i8) + iEntSize + (C_NEO_SIZE * i_ScanLevel));
    
    tEO tNextOffset = * (tEO*)((char*)aScanEntry_sl+tOffset1);
    while (C_OFS_NIL_i4 != tNextOffset) {
      
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
    
    tEO tOffset1 = (tEO)(aScanEntry_sl->aKey()->nElmSeqSize() + iEntSize +
                          (C_NEO_SIZE * i_ScanLevel));
    tEO tOffset2 = (tEO)(sizeof(i8) + iEntSize + (C_NEO_SIZE * i_ScanLevel));
    
    tEO tNextOffset = * (tEO*)((char*)aScanEntry_sl+tOffset1);
    while (C_OFS_NIL_i4 != tNextOffset) {
      
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
    
    tEO tOffset1 = (tEO)(aScanEntry_sl->aKey()->nElmSeqSize() + iEntSize +
                          (C_NEO_SIZE * i_ScanLevel));
    tEO tOffset2 = (tEO)(sizeof(i8) + iEntSize + (C_NEO_SIZE * i_ScanLevel));
    
    tEO tNextOffset = * (tEO*)((char*)aScanEntry_sl+tOffset1);
    while (C_OFS_NIL_i4 != tNextOffset) {
      
      aNextEntry_sl = (Ent*)((char*)aItemArea() + tNextOffset);
      
      Ent_Cur* aEntCur = (Ent_Cur*)aNextEntry_sl;
      
      //--------------------------------------------------------------
      Key* aNextKey = aNextEntry_sl->aKey();
      
      i8 iLeftValue = *((i8*)aNextKey->ayElmSeq());
      i8 iRightValue = *((i8*)aFindKey->ayElmSeq());
      
      ASSERT(aEntCur->m_iIntKey == iLeftValue);
      
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

#define ELM_OF_KEY(aKey)    ( * ((i8*) aKey->ayElmSeq()) )

#define ELM_OF_ENTRY(aEnt)  ( * ((i8*) aEnt->aKey()->ayElmSeq()) )

#define NEXT_TICKET(aEnt,i_Lvl)  ( aEnt->tNEO(i_Lvl) )

#define NEXT_ENTRY(tNxtEnt) ( aGetItem_i4(tNxtEnt) )

#define END_TICKET(tNxtEnt) ( C_OFS_NIL_i4 == tNxtEnt )

#define PRIOR(iFind,iNext)  ( iFindElm < iNextElm )

void EntBuffer::vPositionPriorVecEntry_Ray04(Key* aFindKey,
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
  
  i8 iFindElm = ELM_OF_KEY(aFindKey);

  Ent* aScanEntry = aSkipRoots;
  for (i8 i_Level = nLvlLevels - 1; i_Level >= 0; i_Level--) {
 
    for (tEO tNextEntry = NEXT_TICKET(aScanEntry,i_Level);
         ! END_TICKET(tNextEntry);
         tNextEntry = NEXT_TICKET(aScanEntry,i_Level)) {

      Ent* aNextEntry = NEXT_ENTRY(tNextEntry);
      i8 iNextElm = ELM_OF_ENTRY(aNextEntry);
            
      if (PRIOR(iFindElm,iNextElm))
        break;
              
      aScanEntry = aNextEntry;
    }
    
    aPriorVec[i_Level] = aScanEntry;
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
