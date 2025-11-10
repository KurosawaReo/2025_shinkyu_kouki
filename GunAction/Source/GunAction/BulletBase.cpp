/*
   - BulletBase -
   作成: 怜旺.

   弾の元となる基底クラス.
*/
#include "BulletBase.h"

// Sets default values
ABulletBase::ABulletBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABulletBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABulletBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float SPEED = 2.0; //速度.

	//前方向.
	FVector forward = GetActorForwardVector();
	//移動実行.
	SetActorLocation(GetActorLocation() + forward + SPEED);
}

