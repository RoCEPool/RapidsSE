/*============================================================================*/
/* Operand.cpp -- Operand Buffer                                              */
/*============================================================================*/

#include "BASE.h"



/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Operand* BASE::_pOperand(void* aAddress)
{
  Operand* pOperand = reinterpret_cast<Operand*>(aAddress);
  
  return pOperand;
}

/*----------------------------------------------------------------------------*/
/* is given class a valid operand?                                            */
/*----------------------------------------------------------------------------*/
bool BASE::_bIsValidOperand(eCN eClass)
{
  switch(eClass) {
    case CN_CRWBUFFER:
    case CN_CSVBUFFER:
    case CN_EACHROWBUFFER:
    case CN_INDEXDEF:
    case CN_INFOSET:
    case CN_MARKER:
    case CN_PROFILE:
    case CN_ROWBUFFER:
    case CN_STREAMDEF:
    case CN_STREAMDEFRHT:
    case CN_STREAMSET:
      return true;
    default:
      return false;
  }
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Operand::Operand(eCN eClassNo)
: AlignedObject(eClassNo,sizeof(Operand))
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);
  
  ASSERT(CacheLine::bMultipleCacheLines(sizeof(Operand)));

  m_eTailProcessing = PT_OUTPUT;
  //MemLib::vZero(m__PadToCacheLine,sizeof(m__PadToCacheLine),eClassNo);

  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo);  
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Operand::~Operand(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repair operand                                                             */
/*----------------------------------------------------------------------------*/
void Operand::vRepair(Object* pObject)
{
  _static_

  Operand* pOperand = _pOperand(pObject);

  // delete the operand
  AM::vDeleteOperand(pOperand);
}

/*----------------------------------------------------------------------------*/
/* repair list of operands                                                    */
/*----------------------------------------------------------------------------*/
void Operand::vRepairList(Object* pObject)
{
  _static_

  Operand* pOperand = _pOperand(pObject);

  Operand* pNextOperand;

  while (pOperand) {
    pNextOperand = pOperand->pNextItem();
    AM::vDeleteOperand(pOperand);
    pOperand = pNextOperand;
  }

}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is this operand a buffer ?                                                 */
/*----------------------------------------------------------------------------*/
bool Operand::bIsBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  eCN eClass = this->eGetClassNo();
  
  if (eClass == CN_INFOSET || eClass == CN_MARKER ||
      eClass == CN_PROFILE || eClass == CN_STREAMDEF ||
      eClass == CN_STREAMDEFRHT)
    return false;
  else
    return true;
}

/*----------------------------------------------------------------------------*/
/* is this operand an InfoSet ?                                               */
/*----------------------------------------------------------------------------*/
bool Operand::bIsInfoSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return eGetClassNo() == CN_INFOSET ? true : false;
}

/*----------------------------------------------------------------------------*/
/* is this operand a marker ?                                                 */
/*----------------------------------------------------------------------------*/
bool Operand::bIsMarker(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return eGetClassNo() == CN_MARKER ? true : false;
}

/*----------------------------------------------------------------------------*/
/* is this operand a Profile ?                                                */
/*----------------------------------------------------------------------------*/
bool Operand::bIsProfile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return eGetClassNo() == CN_PROFILE ? true : false;
}

/*----------------------------------------------------------------------------*/
/* is this operand a streamDef ?                                              */
/*----------------------------------------------------------------------------*/
bool Operand::bIsStreamDef(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return eGetClassNo() == CN_STREAMDEF || eGetClassNo() == CN_STREAMDEFRHT ? true : false;
}

/*----------------------------------------------------------------------------*/
/* get receiver instructions                                                  */
/*----------------------------------------------------------------------------*/
eRV Operand::eRecvInstructions(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_eRecvInstructions;
}

/*----------------------------------------------------------------------------*/
/* set receiver instructions                                                  */
/*----------------------------------------------------------------------------*/
void Operand::vRecvInstructions(eRV eRecvInstructions)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_eRecvInstructions = eRecvInstructions;
}

/*----------------------------------------------------------------------------*/
/* get tail processing of the operand                                         */
/*----------------------------------------------------------------------------*/
ePT Operand::eTailProcessing(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_eTailProcessing;
}

/*----------------------------------------------------------------------------*/
/* set tail processing of the operand                                         */
/*----------------------------------------------------------------------------*/
void Operand::vTailProcessing(ePT eTailProcessing)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_eTailProcessing = eTailProcessing;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
