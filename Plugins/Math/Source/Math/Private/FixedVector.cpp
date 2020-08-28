#include "FixedVector.h"

namespace Math
{
	const Fixed C_FixedConst1 = FixedFromInt(1);
	const Fixed C_FixedConstHalf = C_FixedConst1 >> 1;
	const Fixed C_FixedConstError = (Fixed)1;
	const Fixed C_FixedConst1MinusError = C_FixedConst1 - C_FixedConstError;

	const FFixedVector2D C_FixedVector2DZero(0, 0);
	const FFixedVector2D C_FixedVector2DUnitX(C_FixedConst1, 0);
	const FFixedVector2D C_FixedVector2DUnitY(0, C_FixedConst1);

	const FFixedVector C_FixedVectorZero(0, 0, 0);
	const FFixedVector C_FixedVectorUnitX(C_FixedConst1, 0, 0);
	const FFixedVector C_FixedVectorUnitY(0, C_FixedConst1, 0);
	const FFixedVector C_FixedVectorUnitZ(0, 0, C_FixedConst1);
}
