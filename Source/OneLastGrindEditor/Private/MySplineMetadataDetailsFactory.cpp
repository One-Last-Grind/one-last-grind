// Fill out your copyright notice in the Description page of Project Settings.


#include "MySplineMetadataDetailsFactory.h"

#include <IDetailGroup.h>
#include <DetailLayoutBuilder.h>
#include <DetailWidgetRow.h>
#include <Widgets/Input/SNumericEntryBox.h>
#include <ScopedTransaction.h>
#include <ComponentVisualizer.h>
#include <Editor.h>

#include "OneLastGrind/Public/MySplineMetadata.h"

#define LOCTEXT_NAMESPACE "FMySplineMetadataDetailsFactory"

TSharedPtr<ISplineMetadataDetails> UMySplineMetadataDetailsFactory::Create()
{
	return MakeShared<FMySplineMetadataDetails>();
}

UClass* UMySplineMetadataDetailsFactory::GetMetadataClass() const
{
	return UMySplineMetadata::StaticClass();
}

FName FMySplineMetadataDetails::GetName() const
{
	return FName(TEXT("MySplineMetadataDetails"));
}

FText FMySplineMetadataDetails::GetDisplayName() const
{
	return LOCTEXT("MySplineMetadataDetails", "Metadata");
}

template<class T>
bool UpdateMultipleValue(TOptional<T>& CurrentValue, T InValue)
{
	if (!CurrentValue.IsSet())
	{
		CurrentValue = InValue;
	}
	else if (CurrentValue.IsSet() && CurrentValue.GetValue() != InValue)
	{
		CurrentValue.Reset();
		return false;
	}

	return true;
}

void FMySplineMetadataDetails::Update(USplineComponent* InSplineComponent, const TSet<int32>& InSelectedKeys)
{
	SplineComp = InSplineComponent;
	SelectedKeys = InSelectedKeys;
	IsStopPointValue.Reset();
	SplineDirectionValue.Reset();

	if (InSplineComponent)
	{
		bool bUpdateIsStopPoint = true;
		bool bUpdateSplineDirection = true;

		UMySplineMetadata* Metadata = Cast<UMySplineMetadata>(InSplineComponent->GetSplinePointsMetadata());
		if (Metadata)
		{
			for (int32 Index : InSelectedKeys)
			{
				if (Metadata->PointParams.IsValidIndex(Index))
				{
					if (bUpdateIsStopPoint)
					{
						bUpdateIsStopPoint = UpdateMultipleValue(IsStopPointValue, Metadata->PointParams[Index].IsStopPoint);
					}
					if (bUpdateSplineDirection) {
						bUpdateSplineDirection = UpdateMultipleValue(SplineDirectionValue, Metadata->PointParams[Index].SplineDirection);
					}
				}
			}
		}
	}
}

void FMySplineMetadataDetails::GenerateChildContent(IDetailGroup& DetailGroup)
{
	DetailGroup.AddWidgetRow()
		.Visibility(EVisibility::Visible)
		.NameContent()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
				.Text(LOCTEXT("SplineDirection", "Spline Direction"))
				.Font(IDetailLayoutBuilder::GetDetailFont())
		]
		.ValueContent()
		.MinDesiredWidth(125.0f)
		.MaxDesiredWidth(125.0f)
		[
			SNew(SNumericEntryBox<int>)
				.Value(this, &FMySplineMetadataDetails::GetSplineDirection)
				.AllowSpin(false)
				.MinValue(0)
				.MaxValue(4)
				.MinSliderValue(0)
				.MaxSliderValue(4)
				.UndeterminedString(LOCTEXT("Multiple", "Multiple"))
				.OnValueCommitted(this, &FMySplineMetadataDetails::OnSetSplineDirection)
				.Font(IDetailLayoutBuilder::GetDetailFont())
		];
	DetailGroup.AddWidgetRow()
		.Visibility(EVisibility::Visible)
		.NameContent()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
				.Text(LOCTEXT("IsStopPoint", "Is Stop Point"))
				.Font(IDetailLayoutBuilder::GetDetailFont())
		]
		.ValueContent()
		.MinDesiredWidth(125.0f)
		.MaxDesiredWidth(125.0f)
		[
			SNew(SNumericEntryBox<int>)
				.Value(this, &FMySplineMetadataDetails::GetIsStopPoint)
				.AllowSpin(false)
				.MinValue(0)
				.MaxValue(1)
				.MinSliderValue(0)
				.MaxSliderValue(1)
				.UndeterminedString(LOCTEXT("Multiple", "Multiple"))
				.OnValueCommitted(this, &FMySplineMetadataDetails::OnSetIsStopPoint)
				.Font(IDetailLayoutBuilder::GetDetailFont())
		];
	
}

void FMySplineMetadataDetails::OnSetValues(FMySplineMetadataDetails& Details)
{
	Details.SplineComp->GetSplinePointsMetadata()->Modify();
	Details.SplineComp->UpdateSpline();
	Details.SplineComp->bSplineHasBeenEdited = true;
	static FProperty* SplineCurvesProperty = FindFProperty<FProperty>(USplineComponent::StaticClass(), GET_MEMBER_NAME_CHECKED(USplineComponent, SplineCurves));
	FComponentVisualizer::NotifyPropertyModified(Details.SplineComp, SplineCurvesProperty);
	Details.Update(Details.SplineComp, Details.SelectedKeys);

	GEditor->RedrawLevelEditingViewports(true);
}

void FMySplineMetadataDetails::OnSetIsStopPoint(int NewValue, ETextCommit::Type CommitInfo)
{
	if (UMySplineMetadata* Metadata = GetMetadata())
	{
		const FScopedTransaction Transaction(LOCTEXT("SetIsStopPoint", "Set spline point is stop point"));

		for (int32 Index : SelectedKeys)
		{
			Metadata->PointParams[Index].IsStopPoint = NewValue;
		}

		OnSetValues(*this);
	}
}

void FMySplineMetadataDetails::OnSetSplineDirection(int NewValue, ETextCommit::Type CommitInfo)
{
	if (UMySplineMetadata* Metadata = GetMetadata())
	{
		const FScopedTransaction Transaction(LOCTEXT("SetSplineDirection", "Set spline direction at spline point"));

		for (int32 Index : SelectedKeys)
		{
			Metadata->PointParams[Index].SplineDirection = NewValue;
		}

		OnSetValues(*this);
	}
}

UMySplineMetadata* FMySplineMetadataDetails::GetMetadata() const
{
	UMySplineMetadata* Metadata = SplineComp ? Cast<UMySplineMetadata>(SplineComp->GetSplinePointsMetadata()) : nullptr;
	return Metadata;
}

TOptional<int> FMySplineMetadataDetails::GetIsStopPoint() const
{
	TOptional<int> value = IsStopPointValue;
	return value;
}

TOptional<int> FMySplineMetadataDetails::GetSplineDirection() const
{
	TOptional<int> value = SplineDirectionValue;
	return value;
}

#undef LOCTEXT_NAMESPACE