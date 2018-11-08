/*============================================================================*/
/* Wilderness.cpp -- wilderness marker                                        */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Wilderness* Structures::_pWilderness(void* aAddr)
{
  Wilderness* pWilderness = reinterpret_cast<Wilderness*>(aAddr);
  
  RASSERT_TYPE(pWilderness,CN_WILDERNESS);
  
  return pWilderness;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Wilderness::operator new(size_t)
{
  //UNUSED(iSize)
  vFatal("fatal: Wilderness class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Wilderness::operator new(size_t, Wilderness* pWilderness)
{
  //UNUSED(iSize)
  return pWilderness;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Wilderness::Wilderness(void)
: Item(CN_WILDERNESS,sizeof(Wilderness))
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WILDERNESS);

}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Wilderness::~Wilderness(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WILDERNESS);
}

/*----------------------------------------------------------------------------*/
/* assert valid wilderness                                                    */
/*----------------------------------------------------------------------------*/
void Wilderness::vAssertWilderness(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WILDERNESS);
}

/*----------------------------------------------------------------------------*/
/* test for valid wilderness                                                  */
/*----------------------------------------------------------------------------*/
bool Wilderness::bValidWilderness(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WILDERNESS);
  
  if (! bValidMagic())
    return false;
  
  if (CN_WILDERNESS != eClass())
    return false;
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
