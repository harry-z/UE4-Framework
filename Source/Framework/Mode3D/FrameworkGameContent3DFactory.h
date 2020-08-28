#pragma once
#include "../Framework.h"

class FFrameworkGameContent3DFactory final : public FFrameworkGameContentFactory
{
public:
	virtual FFrameworkGameContentRef CreateInstance() override;
};

DECLARE_GAMECONTENT_FACTORY(EGameContentType::Type3D, FFrameworkGameContent3DFactory)