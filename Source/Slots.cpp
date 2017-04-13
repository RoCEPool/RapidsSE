/*============================================================================*/
/* Slots.cpp - table partition slots                                          */
/*============================================================================*/

#include "DBobjects.h"

#include "StorageEngine.h"

extern Grid* gpGrid;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Slots::operator new(size_t iSize)
{
  RASSERT(iSize);
  vFatal("fatal: Slots class cannot be new'd"_Str);
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Slots::operator delete(void* pBufferAddress)
{
  RASSERT(pBufferAddress);
  vFatal("fatal: Slots class cannot be deleted"_Str);
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Slots::Slots(void)
: Object(CN_SLOTS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLOTS,_FOOTER_);
  
  // initialize the partitionId's in the slots
  for (i8 iSlot = 0; iSlot < gpGrid->iSlots(); iSlot++) {
    
    i8 iPartition = Partition::iSlotToPartition(iSlot);
    m_xPartition[iSlot].vPartitionId(iPartition);
    
  }
  
  _checkInitialised_(CN_SLOTS);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Slots::~Slots(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLOTS,_FOOTER_);
  vTidyUp();
}

/*----------------------------------------------------------------------------*/
/* get partition                                                              */
/*----------------------------------------------------------------------------*/
Partition* Slots::pPartition(i8 iSlot)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLOTS,_FOOTER_);
  
  //vTrace("Partition %lld\n"_Str,iPartition);
  
  return &m_xPartition[iSlot];
}

/*----------------------------------------------------------------------------*/
/* tidyup                                                                     */
/*----------------------------------------------------------------------------*/
void Slots::vTidyUp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLOTS,_FOOTER_);
  
  // get IdxVec from any one partition
  IdxVec* pIdxVec = m_xPartition[0].pIdxVec();
  i8 nItems = pIdxVec->nItems();
  
  // delete the indexes (as the partitions will just delete the IdxVec
  // they all share the indexes.
  for (i8 iIndex = 0; iIndex < nItems; iIndex++) {
    Index* pIndex = pIdxVec->pGet(iIndex);
    delete pIndex;
  }
  
  // run the finalize for the partitions
  for (i8 iSlot = 0; iSlot < C_SLOTS_PER_NODE; iSlot++) {
    m_xPartition[iSlot].vTidyUp();
  }
}

/*----------------------------------------------------------------------------*/
/* tuncate all partitions                                                     */
/*----------------------------------------------------------------------------*/
void Slots::vTruncate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLOTS,_FOOTER_);
  i8 iPartn;
  for (iPartn = 0; iPartn < gpGrid->iSlots(); iPartn++)
    m_xPartition[iPartn].pData()->vTruncate();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Slots::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLOTS);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
