/*============================================================================*/
/* Pipe.cpp -- os pipe                                                        */
/*============================================================================*/

#include "System.h"

#include "StorageEngine.h"

extern ResourceTracker gxResourceTracker;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Pipe::Pipe(void)
: Object(CN_PIPE)
{
  _precondition_(_MAGIC_,CN_PIPE,_FOOTER_);
  
  m_sPipePath = "<none>"_Stz;
  //m_oSysCall;
  
  _checkInitialised_(CN_PIPE);
  
  _postcondition_(_MAGIC_,CN_PIPE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Pipe::Pipe(Stz* sPipePath)
: Object(CN_PIPE)
{
  _precondition_(_MAGIC_,CN_PIPE,_FOOTER_);
  
  m_sPipePath = sPipePath;
  //m_oSysCall;

  _checkInitialised_(CN_PIPE);
  
  _postcondition_(_MAGIC_,CN_PIPE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Pipe::~Pipe(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_PIPE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* create pipe                                                                */
/*----------------------------------------------------------------------------*/
bool Pipe::bCreate(void)
{
  _precondition_(_MAGIC_,CN_PIPE,_FOOTER_);
  
  m_oSysCall.vClear();
  
  i8 iResult = mkfifo(m_sPipePath.aChar(),0777);
  
  if (iResult) {
    m_oSysCall.vError(errno);
    vWarn("Pipe::bCreate: mkfifo() error; errno=%d"_Stz,errno);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* create pipe                                                                */
/*----------------------------------------------------------------------------*/
bool Pipe::bCreate(Stz* sPipePath)
{
  _precondition_(_MAGIC_,CN_PIPE,_FOOTER_);
  
  m_sPipePath = sPipePath;
  
  return bCreate();
}

/*----------------------------------------------------------------------------*/
/* remove pipe                                                                */
/*----------------------------------------------------------------------------*/
bool Pipe::bRemove(void)
{
  _precondition_(_MAGIC_,CN_PIPE,_FOOTER_);
  
  m_oSysCall.vClear();
  
  i8 iResult = remove(m_sPipePath.aChar());
  
  if (iResult) {
    m_oSysCall.vError(errno);
    vWarn("Pipe::bRemove: remove() error; errno=%d\n"_Stz,errno);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* remove pipe                                                                */
/*----------------------------------------------------------------------------*/
bool Pipe::bRemove(Stz* sPipePath)
{
  _precondition_(_MAGIC_,CN_PIPE,_FOOTER_);
  
  m_sPipePath = sPipePath;
  
  return bRemove();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Pipe::aFooter(void)
{
  _precondition_(_MAGIC_,CN_PIPE);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
