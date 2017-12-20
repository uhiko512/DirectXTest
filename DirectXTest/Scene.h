#pragma once

class Shader;
class SceneObject;
class PlayerController;

struct VertexBuffer;

class Scene {
public:
	Scene(shared_ptr<Renderer>& renderer);
	void Update();
	void AddScene(shared_ptr<SceneObject>& sceneObject);

private:
	shared_ptr<Renderer> _renderer;
	vector<shared_ptr<SceneObject>> _sceneObjects;
	
};

