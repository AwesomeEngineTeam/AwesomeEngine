#pragma once

#ifndef AWESOME_SDK_RENDER_SYSTEs_GUARD
#define AWESOME_SDK_RENDER_SYSTEs_GUARD

#include <Awesome/Maths/Matrix3.hpp>
#include <Awesome/Maths/Matrix4.hpp>

#include <Awesome/Renderer/RenderQueue.hpp>
#include <Awesome/Renderer/Memory/SubBuffer.hpp>
#include <Awesome/Renderer/Memory/UniformBuffer.hpp>
#include <Awesome/Renderer/Memory/UniformBufferDynamic.hpp>
#include <Awesome/Renderer/Memory/UniformBufferDynamicPool.hpp>
#include <Awesome/Renderer/Framegraph/Framegraph.hpp>
#include <Awesome/Renderer/Framegraph/FrameResourcesManager.hpp>
#include <Awesome/Renderer/ShaderBuilder/ShaderBuilder.hpp>
#include <Awesome/Renderer/Resource/FontAtlas.hpp>

#include <Awesome/SDK/Config.hpp>
#include <Awesome/SDK/Resources/ResourceManager.hpp>
#include <Awesome/SDK/Components/Render/Camera.hpp>
#include <Awesome/SDK/Components/Render/DirectionalLight.hpp>
#include <Awesome/SDK/Components/Render/MeshRenderer.hpp>
#include <Awesome/SDK/Components/Render/PointLight.hpp>
#include <Awesome/SDK/Components/Render/Skybox.hpp>
#include <Awesome/SDK/Components/Render/Text2D.hpp>
#include <Awesome/SDK/Components/Render/Billboard.hpp>

#include <Awesome/Renderer/Control/RenderWindow.hpp>

//#include <Awesome/Physics/Scene/Scene.hpp>
#define MAX_POINT_LIGHTS_COUNT (10)

namespace aw
{
	namespace sdk
	{
		/*#ifdef WITH_EDITOR
		class Landmark : public Component
		{
			friend class RenderSystem;

		protected:
			uint32 m_meshIndex{ 0 };
			uint32 m_materialIndex{ 0 };

			UniformBufferDynamicBlock m_dynamicBlock{};
			vk::DescriptorSet m_geometryDset{};

			inline Landmark(uint32 typeId, const std::string& name = "Landmark") : Component(nullptr, typeId, name) {}
			inline Landmark() : Landmark(uint32(ComponentID::Landmark), "Landmark") {}
		};

		class LandmarkX : public Landmark
		{
		public:
			inline LandmarkX() : Landmark( uint32(ComponentID::LandmarkX), "LandmarkX") {}
		};

		class LandmarkY : public Landmark
		{

		};

		class LandmarkZ : public Landmark
		{

		};
		#endif*/

		enum class PrimitivesIndex : uint8
		{
			Quad = 0,
			DebugQuad = 1,
			Cube = 2,
			DebugCube = 3,
			Sphere = 4,
			DebugSphere = 5,
			Capsule = 6,
			DebugCapsule = 7,
			Cylinder = 8,
			Cone = 9,
			Hemiphere = 10,
			Skybox = 11,
			Line = 12,
			Torus = 13,
			COUNT
		};

		enum class DefaultTextureIndex : uint8
		{
			NoTexture = 0,
			Black,
			White,
			Red,
			Green,
			Blue,
			BlackTransparent,
			WhiteTransparent,
			YellowHalfTransparent,
			CyanHalfTransparent,
			MagentaHalfTransparent,
			COUNT
		};

		/*!
		 *	\struct		ModelUBO
		 *	\attention	componentId is the id of th component being drawn + 1.
		 *				0 is no id, not the first id.
		 */
		struct ModelUBO
		{
			Matrix4 mvp{};
			Matrix4 model{};
			uint32 componentId{ 0 };
			uint32 actorId{ 0 };
			Color4f baseColorFactor{ Color4f::White() };
			Color4f emissiveFactor{ Color4f::Black() };
			float metallicFactor{ 1.0f };
			float roughnessFactor{ 1.0f };
			Vector2 scaleUV{ 1.0f, 1.0f };
			Vector2 offsetUV{ 0.0f, 0.0f };
		};

		struct ViewUBO
		{
			Matrix4 viewMatrix{};
			Vector3 viewPos{};
		};

		struct PointLightUBO
		{
			Color4f color{ Color4f::White() };
			Vector3 position{};
			float intensity{ 16.0f };
		};

