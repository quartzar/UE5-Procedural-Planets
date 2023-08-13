﻿// Copyright TriAxis Games, L.L.C. All Rights Reserved.

#pragma once

#include "RealtimeMeshCore.h"
#include "RealtimeMeshConfig.generated.h"

#define LOCTEXT_NAMESPACE "RealtimeMesh"


USTRUCT(BlueprintType, meta=(
	HasNativeMake="RealtimeMeshComponent.RealtimeMeshBlueprintFunctionLibrary.MakeStreamRange"))
struct REALTIMEMESHCOMPONENT_API FRealtimeMeshStreamRange
{
	GENERATED_BODY()
public:
	static const FRealtimeMeshStreamRange Empty;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RealtimeMesh|Streams")
	FInt32Range Vertices;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RealtimeMesh|Streams")
	FInt32Range Indices;
	
	FRealtimeMeshStreamRange() = default;

	FRealtimeMeshStreamRange(const FInt32Range& VerticesRange, const FInt32Range& IndicesRange)
		: Vertices(VerticesRange), Indices(IndicesRange)
	{
	}

	FRealtimeMeshStreamRange(uint32 VerticesLower, uint32 VerticesUpper, uint32 IndicesLower, uint32 IndicesUpper)
		: Vertices(VerticesLower, VerticesUpper), Indices(IndicesLower, IndicesUpper)
	{
	}

	FRealtimeMeshStreamRange(const FInt32RangeBound& VerticesLower, const FInt32RangeBound& VerticesUpper,
	                            const FInt32RangeBound& IndicesLower, const FInt32RangeBound& IndicesUpper)
		: Vertices(VerticesLower, VerticesUpper), Indices(IndicesLower, IndicesUpper)
	{
	}

	bool Overlaps(const FRealtimeMeshStreamRange& Other) const
	{
		return Vertices.Overlaps(Other.Vertices) && Indices.Overlaps(Other.Indices);
	}

	FRealtimeMeshStreamRange Intersection(const FRealtimeMeshStreamRange& Other) const
	{
		return FRealtimeMeshStreamRange(
			FInt32Range::Intersection(Vertices, Other.Vertices),
			FInt32Range::Intersection(Indices, Other.Indices));
	}

	bool Contains(const FRealtimeMeshStreamRange& Other) const
	{
		return Vertices.Contains(Other.Vertices) && Indices.Contains(Other.Indices);
	}

	int32 NumPrimitives(int32 VertexCountPerPrimitive) const
	{
		if (!Indices.HasLowerBound() || !Indices.HasUpperBound() || Indices.IsDegenerate())
		{
			return 0;
		}
		return (GetMaxIndex() - GetMinIndex() + 1) / VertexCountPerPrimitive;
	}

	int32 NumVertices() const
	{
		if (!Vertices.HasLowerBound() || !Vertices.HasUpperBound() || Vertices.IsDegenerate())
		{
			return 0;
		}
		return GetMaxVertex() - GetMinVertex() + 1;
	}

	int32 GetMinVertex() const { return Vertices.GetLowerBound().IsExclusive() ? Vertices.GetLowerBoundValue() + 1 : Vertices.GetLowerBoundValue(); }
	int32 GetMaxVertex() const { return Vertices.GetUpperBound().IsExclusive() ? Vertices.GetUpperBoundValue() - 1 : Vertices.GetUpperBoundValue(); }

	int32 GetMinIndex() const { return Indices.GetLowerBound().IsExclusive() ? Indices.GetLowerBoundValue() + 1 : Indices.GetLowerBoundValue(); }
	int32 GetMaxIndex() const { return Indices.GetUpperBound().IsExclusive() ? Indices.GetUpperBoundValue() - 1 : Indices.GetUpperBoundValue(); }


	bool operator==(const FRealtimeMeshStreamRange& Other) const
	{
		return Vertices == Other.Vertices && Indices == Other.Indices;
	}
	bool operator!=(const FRealtimeMeshStreamRange& Other) const
	{
		return Vertices != Other.Vertices || Indices != Other.Indices;
	}

	FRealtimeMeshStreamRange Hull(const FRealtimeMeshStreamRange& Other) const
	{
		return FRealtimeMeshStreamRange(FInt32Range::Hull(Vertices, Other.Vertices), FInt32Range::Hull(Indices, Other.Indices));
	}

	friend FArchive& operator<<(FArchive& Ar, FRealtimeMeshStreamRange& Range);
};


USTRUCT(BlueprintType)
struct REALTIMEMESHCOMPONENT_API FRealtimeMeshMaterialSlot
{
	GENERATED_BODY()
public:

