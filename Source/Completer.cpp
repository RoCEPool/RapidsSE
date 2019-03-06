/*============================================================================*/
/* Completer.cpp -- multi-partition completer                                 */
/*============================================================================*/

#include "Tasks.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Completer::operator new(size_t, Completer* pCompleter)
{
  return pCompleter;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void Completer::operator delete(void* )
{
  // do nothing
  // must be explicitly free'd by caller
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Completer::Completer(void)
: Object(CN_COMPLETER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPLETER,_FOOTER_);
  
  m_okLock.vInitLock(LK_COMPLETER);
  
  m_nPartitionsComplete = 0;
  m_nUnitsExpected  = C_UNKNOWN;
  m_nUnitsProcessed = 0;
  
  _checkInitialised_(CN_COMPLETER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_COMPLETER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Completer::~Completer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPLETER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* inc units processed and determine if completed                             */
/*----------------------------------------------------------------------------*/
bool Completer::bIncProcessed(i8 nUnitsExpected)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPLETER,_FOOTER_);
  
  bool bHasCompleted = false;
  
  m_okLock.vGrabWrite();
  
  m_nUnitsProcessed++;
  
  if (C_UNKNOWN != nUnitsExpected) {
    if (C_UNKNOWN == m_nUnitsExpected)
      m_nUnitsExpected = nUnitsExpected;
    else
      m_nUnitsExpected += nUnitsExpected;
      
    m_nPartitionsComplete++;
  }

  if (C_UNKNOWN != m_nUnitsExpected) {
    if (m_nPartitionsComplete == gpAttachMap->m_pGrid->m_nPartitions &&
        m_nUnitsProcessed == m_nUnitsExpected)
      bHasCompleted = true;
  }

  m_okLock.vDropWrite();

  return bHasCompleted;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Completer::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPLETER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
