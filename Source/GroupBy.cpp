/*============================================================================*/
/* GroupBy.cpp -- MockEE Group By                                             */
/*============================================================================*/

#include "MockEE.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
GroupBy* MockEE::_pGroupBy(void* aAddr)
{
  GroupBy* pGroupBy = reinterpret_cast<GroupBy*>(aAddr);
  
  RASSERT( ! pGroupBy || CN_GROUPBY == pGroupBy->eClass() );
 
  return pGroupBy;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* GroupBy::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  void* pObject = Heap::pAllocate(CN_GROUPBY);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void GroupBy::operator delete(void* pDataAddress)
{
  AccessPath* pAccessPath = reinterpret_cast<AccessPath*>(pDataAddress);
  
  Heap::vFree(pAccessPath);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
GroupBy::GroupBy(void)
: Object(CN_GROUPBY)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GROUPBY,_FOOTER_);
  
  m_pAccumVec = new AccumVec;
  
  _checkInitialised_(CN_GROUPBY);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_GROUPBY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
GroupBy::~GroupBy(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GROUPBY,_FOOTER_);
  
  delete m_pAccumVec;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* apply buffer or lineitems                                                  */
/*----------------------------------------------------------------------------*/
void GroupBy::vApplyBuffer(EntryBuffer* pEntryBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GROUPBY,_FOOTER_);

  //
  // we would iterate each LineItem in entry buffer and
  // apply it to the appropriate accumulator
  //
  
//  LineItem* pLineItem = (LineItem*)pEntryBuffer; // temp, not correct
//  
//  i8 i_Group = 0;
//  
//  Accum* pAccum = m_pAccumVec->pGet(i_Group);
//  
  
  Entry* pEntry = pEntryBuffer->pFirstRow();
  while( (pEntry = pEntryBuffer->pNextRow(pEntry)) ) {
    
    LineItem* pLineItem = (LineItem*)pEntry->aRow();
    
    vTrace("\nEntry: %p LineItem: %p String: %p \n>>%s<<\n"_Str,
           pEntry,pLineItem,pLineItem->aStr(),pLineItem->aStr());
    MemLib::vHexDump(pEntry,512);
    vTrace("\n"_Str);
    
    char* aCursor = pLineItem->s();
    i8 iOrderKey;
    bool b = DtmLib::bExtract(DM_BCD,(Str*)&iOrderKey,&aCursor,nullptr);
    
    aCursor = pLineItem->s();
    i8 iQuantity;
    b = StrLib::bFind(&aCursor,'|',5);
    aCursor++;
    b = DtmLib::bExtract(DM_BCD,(Str*)&iQuantity,&aCursor,nullptr);
    
    i8 i_Group = iOrderKey % 10;
    Accum* pAccum = m_pAccumVec->pGet(i_Group);
    
    pAccum->m_iSum += iQuantity;

    if (iQuantity < pAccum->m_iMin)
      pAccum->m_iMin = iQuantity;

    if (iQuantity > pAccum->m_iMax)
      pAccum->m_iMax = iQuantity;

    pAccum->m_iCount++;
  }
  
}

/*----------------------------------------------------------------------------*/
/* end                                                                        */
/*----------------------------------------------------------------------------*/
void GroupBy::vEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GROUPBY,_FOOTER_);
  
  for (i8 i_Group = 0; i_Group < m_pAccumVec->nItems(); i_Group++) {
    Accum* pAccum = m_pAccumVec->pGet(i_Group);
    vTrace("Group: %lld Count: %lld Sum: %lld Min: %lld Max: %lld\n"_Str,
           pAccum->m_iGroup,
           pAccum->m_iCount,
           pAccum->m_iSum,
           pAccum->m_iMin,
           pAccum->m_iMax
           );
  }
}

/*----------------------------------------------------------------------------*/
/* start                                                                      */
/*----------------------------------------------------------------------------*/
void GroupBy::vStart(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GROUPBY,_FOOTER_);
  
  for (i8 i_Group = 0; i_Group < 10; i_Group++) {
    Accum* pAccum = new Accum;
    pAccum->m_iGroup = i_Group;
    m_pAccumVec->vAdd(pAccum);
  }
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

_aFooter_(GroupBy);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
