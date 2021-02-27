/*============================================================================*/
/* Fifo.cpp -- os fifo                                                        */
/*============================================================================*/

#include "BASE.h"

#include "Fifo.hpp" //?

#include <sys/stat.h>

Atomic gixFifo;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Fifo* BASE::_pFifo(void* aAddr)
{
  Fifo* pFifo = reinterpret_cast<Fifo*>(aAddr);
  
  RASSERT_TYPE(pFifo,CN_FIFO);
  
  return pFifo;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Fifo::Fifo(void)
: Resource(CN_FIFO,nullptr)
{
  _precondition_(_MAGIC_,CN_FIFO,_FOOTER_);
  
  m_sFifoPath = C_NO_NAME;
  //m_oSysCall;
  
  _checkInitialised_(CN_FIFO);
  
  ++ gixFifo;
  
  _postcondition_(_MAGIC_,CN_FIFO,_FOOTER_);
  
  vLog(EV_CONSTRUCT,"Fifo constructor"_Stz);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Fifo::Fifo(Stz* sFifoPath)
: Resource(CN_FIFO,nullptr)
{
  _precondition_(_MAGIC_,CN_FIFO,_FOOTER_);
  
  m_sFifoPath = sFifoPath;
  //m_oSysCall;

  ++ gixFifo;
 
  _checkInitialised_(CN_FIFO);
  
  _postcondition_(_MAGIC_,CN_FIFO,_FOOTER_);
  
  vLog(EV_CONSTRUCT,"Fifo constructor"_Stz);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Fifo::~Fifo(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_FIFO,_FOOTER_);
  
  -- gixFifo;

  vLog(EV_DESTRUCT,"Fifo destructor"_Stz);
}

/*----------------------------------------------------------------------------*/
/* make pipe                                                                  */
/*----------------------------------------------------------------------------*/
bool Fifo::bMake(Stz* sFifoPath)
{
  i8 iResult = mkfifo(sFifoPath->azChar(),0777);
  
  if (iResult) {
    vWarn("Fifo::bMake: mkfifo() error; errno=%d"_Stz,errno);
    return false;
  }
  
  AM::vLogFyi("Fifo made : %s"_Stz, //!format-LogFyi
              sFifoPath->azChar());

  return true;
}

/*----------------------------------------------------------------------------*/
/* remove pipe                                                                */
/*----------------------------------------------------------------------------*/
bool Fifo::bRemove(Stz* sFifoPath)
{
  i8 iResult = remove(sFifoPath->azChar());
  
  if (iResult) {
    vWarn("Fifo::bRemove: remove() error; errno=%d\n"_Stz,errno);
    return false;
  }
  
  AM::vLogFyi("Fifo removed : %s"_Stz, //!format-LogFyi
              sFifoPath->azChar());
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* outline                                                                    */
/*----------------------------------------------------------------------------*/
void Fifo::vOutline(s4K& sOutline)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FIFO,_FOOTER_);
  
  sOutline.vSprint("<%s focal>"_Stz,aCN[eGetClassNo()]);
}

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Fifo::aFooter(void)
{
  _precondition_(_MAGIC_,CN_FIFO);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