	UPROPERTY()
	FName SlotName;
	
	UPROPERTY()
	UMaterialInterface* Material;

	FRealtimeMeshMaterialSlot() : SlotName(NAME_None), Material(nullptr) { }

	FRealtimeMeshMaterialSlot(const FName& InSlotName, UMaterialInterface* InMaterial)
		: SlotName(InSlotName), Material(InMaterial) { }
};


/**
*	Struct used to specify a tangent vector for a vertex
*	The Y tangent is computed from the cross product of the vertex normal (Tangent Z) and the TangentX member.
*/
USTRUCT(BlueprintType)
struct REALTIMEMESHCOMPONENT_API FRealtimeMeshTangent
{
	GENERATED_BODY()
public:

	/** Direction of X tangent for this vertex */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RealtimeMesh|Tangent")
	FVector TangentX;

	/** Bool that indicates whether we should flip the Y tangent when we compute it using cross product */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RealtimeMesh|Tangent")
	bool bFlipTangentY;

	FRealtimeMeshTangent()
		: TangentX(FVector::XAxisVector)
		, bFlipTangentY(false)
	{}

	FRealtimeMeshTangent(float X, float Y, float Z)
		: TangentX(X, Y, Z)
		, bFlipTangentY(false)
	{}

	FRealtimeMeshTangent(FVector InTangentX, bool bInFlipTangentY)
		: TangentX(InTangentX)
		, bFlipTangentY(bInFlipTangentY)
	{}

	FRealtimeMeshTangent(FVector InTangentX, FVector InTangentY, FVector InTangentZ)
		: TangentX(InTangentX)
		, bFlipTangentY(GetBasisDeterminantSign(InTangentX, InTangentY, InTangentZ) < 0)
	{}
	
	friend FArchive& operator<<(FArchive& Ar, FRealtimeMeshTangent& Tangent);
};


/* The rendering path to use for this section.
 * Static has lower overhead but requires a proxy recreation on change for all components
 * Dynamic has slightly higher overhead but allows for more efficient section updates
 */
UENUM(BlueprintType)
enum class ERealtimeMeshSectionDrawType : uint8
{
	Static,
	Dynamic,
};


USTRUCT(BlueprintType)
struct REALTIMEMESHCOMPONENT_API FRealtimeMeshSectionConfig
{
	GENERATED_BODY()
public:
	FRealtimeMeshSectionConfig(ERealtimeMeshSectionDrawType InDrawType = ERealtimeMeshSectionDrawType::Static, int32 InMaterialSlot = 0)
		: MaterialSlot(InMaterialSlot)
		, DrawType(InDrawType)
		, bIsVisible(true)
		, bCastsShadow(true)
		, bIsMainPassRenderable(true)
		, bForceOpaque(false)
	{
		
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RealtimeMesh|Section|Config")
	int32 MaterialSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RealtimeMesh|Section|Config")
	ERealtimeMeshSectionDrawType DrawType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RealtimeMesh|Section|Config")
	bool bIsVisible;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RealtimeMesh|Section|Config")
	bool bCastsShadow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RealtimeMesh|Section|Config", AdvancedDisplay)
	bool bIsMainPassRenderable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RealtimeMesh|Section|Config", AdvancedDisplay)
	bool bForceOpaque;
	
	friend FArchive& operator<<(FArchive& Ar, FRealtimeMeshSectionConfig& Config);
};


USTRUCT(BlueprintType)
struct REALTIMEMESHCOMPONENT_API FRealtimeMeshLODConfig
{
	GENERATED_BODY()
public:
	FRealtimeMeshLODConfig(float InScreenSize = 0.0f)
		: bIsVisible(true)
		, ScreenSize(InScreenSize)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RealtimeMesh|LOD|Config")
	bool bIsVisible;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RealtimeMesh|LOD|Config")
	float ScreenSize;
		
	friend FArchive& operator<<(FArchive& Ar, FRealtimeMeshLODConfig& Config);
};



USTRUCT(BlueprintType)
struct REALTIMEMESHCOMPONENT_API FRealtimeMeshConfig
{
	GENERATED_BODY()
public:
	FRealtimeMeshConfig()
		: ForcedLOD(INDEX_NONE)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RealtimeMesh|LOD|Config")
	int32 ForcedLOD;
		
