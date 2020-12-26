/*============================================================================*/
/* Fifo.cpp -- os fifo                                                        */
/*============================================================================*/

#include "Resources.h"

#include "Fifo.hpp" //?

Atomic gixFifo;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Fifo* Resources::_pFifo(void* aAddr)
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
  AM::vLogRes(SL_INF,this,RA_FIO_CONSTRUCT,
              "Fifo construct : %p (%lld fifos exist)"_Stz, //!format-LogRes
              this,* gixFifo);
  
  _postcondition_(_MAGIC_,CN_FIFO,_FOOTER_);
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

  _checkInitialised_(CN_FIFO);
  
  AM::vLogRes(SL_INF,this,RA_FIO_CONSTRUCT,
              "Fifo construct : %p (%lld fifos exist)"_Stz, //!format-LogRes
              this,* gixFifo);
  
  _postcondition_(_MAGIC_,CN_FIFO,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Fifo::~Fifo(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_FIFO,_FOOTER_);
  
  -- gixFifo;
  AM::vLogRes(SL_INF,this,RA_MTX_DESTRUCT,
              "Fifo destruct : %p (%lld fifos exist)"_Stz, //!format-LogRes
              this,* gixFifo);
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
/* focal outline                                                              */
/*----------------------------------------------------------------------------*/
void Fifo::vFocalOutline(s4K& sOutline)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FIFO,_FOOTER_);
  
  sOutline.vSprint("<%s focal>"_Stz,aCN[eClass()]);
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