		struct DirectionalLightUBO
		{
			Color4f color{ Color4f::White() };
			Vector3 direction{};
			float intensity{ 16.0f };
		};

		struct LightScatteringUBO
		{
			float exposure{ 0.0034f };
			float decay{ 1.0f };
			float density{ 0.84f };
			float weight{ 5.65f };

			//float exposure{ 0.92f };
			//float decay{ 0.94815f };
			//float density{ 0.926f };
			//float weight{ 0.58767f };
			Vector2 screenLightPosition{};
		};

		class RenderSystem
		{
		protected:
			struct InstanceMaterialIndices
			{
				uint32 material{ 0 };
				uint32 mesh{ 0 };
				uint32 pass{ 0 };

				bool operator==(const InstanceMaterialIndices& o) const
				{
					return (material == o.material) && (mesh == o.mesh) && (pass == o.pass);
				}
			};

			struct InstanceMaterialIndicesHash
			{
				std::size_t operator()(const InstanceMaterialIndices& i) const noexcept
				{
					static_assert(sizeof(std::size_t) == sizeof(uint64), "std::size_t must be 64 bits long");

					std::size_t res = i.material;
					res = i.mesh | (res << 32u);

					std::size_t mask{ 0 };
					std::size_t k = i.pass;

					for (uint32 j{ 0 }; j < 32; ++j)
						mask |= (k & (1u << j)) << j;

					res ^= mask | (mask << 1u);

					return res;
				}
			};

			static DescriptorSetLayoutInfo s_modelUBDLayoutInfo;
			static vk::UniqueDescriptorSetLayout s_modelUBDLayout;

			struct ModelUBDGroup
			{
				std::unique_ptr<UniformBufferDynamicPool> dynamicPool{ nullptr };
				DescriptorPool descriptorPool{};
				vk::UniqueDescriptorSet descriptorSet{};
			};

			//ph::Scene s_physicalScene{ ph::SceneDescriptor({}, true) };

			static std::vector<ModelUBDGroup> s_modelUBDGroups;

			static std::vector<std::unique_ptr<MeshRenderer>> s_debug3DLines;

			static std::vector<MeshRenderer*> s_meshRenderers;
			static std::vector<Camera*> s_cameras;
			static std::vector<PointLight*> s_pointLights;
			static std::vector<DirectionalLight*> s_directionalLights;
			static std::vector<Text2D*> s_text2Ds;
			static std::vector<Billboard*> s_billboards;

			static std::vector<Component*> s_compsToInstantiate;

			static std::mutex s_instMutex;

			#ifdef WITH_EDITOR
		public:
			struct Gizmo2D
			{
				std::unique_ptr<Billboard> billboard{};
				uint32 componentId{ 0 };
				uint32 actorId{ 0 };
				bool isActive{ true };
			};

			struct Gizmo3D
			{
				std::unique_ptr<MeshRenderer> meshRenderer{};
				uint32 componentId{ 0 };
				uint32 actorId{ 0 };
				bool isActive{ true };
			};

		protected:
			static std::vector<Gizmo2D> s_gizmos2D;
			static std::vector<Gizmo3D> s_gizmos3D;

			static uint32 s_redUnlitMaterialIndex;
			static uint32 s_greenUnlitMaterialIndex;
			static uint32 s_blueUnlitMaterialIndex;

			static uint32 s_redLandmarkMaterialIndex;
			static uint32 s_greenLandmarkMaterialIndex;
			static uint32 s_blueLandmarkMaterialIndex;
			static uint32 s_whiteLandmarkMaterialIndex;
			static uint32 s_yellowLandmarkMaterialIndex;
			static uint32 s_cyanLandmarkMaterialIndex;
			static uint32 s_magentaLandmarkMaterialIndex;

			//std::vector<std::unique_ptr<MeshRenderer>> m_immediateMeshRenderers;
			//std::vector<std::unique_ptr<Text2D>> m_immediateText2Ds;
			//std::vector<std::unique_ptr<Billboard>> m_immediateBillboards;
			#endif

			static uint32 s_firstLightScatteringObstacleMaterialIndex;
			static uint32 s_firstLightScatteringWhiteLightMaterialIndex;
			static uint32 s_lastLightScatteringMaterialIndex;
			static uint32 s_forwardWhiteLightMaterialIndex;
			static uint32 s_selectedOutlineMaterialIndex;
			//uint32 m_hBlurMaterialIndex;
			//uint32 m_vBlurMaterialIndex;

			static uint32 s_width;
			static uint32 s_height;

			static uint32 s_textMaterialIndex;
			static FontAtlas s_fontAtlas;

