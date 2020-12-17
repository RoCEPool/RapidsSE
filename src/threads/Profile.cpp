/*============================================================================*/
/* Profile.cpp - runtime profile of task                                      */
/*============================================================================*/

#include "Threads.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Threads,Profile,CN_PROFILE)
__aNewFunction(Profile,CN_PROFILE)
__vDeleteFunction(Profile,CN_PROFILE)
__aFooterFunction(Profile)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Profile::Profile(Stz* szOpName)
: Operand(CN_PROFILE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PROFILE,_FOOTER_);
  
  m_sOpName = szOpName;
  _checkInitialised_(CN_PROFILE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_PROFILE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Profile::~Profile(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PROFILE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set the OpName                                                             */
/*----------------------------------------------------------------------------*/
void Profile::vOpName(Stz* szName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PROFILE,_FOOTER_);
  
  m_sOpName = szName;
}

/*----------------------------------------------------------------------------*/
/* get cpu usage                                                              */
/*----------------------------------------------------------------------------*/
RU_Cpu* Profile::pRU_Cpu(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PROFILE,_FOOTER_);
  
  return & m_oRU_Cpu;
}

/*----------------------------------------------------------------------------*/
/* get memory usage                                                           */
/*----------------------------------------------------------------------------*/
RU_Memory* Profile::pRU_Memory(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PROFILE,_FOOTER_);
  
  return & m_oRU_Memory;
}

/*----------------------------------------------------------------------------*/
/* write report                                                               */
/*----------------------------------------------------------------------------*/
void Profile::vWriteReport(File* pFile)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PROFILE,_FOOTER_);
  
  AM::vFilePrint(pFile, "OP: %s\n"_Stz, m_sOpName.azChar());
  m_oRU_Memory.vWriteReport(pFile);
  m_oRU_Cpu.vWriteReport(pFile);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
