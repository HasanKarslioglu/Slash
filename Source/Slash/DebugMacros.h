#pragma once
#include "DrawDebugHelpers.h"

#define DRAW_SPHERE(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 32.f, 16, FColor::Red, true);
#define DRAW_SPHERE_ONEFRAME(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 32.f, 16, FColor::Red, false, -1.f);
#define DRAW_LINE(Start, End) if (GetWorld()) DrawDebugLine(GetWorld(), Start, End, FColor::Blue, true, -1.f, 0, 1.f);
#define DRAW_LINE_ONEFRAME(Start, End) if (GetWorld()) DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, -1.f, 0, 1.f);
#define PRINT(Text) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Text);
