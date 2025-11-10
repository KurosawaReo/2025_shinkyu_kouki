// Fill out your copyright notice in the Description page of Project Settings.


#include "PlyerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
// Sets default values
APlyerCharacter::APlyerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//カプセルコライダー設定.
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	//キャラクターの回転速度(カメラ方向に回転させる).
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw   = false;
	bUseControllerRotationRoll  = false;

	//キャラクター移動の設定.
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	//スプリングアーム(カメラブーム)の作成.
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 50.0f);

	//カメラの作成.
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	//初期状態.
	bIsSprinting = false;
}

// Called when the game starts or when spawned
void APlyerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlyerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlyerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Input(PlayerInputComponent);
}

/// <summary>
/// 移動処理.
/// </summary>
/// <param name="PlayerInputComponent"></param>
void APlyerCharacter::Input(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//移動入力.
	PlayerInputComponent->BindAxis("MoveForward", this, &APlyerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlyerCharacter::MoveRight);

	//カメラ入力.
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlyerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlyerCharacter::LookUpAtRate);

	//ジャンプ入力.
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//スプリント入力.
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlyerCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlyerCharacter::StopSprint);

	//弾発射入力.
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlyerCharacter::ShotBullet);
}

void APlyerCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		//カメラの前方向を取得.
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APlyerCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		//カメラの右方向を取得.
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}
void APlyerCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());

}
void APlyerCharacter::LookUpAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());

}
void APlyerCharacter::StartSprint()
{
	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;

}
void APlyerCharacter::StopSprint()
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}
void APlyerCharacter::ShotBullet()
{
	if (FollowCamera == nullptr || GetWorld() == nullptr)
	{
		return;
	}
	
	// BulletClassが設定されているか確認.
	if (BulletClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("BulletClass is not set! Please set it in Blueprint."));
		return;
	}

	//カメラの位置を取得.
	FVector CameraLocation  = FollowCamera->GetComponentLocation();
	FRotator CameraRotation = FollowCamera->GetComponentRotation();
	FVector FotwarVector    = CameraRotation.Vector();

	//弾の発射位置(カメラの少し前方)
	FVector SpawnLocation = CameraLocation + (FotwarVector * 100.0f);

	//弾のスポーンパラメーター設定.
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	// 弾クラスを生成.
	AActor* Bullet = GetWorld()->SpawnActor<AActor>(BulletClass, SpawnLocation, CameraRotation, SpawnParams);



}