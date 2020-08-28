#pragma once
#include "../Framework.h"

class FFrameworkGameContent2DFactory final : public FFrameworkGameContentFactory
{
public:
	virtual FFrameworkGameContentRef CreateInstance() override;
};

DECLARE_GAMECONTENT_FACTORY(EGameContentType::Type2D, FFrameworkGameContent2DFactory)