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
  
  m_zLock.vInitLock(LK_DATA);
  m_nEbStrands = 0;
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
  
  ASSERT( ! m_nEbStrands );
}

/*----------------------------------------------------------------------------*/
/* dump data                                                                  */
/*----------------------------------------------------------------------------*/
void Data::vDump(eCN eClass, Object* pObject, Stz* aTab)
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
void Data::vAppendBuffer(EntryBuffer* pRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DATA,_FOOTER_);
  
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
bool Data::bInsertRow(Transactions::TxRef* pTxRef, Stz* pRecord, i8 iLength)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DATA,_FOOTER_);
  
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
EbStrand* Data::pHeadStrand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DATA,_FOOTER_);
  return m_pHeadStrand;
}

/*----------------------------------------------------------------------------*/
/* get tail strand                                                            */
/*----------------------------------------------------------------------------*/
EbStrand* Data::pTailStrand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DATA,_FOOTER_);
  return m_pTailStrand;
}

/*----------------------------------------------------------------------------*/
/* get number of row strands                                                  */
/*----------------------------------------------------------------------------*/
i8 Data::nEbStrands(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DATA,_FOOTER_);
  return m_nEbStrands;
}

/*----------------------------------------------------------------------------*/
/* release all strands                                                        */
/*----------------------------------------------------------------------------*/
void Data::vTruncate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DATA,_FOOTER_);
  
  m_zLock.vGrabWrite();
  
  EbStrand* pNextEbStrand;
  EbStrand* pEbStrand = m_pHeadStrand;
  
  while (pEbStrand) {
    pEbStrand->vTruncate();
    pNextEbStrand = pEbStrand->pNextStrand();
    delete pEbStrand;
    pEbStrand = pNextEbStrand;
  }
  
  m_nEbStrands = 0;
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_DATA,_FOOTER_);
  
  EbStrand* pNewStrand = new EbStrand;
  
  if (! m_pHeadStrand) {
    ASSERT(! m_nEbStrands);
    ASSERT(! m_pTailStrand);
    
    m_pHeadStrand = pNewStrand;
    m_pTailStrand = pNewStrand;
    
    m_nEbStrands = 1;
  }
  else {
    ASSERT(m_nEbStrands);
    ASSERT(m_pTailStrand);
    
    m_pTailStrand->vNextStrand(pNewStrand);
    pNewStrand->vPrevStrand(m_pTailStrand);
    
    m_pTailStrand = pNewStrand;
    
    m_nEbStrands++;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Data::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DATA);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
