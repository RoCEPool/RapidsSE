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
: Operand(CN_SEEKOFFSETS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEEKOFFSETS);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEEKOFFSETS);
}

/*----------------------------------------------------------------------------*/
/* add offset                                                                 */
/*----------------------------------------------------------------------------*/
bool SeekOffsets::bAddOffset(i8 iOffset, i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEEKOFFSETS);
  
  if (m_nOffsets == C_SET_OFFSETS)
    return false;
  
  m_xSetOffsets[m_nOffsets].n_iOffsets = iOffset;
  m_xSetOffsets[m_nOffsets].m_nBytes   = nBytes;
  m_nOffsets++;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* dump seekOffsets                                                           */
/*----------------------------------------------------------------------------*/
void SeekOffsets::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEEKOFFSETS);
  
  vTrace("***************\n"_Stz);
  vTrace("SeekOffsets:\n"_Stz);
  for (i8 i_Offset = 0; i_Offset < m_nOffsets; i_Offset++)
  {
    vTrace("%d : %d\n"_Stz,
           m_xSetOffsets[i_Offset].n_iOffsets,
           m_xSetOffsets[i_Offset].m_nBytes);
  }
  vTrace("***************\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* is full                                                                    */
/*----------------------------------------------------------------------------*/
bool SeekOffsets::bIsFull(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_nOffsets == C_SET_OFFSETS ? true : false;
}

/*----------------------------------------------------------------------------*/
/* get offsets                                                                 */
/*----------------------------------------------------------------------------*/
i8 SeekOffsets::nOffsets(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  return m_nOffsets;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
