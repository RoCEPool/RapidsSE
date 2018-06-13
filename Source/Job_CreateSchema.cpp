/*============================================================================*/
/* Job_CreateSchema.cpp -- Create Scema Job                                   */
/*============================================================================*/

#include "JobsModel.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_CreateSchema* JobsModel::_pJob_CreateSchema(void* aAddress)
{
  Job_CreateSchema* pJob_CreateSchema = reinterpret_cast<Job_CreateSchema*>(aAddress);
  
  RASSERT( !pJob_CreateSchema || CN_JOB_CREATESCHEMA == pJob_CreateSchema->eClass());
  
  return pJob_CreateSchema;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_CreateSchema::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_CreateTable* pJob_CreateSchema =
    static_cast<Job_CreateTable*>(Heap::pAllocate(CN_JOB_CREATESCHEMA));
  
  return pJob_CreateSchema;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_CreateSchema::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_CreateSchema::operator delete(void* aAddress)
{
  Job_CreateTable* pJob_CreateTable = static_cast<Job_CreateTable*>(aAddress);
  
  Heap::vFree(pJob_CreateTable);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_CreateSchema::Job_CreateSchema(void)
: Job(CN_JOB_CREATESCHEMA)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATESCHEMA,_FOOTER_);
  
  m_sSchemaName = ""_Stz;
  m_iDicDepth = -1;
  
  for (i8 i_Depth = 0; i_Depth < C_MAX_FOLDERS; i_Depth++)
    m_sDicPath[i_Depth] = ""_Stz;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATESCHEMA,_FOOTER_);
  
  _checkInitialised_(CN_JOB_CREATESCHEMA);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_CreateSchema::~Job_CreateSchema(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATESCHEMA,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_CreateSchema::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_CREATESCHEMA == eClass);
  RASSERT(pObject->eClass() == CN_JOB_CREATESCHEMA);
  
  Job_CreateTable* pJob_CreateTable = _pJob_CreateTable(pObject);

  pJob_CreateTable->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_CreateSchema::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATESCHEMA,_FOOTER_);
  
                                                               //! Alka to write
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_CreateSchema::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATESCHEMA);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
