//
// Created on 4/20/21.
//

#include "BASE.h"
#include "Parcel.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Parcel,CN_PARCEL)
__aNewFunction(Parcel,CN_PARCEL)
__vDeleteFunction(Parcel,CN_PARCEL)
__aFooterFunction(Parcel)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Parcel::Parcel(void)
    : Operand(CN_PARCEL) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  m_nObjects = 0;   // number of objects in the parcel

  for (i8 i_Obj = 0; i_Obj < C_MAX_PARCEL_OBJ; i_Obj++)
    m_pObjects[i_Obj] = nullptr;


  _checkInitialised_(CN_PARCEL);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Parcel::Parcel(i8 nObjs)
    : Operand(CN_PARCEL) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  m_nObjects = nObjs;   // number of objects in the parcel

  for (i8 i_Obj = 0; i_Obj < C_MAX_PARCEL_OBJ; i_Obj++)
    m_pObjects[i_Obj] = nullptr;

  _checkInitialised_(CN_PARCEL);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Parcel::~Parcel(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);


}

/*----------------------------------------------------------------------------*/
/* Add object to array                                                        */
/*----------------------------------------------------------------------------*/
void Parcel::vAddObject(Object *pObj)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  if (m_nObjects < C_MAX_PARCEL_OBJ) {
    m_pObjects[m_nObjects++] = pObj;
  }
}

/*----------------------------------------------------------------------------*/
/* Get an object from array                                                   */
/*----------------------------------------------------------------------------*/
Object* Parcel::pGetObject(i8 iIdx)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  if (iIdx < C_MAX_PARCEL_OBJ && iIdx < m_nObjects) {
    return m_pObjects[iIdx];
  }
  return nullptr;
}