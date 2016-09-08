/*============================================================================*/
/* Partition.cpp - table partition                                            */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Partition::operator new(size_t iSize)
{
  vFatal("fatal: Partition class cannot be new'd");
  
  //vExitFatal();
  
  exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Partition::operator delete(void* pBufferAddress)
{
  vFatal("fatal: Partition class cannot be deleted");
  
  //vExitFatal();
  
  exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Partition::Partition(void)
: Object(CN_PARTITION)
{
  _poolAddress_(this);
  
  m_zLock.vCheckInit();
  m_iRowStrands = 0;
  m_pHeadStrand = nullptr;
  m_pTailStrand = nullptr;
  
  Memory::vCheckInitialised(this,sizeof(Partition));
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Partition::~Partition(void)
{
  
}

/*----------------------------------------------------------------------------*/
/* dump partition                                                             */
/*----------------------------------------------------------------------------*/
void Partition::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_PARTITION);
  ASSERT(pObject->eClass() == CN_PARTITION);
  
  Partition* pPartition = reinterpret_cast<Partition*>(pObject);
  
  vTrace("%sPartition %p:\n", pTab, pPartition);
  vTrace("%s  row strands %s \n", pTab, pPartition->m_iRowStrands);
  vTrace("%s  head strand %p\n\n", pTab, pPartition->m_pHeadStrand);
  vTrace("%s  tail strand %p\n\n", pTab, pPartition->m_pTailStrand);
}

/*----------------------------------------------------------------------------*/
/* insert a row into this strand                                              */
/*----------------------------------------------------------------------------*/
bool Partition::bInsertRow(Transactions::TxRef* pTxRef, StrVal* pRowStr)
{
  _poolAddress_(this);
  
  m_zLock.vGrabWrite();
  
  if (! m_pTailStrand) {
    if (! bExtendPartition()) {
      m_zLock.vDropWrite();
      return false;
    }
  }
  
  bool bInserted = m_pTailStrand->bInsertRow(pTxRef,pRowStr);
  if (bInserted) {
    m_zLock.vDropWrite();
    return true;
  }
  
  if (! bExtendPartition()) {
    m_zLock.vDropWrite();
    return false;
  }
  
  bInserted = m_pTailStrand->bInsertRow(pTxRef,pRowStr);
  ASSERT(bInserted);
  
  m_zLock.vDropWrite();
  
  return bInserted;
}

/*----------------------------------------------------------------------------*/
/* get head strand                                                            */
/*----------------------------------------------------------------------------*/
RowStrand* Partition::pHeadStrand(void)
{
  return m_pHeadStrand;
}

/*----------------------------------------------------------------------------*/
/* get tail strand                                                            */
/*----------------------------------------------------------------------------*/
RowStrand* Partition::pTailStrand(void)
{
  return m_pTailStrand;
}

/*----------------------------------------------------------------------------*/
/* release all strands                                                        */
/*----------------------------------------------------------------------------*/
void Partition::vTruncate(void)
{
  _poolAddress_(this);
  
  m_zLock.vGrabWrite();
  
  RowStrand* pRowStrand = m_pHeadStrand;
  while (pRowStrand) {
    pRowStrand->vTruncate();
    pRowStrand = pRowStrand->pNextStrand();
  }
  
  m_iRowStrands = 0;
  m_pHeadStrand = nullptr;
  m_pTailStrand = nullptr;
  
  m_zLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* extend this partition                                                      */
/*----------------------------------------------------------------------------*/
bool Partition::bExtendPartition(void)
{
  _poolAddress_(this);
  
   RowStrand* pNewStrand = new RowStrand;
  
  if (! m_pHeadStrand) {
    ASSERT(! m_iRowStrands);
    ASSERT(! m_pTailStrand);
    
    m_pHeadStrand = pNewStrand;
    m_pTailStrand = pNewStrand;
    
    m_iRowStrands = 1;
  }
  else {
    ASSERT(m_iRowStrands);
    ASSERT(m_pTailStrand);
    
    m_pTailStrand->vNextStrand(pNewStrand);
    pNewStrand->vPrevStrand(m_pTailStrand);
    
    m_pTailStrand = pNewStrand;
    
    m_iRowStrands++;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Partition::pFooter(void)
{
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/