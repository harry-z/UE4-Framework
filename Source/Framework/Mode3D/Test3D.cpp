#include "Test3D.h"
#include "../Framework.h"
#include "Subsystems/ElementDrawerSubsystem.h"
#include "Engine/Engine.h"

void FTest3D::NotifySwitchGameContent()
{
	static bool bFirstTime = true;
	if (bFirstTime)
	{
		TestElementDrawer();

		bFirstTime = false;
	}
}

void FTest3D::TestElementDrawer()
{
	GWorld->GetSubsystem<UElementDrawerSubsystem>()->GetElementDrawer()->AddLine(FVector(0.0f, -20.0f, 40.0f), FVector(0.0f, 20.0f, 40.0f), FColor::White, 0.0f, true);
}