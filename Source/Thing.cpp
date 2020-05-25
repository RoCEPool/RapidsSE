/*============================================================================*/
/* Thing.cpp -- Arrangeable Thing                                             */
/*============================================================================*/

#include "Utilities.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Utilities,Thing,CN_THING)
__aNewFunction(Thing,CN_THING)
__vDeleteFunction(Thing,CN_THING)
__aFooterFunction(Thing)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Thing::Thing(Stz* szThingNm, Object* pObject)
: Object(CN_THING)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THING,_FOOTER_);
    
  m_sThingNm = szThingNm;
  m_pObject  = pObject;
  m_iSeqNo   = C_UNKNOWN;
  
  _checkInitialised_(CN_THING);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_THING,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Thing::~Thing(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THING,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get the name of the thing                                                  */
/*----------------------------------------------------------------------------*/
Stz* Thing::szThingNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THING,_FOOTER_);
  
  return m_sThingNm;
}

/*----------------------------------------------------------------------------*/
/* get the object of the thing                                                */
/*----------------------------------------------------------------------------*/
Object* Thing::pObject(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THING,_FOOTER_);
  
  return m_pObject;
}

/*----------------------------------------------------------------------------*/
/* get the seq no of the thing                                                */
/*----------------------------------------------------------------------------*/
i8 Thing::iSeqNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THING,_FOOTER_);
  
  return m_iSeqNo;
}

/*----------------------------------------------------------------------------*/
/* set the seq no of the thing                                                */
/*----------------------------------------------------------------------------*/
void Thing::vSeqNo(i8 iSeqNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THING,_FOOTER_);
  
  m_iSeqNo = iSeqNo;
}


/*----------------------------------------------------------------------------*/
/*                         -- Compare Function --                             */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compare two things by their name                                           */
/*----------------------------------------------------------------------------*/
bool bThingCompare(void* vpInsert, void* vpScan, bool& bMatch)
{
  Thing* pInsertThing = _pThing(vpInsert);
  Thing* pScanThing   = _pThing(vpScan);
    
  if (! pScanThing)
    return false;
  
  // names match?
  if (pInsertThing->szThingNm()->bSame(pScanThing->szThingNm()))
    bMatch = true;
  else
    bMatch = false;
  
  // insert after scan?
  if (pInsertThing->szThingNm()->bGreater(pScanThing->szThingNm()))
    return true;
  else
    return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
