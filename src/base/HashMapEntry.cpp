/*============================================================================*/
/* HashMapEntry.cpp -- HashMapEntry                                           */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
HashMapEntry* BASE::_pHashMapEntry(void* aAddr)
{
  HashMapEntry* pHashMapEntry = reinterpret_cast<HashMapEntry*>(aAddr);

  RASSERT_TYPE(pHashMapEntry,CN_HASHMAPENTRY);

  return pHashMapEntry;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* HashMapEntry::operator new(size_t)
{
  vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
         "fatal: Row class cannot be new'd\n"_Stz);

  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* HashMapEntry::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
HashMapEntry::HashMapEntry(void* pKey, Object* pValue, eBI eBufferIdType)
: Object(CN_HASHMAPENTRY)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HASHMAPENTRY,_FOOTER_);

  m_ixAtomicLock.vCheckInit();
  vGrabLock();
  Task* pTask = AM::pThisTLSTask();
  if(pTask)
    pTask->vAddHashMapEntry(this);
  m_eBufferIdType = eBufferIdType;
  m_pKeyObject = pKey;
  m_pValObject = pValue;

  _checkInitialised_(CN_HASHMAPENTRY);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_HASHMAPENTRY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
HashMapEntry::~HashMapEntry(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HASHMAPENTRY,_FOOTER_);
  vDropLock();
  Task* pTask = AM::pThisTLSTask();
  if(pTask) {
    bool bResult = pTask->bDeleteHashMapEntry(this);
    UNUSED(bResult);
    ASSERT(bResult);
  }

  ASSERT((*m_ixAtomicLock) == 0);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Decrement atomic lock                                                      */
/*----------------------------------------------------------------------------*/
void HashMapEntry::vDropLock(void)
{
  ASSERT((*m_ixAtomicLock) > 0);
  m_ixAtomicLock.iDec();
  Task* pTask = AM::pThisTLSTask();
  if(pTask) {
    bool bResult = pTask->bDeleteHashMapEntry(this);
    UNUSED(bResult);
//    ASSERT(bResult);
  }
}

/*----------------------------------------------------------------------------*/
/* Increment atomic lock                                                      */
/*----------------------------------------------------------------------------*/
void HashMapEntry::vGrabLock(void)
{
  ASSERT((*m_ixAtomicLock) >= 0);
  m_ixAtomicLock.iInc();
  Task* pTask = AM::pThisTLSTask();
  if(pTask)
    pTask->vAddHashMapEntry(this);
}

/*----------------------------------------------------------------------------*/
/* Decriment atomic lock                                                      */
/*----------------------------------------------------------------------------*/
bool HashMapEntry::bLocked(void)
{
  ASSERT((*m_ixAtomicLock) >= 0);
  return (*m_ixAtomicLock) > 0;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* HashMapEntry::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HASHMAPENTRY);

  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/