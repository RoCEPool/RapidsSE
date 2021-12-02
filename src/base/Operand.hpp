/*============================================================================*/
/* Operand.hpp -- Operand                                                     */
/*============================================================================*/
#pragma once

#include "ePT.h"
#include "eRV.h"
#include "eZK.h"

namespace BASE
{
  class Operand : public AlignedObject,
                  public ListItem_t<Operand>
  {
  private:
    eRV     m_eRecvInstructions;   // receiver instructions
    ePT     m_eTailProcessing;     // pipeline tail processing
 
    PadWord m__PadToCacheLine[4];  // padding to cache line

  public:
    Operand(eCN eClass);
    ~Operand(void) noexcept(false);

    static void vRepair(Object* pObject);
    static void vRepairList(Object* pObject);

    bool bIsBuffer(void);
    bool bIsInfoSet(void);
    bool bIsMarker(void);
    bool bIsProfile(void);
    bool bIsStreamDef(void);
    eZK  eZkKind(void);
    eRV  eRecvInstructions(void);
    void vRecvInstructions(eRV eRecvInstructions);
    ePT  eTailProcessing(void);
    void vTailProcessing(ePT eTailProcessing);
  };
  
  Operand* _pOperand(void* aAddr);
  bool _bIsValidOperand(eCN eClass);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
