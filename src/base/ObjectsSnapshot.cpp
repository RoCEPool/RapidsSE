/*============================================================================*/
/* ObjectSnapshot.cpp -- count of objects by class in the buffer pool         */
/*============================================================================*/

#include "BASE.h"

ToolFn __ObjectSnapshot_List
   ("ObjectCounts"_Stz,(fpSELFFN*)ObjectSnapshot::vObjectCounts,
                     ""_Stz,
                     nullptr);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,ObjectSnapshot,CN_OBJECTSNAPSHOT)
__aNewFunction(ObjectSnapshot,CN_OBJECTSNAPSHOT)
__vDeleteFunction(ObjectSnapshot,CN_OBJECTSNAPSHOT)
__aFooterFunction(ObjectSnapshot)

/*----------------------------------------------------------------------------*/
/* constructor (new region)                                                   */
/*----------------------------------------------------------------------------*/
ObjectSnapshot::ObjectSnapshot(void)
: Object(CN_OBJECTSNAPSHOT)
{
  m_iWhenSnapshot = 0;
  
  for (i8 i_enum = 0; i_enum < CN_ENUMS; i_enum++)
    m_nObjects[i_enum] = 0;
  
  _checkInitialised_(CN_OBJECTSNAPSHOT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_OBJECTSNAPSHOT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ObjectSnapshot::~ObjectSnapshot(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OBJECTSNAPSHOT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void ObjectSnapshot::vObjectCounts(void*)
{
  _static_
  
  ObjectSnapshot* pObjectSnapshot = new ObjectSnapshot;
  
  pObjectSnapshot->vTakeSnapshot();
  pObjectSnapshot->vPrintCounts();

  delete pObjectSnapshot;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* print class couunts                                                        */
/*----------------------------------------------------------------------------*/
void ObjectSnapshot::vPrintCounts(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OBJECTSNAPSHOT,_FOOTER_);
  
  i8 iCount = 0;
  for (i8 i_Class = 0 ; i_Class < CN_ENUMS; i_Class++) {
    
    eCN eClass = (eCN)i_Class;
    
    i8 nObjects = m_nObjects[eClass];
    
    if (!nObjects)
      continue;
    
    vTrace("  %20s %6lld"_Stz,aCN[eClass],m_nObjects[eClass]);
    if (! (++iCount % 4))
      vTrace("\n"_Stz);
  }
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* print class couunts                                                        */
/*----------------------------------------------------------------------------*/
void ObjectSnapshot::vDeltaList(ObjectSnapshot* pWithObjectSnapshot_newer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OBJECTSNAPSHOT,_FOOTER_);
  
  //i8 iCount = 0;
  for (i8 i_Class = 0 ; i_Class < CN_ENUMS; i_Class++) {
    
    eCN eClass = (eCN)i_Class;
        
    i8 nWas = m_nObjects[eClass];
    i8 nNow = pWithObjectSnapshot_newer->m_nObjects[eClass];
    i8 nDelta = nWas - nNow;
        
    if (0 == nDelta)
      continue;

    vTrace("  %20s was %lld now %lld delta %lld\n"_Stz,
           aCN[eClass],nWas,nNow,nDelta);
//    if (! (++iCount % 4))
//      vTrace("\n"_Stz);
  }
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* take snapshot                                                              */
/*----------------------------------------------------------------------------*/
void ObjectSnapshot::vTakeSnapshot(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OBJECTSNAPSHOT,_FOOTER_);
  
  m_iWhenSnapshot = Hardware::iReadTsc();

  PoolHelper::vObjectSnapshot(this);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
