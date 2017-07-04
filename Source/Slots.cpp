/*============================================================================*/
/* Slots.cpp - table partition slots                                          */
/*============================================================================*/

#include "DBobjects.h"

#include "StorageEngine.h"

extern Grid* gpGrid;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Slots* DBobjects::_pSlots(void* aAddr)
{
  Slots* pSlots = reinterpret_cast<Slots*>(aAddr);
  
  RASSERT( !pSlots || CN_SLOTS == pSlots->eClass() );
  
  return pSlots;
}

/*----------------------------------------------------------------------------*/
/* dump data                                                                  */
/*----------------------------------------------------------------------------*/
void Slots::vDump(eCN eClass, Object* pObject, Str* aTab)
{
  ASSERT(eClass == CN_SLOTS);
  ASSERT(pObject->eClass() == CN_SLOTS);
  
  
}

/*----------------------------------------------------------------------------*/
/* finalize                                                                   */
/*----------------------------------------------------------------------------*/
void Slots::vFinalize(eCN eClass, Object* pObject)
{
  ASSERT(eClass == CN_SLOTS);
  ASSERT(pObject->eClass() == CN_SLOTS);
  
  Slots* pSlots = reinterpret_cast<Slots*>(pObject);
  ASSERT(pSlots->bWordBody());
  
  pSlots->vTidyUp();
}

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Slots::operator new(size_t iSize)
{
  RASSERT(iSize);
  vFatal("fatal: Slots class cannot be new'd"_Str);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Slots::operator delete(void* pBufferAddress)
{
  RASSERT(pBufferAddress);
  vFatal("fatal: Slots class cannot be deleted"_Str);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Slots::Slots(void)
: Object(CN_SLOTS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLOTS,_FOOTER_);
  
  m_pTable = nullptr;
  
  //m_xSubDic
  
  m_pPartitionVec = new PartitionVec;
  
  // initialize the partitionId's in the slots
  for (i8 i_Slot = 0; i_Slot < gpGrid->iSlots(); i_Slot++) {
    
    Partition* pPartition = new Partition;
    m_pPartitionVec->vAdd(pPartition);
    
    i8 iPartition = Partition::iSlotToPartition(i_Slot);
    m_pPartitionVec->pGet(i_Slot)->vPartitionId(iPartition);
    
  }
  
  m_pPtnColVec = new PtnColVec;
  
  _checkInitialised_(CN_SLOTS);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Slots::~Slots(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLOTS,_FOOTER_);
  vTidyUp();
  
  delete m_pPartitionVec;
}

/*----------------------------------------------------------------------------*/
/* create column vector                                                       */
/*----------------------------------------------------------------------------*/
void Slots::vCreateColumnVec(s32& sColumnNm, s48K& sColumnDefn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLOTS,_FOOTER_);
  
  Column* pColumn = nullptr;
  
  // find the column from the table columnVector
  i8 iColumns = m_pTable->pColumnVec()->nItems();
  
  bool bFound = false;
  for (i8 i_Col = 0; i_Col < iColumns; i_Col++) {
    pColumn = m_pTable->pColumnVec()->pGet(i_Col);
    if (pColumn->sColumnName().bSame(sColumnNm)) {
      bFound = true;
      break;
    }
  }
  ASSERT(bFound);
  
  m_pPtnColVec->vInsert(pColumn);
}

/*----------------------------------------------------------------------------*/
/* get partition                                                              */
/*----------------------------------------------------------------------------*/
Partition* Slots::pPartition(i8 iSlot)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLOTS,_FOOTER_);
  
  //vTrace("Partition %lld\n"_Str,iPartition);
  
  return m_pPartitionVec->pGet(iSlot);
}

/*----------------------------------------------------------------------------*/
/* populate index of all partitions                                           */
/*----------------------------------------------------------------------------*/
void Slots::vPopulateIndex(i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLOTS,_FOOTER_);
  
  for (i8 i_Partn = 0; i_Partn < gpGrid->iSlots(); i_Partn++)
    m_pPartitionVec->pGet(i_Partn)->vPopulateIndex(iIndexNo);
}

/*----------------------------------------------------------------------------*/
/* get ptnColVec                                                              */
/*----------------------------------------------------------------------------*/
PtnColVec* Slots::pPtnColVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLOTS,_FOOTER_);
  
  return m_pPtnColVec;
}

/*----------------------------------------------------------------------------*/
/* show column vector                                                         */
/*----------------------------------------------------------------------------*/
void Slots::vShowColumnVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLOTS,_FOOTER_);
  
  for (i8 i_Column = 0; i_Column < m_pPtnColVec->nItems(); i_Column++) {
    Column* pColumn = m_pPtnColVec->pGet(i_Column);
    pColumn->vShow();
  }
}

/*----------------------------------------------------------------------------*/
/* get partitioning sub dictionary                                            */
/*----------------------------------------------------------------------------*/
Dic* Slots::pSubDic(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLOTS,_FOOTER_);
  
  return &m_xSubDic;
}

/*----------------------------------------------------------------------------*/
/* set table                                                                  */
/*----------------------------------------------------------------------------*/
void Slots::vTable(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLOTS,_FOOTER_);
  
  m_pTable = pTable;
}

/*----------------------------------------------------------------------------*/
/* tidyup                                                                     */
/*----------------------------------------------------------------------------*/
void Slots::vTidyUp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLOTS,_FOOTER_);
  
  // get IdxDataVec from any one partition
  IdxDataVec* pIdxDataVec = m_pPartitionVec->pGet(0)->pIdxDataVec();
  i8 nItems = pIdxDataVec->nItems();
  
  // run the finalize for the partitions
  for (i8 i_Slot = 0; i_Slot < C_SLOTS_PER_NODE; i_Slot++) {
    m_pPartitionVec->pGet(i_Slot)->vTidyUp();
  }
  
  // Since the column is picked from the ColVec of table
  // we need to just remove it from the PtnColVec before deleting it
  m_pPtnColVec->vClear();
  
  delete m_pPtnColVec;
}

/*----------------------------------------------------------------------------*/
/* tidyup a particular index                                                  */
/*----------------------------------------------------------------------------*/
void Slots::vTidyUpIndex(i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLOTS,_FOOTER_);
  
  // run the finalize for the partitions
  for (i8 i_Slot = 0; i_Slot < gpGrid->iSlots(); i_Slot++) {
    m_pPartitionVec->pGet(i_Slot)->vTidyUpIndex(iIndexNo);
  }
}

/*----------------------------------------------------------------------------*/
/* tuncate all partitions                                                     */
/*----------------------------------------------------------------------------*/
void Slots::vTruncate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLOTS,_FOOTER_);
  i8 i_Partn;
  for (i_Partn = 0; i_Partn < gpGrid->iSlots(); i_Partn++)
    m_pPartitionVec->pGet(i_Partn)->pData()->vTruncate();
}

/*----------------------------------------------------------------------------*/
/* tuncate all partitions                                                     */
/*----------------------------------------------------------------------------*/
void Slots::vTruncateIndex(i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLOTS,_FOOTER_);
  
  i8 i_Partn;
  for (i_Partn = 0; i_Partn < gpGrid->iSlots(); i_Partn++)
    m_pPartitionVec->pGet(i_Partn)->pIndexData(iIndexNo)->vTruncate();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Slots::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLOTS);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
