/*============================================================================*/
/* EntBuffer_position_opt.cpp -- position related functions for EntBuffer     */
/*============================================================================*/

#include "Indexing.h"

#define ELM0_OF_KEY(aKey)   ( aKey->iGetIntElm() )

#define ELM0_OF_ENTRY(aEnt) ( aEnt->aKey()->iGetIntElm() )

#define END_TICKET(tNxtEnt) ( C_ENT_END_TICKET == tNxtEnt )

#define GET_ENTRY(tTicket)  ( aGetItem(iToEntryOffset(tTicket)) )

#define NEXT_TICKET(aEnt,i_Lvl) ( aEnt->tNextEntry(i_Lvl) )

#define SCAN_IS_AT_PRIOR_POS(iKeyElm,iNextElm) ( iKeyElm >= iNextElm )

#define STORE_PRIOR_POS(aVec,i_Lvl,aEnt) ( aVec[i_Lvl] = aEnt )

/*----------------------------------------------------------------------------*/
/* compute prior vector for given int key                                     */
/*----------------------------------------------------------------------------*/
void EntBuffer::vPosPriorVecEntry_IntOpt(Key* aKey_int, Ent* aPriorVec[])
{
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
  
  i8 iElm0_int = ELM0_OF_KEY(aKey_int);

  Ent* aScanEnt = aSkipRoots;
  for (i8 i_Level = nLvlLevels - 1; i_Level >= 0; i_Level--) {
    
    for (tET tNextEnt = NEXT_TICKET(aScanEnt,i_Level);
         ! END_TICKET(tNextEnt);
         tNextEnt = NEXT_TICKET(aScanEnt,i_Level)) {

      Ent* aNextEnt = GET_ENTRY(tNextEnt);
      i8 iNextElm0_int = ELM0_OF_ENTRY(aNextEnt);
            
      if (SCAN_IS_AT_PRIOR_POS(iElm0_int,iNextElm0_int))
        break;
      
      aScanEnt = aNextEnt;
      
    } // next aScanEnt at i_Level

    STORE_PRIOR_POS(aPriorVec,i_Level,aScanEnt);
    
  } // next i_Level
  
}

#undef ELM0_OF_KEY
#undef ELM0_OF_ENTRY
#undef END_TICKET
#undef GET_ENTRY
#undef NEXT_TICKET
#undef SCAN_IS_AT_PRIOR_POS
#undef STORE_PRIOR_POS


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


#define ELM0_OF_KEY(aKey) ( aKey->iGetIntElm() )

#define ELM0_OF_ENTRY(aEnt_int) ( aEnt_int->m_iIntElm0 )

#define END_TICKET(tNxtEnt) ( C_ENT_END_TICKET == tNxtEnt )

#define GET_ENTRY(tTicket) ( &aItemArray[tTicket] )

#define NEXT_TICKET(aEnt_int,i_Lvl) ( aEnt_int->m_taNextEntry[i_Lvl] )

#define SCAN_IS_AT_PRIOR_POS(iKeyElm,iNextElm) ( iKeyElm <= iNextElm )

/*----------------------------------------------------------------------------*/
/* find entry given int key                                                   */
/*----------------------------------------------------------------------------*/
Ent* EntBuffer::aFindEnt_IntOpt(Key* aKey_int)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);

  RASSERT(KE_INT == m_eEntKind);
  
  // if there are no items then the prior vec is the set of skip roots
  if (nItems() == 0)
    return nullptr;
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 nLvlLevels = AM::nLvlLevels();
    
  Ent* aSkipRoots = this->aSkipRoots();
  
  Ent_int* aItemArray = (Ent_int*)this->aItemArray();
  
  i8 iKey_IntElm = ELM0_OF_KEY(aKey_int);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Ent_int* aScanEnt = (Ent_int*)aSkipRoots;
  for (i4 i_Level = (i4)nLvlLevels - 1; i_Level >= 0; i_Level--) {
    
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

#undef ELM0_OF_KEY
#undef ELM0_OF_ENTRY
#undef END_TICKET
#undef GET_ENTRY
#undef STORE_PRIOR_POS
#undef NEXT_TICKET
#undef SCAN_IS_AT_PRIOR_POS

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
