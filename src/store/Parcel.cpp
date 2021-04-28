/*============================================================================*/
/* Parcel.cpp -- Send Parcel                                                  */
/*============================================================================*/

#include "BASE.h"

#include "ePX.h"

#include "Op.hpp"
#include "OpSpec.hpp"
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
: Object(CN_PARCEL)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);
  
  m_iPipelineJobId = C_UNKNOWN;
  m_i_Node         = C_UNKNOWN;
  m_tPtnNo         = C_UNKNOWN;
  
  m_eCrossTopo     = PX_NONE;
  m_iNextStage     = C_UNKNOWN;
  m_iNextOp        = C_UNKNOWN;
  
  m_nObjects = 0;
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
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* new an operand job                                                         */
/*----------------------------------------------------------------------------*/
Parcel* Parcel::pNewParcel(Operand* pOperand,
                                         i8 iPipelineJobId,
                                         ePX eCrossTopo, i8 iNextStage, i8 iNextOp,
                                         i8 i_Node, tPN tPtnNo)
{
  Parcel* pParcel = new Parcel;
  
  pParcel->m_iPipelineJobId = iPipelineJobId;
  pParcel->m_i_Node         = i_Node;
  pParcel->m_tPtnNo         = tPtnNo;
  pParcel->vSetOperand(pOperand);
  
  pParcel->m_eCrossTopo     = eCrossTopo;
  pParcel->m_iNextStage     = iNextStage;
  pParcel->m_iNextOp        = iNextOp;
  
  return pParcel;
}

/*----------------------------------------------------------------------------*/
/* send parcel                                                                */
/*----------------------------------------------------------------------------*/
void Parcel::vSendParcel(Operand* pOperand,
                         Op*      pOp,
                         Task*    pTriggeringTask,
                         bool     bSet)
{
  pOp->vSendParcel(pOperand,pTriggeringTask,bSet);
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
Object* Parcel::pGetObject(i8 i_Object)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  if (i_Object < C_MAX_PARCEL_OBJ && i_Object < m_nObjects)
    return m_pObjects[i_Object];
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* Get Operand                                                                */
/*----------------------------------------------------------------------------*/
Operand* Parcel::pGetOperand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  return (Operand*) m_pObjects[0];
}

/*----------------------------------------------------------------------------*/
/* Set Operand                                                                */
/*----------------------------------------------------------------------------*/
void Parcel::vSetOperand(Operand *pOperand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  m_pObjects[0] = pOperand;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
