#ifndef SCENE_H
#define SCENE_H



class Scene
{
public:
	Scene(){}
	virtual ~Scene() {}

	virtual void init() = 0;
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual void pDelete() = 0;
};


#endif