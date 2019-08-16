/*============================================================================*/
/* Fifo.cpp -- os fifo                                                        */
/*============================================================================*/

#include "Resources.h"


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Fifo::Fifo(void)
: Resource(CN_PIPE,RK_PIPE)
{
  _precondition_(_MAGIC_,CN_PIPE,_FOOTER_);
  
  m_sFifoPath = "<none>"_Stz;
  //m_oSysCall;
  
  _checkInitialised_(CN_PIPE);
  
  _postcondition_(_MAGIC_,CN_PIPE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Fifo::Fifo(Stz* sFifoPath)
: Resource(CN_PIPE,RK_PIPE)
{
  _precondition_(_MAGIC_,CN_PIPE,_FOOTER_);
  
  m_sFifoPath = sFifoPath;
  //m_oSysCall;

  _checkInitialised_(CN_PIPE);
  
  _postcondition_(_MAGIC_,CN_PIPE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Fifo::~Fifo(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_PIPE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* make pipe                                                                  */
/*----------------------------------------------------------------------------*/
bool Fifo::bMake(Stz* sFifoPath)
{
  i8 iResult = mkfifo(sFifoPath->aChar(),0777);
  
  if (iResult) {
    vWarn("Fifo::bMake: mkfifo() error; errno=%d"_Stz,errno);
    return false;
  }
  
  AM::vLogRes(nullptr,RA_PIP_MADE,
              "Fifo made : %s"_Stz,sFifoPath->aChar());

  return true;
}

/*----------------------------------------------------------------------------*/
/* remove pipe                                                                */
/*----------------------------------------------------------------------------*/
bool Fifo::bRemove(Stz* sFifoPath)
{
  i8 iResult = remove(sFifoPath->aChar());
  
  if (iResult) {
    vWarn("Fifo::bRemove: remove() error; errno=%d\n"_Stz,errno);
    return false;
  }
  
  AM::vLogRes(nullptr,RA_PIP_REMOVED,
              "Fifo removed : %s"_Stz,sFifoPath->aChar());
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Fifo::aFooter(void)
{
  _precondition_(_MAGIC_,CN_PIPE);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
