// Fill out your copyright notice in the Description page of Project Settings.


#include "MySplineMetadataDetailsFactory.h"

#include <IDetailGroup.h>
#include <DetailLayoutBuilder.h>
#include <DetailWidgetRow.h>
#include <Widgets/Input/SNumericEntryBox.h>
#include <ScopedTransaction.h>
#include <ComponentVisualizer.h>
#include <Editor.h>

#include "OneLastGrind/MySplineMetadata.h"

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

	if (InSplineComponent)
	{
		bool bUpdateTestFloat = true;

		UMySplineMetadata* Metadata = Cast<UMySplineMetadata>(InSplineComponent->GetSplinePointsMetadata());
		if (Metadata)
		{
			for (int32 Index : InSelectedKeys)
			{
				if (Metadata->PointParams.IsValidIndex(Index))
				{
					if (bUpdateTestFloat)
					{
						bUpdateTestFloat = UpdateMultipleValue(IsStopPointValue, Metadata->PointParams[Index].IsStopPoint);
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
				.Text(LOCTEXT("IsStopPoint", "IsStopPoint"))
				.Font(IDetailLayoutBuilder::GetDetailFont())
		]
		.ValueContent()
		.MinDesiredWidth(125.0f)
		.MaxDesiredWidth(125.0f)
		[
			SNew(SCheckBox)
				.IsChecked(this, &FMySplineMetadataDetails::OnIsCheckedIsOnPoint)
				.OnCheckStateChanged(this, &FMySplineMetadataDetails::OnSetIsStopPoint)
		];
	
}

ECheckBoxState FMySplineMetadataDetails::OnIsCheckedIsOnPoint() const {
	ECheckBoxState res = GetIsStopPoint().GetValue();
	return res;
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

void FMySplineMetadataDetails::OnSetIsStopPoint(ECheckBoxState NewValue)
{
	if (UMySplineMetadata* Metadata = GetMetadata())
	{
		const FScopedTransaction Transaction(LOCTEXT("SetIsStopPoint", "Set spline point is stop point"));

		for (int32 Index : SelectedKeys)
		{
			Metadata->PointParams[Index].IsStopPoint = NewValue == ECheckBoxState::Checked;
		}

		OnSetValues(*this);
	}
}

UMySplineMetadata* FMySplineMetadataDetails::GetMetadata() const
{
	UMySplineMetadata* Metadata = SplineComp ? Cast<UMySplineMetadata>(SplineComp->GetSplinePointsMetadata()) : nullptr;
	return Metadata;
}

TOptional<ECheckBoxState> FMySplineMetadataDetails::GetIsStopPoint() const
{
	return IsStopPointValue.GetValue() ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

#undef LOCTEXT_NAMESPACE