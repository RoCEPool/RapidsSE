/*============================================================================*/
/* ActionSequence.cpp - Activity Map                                           */
/*============================================================================*/

#include "Maps.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

ActionSequence* gpActionSequence = nullptr;


/*----------------------------------------------------------------------------*/
/* -- LifeCycle --------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ActionSequence::operator new(size_t, ActionSequence* pActionSequence)
{
  RASSERT(! gpActionSequence);
  
  return pActionSequence;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void ActionSequence::operator delete(void* aAddress)
{
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ActionSequence::ActionSequence(void)
: Map(CN_ACTIONSEQUENCE)
{
  _precondition_(_MAGIC_,CN_ACTIONSEQUENCE,_FOOTER_);
  
  m_okLock.vInitLock(LK_ACTION_JOURNAL);
  
  m_i_Action = 0;
  m_nActions = C_ACTION_SEQUENCE;
  
  _checkInitialised_(CN_ACTIONSEQUENCE);
  
  _postcondition_(_MAGIC_,CN_ACTIONSEQUENCE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ActionSequence::~ActionSequence(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_ACTIONSEQUENCE,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* create or open ActivityMap (called by SE)                                  */
/*----------------------------------------------------------------------------*/
void ActionSequence::vOpenBySE(bool bStarting)
{
#ifdef WITH_ACTION_SEQUENCE
  if (! gpAttachMap->Arg.m_bActionSequence)
    return;
  
  if (gpActionSequence)
    vFatal("ActivityMap::vOpenBySE() -- activity map already opened\n"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  s32 sMapNm("/SE_ActionSequence"_Stz);
  i8 nMapSz = sizeof(ActionSequence);
  
  ActionSequence* pActionSequence = nullptr;
  
  if (bStarting) {
    pActionSequence = (ActionSequence*)Map::aMapCreate(sMapNm,nMapSz);
    if (gpAttachMap->m_bVerbose)
      vTrace("ActionSequence: created new map %s %lldB\n"_Stz,
             sMapNm.aStz(),
             nMapSz);
    gpActionSequence = new(pActionSequence) ActionSequence;
    
  }
  else {
    pActionSequence = (ActionSequence*)Map::aMapExisting(sMapNm,nMapSz);
    if (gpAttachMap->m_bVerbose)
      vTrace("ActionSequence: attached to existing map %s %lldB\n"_Stz,
             sMapNm.aStz(),
             nMapSz);

    gpActionSequence = pActionSequence;
  }
  
#endif
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* ActionSequence::aFooter(void)
{
  _precondition_(_MAGIC_,CN_ACTIONSEQUENCE);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
