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
      
      aNextEntry_sl = (Ent*)((char*)aItemArray() + tNextOffset);
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
    
    aNextEntry_sl = (Ent*)((char*)aItemArray() + tNextOffset);
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
    
    aNextEntry_sl = (Ent*)((char*)aItemArray() + tNextOffset);
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
    
    aNextEntry_sl = (Ent*)((char*)aItemArray() + tNextOffset);
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
      
      aNextEntry_sl = (Ent*)((char*)aItemArray() + tNextOffset);
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
      
      aNextEntry_sl = (Ent*)((char*)aItemArray() + tNextOffset);
      
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
      
      aNextEntry_sl = (Ent*)((char*)aItemArray() + tNextOffset);
      
      Ent_flat* aEnt_flat = (Ent_flat*)aNextEntry_sl;
      
      //--------------------------------------------------------------
      Key* aNextKey = aNextEntry_sl->aKey();
      
      i8 iLeftValue = *((i8*)aNextKey->ayElmSeq());
      i8 iRightValue = *((i8*)aFindKey->ayElmSeq());
      
      ASSERT(aEnt_flat->m_iIntElm0 == iLeftValue);
      
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

#define ELM0_OF_KEY(aKey)    ( aKey->iGetIntElm() )

#define ELM0_OF_ENTRY(aEnt)  ( aEnt->aKey()->iGetIntElm() )

#define END_TICKET(tNxtEnt)  ( C_ENT_END_TICKET == tNxtEnt )

#define GET_ENTRY(tTicket)   ( aGetItem(iToEntryOffset(tTicket)) )

#define MARK_PRIOR_POS(aVec,i_Lvl,aEnt) ( aVec[i_Lvl] = aEnt )

#define NEXT_TICKET(aEnt,i_Lvl) ( aEnt->tNextEntry(i_Lvl) )

#define SCAN_IS_AT_PRIOR_POS(iKeyElm,iNextElm) ( iKeyElm >= iNextElm )


void EntBuffer::vPositionPriorVecEntry_Ray04(Key* aKey,
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
  
  i8 iKey_IntElm = ELM0_OF_KEY(aKey);

  Ent* aScanEnt = aSkipRoots;
  for (i8 i_Level = nLvlLevels - 1; i_Level >= 0; i_Level--) {
    
    for (tET tNextEnt = NEXT_TICKET(aScanEnt,i_Level);
         ! END_TICKET(tNextEnt);
         tNextEnt = NEXT_TICKET(aScanEnt,i_Level)) {

      Ent* aNextEnt = GET_ENTRY(tNextEnt);
      i8 iNextEnt_IntElm = ELM0_OF_ENTRY(aNextEnt);
            
      if (SCAN_IS_AT_PRIOR_POS(iKey_IntElm,iNextEnt_IntElm))
        break;
      
      aScanEnt = aNextEnt;
    }

    MARK_PRIOR_POS(aPriorVec,i_Level,aScanEnt);
  }
  
}

#undef ELM0_OF_KEY
#undef ELM0_OF_ENTRY
#undef END_TICKET
#undef GET_ENTRY
#undef MARK_PRIOR_POS
#undef NEXT_TICKET
#undef SCAN_IS_AT_PRIOR_POS

/*----------------------------------------------------------------------------*/
/* compute prior vector for given key                                         */
/*----------------------------------------------------------------------------*/

#define ELM0_OF_ENTRY(aEnt_int) ( aEnt_int->m_iIntElm0 )

#define END_TICKET(tNxtEnt) ( C_ENT_END_TICKET == tNxtEnt )

#define GET_ENTRY(tTicket) ( &aItemArray[tTicket] )

#define MARK_PRIOR_POS(aVec,i_Lvl,aEnt) ( aVec[i_Lvl] = (Ent*)aEnt )

#define NEXT_TICKET(aEnt_int,i_Lvl) ( aEnt_int->m_taNextEntry[i_Lvl] )

#define SCAN_IS_AT_PRIOR_POS(iKeyElm,iNextElm) ( iKeyElm <= iNextElm )


