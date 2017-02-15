/*============================================================================*/
/* Data.cpp - data portion of table partition                                 */
/*============================================================================*/

#include "DBobjects.h"

extern BufferPool*  gpBufferPool;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Data::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  void* pObject = Heap::pAllocate(CN_DATA);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Data::operator delete(void* pDataAddress)
{
  Data* pData = reinterpret_cast<Data*>(pDataAddress);
  
  Heap::vFree(pData);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Data::Data(void)
: Object(CN_DATA)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DATA,_FOOTER_);
  
  m_zLock.vCheckInit();
  m_iRowStrands = 0;
  m_pHeadStrand = nullptr;
  m_pTailStrand = nullptr;
    
  _checkInitialised_(CN_DATA);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_DATA,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Data::~Data(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DATA,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump data                                                                  */
/*----------------------------------------------------------------------------*/
void Data::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_DATA);
  ASSERT(pObject->eClass() == CN_DATA);
  
  Data* pData = reinterpret_cast<Data*>(pObject);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append row buffer                                                          */
/*----------------------------------------------------------------------------*/
void Data::vAppendBuffer(RowBuffer* pRowBuffer)
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
bool Data::bInsertRow(Transactions::TxRef* pTxRef, char* pRecord, i8 iLength)
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
RowStrand* Data::pHeadStrand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITION,_FOOTER_);
  return m_pHeadStrand;
}

/*----------------------------------------------------------------------------*/
/* get tail strand                                                            */
/*----------------------------------------------------------------------------*/
RowStrand* Data::pTailStrand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARTITION,_FOOTER_);
  return m_pTailStrand;
}

/*----------------------------------------------------------------------------*/
/* release all strands                                                        */
/*----------------------------------------------------------------------------*/
void Data::vTruncate(void)
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
bool Data::bExtendPartition(void)
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
Footer* Data::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DATA);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
