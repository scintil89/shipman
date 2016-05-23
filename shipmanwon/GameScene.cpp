#include "stdafx.h"
#include "GameScene.h"
#include "StageObject.h"
#include "SkyObject.h"
#include "TreeObject.h"
#include "GroundObject.h"

void CGameScene::initialize()
{
	setSceneColor(0.6f, 1.0f, 0.6f, 1.0f);
	//setSceneColor(0.0f, 0.28f, 0.67f, 1.0f); //cobalt blue

	CStageObject* stage = new CStageObject();
	pushBack(stage, 10);

	CMyObject* sky = new CSkyObject();
	pushBack(sky, 10);

//	test code
	CMyObject* tree = new CTreeObject();
	tree->setTranslate(5.0f, -5.0f, 7.0f);
	tree->setScale(0.5f, 0.5f, 0.5f);
	pushBack(tree, 10);

	CMyObject* tree2 = new CTreeObject();
	tree2->setTranslate(6.0f, -5.0f, 4.0f);
	tree2->setRotate(0.0f, 15.0f, 0.0f);
	tree2->setScale(0.7f, 0.7f, 0.7f);
	pushBack(tree2, 10);

	CMyObject* tree3 = new CTreeObject();
	tree3->setTranslate(10.0f, -5.0f, 17.0f);
	pushBack(tree3, 10);

	CMyObject* tree4 = new CTreeObject();
	tree4->setTranslate(5.0f, -5.0f, 13.0f);
	pushBack(tree4, 10);

	CMyObject* ground = new CGroundObject();
	ground->setTranslate(0.0f, -10.0f, 0.0f);
	pushBack(ground, 10);
}