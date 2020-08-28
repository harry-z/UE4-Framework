#include "FrameworkGameContent2DFactory.h"

#define LOCTEXT_NAMESPACE "Framework"

DEFINE_GAMECONTENT_FACTORY(FFrameworkGameContent2DFactory)

FFrameworkGameContentRef FFrameworkGameContent2DFactory::CreateInstance()
{
	return MakeShared<FFrameworkGameContent, ESPMode::ThreadSafe>(EGameContentType::Type2D, LOCTEXT("Framework_2D", "2D"), TEXT("/Game/Maps/Map2D"));
}

#undef LOCTEXT_NAMESPACE