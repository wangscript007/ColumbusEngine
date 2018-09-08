#pragma once

#include <Core/Types.h>
#include <Math/Vector3.h>

namespace Columbus
{

	class OpenGL
	{
	private:
		static int32 MaxCombinedTextureImageUnits;
		static int32 MaxCubemapTextureSize;
		static int32 MaxFragmentUniformVectors;
		static int32 MaxRenderbufferSize;
		static int32 MaxTextureImageUnits;
		static int32 MaxTextureSize;
		static int32 MaxVaryingVectors;
		static int32 MaxVertexAttribs;
		static int32 MaxVertexTextureImageUnits;
		static int32 MaxVertexUniformVectors;
		static int32 MaxViewportDims;
		static int32 MaxComputeWorkGroupInvocations;
		static int32 MaxComputeSharedMemorySize;
		static iVector3 MaxComputeWorkGroupSize;
		static iVector3 MaxComputeWorkGroupCount;

		static bool bSupportsUniformBuffer;
		static bool bSupportsInstancing;
		static bool bSupportsTransformFeedback;
		static bool bSupportsShader;
		static bool bSupportsGeometryShader;
		static bool bSupportsTesselation;
		static bool bSupportsComputeShader;
		static bool bSupportsDirectStateAccess;
		static bool bSupportsProgramBinary;
		static bool bSupportsShaderStorageBuffer;
		static bool bSupportsShaderImageLoadStore;
		static bool bSupportsMultiBind;
		static bool bSupportsBaseInstance;
		static bool bSupportsDrawIndirect;
		static bool bSupportsMultiDrawIndirect;
	public:
		static void Init();

		static inline int32 GetMaxCombinedTextureImageUnits() { return MaxCombinedTextureImageUnits; }
		static inline int32 GetMaxCubemapTextureSize() { return MaxCubemapTextureSize; }
		static inline int32 GetMaxFragmentUniformVectors() { return MaxFragmentUniformVectors; }
		static inline int32 GetMaxRenderbufferSize() { return MaxRenderbufferSize; }
		static inline int32 GetMaxTextureImageUnits() { return MaxTextureImageUnits; }
		static inline int32 GetMaxTextureSize() { return MaxTextureSize; }
		static inline int32 GetMaxVaryingVectors() { return MaxVaryingVectors; }
		static inline int32 GetMaxVertexAttribs() { return MaxVertexAttribs; }
		static inline int32 GetMaxVertexTextureImageUnits() { return MaxVertexTextureImageUnits; }
		static inline int32 GetMaxVertexUniformVectors() { return MaxVertexUniformVectors; }
		static inline int32 GetMaxViewportDims() { return MaxViewportDims; }
		static inline int32 GetMaxComputeWorkGroupInvocations() { return MaxComputeWorkGroupInvocations; }
		static inline int32 GetMaxComputeSharedMemorySize() { return MaxComputeSharedMemorySize; }
		static inline iVector3 GetMaxComputeWorkGroupSize() { return MaxComputeWorkGroupSize; }
		static inline iVector3 GetMaxComputeWorkGroupCount() { return MaxComputeWorkGroupCount; }

		static inline uint64 GetVideoMemorySize() { return 0; }
		static inline uint64 GetFreeVideoMemorySize() { return 0; }

		static inline bool SupportsUniformBuffer() { return bSupportsUniformBuffer; }
		static inline bool SupportsInstancing() { return bSupportsInstancing; }
		static inline bool SupportsTransformFeedback() { return bSupportsTransformFeedback; }
		static inline bool SupportsShader() { return bSupportsShader; }
		static inline bool SupportsGeometryShader() { return bSupportsGeometryShader; }
		static inline bool SupportsTesselation() { return bSupportsTesselation; }
		static inline bool SupportsComputeShader() { return bSupportsComputeShader; }
		static inline bool SupportsDirectStateAccess() { return bSupportsDirectStateAccess; }
		static inline bool SupportsProgramBinary() { return bSupportsProgramBinary; }
		static inline bool SupportsShaderStorageBuffer() { return bSupportsShaderStorageBuffer; }
		static inline bool SupportsShaderImageLoadStore() { return bSupportsShaderImageLoadStore; }
		static inline bool SupportsMultiBind() { return bSupportsMultiBind; }
		static inline bool SupportsBaseInstance() { return bSupportsBaseInstance; }
		static inline bool SupportsDrawIndirect() { return bSupportsDrawIndirect; }
		static inline bool SupportsMultiDrawIndirect() { return bSupportsMultiDrawIndirect; }
	};

}











