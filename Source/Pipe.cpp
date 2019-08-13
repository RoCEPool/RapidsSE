/*============================================================================*/
/* Pipe.cpp -- os pipe                                                        */
/*============================================================================*/

#include "Resources.h"


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Pipe::Pipe(void)
: Resource(CN_PIPE)
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
: Resource(CN_PIPE)
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
/* make pipe                                                                  */
/*----------------------------------------------------------------------------*/
bool Pipe::bMake(Stz* sPipePath)
{
  i8 iResult = mkfifo(sPipePath->aChar(),0777);
  
  if (iResult) {
    vWarn("Pipe::bMake: mkfifo() error; errno=%d"_Stz,errno);
    return false;
  }
  
  AM::vLogRes(0,RA_PIP_MADE,RK_PIPE,
              "Pipe made : %s"_Stz,sPipePath->aChar());

  return true;
}

/*----------------------------------------------------------------------------*/
/* remove pipe                                                                */
/*----------------------------------------------------------------------------*/
bool Pipe::bRemove(Stz* sPipePath)
{
  i8 iResult = remove(sPipePath->aChar());
  
  if (iResult) {
    vWarn("Pipe::bRemove: remove() error; errno=%d\n"_Stz,errno);
    return false;
  }
  
  AM::vLogRes(0,RA_PIP_REMOVED,RK_PIPE,
              "Pipe removed : %s"_Stz,sPipePath->aChar());
  
  return true;
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
