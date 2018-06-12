/*============================================================================*/
/* Job_CreateTable.cpp -- Create Table Job                                    */
/*============================================================================*/

#include "JobsModel.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_CreateTable* JobsModel::_pJob_CreateTable(void* aAddress)
{
  Job_CreateTable* pJob_CreateTable = reinterpret_cast<Job_CreateTable*>(aAddress);
  
  RASSERT( !pJob_CreateTable || CN_JOB == pJob_CreateTable->eClass());
  
  return pJob_CreateTable;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_CreateTable::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_CreateTable* pJob_CreateTable =
    static_cast<Job_CreateTable*>(Heap::pAllocate(CN_JOB));
  
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
Job_CreateTable::Job_CreateTable(void)
: Job(CN_JOB)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB,_FOOTER_);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB,_FOOTER_);
  
  _checkInitialised_(CN_JOB);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_CreateTable::~Job_CreateTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump entry buffer                                                          */
/*----------------------------------------------------------------------------*/
void Job_CreateTable::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB == eClass);
  RASSERT(pObject->eClass() == CN_JOB);
  
  Job_CreateTable* pJob_CreateTable = _pJob_CreateTable(pObject);

  pJob_CreateTable->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show contents of buffer                                                    */
/*----------------------------------------------------------------------------*/
void Job_CreateTable::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
                                                               //! Alka to write
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_CreateTable::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