	friend FArchive& operator<<(FArchive& Ar, FRealtimeMeshConfig& Config);
};

namespace RealtimeMesh
{
	struct FRealtimeMeshKeyHelpers;
}

USTRUCT(BlueprintType, meta=(
	HasNativeBreak="RealtimeMeshComponent.RealtimeMeshBlueprintFunctionLibrary.BreakLODKey",
	HasNativeMake="RealtimeMeshComponent.RealtimeMeshBlueprintFunctionLibrary.MakeLODKey"))
struct REALTIMEMESHCOMPONENT_API FRealtimeMeshLODKey
{
	GENERATED_BODY()
	friend struct RealtimeMesh::FRealtimeMeshKeyHelpers;
protected:
	UPROPERTY(VisibleAnywhere, Category="RealtimeMesh|Key")
	uint8 LODIndex;

public:
	FRealtimeMeshLODKey() : LODIndex(INDEX_NONE) { }
	FRealtimeMeshLODKey(uint8 InLODIndex) : LODIndex(InLODIndex) { }

	bool IsValid() const { return LODIndex != (uint8)INDEX_NONE; }

	bool operator==(const FRealtimeMeshLODKey& Other) const { return LODIndex == Other.LODIndex; }
	bool operator!=(const FRealtimeMeshLODKey& Other) const { return LODIndex != Other.LODIndex; }

	FString ToString() const { return TEXT("[LODKey:") + FString::FromInt(LODIndex) + TEXT("]"); }
	
	friend FArchive& operator<<(FArchive& Ar, FRealtimeMeshLODKey& Key);
};

USTRUCT(BlueprintType, meta=(HasNativeMake="RealtimeMeshComponent.RealtimeMeshBlueprintFunctionLibrary.MakeSectionGroupKey"))
struct REALTIMEMESHCOMPONENT_API FRealtimeMeshSectionGroupKey : public FRealtimeMeshLODKey
{
	GENERATED_BODY()
	friend struct RealtimeMesh::FRealtimeMeshKeyHelpers;
protected:
	UPROPERTY(VisibleAnywhere, Category="RealtimeMesh|Key")
	uint8 SectionGroupIndex;

public:
	FRealtimeMeshSectionGroupKey()
		: SectionGroupIndex(INDEX_NONE) { }
	FRealtimeMeshSectionGroupKey(uint8 InLODIndex, uint8 InSectionGroupIndex)
		: FRealtimeMeshLODKey(InLODIndex), SectionGroupIndex(InSectionGroupIndex) { }
	FRealtimeMeshSectionGroupKey(FRealtimeMeshLODKey InLODKey, uint8 InSectionGroupIndex)
		: FRealtimeMeshLODKey(InLODKey), SectionGroupIndex(InSectionGroupIndex) { }

	bool IsValid() const { return LODIndex != (uint8)INDEX_NONE && SectionGroupIndex != (uint8)INDEX_NONE; }

	FRealtimeMeshLODKey GetLODKey() const { return FRealtimeMeshLODKey(*this); }
	bool IsPartOf(const FRealtimeMeshLODKey& InLOD) const { return GetLODKey() == InLOD; }
	
	bool operator==(const FRealtimeMeshSectionGroupKey& Other) const
	{
		return LODIndex == Other.LODIndex && SectionGroupIndex == Other.SectionGroupIndex;
	}

	bool operator!=(const FRealtimeMeshSectionGroupKey& Other) const
	{
		return LODIndex != Other.LODIndex || SectionGroupIndex != Other.SectionGroupIndex;
	}

	FString ToString() const { return TEXT("[LODKey:") + FString::FromInt(LODIndex) + TEXT(", SectionGroupKey:") + FString::FromInt(SectionGroupIndex) + TEXT("]"); }
	
	friend FArchive& operator<<(FArchive& Ar, FRealtimeMeshSectionGroupKey& Key);
};

USTRUCT(BlueprintType, meta=(HasNativeMake="RealtimeMeshComponent.RealtimeMeshBlueprintFunctionLibrary.MakeSectionKey"))
struct REALTIMEMESHCOMPONENT_API FRealtimeMeshSectionKey : public FRealtimeMeshSectionGroupKey
{
	GENERATED_BODY()
	friend struct RealtimeMesh::FRealtimeMeshKeyHelpers;
protected:
	UPROPERTY(VisibleAnywhere, Category="RealtimeMesh|Key")
	uint16 SectionIndex;
public:
	FRealtimeMeshSectionKey()
		: SectionIndex(INDEX_NONE) { }
	FRealtimeMeshSectionKey(uint8 InLODIndex, uint8 InSectionGroupIndex, uint16 InSectionKey)
		: FRealtimeMeshSectionGroupKey(InLODIndex, InSectionGroupIndex), SectionIndex(InSectionKey) { }
	FRealtimeMeshSectionKey(FRealtimeMeshSectionGroupKey InSectionGroupKey, uint16 InSectionKey)
		: FRealtimeMeshSectionGroupKey(InSectionGroupKey), SectionIndex(InSectionKey) { }

