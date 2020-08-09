/*============================================================================*/
/* LogicTimer.cpp -- Logic Timer                                              */
/*============================================================================*/

#include "Expressions.h"



/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Expressions,LogicTimer,CN_LOGICTIMER)
__aNewFunction(LogicTimer,CN_LOGICTIMER)
__vDeleteFunction(LogicTimer,CN_LOGICTIMER)
__aFooterFunction(LogicTimer)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
LogicTimer::LogicTimer(void)
: Object(CN_LOGICTIMER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICTIMER,_FOOTER_);
  
  _checkInitialised_(CN_LOGICTIMER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_LOGICTIMER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
LogicTimer::~LogicTimer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICTIMER,_FOOTER_);
  
  vReport();
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* begin timing                                                               */
/*----------------------------------------------------------------------------*/
void LogicTimer::vBeginTmr(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICTIMER,_FOOTER_);
  
  m_oTmr.vStart();
}

/*----------------------------------------------------------------------------*/
/* end timing                                                                 */
/*----------------------------------------------------------------------------*/
void LogicTimer::vEndTmr(eEO eCode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICTIMER,_FOOTER_);
  
  m_oTmr.vUpdate();
  
  m_oAggTmr[eCode].vAdd(&m_oTmr);
}

/*----------------------------------------------------------------------------*/
/* report                                                                     */
/*----------------------------------------------------------------------------*/
void LogicTimer::vReport(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICTIMER,_FOOTER_);

  for (i8 i_Code = 1; i_Code < EO_ENUMS; i_Code++) {
    vTrace(" %lld,%s,%lld,%lld,%lld\n"_Stz,
           
           i_Code,
           aEO[i_Code],
           
           m_oAggTmr[i_Code].nTimes(),
           m_oAggTmr[i_Code].iNanoseconds(),
           
           m_oAggTmr[i_Code].nTimes()
             ? m_oAggTmr[i_Code].iNanoseconds() / m_oAggTmr[i_Code].nTimes()
             : 0
           );
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
