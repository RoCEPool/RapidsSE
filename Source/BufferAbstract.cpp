/*============================================================================*/
/* BufferAbstract.cpp -- Abstract Buffer                                      */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
BufferAbstract* DBobjects::_pBufferAbstract(void* aAddr)
{
  BufferAbstract* pBufferAbstract = reinterpret_cast<BufferAbstract*>(aAddr);
  
  RASSERT(    !pBufferAbstract
           || CN_ENTRYBUFFER == pBufferAbstract->eClass()
           || CN_RESULTBUFFER == pBufferAbstract->eClass()
          );
  
  return pBufferAbstract;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
BufferAbstract::BufferAbstract(eCN eClassNo)
: Object(eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo);
  
  _checkInitialised_(eClassNo);
}


/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
BufferAbstract::~BufferAbstract(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
