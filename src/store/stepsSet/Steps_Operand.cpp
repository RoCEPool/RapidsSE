/*============================================================================*/
/* Steps_Operand.cpp -- Register Operand step group                           */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "eYX.h"

#include "Marker.hpp"

#include "RowBuffer.hpp"
#include "EachRowBuffer.hpp"

#include "RowCoder.hpp"
#include "HeadLine.hpp"
#include "Crw.hpp"
#include "CrwPacker.hpp"
#include "CrwBuffer.hpp"
#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "CrvBuffer.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"
#include "Node_I.hpp"
#include "Op.hpp"
#include "OpSpec.hpp"
#include "Job.hpp"
#include "LogicUnit.hpp"

#include "Parcel.hpp"
#include "MY.hpp"


#ifdef WITH_PYTHON_SUPPORT
  #include "/usr/include/python2.7/Python.h"
#endif

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Operand(void)
{
  vAddStep("operand.toParcel"_Stz,         &Step::vOperandToParcel);
  
}

/*----------------------------------------------------------------------------*/
/* operand.toParcel -- convert operand to parcel                              */
/*----------------------------------------------------------------------------*/
void Step::vOperandToParcel(void)
{
  ENTRY();
  
  // revised job/op model
  Op* pOp = *m_pOp;
  OpSpec* pOpSpec = pOp->pOpSpec();;

  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));

  Parcel* pParcel =  new Parcel;

  pParcel->vAddItem(pOperand);
  
  pParcel->vLabel(pOp->pJobOfOp()->tJobId().iAsI8(),
                  pOpSpec->eParcelForward());
  
  m_pDataStack->vPushPtr(pParcel);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