			static std::vector<Material> s_materials;
			static std::vector<Mesh*> s_meshes;
			static std::vector<Pass> s_passes;
			static std::vector<SampledTextureHandle> s_textures;

			static uint64 s_modelUBDSize;
			static uint64 s_modelUBDCountPerPool;

			static std::unordered_map<InstanceMaterialIndices, ShaderHandle, InstanceMaterialIndicesHash> s_instanceMaterials;

			static Mesh s_sceenQuad;
			static Mesh s_3dQuad;

			static Skybox* s_skybox;

			static Framegraph s_framegraph;
			static FrameResourcesManager s_frameResourcesManager;
			static RenderQueue s_opaqueRQ;
			static RenderQueue s_transparentRQ;
			static RenderQueue s_lightScatteringEmitterRQ;
			static RenderQueue s_lightScatteringObstacleRQ;

			static Pass s_forwardPass;
			static Pass s_transparencyPass;
			static Pass s_firstLightScatteringPass;
			static Pass s_lastLightScatteringPass;

			static uint32 s_forwardPassIndex;
			static uint32 s_transparencyPassIndex;
			static uint32 s_firstLightScatteringPassIndex;
			static uint32 s_lastLightScatteringPassIndex;

			static uint32 s_presentImageIndex;
			static uint32 s_firstLightScatterImageIndex;

			#ifdef WITH_EDITOR
			static uint32 s_componentIdImageIndex;
			static SubBuffer s_componentIdSubBuffer;
			#endif

			static UniformBuffer s_viewUBO;
			static UniformBuffer s_lightsUBO;
			static UniformBuffer s_lightScatteringUBO;

			static vk::UniqueCommandBuffer s_copyCommandBuffer;

			static ResourceManager s_resourceManager;

			static constexpr const uint64 s_maxPointLights{MAX_POINT_LIGHTS_COUNT};
			static constexpr const uint64 s_maxDirectionalLights{MAX_POINT_LIGHTS_COUNT};

		public:
			static LightScatteringUBO LightScatteringUBO;

			RenderSystem() = delete;
			~RenderSystem() = default;

			static AWESOME_SDK_API void Init();

			static AWESOME_SDK_API void Start(RenderWindow& window, Camera& editorCamera, std::atomic_bool& shouldUpdate, const std::atomic_bool& standardMode, const std::atomic_bool& landmarkControlAbsolute, const std::atomic_int& currentTool, Component** ptrToSelected = nullptr);

			//TODO Get rid of this
			static AWESOME_SDK_API void UnInit();

		private:
			static AWESOME_SDK_API void InstantiatePendingComponents();
			static AWESOME_SDK_API void GarbageCollect();

			static AWESOME_SDK_API void LoadPrimitives();
			static AWESOME_SDK_API void LoadDefaultTextures();

			static AWESOME_SDK_API ShaderHandle GetOrCreateInstanceMaterialShader(InstanceMaterialIndices);

			static AWESOME_SDK_API void UpdateCopyCommandBuffer();

			static inline void InitModelUBD(uint64 size, uint64 countPerPool);
			static inline void ClearModelUBD();

			static inline void ReserveModelUBD(MeshRenderer*);
			static inline void ReserveModelUBD(Text2D*);
			static inline void ReserveModelUBD(Billboard*);

			static AWESOME_RENDERER_API void CopyModelUBD(vk::CommandBuffer);

			static AWESOME_RENDERER_API ModelUBDGroup& FindOrCreateSuitableGroupModelUBD();

			static AWESOME_SDK_API bool Register(MeshRenderer*);
			static AWESOME_SDK_API bool Register(Skybox*);
			static AWESOME_SDK_API bool Register(Camera*);
			static AWESOME_SDK_API bool Register(Light*);
			static AWESOME_SDK_API bool Register(PointLight*);
			static AWESOME_SDK_API bool Register(DirectionalLight*);
			static AWESOME_SDK_API bool Register(Text2D*);
			static AWESOME_SDK_API bool Register(Billboard*);

		public:
			static AWESOME_SDK_API bool Register(Component*);

			//static AWESOME_SDK_API void Unregister(Component*);
			//static AWESOME_SDK_API void Unregister(MeshRenderer*);
			//static AWESOME_SDK_API void Unregister(Skybox*);
			//static AWESOME_SDK_API void Unregister(Camera*);
			//static AWESOME_SDK_API void Unregister(Light*);
			//static AWESOME_SDK_API void Unregister(PointLight*);
			//static AWESOME_SDK_API void Unregister(DirectionalLight*);
			//static AWESOME_SDK_API void Unregister(Text2D*);
			//static AWESOME_SDK_API void Unregister(Billboard*);

