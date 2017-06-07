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
  
  m_pTable = nullptr;
  
  //m_xSubDic
  
  // initialize the partitionId's in the slots
  for (i8 iSlot = 0; iSlot < gpGrid->iSlots(); iSlot++) {
    
    i8 iPartition = Partition::iSlotToPartition(iSlot);
    m_xPartition[iSlot].vPartitionId(iPartition);
    
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
  for (i8 iCol = 0; iCol < iColumns; iCol++) {
    pColumn = m_pTable->pColumnVec()->pGet(iCol);
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
  
  return &m_xPartition[iSlot];
}

/*----------------------------------------------------------------------------*/
/* populate index of all partitions                                           */
/*----------------------------------------------------------------------------*/
void Slots::vPopulateIndex(i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLOTS,_FOOTER_);
  
  for (i8 iPartn = 0; iPartn < gpGrid->iSlots(); iPartn++)
    m_xPartition[iPartn].vPopulateIndex(iIndexNo);
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
  
  for (i8 iColumn = 0; iColumn < m_pPtnColVec->nItems(); iColumn++) {
    Column* pColumn = m_pPtnColVec->pGet(iColumn);
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
  
  // get IdxVec from any one partition
  IdxVec* pIdxVec = m_xPartition[0].pIdxVec();
  i8 nItems = pIdxVec->nItems();
  
  // run the finalize for the partitions
  for (i8 iSlot = 0; iSlot < C_SLOTS_PER_NODE; iSlot++) {
    m_xPartition[iSlot].vTidyUp();
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
  for (i8 iSlot = 0; iSlot < gpGrid->iSlots(); iSlot++) {
    m_xPartition[iSlot].vTidyUpIndex(iIndexNo);
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
/* tuncate all partitions                                                     */
/*----------------------------------------------------------------------------*/
void Slots::vTruncateIndex(i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLOTS,_FOOTER_);
  
  i8 iPartn;
  for (iPartn = 0; iPartn < gpGrid->iSlots(); iPartn++)
    m_xPartition[iPartn].pIndexData(iIndexNo)->vTruncate();
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
