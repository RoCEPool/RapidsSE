/*============================================================================*/
/* SP_Ent.cpp - Ent sprint helper                                             */
/*============================================================================*/

#include "Entries.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_Ent::SP_Ent(void)

{
  _precondition_(_MAGIC_,CN_SPSMALL,_FOOTER_);
  
  // set default enclosing char
  vFormat(F_VCH_enclose);

#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caEnt,sizeof(m_caEnt),CN_SPSMALL);
#else
  m_caEnt[0] = '\0';
#endif
  
  //! FIX RAY
  //_checkInitialised_(CN_SPSMALL);
  
  _postcondition_(_MAGIC_,CN_SPSMALL,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_Ent::SP_Ent(Ent* aEnt, IndexDef* pIndexDef)
{
  _precondition_(_MAGIC_,CN_SPSMALL,_FOOTER_);
  
  // set default enclosing char
  vFormat(F_VCH_enclose);
  
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caEnt,sizeof(m_caEnt),CN_SPSMALL);
#else
  m_caEnt[0] = '\0';
#endif
  
  //! FIX RAY
  //_checkInitialised_(CN_SPSMALL);
  
  vSprintEnt(aEnt,pIndexDef);
  
  _postcondition_(_MAGIC_,CN_SPSMALL,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SP_Ent::~SP_Ent(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SPSMALL,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* reset                                                                      */
/*----------------------------------------------------------------------------*/
void SP_Ent::vReset(void)
{
  _precondition_(_MAGIC_,CN_SPSMALL);
  
  m_caEnt[0] = '\0';
}

/*----------------------------------------------------------------------------*/
/* return Stz* of result                                                      */
/*----------------------------------------------------------------------------*/
Stz* SP_Ent::sz(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return (Stz*) &m_caEnt[0];
}

/*----------------------------------------------------------------------------*/
/* sprint Vch and return Stz* to result                                       */
/*----------------------------------------------------------------------------*/
Stz* SP_Ent::sz(Ent* aEnt, IndexDef* pIndexDef)
{
  _precondition_(_MAGIC_,_FOOTER_);

  vSprintEnt(aEnt,pIndexDef);
  
  return (Stz*) &m_caEnt[0];
}

/*----------------------------------------------------------------------------*/
/* sprint the entry                                                           */
/*----------------------------------------------------------------------------*/
void SP_Ent::vSprintEnt(Ent* aEnt, IndexDef* pIndexDef)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  SP_Key spKey;
  SP_Ref spRef;

  Stz* szKey = spKey.sz(aEnt->aKey(),pIndexDef);

  Stz* szRef = spRef.sz(aEnt->rxRef());

  StrLib::vSprint(m_caEnt,
                  "%s %s; NEO:"_Stz,
                  szRef,
                  szKey);
  
  for (i8 i_Level = 0; i_Level < aEnt->nSkipLevels(); i_Level++)
    StrLib::vSprintCat((Stz*)m_caEnt," %d"_Stz,aEnt->tNextEntry(i_Level));

}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(SP_Ent)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
