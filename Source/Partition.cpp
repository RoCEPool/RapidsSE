/*============================================================================*/
/* Partition.cpp - table partition                                            */
/*============================================================================*/

#include "StorageEngine.h"
#include "DBobjects.h"

extern Arguments       gxArguments;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Partition::operator new(size_t iSize)
{
  RASSERT(iSize);
  vFatal("fatal: Partition class cannot be new'd");
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Partition::operator delete(void* pBufferAddress)
{
  RASSERT(pBufferAddress);
  vFatal("fatal: Partition class cannot be deleted");
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Partition::Partition(void)
: Object(CN_PARTITION)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITION,_FOOTER_);
  
  m_zLock.vCheckInit();
  m_iRowStrands = 0;
  m_pHeadStrand = nullptr;
  m_pTailStrand = nullptr;
  
  _checkInitialised_(CN_PARTITION);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_PARTITION,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Partition::~Partition(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITION,_FOOTER_);
  
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
/* convert partition number to node number                                    */
/*----------------------------------------------------------------------------*/
i8 Partition::iPartitionToNode(i8 iPartition)
{
  i8 iNode = iPartition % gxArguments.m_nNodes;

  return iNode;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append row buffer                                                          */
/*----------------------------------------------------------------------------*/
void Partition::vAppendBuffer(RowBuffer* pRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITION,_FOOTER_);
  
  m_zLock.vGrabWrite();
  
  if (! m_pTailStrand) {
    if (! bExtendPartition()) {
      m_zLock.vDropWrite();
      return;
    }
  }
  
  bool bAppended = m_pTailStrand->bAppendBuffer(pRowBuffer);
  if (bAppended) {
    m_zLock.vDropWrite();
    return;
  }
  
  if (! bExtendPartition()) {
    m_zLock.vDropWrite();
    return;
  }
  
  bAppended = m_pTailStrand->bAppendBuffer(pRowBuffer);
  
  m_zLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* insert a row into this strand                                              */
/*----------------------------------------------------------------------------*/
bool Partition::bInsertRow(Transactions::TxRef* pTxRef, char* pRecord, i8 iLength)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITION,_FOOTER_);
  
  m_zLock.vGrabWrite();
  
  if (! m_pTailStrand) {
    if (! bExtendPartition()) {
      m_zLock.vDropWrite();
      return false;
    }
  }
  
  bool bInserted = m_pTailStrand->bInsertRow(pTxRef,pRecord,iLength);
  if (bInserted) {
    m_zLock.vDropWrite();
    return true;
  }
  
  if (! bExtendPartition()) {
    m_zLock.vDropWrite();
    return false;
  }
  
  bInserted = m_pTailStrand->bInsertRow(pTxRef,pRecord,iLength);
  ASSERT(bInserted);
  
  m_zLock.vDropWrite();
  
  return bInserted;
}

/*----------------------------------------------------------------------------*/
/* get head strand                                                            */
/*----------------------------------------------------------------------------*/
RowStrand* Partition::pHeadStrand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITION,_FOOTER_);
  return m_pHeadStrand;
}

/*----------------------------------------------------------------------------*/
/* get tail strand                                                            */
/*----------------------------------------------------------------------------*/
RowStrand* Partition::pTailStrand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITION,_FOOTER_);
  return m_pTailStrand;
}

/*----------------------------------------------------------------------------*/
/* release all strands                                                        */
/*----------------------------------------------------------------------------*/
void Partition::vTruncate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITION,_FOOTER_);
  
  m_zLock.vGrabWrite();
  
  RowStrand* pNextRowStrand;
  RowStrand* pRowStrand = m_pHeadStrand;
  
  while (pRowStrand) {
    pRowStrand->vTruncate();
    pNextRowStrand = pRowStrand->pNextStrand();
    delete pRowStrand;
    pRowStrand = pNextRowStrand;
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITION,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITION);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
