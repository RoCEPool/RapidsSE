/*============================================================================*/
/* NmlStr.hpp -- Normalised String Set                                        */
/*============================================================================*/

#include "Rows.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Rows,NmlStr,CN_NMLSTR)
__aNewFunction(NmlStr,CN_NMLSTR)
__vDeleteFunction(NmlStr,CN_NMLSTR)
__aFooterFunction(NmlStr)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
NmlStr::NmlStr(void)
: Object(CN_NMLSTR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NMLSTR,_FOOTER_);
  
  MemLib::vZero(m__PadToCacheLine,sizeof(m__PadToCacheLine),CN_NMLSTR);

  #ifdef WITH_DEBUG_VERSION
    m_xWsp.vZero(CN_NMLSTR);
  #endif

  _checkInitialised_(CN_NMLSTR);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_NMLSTR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
NmlStr::~NmlStr(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NMLSTR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add given arg string                                                       */
/*----------------------------------------------------------------------------*/
Stz* NmlStr::szAdd(Stz* szArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NMLSTR,_FOOTER_);
  
  //vTrace("Adding: %s\n"_Stz,szArg);

  i8 nSize = StrLib::nSize(szArg->azChar());
  
  Stz* szNew = (Stz*)m_xWsp.ayAllot(nSize);
  
  StrLib::vCopy(szNew->azChar(),szArg->azChar());
  
  //vDump();
  
  return szNew;
}

/*----------------------------------------------------------------------------*/
/* get each normal string in turn                                             */
/*----------------------------------------------------------------------------*/
Stz* NmlStr::szEach(Stz* szPrior)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NMLSTR,_FOOTER_);
  
  if (! szPrior) {
    
    if (m_xWsp.ayArea() == m_xWsp.ayLatter())
      return nullptr;
    
    Stz* szFirst = (Stz*)m_xWsp.ayArea();
    
    return szFirst;
  }
  
  Stz* szNext = aAdd<Stz*>(szPrior,szPrior->nSize());

  if (szNext->azChar() < (char*)m_xWsp.ayLatter())
    return szNext;
      
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find given arg string                                                      */
/*----------------------------------------------------------------------------*/
Stz* NmlStr::szFind(Stz* szArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NMLSTR,_FOOTER_);
  
  Stz* szStr = (Stz*)m_xWsp.ayArea();
  
  while (szStr->azChar() < (char*)m_xWsp.ayLatter()) {
    
    if (szStr->bEqual(szArg))
      return szStr;
    
    szStr = aAdd<Stz*>(szStr,szStr->nSize());
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* dump all the strings                                                       */
/*----------------------------------------------------------------------------*/
void NmlStr::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_NMLSTR,_FOOTER_);
  
  Stz* szStr = (Stz*)m_xWsp.ayArea();
  
  while (szStr->azChar() < (char*)m_xWsp.ayLatter()) {
    
    vTrace("NmlStr: %s\n"_Stz,szStr);
    
    szStr = aAdd<Stz*>(szStr,szStr->nSize());
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
