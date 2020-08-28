#include "FrameworkGameContent3DFactory.h"

#define LOCTEXT_NAMESPACE "Framework"

DEFINE_GAMECONTENT_FACTORY(FFrameworkGameContent3DFactory)

FFrameworkGameContentRef FFrameworkGameContent3DFactory::CreateInstance()
{
	return MakeShared<FFrameworkGameContent, ESPMode::ThreadSafe>(EGameContentType::Type3D, LOCTEXT("Framework_3D", "3D"), TEXT("/Game/Maps/Map3D"));
}

#undef LOCTEXT_NAMESPACE