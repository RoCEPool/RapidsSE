/*============================================================================*/
/* SeekOffsets.cpp -- Seek Offsets                                            */
/*============================================================================*/

#include "Load.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
SeekOffsets* Load::_pSeekOffsets(void* aAddr)
{
  SeekOffsets* pSeekOffsets = reinterpret_cast<SeekOffsets*>(aAddr);
  
  return pSeekOffsets;
}

/*----------------------------------------------------------------------------*/
/* allocate from heap                                                         */
/*----------------------------------------------------------------------------*/
void* SeekOffsets::operator new(size_t)
{
  SeekOffsets* pSeekOffsets =
  static_cast<SeekOffsets*>(HeapMem::pAllocate(CN_MARKER));
  
  return pSeekOffsets;
}

/*----------------------------------------------------------------------------*/
/* free back to the heap                                                      */
/*----------------------------------------------------------------------------*/
void SeekOffsets::operator delete(void* aAddress)
{
  Object* pObject = static_cast<Object*>(aAddress);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SeekOffsets::SeekOffsets(void)
: Operand(CN_MARKER)
{
  _precondition_(_MAGIC_,CN_MARKER);
  
  m_nOffsets = 0;
  
  for (i8 i_Offset = 0; i_Offset < C_SET_OFFSETS; i_Offset++)
  {
    m_xSetOffsets[i_Offset].n_iOffsets = C_UNKNOWN;
    m_xSetOffsets[i_Offset].m_nBytes    = C_UNKNOWN;
  }
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SeekOffsets::~SeekOffsets(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
