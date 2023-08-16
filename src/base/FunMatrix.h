/*============================================================================*/
/* FunMatrix.h -- Function Matrix                                             */
/*============================================================================*/
#pragma once

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// CONVERSION OPERATORS
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

extern eFM eMacro_Cast[DM_ENUMS][DM_ENUMS];
extern eDM eMacro_CastResult[DM_ENUMS][DM_ENUMS];

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// EXTRACT OPERATORS
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

extern eFM eMacro_ExtractDay[DM_ENUMS];
extern eFM eMacro_ExtractMonth[DM_ENUMS];
extern eFM eMacro_ExtractYear[DM_ENUMS];
extern eFM eMacro_ExtractHour[DM_ENUMS];
extern eFM eMacro_ExtractMinute[DM_ENUMS];
extern eFM eMacro_ExtractSecond[DM_ENUMS];

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// MATHEMATICAL OPERATORS
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// ADD

extern eFM eMacro_Add[DM_ENUMS][DM_ENUMS];
extern eDM eMacro_AddResult[DM_ENUMS][DM_ENUMS];


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// DIVIDE

extern eFM eMacro_Div[DM_ENUMS][DM_ENUMS];
extern eDM eMacro_DivResult[DM_ENUMS][DM_ENUMS];

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// MODULUS

extern eFM eMacro_Mod[DM_ENUMS][DM_ENUMS];
extern eDM eMacro_ModResult[DM_ENUMS][DM_ENUMS];

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// MULTIPLY

extern eFM eMacro_Mul[DM_ENUMS][DM_ENUMS];
extern eDM eMacro_MulResult[DM_ENUMS][DM_ENUMS];
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// NEGATE

extern eDM eMacro_NegResult[DM_ENUMS];
extern eFM eMacro_Neg[DM_ENUMS];
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// BETWEEN

extern eFM eMacro_Bet[DM_ENUMS][DM_ENUMS];

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// DUP

extern eFM eMacro_Dup[DM_ENUMS];
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// PSH

extern eFM eMacro_Push[DM_ENUMS];
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// INX ( in expression)

extern eFM eMacro_Inx[DM_ENUMS][DM_ENUMS];
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// SUBTRACT

extern eFM eMacro_Sub[DM_ENUMS][DM_ENUMS];
extern eDM eMacro_SubResult[DM_ENUMS][DM_ENUMS];

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// SUM

extern eFM eMacro_Sum[DM_ENUMS][DM_ENUMS];


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// RELATIONAL OPERATORS
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

extern eFM eMacro_Eq[DM_ENUMS][DM_ENUMS];
extern eFM eMacro_Ge[DM_ENUMS][DM_ENUMS];
extern eFM eMacro_Gt[DM_ENUMS][DM_ENUMS];
extern eFM eMacro_Le[DM_ENUMS][DM_ENUMS];
extern eFM eMacro_Lt[DM_ENUMS][DM_ENUMS];
extern eFM eMacro_Ne[DM_ENUMS][DM_ENUMS];
extern eDM eMacro_CmpResult[DM_ENUMS][DM_ENUMS];
extern eFM eMacro_Gta[DM_ENUMS][DM_ENUMS];
extern eFM eMacro_Lta[DM_ENUMS][DM_ENUMS];


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