	bool IsValid() const { return LODIndex != (uint8)INDEX_NONE && SectionGroupIndex != (uint8)INDEX_NONE && SectionIndex != (uint16)INDEX_NONE; }

	FRealtimeMeshSectionGroupKey GetSectionGroupKey() const { return FRealtimeMeshSectionGroupKey(*this); }
	bool IsPartOf(const FRealtimeMeshSectionGroupKey& InSectionGroup) const { return GetSectionGroupKey() == InSectionGroup; }
	
	bool operator==(const FRealtimeMeshSectionKey& Other) const
	{
		return LODIndex == Other.LODIndex && SectionGroupIndex == Other.SectionGroupIndex && SectionIndex == Other.SectionIndex;;
	}

	bool operator!=(const FRealtimeMeshSectionKey& Other) const
	{
		return LODIndex != Other.LODIndex || SectionGroupIndex != Other.SectionGroupIndex || SectionIndex != Other.SectionIndex;
	}

	FString ToString() const { return TEXT("[LODKey:") + FString::FromInt(LODIndex) + TEXT(", SectionGroupKey:") + FString::FromInt(SectionGroupIndex) + TEXT(", SectionKey:") + FString::FromInt(SectionIndex) + TEXT("]"); }

	friend FArchive& operator<<(FArchive& Ar, FRealtimeMeshSectionKey& Key);
};


namespace RealtimeMesh
{
	
	struct FRealtimeMeshKeyHelpers
	{
		static uint32 GetLODIndex(const FRealtimeMeshLODKey& LODKey) { return LODKey.LODIndex; }
		static uint32 GetSectionGroupIndex(const FRealtimeMeshSectionGroupKey& SectionGroupKey) { return SectionGroupKey.SectionGroupIndex; }
		static uint32 GetSectionIndex(const FRealtimeMeshSectionKey& SectionKey) { return SectionKey.SectionIndex; }
	};

	class REALTIMEMESHCOMPONENT_API FRealtimeMeshClassFactory : public TSharedFromThis<FRealtimeMeshClassFactory>
	{
	public:
		virtual ~FRealtimeMeshClassFactory() { }
		
		virtual FRealtimeMeshVertexFactoryRef CreateVertexFactory(ERHIFeatureLevel::Type InFeatureLevel) const;
		virtual FRealtimeMeshSectionProxyRef CreateSectionProxy(const FRealtimeMeshProxyRef& InProxy, FRealtimeMeshSectionKey InKey, const FRealtimeMeshSectionProxyInitializationParametersRef& InitParams) const;
		virtual FRealtimeMeshSectionGroupProxyRef CreateSectionGroupProxy(const FRealtimeMeshProxyRef& InProxy, FRealtimeMeshSectionGroupKey InKey, const FRealtimeMeshSectionGroupProxyInitializationParametersRef& InitParams) const;
		virtual FRealtimeMeshLODProxyRef CreateLODProxy(const FRealtimeMeshProxyRef& InProxy, FRealtimeMeshLODKey InKey, const FRealtimeMeshLODProxyInitializationParametersRef& InitParams) const;
		virtual FRealtimeMeshProxyRef CreateRealtimeMeshProxy(const TSharedRef<FRealtimeMesh>& InMesh) const;

		virtual FRealtimeMeshSectionDataRef CreateSection(const FRealtimeMeshRef& InMesh, FRealtimeMeshSectionKey InKey, const FRealtimeMeshSectionConfig& InConfig, const FRealtimeMeshStreamRange& InStreamRange) const;
		virtual FRealtimeMeshSectionGroupRef CreateSectionGroup(const FRealtimeMeshRef& InMesh, FRealtimeMeshSectionGroupKey InKey) const;
		virtual FRealtimeMeshLODDataRef CreateLOD(const FRealtimeMeshRef& InMesh, FRealtimeMeshLODKey InKey, const FRealtimeMeshLODConfig& InConfig) const;

		virtual FRealtimeMeshRef CreateRealtimeMesh() const { check(false && "Cannot create abstract FRealtimeMesh"); return MakeShareable(static_cast<FRealtimeMesh*>(nullptr)); }
	};
	using FRealtimeMeshClassFactoryPtr = TSharedPtr<const FRealtimeMeshClassFactory, ESPMode::ThreadSafe>;
	using FRealtimeMeshClassFactoryRef = TSharedRef<const FRealtimeMeshClassFactory, ESPMode::ThreadSafe>;
}


#undef LOCTEXT_NAMESPACE
