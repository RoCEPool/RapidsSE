/*============================================================================*/
/* Job_DropSchema.cpp -- Drop Schema Job                                      */
/*============================================================================*/

#include "JobsModel.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_DropSchema* JobsModel::_pJob_DropSchema(void* aAddress)
{
  Job_DropSchema* pJob_DropSchema = reinterpret_cast<Job_DropSchema*>(aAddress);
  
  RASSERT( !pJob_DropSchema || CN_JOB_DROPSCHEMA == pJob_DropSchema->eClass());
  
  return pJob_DropSchema;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_DropSchema::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_DropSchema* pJob_DropSchema =
    static_cast<Job_DropSchema*>(Heap::pAllocate(CN_JOB_CREATESCHEMA));
  
  return pJob_DropSchema;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_DropSchema::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_DropSchema::operator delete(void* aAddress)
{
  Job_DropSchema* pJob_DropSchema = static_cast<Job_DropSchema*>(aAddress);
  
  Heap::vFree(pJob_DropSchema);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_DropSchema::Job_DropSchema(void)
: Job(CN_JOB_DROPSCHEMA)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPSCHEMA,_FOOTER_);
  
  m_sSchemaName = ""_Stz;
  m_iDicDepth = -1;
  
  for (i8 i_Depth = 0; i_Depth < C_MAX_FOLDERS; i_Depth++)
    m_sDicPath[i_Depth] = ""_Stz;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPSCHEMA,_FOOTER_);
  
  _checkInitialised_(CN_JOB_DROPSCHEMA);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_DropSchema::~Job_DropSchema(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPSCHEMA,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_DropSchema::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_CREATESCHEMA == eClass);
  RASSERT(pObject->eClass() == CN_JOB_DROPSCHEMA);
  
  Job_CreateTable* pJob_CreateTable = _pJob_CreateTable(pObject);

  pJob_CreateTable->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_DropSchema::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPSCHEMA,_FOOTER_);
  
                                                               //! Alka to write
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_DropSchema::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPSCHEMA);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
