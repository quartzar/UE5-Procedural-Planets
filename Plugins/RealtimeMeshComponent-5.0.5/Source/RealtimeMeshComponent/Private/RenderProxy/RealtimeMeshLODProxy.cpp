﻿// Copyright TriAxis Games, L.L.C. All Rights Reserved.

#include "RenderProxy/RealtimeMeshLODProxy.h"
#include "RenderProxy/RealtimeMeshSectionGroupProxy.h"
#include "RenderProxy/RealtimeMeshSectionProxy.h"

namespace RealtimeMesh
{
	FRealtimeMeshLODProxy::FRealtimeMeshLODProxy(const FRealtimeMeshClassFactoryRef& InClassFactory, const FRealtimeMeshProxyRef& InProxy,
					FRealtimeMeshLODKey InKey, const FRealtimeMeshLODProxyInitializationParametersRef& InInitParams)
		: ClassFactory(InClassFactory)
		, ProxyWeak(InProxy)
		, Key(InKey)
		, Config(InInitParams->Config)
		, bIsStateDirty(true)
	{		
		// Setup all the buffer sets
		for (TSparseArray<FRealtimeMeshSectionGroupProxyInitializationParametersRef>::TConstIterator It(InInitParams->SectionGroups); It; ++It)
		{
			SectionGroups.Insert(It.GetIndex(), ClassFactory->CreateSectionGroupProxy(InProxy, FRealtimeMeshSectionGroupKey(Key, It.GetIndex()), *It));
		}
	}

	FRealtimeMeshLODProxy::~FRealtimeMeshLODProxy()
	{
		check(IsInRenderingThread());
	}

	FRealtimeMeshSectionGroupProxyPtr FRealtimeMeshLODProxy::GetSectionGroup(FRealtimeMeshSectionGroupKey SectionKey) const
	{ 
		if (SectionKey.IsPartOf(Key) && SectionGroups.IsValidIndex(FRealtimeMeshKeyHelpers::GetSectionGroupIndex(SectionKey)))
		{
			return SectionGroups[FRealtimeMeshKeyHelpers::GetSectionGroupIndex(SectionKey)];
		}
		return FRealtimeMeshSectionGroupProxyPtr();
	}

	void FRealtimeMeshLODProxy::UpdateConfig(const FRealtimeMeshLODConfig& NewConfig)
	{
		Config = NewConfig;
		MarkStateDirty();
	}

	void FRealtimeMeshLODProxy::CreateSectionGroup(FRealtimeMeshSectionGroupKey SectionGroupKey,
		const FRealtimeMeshSectionGroupProxyInitializationParametersRef& InitParams)
	{
		check(SectionGroupKey.IsPartOf(Key));
		check(!SectionGroups.IsValidIndex(FRealtimeMeshKeyHelpers::GetSectionGroupIndex(SectionGroupKey)));

		const int32 SectionGroupIndex = FRealtimeMeshKeyHelpers::GetSectionGroupIndex(SectionGroupKey);
		SectionGroups.Insert(SectionGroupIndex, ClassFactory->CreateSectionGroupProxy(ProxyWeak.Pin().ToSharedRef(),
			FRealtimeMeshSectionGroupKey(Key, SectionGroupIndex), InitParams));

		MarkStateDirty();
	}

	void FRealtimeMeshLODProxy::RemoveSectionGroup(FRealtimeMeshSectionGroupKey SectionGroupKey)
	{
		check(SectionGroupKey.IsPartOf(Key));
		check(SectionGroups.IsValidIndex(FRealtimeMeshKeyHelpers::GetSectionGroupIndex(SectionGroupKey)));

		const int32 SectionGroupIndex = FRealtimeMeshKeyHelpers::GetSectionGroupIndex(SectionGroupKey);
		SectionGroups.RemoveAt(SectionGroupIndex);
		
		MarkStateDirty();
	}

	void FRealtimeMeshLODProxy::RemoveAllSectionGroups()
	{
		SectionGroups.Empty();

		MarkStateDirty();
	}

	void FRealtimeMeshLODProxy::CreateMeshBatches(const FRealtimeMeshBatchCreationParams& Params, const TMap<int32, TTuple<FMaterialRenderProxy*, bool>>& Materials,
		const FMaterialRenderProxy* WireframeMaterial, ERealtimeMeshSectionDrawType DrawType, bool bForceAllDynamic) const
	{
		const ERealtimeMeshDrawMask DrawTypeMask = bForceAllDynamic ? ERealtimeMeshDrawMask::DrawPassMask :
			DrawType == ERealtimeMeshSectionDrawType::Dynamic ? ERealtimeMeshDrawMask::DrawDynamic :	ERealtimeMeshDrawMask::DrawStatic;

		if (DrawMask.IsAnySet(DrawTypeMask))
		{
			for (const auto& SectionGroup : SectionGroups)			
			{
				if (SectionGroup->GetDrawMask().IsAnySet(DrawTypeMask))
				{
					SectionGroup->CreateMeshBatches(Params, Materials, WireframeMaterial, DrawType, bForceAllDynamic);
				}
			}
		}
	}
	
	void FRealtimeMeshLODProxy::MarkStateDirty()
	{
		bIsStateDirty = true;
	}

	bool FRealtimeMeshLODProxy::HandleUpdates()
	{
		// Handle all SectionGroup updates
		for (const auto& SectionGroup : SectionGroups)
		{
			bIsStateDirty |= SectionGroup->HandleUpdates();
		}
		
		if (!bIsStateDirty)
		{
			return false;
		}
		
		FRealtimeMeshDrawMask NewDrawMask;
		if (Config.bIsVisible && Config.ScreenSize >= 0)
		{
			for (TSparseArray<FRealtimeMeshSectionGroupProxyRef>::TIterator It(SectionGroups); It; ++It)
			{
				NewDrawMask |= (*It)->GetDrawMask();
			}
		}
		
		const bool bStateChanged = DrawMask != NewDrawMask;
		DrawMask = NewDrawMask;		
		bIsStateDirty = false;
		return bStateChanged;
	}
	
	void FRealtimeMeshLODProxy::Reset()
	{
		// Reset all the section groups
		SectionGroups.Empty();
		
		Config = FRealtimeMeshLODConfig();
		DrawMask = FRealtimeMeshDrawMask();
		bIsStateDirty = false;
	}
}