void EntBuffer::vPriorVecInsertedKey_Ray01(Ent* aInsertedEnt,
                                           Ent* aPriorVec[])
{
  /*
   
     this optimisation explores bringing Key::bGreaterEqual() into line
   
   */
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);

  RASSERT(KE_INT == m_eEntKind);
  
  i8 nLvlLevels = AM::nLvlLevels();
    
  Ent* aSkipRoots = this->aSkipRoots();
  for (i8 i_Level = 0; i_Level < nLvlLevels; i_Level++)
    aPriorVec[i_Level] = aSkipRoots;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // if there are no items then the prior vec is the set of skip roots
  if (nItems() == 0)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Ent_int* aItemArray = (Ent_int*)this->aItemArray();
  
  Ent_int* aInsertedEnt_int = (Ent_int*)aInsertedEnt;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 iKey_IntElm = ELM0_OF_ENTRY(aInsertedEnt_int);

  Ent_int* aScanEnt = (Ent_int*)aSkipRoots;
  for (i8 i_Level = nLvlLevels - 1; i_Level >= 0; i_Level--) {
    
    for (tET tNextEnt = NEXT_TICKET(aScanEnt,i_Level);
         ! END_TICKET(tNextEnt);
         tNextEnt = NEXT_TICKET(aScanEnt,i_Level)) {

      Ent_int* aNextEnt_int = GET_ENTRY(tNextEnt);
      i8 iNextEnt_IntElm = ELM0_OF_ENTRY(aNextEnt_int);
            
      if (SCAN_IS_AT_PRIOR_POS(iKey_IntElm,iNextEnt_IntElm))
        break;
      
      aScanEnt = aNextEnt_int;
    }

    MARK_PRIOR_POS(aPriorVec,i_Level,aScanEnt);
  }
  
}

#undef ELM0_OF_ENTRY
#undef END_TICKET
#undef GET_ENTRY
#undef MARK_PRIOR_POS
#undef NEXT_TICKET
#undef SCAN_IS_AT_PRIOR_POS

/*----------------------------------------------------------------------------*/
/* compute prior vector for given key                                         */
/*----------------------------------------------------------------------------*/

#define ELM0_OF_ENTRY(aEnt_int) ( aEnt_int->m_iIntElm0 )

#define END_TICKET(tNxtEnt) ( C_ENT_END_TICKET == tNxtEnt )

#define GET_ENTRY(tTicket) ( &aItemArray[tTicket] )

#define NEXT_TICKET(aEnt_int,i_Lvl) ( aEnt_int->m_taNextEntry[i_Lvl] )

#define SCAN_IS_AT_PRIOR_POS(iKeyElm,iNextElm) ( iKeyElm <= iNextElm )


Ent* EntBuffer::aFindEnt_Ray01(i8 iKey_IntElm)
{
  /*
   
     this optimisation explores bringing Key::bGreaterEqual() into line
   
   */
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);

  RASSERT(KE_INT == m_eEntKind);
  
  i8 nLvlLevels = AM::nLvlLevels();
    
  Ent* aSkipRoots = this->aSkipRoots();
  
  Ent_int* aItemArray = (Ent_int*)this->aItemArray();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // if there are no items then the prior vec is the set of skip roots
  if (nItems() == 0)
    return nullptr;
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Ent_int* aScanEnt = (Ent_int*)aSkipRoots;
  for (i4 i_Level = nLvlLevels - 1; i_Level >= 0; i_Level--) {
    
    for (tET tNextEnt = NEXT_TICKET(aScanEnt,i_Level);
         ! END_TICKET(tNextEnt);
         tNextEnt = NEXT_TICKET(aScanEnt,i_Level)) {

      Ent_int* aNextEnt_int = GET_ENTRY(tNextEnt);
      i8 iNextEnt_IntElm = ELM0_OF_ENTRY(aNextEnt_int);
      
      //vTrace("@%lld : %lld v %lld\n"_Stz,i_Level,iKey_IntElm,iNextEnt_IntElm);
            
      if (SCAN_IS_AT_PRIOR_POS(iKey_IntElm,iNextEnt_IntElm))
        break;
      
      aScanEnt = aNextEnt_int;
    }

  }
  
  Ent_int* aFoundEnt = (Ent_int*)GET_ENTRY(aScanEnt->m_taNextEntry[0]);

  if (aFoundEnt) {
    bool b = (iKey_IntElm == ((Ent_int*)aFoundEnt)->m_iIntElm0);
    if (b)
      return (Ent*)aFoundEnt;
  }
  vTrace("DID NOT FIND %lld\n"_Stz,iKey_IntElm);
  
  NEVER_REACH;
  
  return nullptr;
}

#undef ELM0_OF_ENTRY
#undef END_TICKET
#undef GET_ENTRY
#undef MARK_PRIOR_POS
#undef NEXT_TICKET
#undef SCAN_IS_AT_PRIOR_POS

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
