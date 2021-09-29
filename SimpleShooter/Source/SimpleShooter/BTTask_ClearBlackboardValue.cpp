// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ClearBlackboardValue.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_ClearBlackboardValue::UBTTask_ClearBlackboardValue() 
{
    NodeName = TEXT("Clear Blackboard Value");
}

EBTNodeResult::Type UBTTask_ClearBlackboardValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) 
{
    Super::ExecuteTask(OwnerComp, NodeMemory); // Always do this when you do overriding

    // You can Find "GetSelectedBlackboardKey" in the parent class "UBTTask_BlackboardBase"
    // OwnerComp is reference, so there is no case reference becomes null
    OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());

    // Just visiting EBTNodeResult type with F12
    return EBTNodeResult::Succeeded;
}
