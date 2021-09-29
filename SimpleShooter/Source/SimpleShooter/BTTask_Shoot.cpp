// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shoot.h"
#include "ShooterCharacter.h"
#include "AIController.h"
UBTTask_Shoot::UBTTask_Shoot() 
{
    NodeName = "Shoot";
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) 
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    if(OwnerComp.GetAIOwner() == nullptr) return EBTNodeResult::Failed; 

    // UBehaviorTreeComponent -> UBrainComponent -> GetAIOwner()
	AShooterCharacter *Character = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
    // AShooterCharacter *Character = Cast<AShooterCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("SelfActor")));
    
    if(Character == nullptr) return EBTNodeResult::Failed; 

    Character->Shoot();
    return EBTNodeResult::Succeeded;
}
