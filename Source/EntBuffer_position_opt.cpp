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
    
    i8 iOffset1 = aScanEntry_sl->aKey()->nElmSeqSize() + iEntSize +
                    (C_NEO_SIZE * i_ScanLevel);
    i8 iOffset2 = sizeof(i8) + iEntSize + (C_NEO_SIZE * i_ScanLevel);
    
    i4 iNextOffset = * (i4*)((char*)aScanEntry_sl+iOffset1);
    while (C_OFS_NIL_i4 != iNextOffset) {
      
      aNextEntry_sl = (Ent*)((char*)aItemArea() + iNextOffset);
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
      iNextOffset = * (i4*)((char*)aScanEntry_sl+iOffset2);
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
  
  i8 iOffset1 = aScanEntry_sl->aKey()->nElmSeqSize() + iEntSize +
  (C_NEO_SIZE * i_ScanLevel);
  i8 iOffset2 = sizeof(i8) + iEntSize + (C_NEO_SIZE * i_ScanLevel);
  
  i4 iNextOffset = * (i4*)((char*)aScanEntry_sl+iOffset1);
  while (C_OFS_NIL_i4 != iNextOffset) {
    
    aNextEntry_sl = (Ent*)((char*)aItemArea() + iNextOffset);
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
    iNextOffset = * (i4*)((char*)aScanEntry_sl+iOffset2);
  }
  aPriorVec[i_ScanLevel] = aScanEntry_sl;
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // SCAN LEVEL 1
  i_ScanLevel = 1;
  
  iOffset1 = aScanEntry_sl->aKey()->nElmSeqSize() + iEntSize + C_NEO_SIZE;
  iOffset2 = sizeof(i8) + iEntSize + C_NEO_SIZE;
  
  iNextOffset = * (i4*)((char*)aScanEntry_sl+iOffset1);
  while (C_OFS_NIL_i4 != iNextOffset) {
    
    aNextEntry_sl = (Ent*)((char*)aItemArea() + iNextOffset);
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
    iNextOffset = * (i4*)((char*)aScanEntry_sl+iOffset2);
  }
  aPriorVec[i_ScanLevel] = aScanEntry_sl;
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // SCAN LEVEL 0
  i_ScanLevel = 0;
  
  iOffset1 = aScanEntry_sl->aKey()->nElmSeqSize() + iEntSize;
  iOffset2 = sizeof(i8) + iEntSize;
  
  iNextOffset = * (i4*)((char*)aScanEntry_sl+iOffset1);
  while (C_OFS_NIL_i4 != iNextOffset) {
    
    aNextEntry_sl = (Ent*)((char*)aItemArea() + iNextOffset);
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
    iNextOffset = * (i4*)((char*)aScanEntry_sl+iOffset2);
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
    
    i8 iOffset1 = aScanEntry_sl->aKey()->nElmSeqSize() + iEntSize +
                    (C_NEO_SIZE * i_ScanLevel);
    i8 iOffset2 = sizeof(i8) + iEntSize + (C_NEO_SIZE * i_ScanLevel);
    
    i4 iNextOffset = * (i4*)((char*)aScanEntry_sl+iOffset1);
    while (C_OFS_NIL_i4 != iNextOffset) {
      
      aNextEntry_sl = (Ent*)((char*)aItemArea() + iNextOffset);
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
      iNextOffset = * (i4*)((char*)aScanEntry_sl+iOffset2);
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
    
    i8 iOffset1 = aScanEntry_sl->aKey()->nElmSeqSize() + iEntSize +
                    (C_NEO_SIZE * i_ScanLevel);
    i8 iOffset2 = sizeof(i8) + iEntSize + (C_NEO_SIZE * i_ScanLevel);
    
    i4 iNextOffset = * (i4*)((char*)aScanEntry_sl+iOffset1);
    while (C_OFS_NIL_i4 != iNextOffset) {
      
      aNextEntry_sl = (Ent*)((char*)aItemArea() + iNextOffset);
      
      //--------------------------------------------------------------
      Key* aNextKey = aNextEntry_sl->aKey();
      
      i8 iLeftValue = *((i8*)aNextKey->ayElmSeq());
      i8 iRightValue = *((i8*)aFindKey->ayElmSeq());
      
      if (iLeftValue >= iRightValue)
        break;
              
      aScanEntry_sl = aNextEntry_sl;
      iNextOffset = * (i4*)((char*)aScanEntry_sl+iOffset2);
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
    
    i8 iOffset1 = aScanEntry_sl->aKey()->nElmSeqSize() + iEntSize +
                    (C_NEO_SIZE * i_ScanLevel);
    i8 iOffset2 = sizeof(i8) + iEntSize + (C_NEO_SIZE * i_ScanLevel);
    
    i4 iNextOffset = * (i4*)((char*)aScanEntry_sl+iOffset1);
    while (C_OFS_NIL_i4 != iNextOffset) {
      
      aNextEntry_sl = (Ent*)((char*)aItemArea() + iNextOffset);
      
      Ent_Cur* aEntCur = (Ent_Cur*)aNextEntry_sl;
      
      //--------------------------------------------------------------
      Key* aNextKey = aNextEntry_sl->aKey();
      
      i8 iLeftValue = *((i8*)aNextKey->ayElmSeq());
      i8 iRightValue = *((i8*)aFindKey->ayElmSeq());
      
      ASSERT(aEntCur->m_iIntKey == iLeftValue);
      
      if (iLeftValue >= iRightValue)
        break;
              
      aScanEntry_sl = aNextEntry_sl;
      iNextOffset = * (i4*)((char*)aScanEntry_sl+iOffset2);
    }
    aPriorVec[i_ScanLevel] = aScanEntry_sl;
  }
  
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
