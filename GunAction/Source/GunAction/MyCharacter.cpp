/*
   - MyCharacter -
   作成: 怜旺.

   仮のプレイヤーとして使ってるだけのもの.
*/
#include "MyCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

//コンストラクタ.
AMyCharacter::AMyCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // SpringArmの作成
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 300.0f;
    SpringArm->bUsePawnControlRotation = true;

    // カメラの作成
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

    // キャラクターの回転設定
    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
}

void AMyCharacter::BeginPlay()
{
    Super::BeginPlay();

    Init(); //初期化.
}

void AMyCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    ShotTimer(); //発射処理.
}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // 移動
    PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);

    // カメラ
    PlayerInputComponent->BindAxis("LookUp", this, &AMyCharacter::LookUp);
    PlayerInputComponent->BindAxis("Turn", this, &AMyCharacter::Turn);

    // ジャンプ
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
}

void AMyCharacter::MoveForward(float Value)
{
    if (Controller && Value != 0.0f)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

void AMyCharacter::MoveRight(float Value)
{
    if (Controller && Value != 0.0f)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        AddMovementInput(Direction, Value);
    }
}

void AMyCharacter::LookUp(float Value)
{
    AddControllerPitchInput(Value);
}

void AMyCharacter::Turn(float Value)
{
    AddControllerYawInput(Value);
}

////////////////////////////////////////////////

//初期化.
void AMyCharacter::Init()
{
//    startTime = clock(); //初期時刻.

    // 2秒ごとに ShotExe を呼ぶ（繰り返し）
    GetWorldTimerManager().SetTimer(timerHandle_Shot, this, &AMyCharacter::ShotExe, 2.0f, true);
}

//発射処理.
void AMyCharacter::ShotTimer() {

}

//発射実行.
void AMyCharacter::ShotExe()
{
    // アクターのパス。BP_Actor を生成したい場合は後ろに「.BP_Actor_C」を追加する
    FString aAssetPath = "/Game/GameObjects/Bullet/BP_BulletBase.BP_Actor_C";

    // アセットを読み込む（同期）
    TSubclassOf<AActor> aActorClass = TSoftClassPtr<AActor>(FSoftObjectPath(*aAssetPath)).LoadSynchronous();
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("b"));

    if (aActorClass != nullptr) {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("c"));

        // スポーン
//        FTransform aTransform = GetActorTransform();
        TObjectPtr<AActor> aActor = GetWorld()->SpawnActor<AActor>(aActorClass, GetActorTransform());

        // スポーンしたアクターへ何か処理をしたい場合
//        aActor->SetXXX();
    }
}