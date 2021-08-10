/*============================================================================*/
/* Steps_BucketRecv.cpp -- metaRecv step group                                  */
/*============================================================================*/


#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include <eTT.h>
#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "CrwBuffer.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"
#include "Parcel.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_BucketRecv(void)
{
  vAddStep("BucketRecv.insertRows"_Stz,&Step::vBucketRecvInsertRows);
}

/*----------------------------------------------------------------------------*/
/* BucketRecv::insertRows -- put parts of data                                  */
/*----------------------------------------------------------------------------*/
void Step::vBucketRecvInsertRows(void)
{
  ENTRY();

  // ... code which pipesi in all the parts of a meta-object and
  //     gets the meta-object to reassemble itself ...

  vTrace("C++ gets called: vBucketRecvInsertRows\n"_Stz);

  CrwBuffer* pCrwBuffer = _pCrwBuffer(m_pDataStack->pPopPtr(__func__));





  //m_pDataStack->vPushPtr(pTable);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/