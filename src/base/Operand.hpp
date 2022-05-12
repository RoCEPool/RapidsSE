/*============================================================================*/
/* Operand.hpp -- Operand                                                     */
/*============================================================================*/
#pragma once

#include "ePT.h"
#include "eRV.h"
#include "eEV.h"

namespace BASE
{
  class Operand : public AlignedObject,
                  public ListItem_t<Operand>
  {
  private:
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
    ePT  eTailProcessing(void);
    void vTailProcessing(ePT eTailProcessing);
  };
  
  Operand* _pOperand(void* aAddr);
  bool _bIsValidOperand(eCN eClass);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