			static AWESOME_SDK_API void Update();

			#ifdef WITH_EDITOR
		private:
			static uint32 s_quadMeshIndex;
			static uint32 s_debugMaterialIndex;
			static uint32 s_lightGizmoMaterialIndex;
			static uint32 s_sunGizmoMaterialIndex;
			static uint32 s_cameraGizmoMaterialIndex;

			static uint32 s_landmarkForwardPassIndex;
			static Pass s_landmarkForwardPass;

			static Gizmo3D s_xLandmark;
			static Gizmo3D s_yLandmark;
			static Gizmo3D s_zLandmark;
			static Gizmo3D s_cLandmark;
			static Gizmo3D s_xyLandmark;
			static Gizmo3D s_yzLandmark;
			static Gizmo3D s_zxLandmark;

		public:
			static Camera* editorCamera;
			static AWESOME_SDK_API void UpdateInEditor(Component* selectedComponent, bool landmarkControlAbsolute, int currentTool); // 0 = translation, 1 = rotation, 2 = scale

			/*!
			 *	\return	pair of ids. first is component, second is actor.
			 */
			static AWESOME_SDK_API std::pair<uint32, uint32> GetIdsAtPixel(const MousePos&);

			static AWESOME_SDK_API void DrawInEditor(vk::CommandBuffer, Camera& editorCamera);

			static AWESOME_SDK_API bool RegisterImmediate(Gizmo2D);
			static AWESOME_SDK_API bool RegisterImmediate(Gizmo3D);
			#endif

			struct CameraViewInfo
			{
				Matrix4 projMat = Matrix4::Identity();
				Matrix4 viewMat = Matrix4::Identity();
				Matrix4 viewProjMat = Matrix4::Identity();
				Matrix4 textViewProjMat = Matrix4::Identity();
				Matrix4 billboardViewProjMat = Matrix4::Identity();
				Matrix4 billboardViewRotationMat = Matrix4::Identity();
				UniformTransform viewTr = UniformTransform::Identity();
			};

		private:
			static AWESOME_SDK_API void RecordMeshRenderer(MeshRenderer&, const CameraViewInfo&);
			static AWESOME_SDK_API void RecordDebug3DLine(MeshRenderer&, const CameraViewInfo&);
			static AWESOME_SDK_API void RecordText2D(Text2D&, const CameraViewInfo&);
			static AWESOME_SDK_API void RecordBillboard(Billboard&, const CameraViewInfo&);

		public:
			static AWESOME_SDK_API void Draw(vk::CommandBuffer);

			static AWESOME_SDK_API void DrawDebug3DLine(const Vector3& begin, const Vector3& end); // Use this function, I dare you! I double dare you!

			static AWESOME_SDK_API void Resize(uint32 width, uint32 height);

			static inline const Camera& GetFirstCamera();
			//static inline Camera& GetFirstCamera();

			static inline Framegraph& GetFramegraph();
			static inline RenderQueue& GetOpaqueRenderQueue();
			static inline ResourceManager& GetResourceManager();

			static AWESOME_SDK_API uint32 RegisterMaterial(Material&&);
			static inline Material& GetMaterial(uint32);

			static AWESOME_SDK_API uint32 RegisterMesh(Mesh*);
			static inline Mesh* GetMesh(uint32);
			static inline uint32 GetMeshIndex(Mesh*);

			static inline uint32 RegisterTexture(SampledTextureHandle);
			static inline void UnRegisterTexture(uint32);
			static inline SampledTextureHandle GetTexture(uint32);

			static AWESOME_SDK_API SampledTextureHandle GetPresentImage();
		};
	}

	/*#ifdef WITH_EDITOR
	template <>
	class TypeInfo<sdk::Landmark>
	{
	public:
		static const uint32 id = uint32(sdk::ComponentID::Landmark);
	};

	template <>
	class TypeInfo<sdk::LandmarkX>
	{
	public:
		static const uint32 id = uint32(sdk::ComponentID::LandmarkX);
	};

	template <>
	class TypeInfo<sdk::LandmarkY>
	{
	public:
		static const uint32 id = uint32(sdk::ComponentID::LandmarkY);
	};

	template <>
	class TypeInfo<sdk::LandmarkZ>
	{
	public:
		static const uint32 id = uint32(sdk::ComponentID::LandmarkZ);
	};
	#endif*/
}

#include <Awesome/SDK/System/RenderSystem.inl>

#endif
