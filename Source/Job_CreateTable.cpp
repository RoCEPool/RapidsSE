/*============================================================================*/
/* Job_CreateTable.cpp -- Create Table Job                                    */
/*============================================================================*/

#include "JobModel.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_CreateTable* JobsModel::_pJob_CreateTable(void* aAddress)
{
  Job_CreateTable* pJob_CreateTable = reinterpret_cast<Job_CreateTable*>(aAddress);
  
  RASSERT( !pJob_CreateTable || CN_JOB_CREATETABLE == pJob_CreateTable->eClass());
  
  return pJob_CreateTable;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_CreateTable::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_CreateTable* pJob_CreateTable =
    static_cast<Job_CreateTable*>(Heap::pAllocate(CN_JOB_CREATETABLE));
  
  return pJob_CreateTable;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_CreateTable::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_CreateTable::operator delete(void* aAddress)
{
  Job_CreateTable* pJob_CreateTable = static_cast<Job_CreateTable*>(aAddress);
  
  Heap::vFree(pJob_CreateTable);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_CreateTable::Job_CreateTable(Interface::SEitf* pSEitf)
: Job(CN_JOB_CREATETABLE), JobDicPath(pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATETABLE,_FOOTER_);
  
  m_sExecute = "Job_CreateTable.execute"_Stz;
  
  m_eTableType = TT_NONE;
  m_iColumns   = 0;
  
  for (i8 i_Column = 0; i_Column < C_MAX_COLUMNS; i_Column++) {
    m_xColumns[i_Column].m_iColumnNo = -1;
    m_xColumns[i_Column].m_sColumnName = ""_Stz;
    m_xColumns[i_Column].m_sColumnType = ""_Stz;
    m_xColumns[i_Column].m_iColumnLength = -1;
    m_xColumns[i_Column].m_iColumnScale = -1;
    m_xColumns[i_Column].m_bIsNullable = true;
    m_xColumns[i_Column].m_bIsUnique = false;
    m_xColumns[i_Column].m_bIsPartitionKey = false;
    m_xColumns[i_Column].m_eDatumType = DM_NONE;
  }
  
  _checkInitialised_(CN_JOB_CREATETABLE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATETABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_CreateTable::~Job_CreateTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATETABLE,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_CreateTable::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_CREATETABLE == eClass);
  RASSERT(pObject->eClass() == CN_JOB_CREATETABLE);
  
  Job_CreateTable* pJob_CreateTable = _pJob_CreateTable(pObject);

  pJob_CreateTable->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_CreateTable::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATETABLE,_FOOTER_);
  
  vTrace("\nJob_CreateTable: %s\n"_Stz, m_sObjectName.aChar());
  
  vTrace("m_sExecute = %s\n"_Stz,m_sExecute.aChar());
  vTrace("m_eTableType = %s\n"_Stz,aTT[m_eTableType]);
  vTrace("m_iColumns = %lld\n"_Stz,m_iColumns);
  
  for (i8 i_Column = 0; i_Column < m_iColumns; i_Column++) {
    vTrace("m_xColumns[%lld].m_iColumnNo = %lld\n"_Stz, i_Column, m_xColumns[i_Column].m_iColumnNo);
    vTrace("m_xColumns[%lld].m_sColumnName = %s\n"_Stz, i_Column, m_xColumns[i_Column].m_sColumnName.aChar());
    vTrace("m_xColumns[%lld].m_sColumnType = %s\n"_Stz, i_Column, m_xColumns[i_Column].m_sColumnType.aChar());
    vTrace("m_xColumns[%lld].m_iColumnLength = %lld\n"_Stz, i_Column, m_xColumns[i_Column].m_iColumnLength);
    vTrace("m_xColumns[%lld].m_iColumnScale = %lld\n"_Stz, i_Column, m_xColumns[i_Column].m_iColumnScale);
    vTrace("m_xColumns[%lld].m_bIsNullable = %s\n"_Stz, i_Column, m_xColumns[i_Column].m_bIsNullable ? "true" : "false");
    vTrace("m_xColumns[%lld].m_bIsUnique = %s\n"_Stz, i_Column, m_xColumns[i_Column].m_bIsUnique ? "true" : "false");
    vTrace("m_xColumns[%lld].m_bIsPartitionKey = %s\n"_Stz, i_Column, m_xColumns[i_Column].m_bIsPartitionKey ? "true" : "false");
    vTrace("m_xColumns[%lld].m_eDatumType = %s\n"_Stz, i_Column, aDT[m_xColumns[i_Column].m_eDatumType]);
  }
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_CreateTable::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATETABLE);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
