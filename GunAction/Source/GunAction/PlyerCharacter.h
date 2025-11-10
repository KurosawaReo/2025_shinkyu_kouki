// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlyerCharacter.generated.h"

UCLASS()
class GUNACTION_API APlyerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlyerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:

	//カメラコンポーネント.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent * FollowCamera;
 
	// 弾クラスの参照（Blueprintで設定可能）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bullet)
	TSubclassOf<AActor> BulletClass;

	//移動パラメーター.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float BaseTurnRate = 45.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float BaseLookUpRate = 45.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float WalkSpeed = 600.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float RunSpeed = 800.0f;

	//入力処理.

	/// <summary>
	/// 移動処理.
	/// </summary>
	/// <param name="PlayerInputComponent"></param>
	void Input(UInputComponent* PlayerInputComponent);
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rete);
	void StartSprint();
	void StopSprint();

	//弾発射処理.
	void ShotBullet();


	//状態フラグ.
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool bIsSprinting;
};
