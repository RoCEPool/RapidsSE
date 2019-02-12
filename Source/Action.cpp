/*============================================================================*/
/* Action.cpp - when did an action occur and what happened                    */
/*============================================================================*/

#include "Maps.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Action* Maps::_pAction(void* aAddr)
{
  Action* pAction = reinterpret_cast<Action*>(aAddr);
  
  RASSERT_TYPE(pAction,CN_ACTION);
  
  return pAction;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Action::Action(void)
: Object(CN_ACTION)
{
  _precondition_(_MAGIC_,CN_ACTION,_FOOTER_);
  
  m_i_Action = 0;
  m_i_Root   = 0;
  m_iTick    = 0;
  
  m_aJob     = nullptr;
  m_iJobId   = 0;
  m_eClassNo = CN_NONE;
  m_bWasRecv = false;
  
  m_eAction  = AN_NONE;
  m_iArg0    = 0;
  m_iArg1    = 0;
  
  ASSERT(0 == *m_wLink);

 _checkInitialised_(CN_ACTION);
  
  _postcondition_(_MAGIC_,CN_ACTION,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Action::~Action(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_ACTION,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* init                                                                       */
/*----------------------------------------------------------------------------*/
void Action::vInit(i8 i_Action)
{
  _precondition_(_MAGIC_,CN_ACTION);
  
  m_i_Action = i_Action;
  m_i_Root   = C_UNKNOWN;
  m_iTick    = 0;
  
  m_aJob     = nullptr;
  m_iJobId   = C_UNKNOWN;
  m_bWasRecv = false;
  
  m_eAction  = AN_NONE;
  m_iArg0    = 0;
  m_iArg1    = 0;
  
  m_wLink.vReset();
}
/*----------------------------------------------------------------------------*/
/* is this action a root action?                                              */
/*----------------------------------------------------------------------------*/
bool Action::bIsRoot(void)
{
  _precondition_(_MAGIC_,CN_ACTION);
  
  if (m_i_Action == m_i_Root && m_iTick)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* reset                                                                      */
/*----------------------------------------------------------------------------*/
void Action::vReset(void)
{
  _precondition_(_MAGIC_,CN_ACTION);
  
  m_i_Action = 0;
  m_i_Root   = 0;
  m_iTick    = 0;
  
  m_aJob     = nullptr;
  m_iJobId   = 0;
  m_bWasRecv = false;
  
  m_eAction  = AN_NONE;
  m_iArg0    = 0;
  m_iArg1    = 0;
  
  m_wLink.vReset();
}

/*----------------------------------------------------------------------------*/
/* reset                                                                      */
/*----------------------------------------------------------------------------*/
void Action::vWrite(File* pFile)
{
  _precondition_(_MAGIC_,CN_ACTION);
  
  pFile->vPrint(/* 0 */ "%03lld, %03lld, %lld, "
                /* 1 */ "%03lld, %lld, %s, %03lld, %lls, "
                /* 2 */ "%2lld, %s, "
                /* 3 */ "%lld, %lld, "
                /* 4 */ "%p, %lld\n"_Stz,
                
                /* 0 */ m_i_Action,
                        m_i_Root,
                        m_iTick,
               
                /* 1 */ m_iJobId,
                        (i8)m_eClassNo,
                        aCN[m_eClassNo],
                        m_iParentId,
                        m_bWasRecv ? "RCVJOB" : "ntvjob",
                
                /* 2 */ (i8)m_eAction,
                        aAN[m_eAction],

                /* 3 */ m_iArg0,
                        m_iArg1,
               
                /* 4 */ m_aJob,
                        * m_wLink
               );
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Action::aFooter(void)
{
  _precondition_(_MAGIC_,CN_ACTION);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
