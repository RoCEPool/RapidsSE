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
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Partition::~Partition(void)
{
  
}

/*----------------------------------------------------------------------------*/
/* insert a row into this strand                                              */
/*----------------------------------------------------------------------------*/
bool Partition::bInsertRow(StrVal* pRowStr)
{
  _poolAddress_(this);
  
  m_zLock.vGrabWrite();
  
  if (! m_pTailStrand) {
    if (! bExtendPartition()) {
      m_zLock.vDropWrite();
      return false;
    }
  }
  
  bool bInserted = m_pTailStrand->bInsertRow(pRowStr);
  if (bInserted) {
    m_zLock.vDropWrite();
    return true;
  }
  
  if (! bExtendPartition()) {
    m_zLock.vDropWrite();
    return false;
  }
  
  bInserted = m_pTailStrand->bInsertRow(pRowStr);
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
/* calculate location of footer                                               */
/*----------------------------------------------------------------------------*/
Footer* Partition::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/