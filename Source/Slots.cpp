/*============================================================================*/
/* Slots.cpp - table partition slots                                          */
/*============================================================================*/

#include "DBobjects.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Slots* DBobjects::_pSlots(void* aAddr)
{
  Slots* pSlots = reinterpret_cast<Slots*>(aAddr);
  
  RASSERT_TYPE(pSlots,CN_SLOTS);
  
  return pSlots;
}

/*----------------------------------------------------------------------------*/
/* dump data                                                                  */
/*----------------------------------------------------------------------------*/
void Slots::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
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
void* Slots::operator new(size_t)
{
  vFatal("fatal: Slots class cannot be new'd"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Slots::operator delete(void*)
{
  vFatal("fatal: Slots class cannot be deleted"_Stz);
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
  
  m_pTablePtnVec = new TablePtnVec;
  
  // initialize the partitionId's in the slots
  for (i8 i_Slot = 0; i_Slot < gpAttachMap->m_pGrid->nSlots(); i_Slot++) {
    
    TablePtn* pPartition = new TablePtn;
    m_pTablePtnVec->vAdd(pPartition);
    
    i8 iPartition = TablePtn::iSlotToPartition(i_Slot);
    m_pTablePtnVec->pGet(i_Slot)->vPartitionId(iPartition);
    
  }
  
  m_pPtnColVec = new PtnColVec;
  
  _checkInitialised_(CN_SLOTS);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Slots::~Slots(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLOTS,_FOOTER_);
  vTidyUp();
  
  delete m_pTablePtnVec;
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
  
  // mark teh column as a partion key column
  pColumn->vIsPartitionKey(true);
  
  m_pPtnColVec->vInsert(pColumn);
}

/*----------------------------------------------------------------------------*/
/* create column vector                                                       */
/*----------------------------------------------------------------------------*/
void Slots::vCreateColumnVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLOTS,_FOOTER_);
  
  Column* pColumn = nullptr;
  
  // find the column from the table columnVector
  i8 iColumns = m_pTable->pColumnVec()->nItems();
  
  bool bFound = false;
  for (i8 i_Col = 0; i_Col < iColumns; i_Col++) {
    pColumn = m_pTable->pColumnVec()->pGet(i_Col);
    if (pColumn->bIsPartitionKey()) {
      m_pPtnColVec->vInsert(pColumn);
    }
  }
}

/*----------------------------------------------------------------------------*/
/* get partition                                                              */
/*----------------------------------------------------------------------------*/
TablePtn* Slots::pPartition(i8 iSlot)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLOTS,_FOOTER_);
  
  //vTrace("Partition %lld\n"_Stz,iPartition);
  
  return m_pTablePtnVec->pGet(iSlot);
}

/*----------------------------------------------------------------------------*/
/* populate index of all partitions                                           */
/*----------------------------------------------------------------------------*/
bool Slots::bPopulateIndex(i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLOTS,_FOOTER_);
  
  for (i8 i_Partn = 0; i_Partn < gpAttachMap->m_pGrid->nSlots(); i_Partn++) {
    bool bPopulated = m_pTablePtnVec->pGet(i_Partn)->bPopulateIndex(iIndexNo);
    
    if (! bPopulated)
      return false;
  }
  
  return true;
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
/* publish partition buffers                                                  */
/*----------------------------------------------------------------------------*/
void Slots::vPublishIndex(i8 iPartitionNO, i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLOTS,_FOOTER_);
  
  m_pTablePtnVec->pGet(iPartitionNO)->pIndexTree(iIndexNo)->vPublish();
}

/*----------------------------------------------------------------------------*/
/* purge partition buffers                                                    */
/*----------------------------------------------------------------------------*/
void Slots::vPurgeIndex(i8 iPartitionNO, i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLOTS,_FOOTER_);
  
  m_pTablePtnVec->pGet(iPartitionNO)->pIndexTree(iIndexNo)->vPurge();
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
  IndexTreeVec* pIndexTreeVec = m_pTablePtnVec->pGet(0)->pIndexTreeVec();
  i8 nItems = pIndexTreeVec->nItems();
  
  // run the finalize for the partitions
  for (i8 i_Slot = 0; i_Slot < gpAttachMap->m_pGrid->nSlots(); i_Slot++) {
    //m_pTablePtnVec->pGet(i_Slot)->vTidyUp();
    delete m_pTablePtnVec->pGet(i_Slot);
  }
  m_pTablePtnVec->vClear();
  
  // delete the partition vector
  delete m_pTablePtnVec;
  
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
  for (i8 i_Slot = 0; i_Slot < gpAttachMap->m_pGrid->nSlots(); i_Slot++) {
    m_pTablePtnVec->pGet(i_Slot)->vTidyUpIndex(iIndexNo);
  }
}

/*----------------------------------------------------------------------------*/
/* tuncate all partitions                                                     */
/*----------------------------------------------------------------------------*/
void Slots::vTruncate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLOTS,_FOOTER_);

  i8 i_Partn;
  for (i_Partn = 0; i_Partn < gpAttachMap->m_pGrid->nSlots(); i_Partn++)
    m_pTablePtnVec->pGet(i_Partn)->pHiddenIndexTree()->vTruncate();
}

/*----------------------------------------------------------------------------*/
/* tuncate partition                                                          */
/*----------------------------------------------------------------------------*/
void Slots::vTruncateIndex(i8 iPartitionNO, i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLOTS,_FOOTER_);
  
  m_pTablePtnVec->pGet(iPartitionNO)->pIndexTree(iIndexNo)->vTruncate();
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
