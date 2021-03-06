#include "TimeAttack.h"
#include "TimeAttackFunctionLibrary.h"

#pragma optimize ("", off)

UProperty* UTimeAttackFunctionLibrary::RetrieveProperty(UObject* Object, FString InPath, UObject*& OutTargetObject)
{
	if (Object == nullptr)
	{
		return nullptr;
	}

	TArray<FString> Path;
	if (!InPath.ParseIntoArray(Path, TEXT("."), true))
	{
		return nullptr; // Invalid path specified.
	}

	OutTargetObject = Object;

	UClass* Class = Object->GetClass();
	UStruct* Struct = nullptr;

	// Check if first token references an ActorComponent ID.

	AActor* Actor = Cast<AActor>(OutTargetObject);

	if (Actor != nullptr && Path.Num() > 1)
	{
		// Do you want to reference a Pawn Controller?

		if (Path[0].Equals(TEXT("Controller")))
		{
			APawn* Pawn = Cast<APawn>(Actor);
			if (Pawn != nullptr)
			{
				Actor = Pawn->GetController();
				OutTargetObject = Actor;
				Class = OutTargetObject->GetClass();
				Path.RemoveAt(0);
			}
		}

		// Do you want to reference an Actor Component?

		if (Path.Num() > 1)
		{
			for (auto Component : Actor->GetComponents())
			{
				// Store the component in OutTargetObject
				if (Path[0].Equals(Component->GetName()))
				{
					Class = Component->GetClass();
					OutTargetObject = Component;
					Path.RemoveAt(0);
					break;
				}
			}
		}
	}

	int32 Index = 0;

	// Parse path.

	while (Index < Path.Num() - 1)
	{
		FString Member, ArrayIndexStr;
		int32 ArrayIndex = 0;

		// Check if the value is an array: if it it, store the property name in Member and save the index in ArrayIndexStr ...
		if (Path[Index].Split(FString(TEXT("[")), &Member, &ArrayIndexStr))
		{
			ArrayIndex = FCString::Atoi(*ArrayIndexStr);
		}
		else // ... otherwise just store the property name in Member
		{
			Member = Path[Index];
		}

		UProperty* Property;

		if (Class != nullptr)
		{
			Property = Class->FindPropertyByName(*Member);
		}
		else
		{
			Property = Struct->FindPropertyByName(*Member);
		}

		if (Property != nullptr)
		{
			UStructProperty* StructProp = Cast<UStructProperty>(Property);
			if (StructProp != nullptr)
			{
				Class = nullptr;
				Struct = StructProp->Struct;

				OutTargetObject = StructProp->ContainerPtrToValuePtr<UStruct>(OutTargetObject, ArrayIndex);
			}
			else
			{
				UObjectProperty* ObjectProp = Cast<UObjectProperty>(Property);
				if (ObjectProp != nullptr)
				{
					Class = ObjectProp->PropertyClass;
					Struct = nullptr;

					OutTargetObject = ObjectProp->GetObjectPropertyValue(OutTargetObject);
				}

				// Check if the array property contains class or struct
				UArrayProperty* ArrayProp = Cast<UArrayProperty>(Property);
				if (ArrayProp != nullptr)
				{
					Class = nullptr;
					Struct = nullptr;

					UProperty* InnerProp = ArrayProp->Inner;
					UObjectProperty* ObjectProp = Cast<UObjectProperty>(InnerProp);
					if (ObjectProp != nullptr) 
					{
						Class = ObjectProp->PropertyClass;
						FScriptArrayHelper InnerHelper { ArrayProp, ArrayProp->ContainerPtrToValuePtr<void>(OutTargetObject) };
						OutTargetObject = (UObject*)(InnerHelper.GetRawPtr(ArrayIndex));
					}
					else 
					{
						StructProp = Cast<UStructProperty>(InnerProp);
						if (StructProp != nullptr) 
						{
							Struct = StructProp->Struct;
							FScriptArrayHelper InnerHelper { ArrayProp, ArrayProp->ContainerPtrToValuePtr<void>(OutTargetObject) };
							OutTargetObject = (UStruct*)(InnerHelper.GetRawPtr(ArrayIndex));
						}
					}
				}
				else
				{
					OutTargetObject = nullptr;
					break;
				}
			}
		}

		Index++;
	}

	UProperty* Property = nullptr;

	// If path is correct, retrieve final float property.

	if (OutTargetObject != nullptr)
	{
		if (Class != nullptr && OutTargetObject->GetClass() != nullptr)
		{
			Class = OutTargetObject->GetClass();
		}

		if (Class != nullptr)
		{
			Property = FindField<UProperty>(Class, *(Path[Index]));
		}
		else
		{
			Property = FindField<UProperty>(Struct, *(Path[Index]));
		}
	}

	return Property;
}