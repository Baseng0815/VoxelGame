project('voxelgame', 'cpp', license: 'GPLv3')

src = [ 'src/Application.cpp', 'src/Configuration.cpp',
'src/Gui/IngameLayerGUI.cpp', 'src/Gui/InventoryLayout.cpp',
'src/Gui/Widget.cpp', 'src/Gui/Text.cpp', 'src/Gui/Rectangle.cpp',
'src/Gui/Layout.cpp', 'src/Gui/DebugLayout.cpp', 'src/Gui/ItemWidget.cpp',
'src/Gui/MainMenuLayout.cpp', 'src/Gui/UiMargin.cpp', 'src/Gui/ItemLayout.cpp',
'src/Gui/Image.cpp', 'src/Gui/UiConstraints.cpp', 'src/Gui/GUI.cpp',
'src/Gui/HotbarLayout.cpp', 'src/Resources/ResourceManager.cpp',
'src/Resources/Font.cpp', 'src/Resources/Geometry.cpp',
'src/Resources/TextureAtlas.cpp', 'src/Resources/Material.cpp',
'src/Resources/Shader.cpp', 'src/Resources/Texture.cpp', 'src/Utility.cpp',
'src/Systems/ChunkUpdateSystem.cpp', 'src/Systems/CloudSystem.cpp',
'src/Systems/DayNightSystem.cpp', 'src/Systems/PhysicsSystem.cpp',
'src/Systems/PlayerMovementSystem.cpp', 'src/Systems/ChunkCreateSystem.cpp',
'src/Systems/InputSystem.cpp', 'src/Systems/CameraSystem.cpp',
'src/Systems/MeshRenderSystem.cpp', 'src/Systems/ItemSystem.cpp',
'src/Systems/SkyboxSystem.cpp', 'src/Systems/DebugRenderSystem.cpp',
'src/Systems/CollisionSystem.cpp', 'src/Systems/System.cpp',
'src/Components/CameraComponent.cpp', 'src/Components/RigidBodyComponent.cpp',
'src/Components/TransformationComponent.cpp',
'src/Components/ChunkComponent.cpp',
'src/Components/BlockSelectionComponent.cpp',
'src/Components/CollisionComponent.cpp', 'src/Math/Ray.cpp',
'src/Math/Plane.cpp', 'src/Math/Face.cpp', 'src/Math/Cuboid.cpp',
'src/MainMenuLayer.cpp', 'src/Collision.cpp', 'src/GameLayer.cpp',
'src/Rendering/RenderQuad.cpp', 'src/Rendering/GBuffer.cpp',
'src/Rendering/Item.cpp', 'src/Rendering/WaterRenderBuffers.cpp',
'src/Rendering/Window.cpp', 'src/Logger.cpp', 'src/IngameLayer.cpp',
'src/Events/Event.cpp', 'src/Events/EventDispatcher.cpp', 'src/main.cpp',
'src/GameData/BlockStates/BlockStateContainer.cpp', 'src/GameData/Block.cpp',
'src/GameData/BlockGeometry.cpp', 'src/GameData/GameData.cpp', 'src/World.cpp',
'src/WorldGeneration/TreeNoise.cpp',
'src/WorldGeneration/StructureGenerator.cpp',
'src/WorldGeneration/CaveGenerator.cpp',
'src/WorldGeneration/TerrainGenerator.cpp',
'src/WorldGeneration/TerrainHeightGenerator.cpp',
'src/WorldGeneration/noiseutils.cpp', 'src/WorldGeneration/WorldGenerator.cpp',
'src/WorldGeneration/Biome.cpp', 'src/Color.cpp' ]

dep_entt = dependency('entt')
dep_glfw = dependency('glfw3')
dep_opengl = dependency('opengl')
dep_glew = dependency('glew')
dep_freetype = dependency('freetype2')
dep_libnoise = dependency('libnoise')
dep_assimp = dependency('assimp')

cc = meson.get_compiler('cpp')
executable('voxelgame', src,
  dependencies: [ dep_entt, dep_glfw, dep_opengl, dep_glew, dep_freetype, dep_libnoise, dep_assimp ],
  override_options: [ 'cpp_std=c++20' ],
  install: true)
