/*============================================================================*/
/* Operand.cpp -- Operand Buffer                                              */
/*============================================================================*/

#include "Operands.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Operand* Operands::_pOperand(void* aAddress)
{
  Operand* pOperand = reinterpret_cast<Operand*>(aAddress);
  
  return pOperand;
}

/*----------------------------------------------------------------------------*/
/* is given class a valid operand?                                            */
/*----------------------------------------------------------------------------*/
bool Operands::_bIsValidOperand(eCN eClass)
{
  switch(eClass) {
    case CN_CRWBUFFER:
    case CN_CSVBUFFER:
    case CN_INDEXDEF:
    case CN_INFOSET:
    case CN_IRWBUFFER:
    case CN_MARKER:
    case CN_PROFILE:
    case CN_RSBBUFFER:
    case CN_STREAMDEF:
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

  m_eTailProcessing = TP_OUTPUT;
  MemLib::vZero(m__PadToCacheLine,sizeof(m__PadToCacheLine),eClassNo);

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
/* is this operand an InfoSet ?                                               */
/*----------------------------------------------------------------------------*/
bool Operand::bIsInfoSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return eClass() == CN_INFOSET ? true : false;
}

/*----------------------------------------------------------------------------*/
/* is this operand a marker ?                                                 */
/*----------------------------------------------------------------------------*/
bool Operand::bIsMarker(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return eClass() == CN_MARKER ? true : false;
}

/*----------------------------------------------------------------------------*/
/* is this operand a streamDef ?                                              */
/*----------------------------------------------------------------------------*/
bool Operand::bIsStreamDef(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return eClass() == CN_STREAMDEF ? true : false;
}

/*----------------------------------------------------------------------------*/
/* set tail processing of the operand                                         */
/*----------------------------------------------------------------------------*/
void Operand::vTailProcessing(eTP eTailProcessing)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_eTailProcessing = eTailProcessing;
}

/*----------------------------------------------------------------------------*/
/* get tail processing of the operand                                         */
/*----------------------------------------------------------------------------*/
eTP Operand::eTailProcessing(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_eTailProcessing;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
