/*
   - MyCharacter -
   作成: 怜旺.

   仮のプレイヤーとして使ってるだけのもの.
*/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class AMyCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    //コンストラクタ.
    AMyCharacter();

protected:
    virtual void BeginPlay() override;

    // カメラコンポーネント
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    class UCameraComponent* Camera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    class USpringArmComponent* SpringArm;

    // 入力関数
    void MoveForward(float Value);
    void MoveRight(float Value);
    void LookUp(float Value);
    void Turn(float Value);

public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

//オリジナル変数.
private:
    FTimerHandle timerHandle_Shot; //宣言が必要らしい.

//オリジナル関数.
public:
    void Init();
    void ShotTimer();
    void ShotExe();
};