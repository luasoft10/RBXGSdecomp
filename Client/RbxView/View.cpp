#include "RbxView/View.h"
#include "BrickMesh.h"
#include "CylinderMesh.h"
#include "SphereMesh.h"
#include "PBBMesh.h"
#include "RenderLib/AggregatingSceneManager.h"
#include "v8datamodel/Lighting.h"
#include "v8datamodel/DataModel.h"
#include "v8datamodel/Workspace.h"

namespace RBX
{
	namespace View
	{
		View::View(boost::shared_ptr<DataModel> dataModel)
			: dataModel(dataModel),
			  renderLibScene(new Render::RenderScene),
			  textureManager(new G3D::TextureManager),
			  lightingValid(false)
		{
			sceneManager.reset(new Render::AggregatingSceneManager(renderLibScene.get()));
			materialFactory.reset(new MaterialFactory(textureManager.get()));

			renderLibScene->setThrottle(0.0f, 0.0f, false, 0.0f);

			Lighting* lighting = ServiceProvider::find<Lighting>(dataModel.get());
			lightingChangedConnection = Lighting::event_LightingChanged.connect(lighting, boost::bind(&View::invalidateLighting, this, _1));

			dataModel->getWorkspace()->visitDescendents(boost::bind(&View::onWorkspaceDescendentAdded, this, _1));
			workspaceDescendentAddedConnection = Instance::event_descendentAdded.connect(dataModel->getWorkspace(), boost::bind(&View::onWorkspaceDescendentAdded, this, _1));
		}

		View::~View()
		{
			sceneManager->clear();

			Lighting* unused = ServiceProvider::create<Lighting>(dataModel.get());

			MeshFactory<CylinderAlongXMesh, 1>::flushCache();
			MeshFactory<SphereMesh, 1>::flushCache();
			MeshFactory<PBBMesh, 1>::flushCache();
			BrickMesh::flushCache();
		}
	}
}
