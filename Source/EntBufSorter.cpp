/*============================================================================*/
/* SkipRoots.cpp -- roots of the skips list in an entry buffer                */
/*============================================================================*/

#include "Indexing.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Indexing,EntBufSorter,CN_ENTBUFSORTER)
__aNewFunction(EntBufSorter,CN_ENTBUFSORTER)
__vDeleteFunction(EntBufSorter,CN_ENTBUFSORTER)
__aFooterFunction(EntBufSorter)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntBufSorter::EntBufSorter(void)
: Object(CN_ENTBUFSORTER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFSORTER,_FOOTER_);
  
  MemLib::vZero(m_taRevisedTicket,sizeof(m_taRevisedTicket),CN_ENTBUFSORTER);
    
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFSORTER,_FOOTER_);
  
  _checkInitialised_(CN_ENTBUFSORTER);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
EntBufSorter::~EntBufSorter(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

int Indexing::iEntBufSortCompare_IntElm0(const void* aLeft, const void* aRight)
{
  Ent_int* aEntLeft  = (Ent_int*)aLeft;
  Ent_int* aEntRight = (Ent_int*)aRight;

  if (aEntLeft->m_iIntElm0 < aEntRight->m_iIntElm0)
    return -1;
  else
  if (aEntLeft->m_iIntElm0 > aEntRight->m_iIntElm0)
    return 1;
  else
    return 0;
}

int Indexing::iEntBufSortCompare_Lvl_IntElm0(const void* aLeft, const void* aRight)
{
  Ent_int* aEntLeft  = (Ent_int*)aLeft;
  Ent_int* aEntRight = (Ent_int*)aRight;
  
  if (aEntLeft->Ref.m_uLevels > aEntRight->Ref.m_uLevels)
    return -1;
  else
  if (aEntLeft->Ref.m_uLevels < aEntRight->Ref.m_uLevels)
    return 1;

  if (aEntLeft->m_iIntElm0 < aEntRight->m_iIntElm0)
    return -1;
  else
  if (aEntLeft->m_iIntElm0 > aEntRight->m_iIntElm0)
    return 1;
  else
    return 0;
}

/*----------------------------------------------------------------------------*/
/* sort ent buffer of KE_INT                                                  */
/*----------------------------------------------------------------------------*/
void EntBufSorter::vSort_int(EntBuffer* pEntBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFSORTER,_FOOTER_);
  
  i8 nItems = pEntBuffer->nItems();
  Ent_int* aItemArray = (Ent_int*)pEntBuffer->aItemArray();
  
  //pEntBuffer->vShow(true);

  // number the entries
  pEntBuffer->vRecordTickets();
  
  //pEntBuffer->vShow(true);

  // sort entry array into ascending order
  qsort(aItemArray,
        nItems,
        sizeof(Ent_int),
        //Indexing::iEntBufSortCompare_IntElm0    // by IntElm0
        Indexing::iEntBufSortCompare_Lvl_IntElm0  // by Level,IntElm0
        );
  
  // for each entry in array, record its revised ticket
  for (i8 i_Item = 0; i_Item < nItems; i_Item++) {
    Ent_int* aEnt_int = &aItemArray[i_Item];
    tET tOriginalTicket = aEnt_int->m_uEntryNo;
    tET tRevisedTicket  = pEntBuffer->tToTicket((Ent*)aEnt_int);
    m_taRevisedTicket[tOriginalTicket] = tRevisedTicket;
    //vTrace("[%lld] = %lld\n"_Stz,(i8)tOriginalTicket,(i8)tRevisedTicket);
  }
  
//  for (i8 i_Item = 0; i_Item < nItems; i_Item++) {
//    Ent_int* aEnt_int = &aItemArray[i_Item];
//    pEntBuffer->vShowEntry((Ent*)aEnt_int);
//  }
  
  SkipRoots* aSkipRoots = pEntBuffer->aSkipRoots();
  for (i8 i_Lvl = 0; i_Lvl < AM::nLvlLevels(); i_Lvl++) {
    tET tOriginalTicket = aSkipRoots->tNextEntry(i_Lvl);
    if (C_ENT_END_TICKET == tOriginalTicket)
      continue;
    
    tET tRevisedTicket = m_taRevisedTicket[tOriginalTicket];
    aSkipRoots->vNextEntry(i_Lvl,tRevisedTicket);
  }
  
  // for each entry in array, update each level
  for (i8 i_Item = 0; i_Item < nItems; i_Item++) {
    Ent_int* aEnt_int = &aItemArray[i_Item];

    i8 nLevels = aEnt_int->Ref.m_uLevels;
    for (i8 i_Lvl = 0; i_Lvl < nLevels; i_Lvl++) {
      tET tOriginalTicket = aEnt_int->m_taNextEntry[i_Lvl];
      if (C_ENT_END_TICKET == tOriginalTicket)
        continue;

      tET tRevisedTicket = m_taRevisedTicket[tOriginalTicket];
      aEnt_int->m_taNextEntry[i_Lvl] = tRevisedTicket;
    }
    
    //pEntBuffer->vShowEntry((Ent*)aEnt_int);
  }

//  for (i8 i_Item = 0; i_Item < nItems; i_Item++) {
//    Ent_int* aEnt_int = &aItemArray[i_Item];
//    pEntBuffer->vShowEntry((Ent*)aEnt_int);
//  }

  //pEntBuffer->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
