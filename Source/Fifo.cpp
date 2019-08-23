/*============================================================================*/
/* Fifo.cpp -- os fifo                                                        */
/*============================================================================*/

#include "Resources.h"

Atomic gixFifo;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Fifo::Fifo(void)
: Resource(CN_FIFO)
{
  _precondition_(_MAGIC_,CN_FIFO,_FOOTER_);
  
  m_sFifoPath = "<none>"_Stz;
  //m_oSysCall;
  
  _checkInitialised_(CN_FIFO);
  
  ++ gixFifo;
  AM::vLogRes(this,RA_FIO_CONSTRUCTOR,
              "Fifo constructed : %p (%lld fifos exist)"_Stz,this,* gixFifo);
  
  _postcondition_(_MAGIC_,CN_FIFO,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Fifo::Fifo(Stz* sFifoPath)
: Resource(CN_FIFO)
{
  _precondition_(_MAGIC_,CN_FIFO,_FOOTER_);
  
  m_sFifoPath = sFifoPath;
  //m_oSysCall;

  _checkInitialised_(CN_FIFO);
  
  AM::vLogRes(this,RA_FIO_CONSTRUCTOR,
              "Fifo constructed : %p (%lld fifos exist)"_Stz,this,* gixFifo);
  
  _postcondition_(_MAGIC_,CN_FIFO,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Fifo::~Fifo(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_FIFO,_FOOTER_);
  
  -- gixFifo;
  AM::vLogRes(this,RA_MTX_DESTRUCTOR,
              "Fifo destructed : %p (%lld fifos exist)"_Stz,this,* gixFifo);
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
  
  AM::vLogFyi("Fifo made : %s"_Stz,sFifoPath->aChar());

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
  
  AM::vLogFyi("Fifo removed : %s"_Stz,sFifoPath->aChar());
  
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
  
  sOutline.vSprint("<Fifo %p>"_Stz,
                   
                   this
                   );
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
