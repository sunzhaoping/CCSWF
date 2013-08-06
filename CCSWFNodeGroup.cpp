//
//  CCSWFNodeGroup.cpp
//  TangooBaby
//
//  Created by chenee on 13-5-20.
//  Copyright chenee543216@gmail.com 2013. All rights reserved.
//

#import "CCSWFNodeGroup.h"

CCSWFNodeGroup::CCSWFNodeGroup()
{
    isSingleton = true;
}
CCSWFNodeGroup::~CCSWFNodeGroup()
{
    CCLOG("delete group");
   
    stopAction();
    
    this->removeAllChildrenWithCleanup(true);
    this->removeFromParentAndCleanup(true);
}
void CCSWFNodeGroup::addChild(cocos2d::Node *child,int zOrder)
{
    Node::addChild(child,zOrder);
    ((CCSWFNode*)child)->setGrouped(true);
}
void CCSWFNodeGroup::addChild(cocos2d::Node *child)
{
    this->addChild(child, 0);
}

void CCSWFNodeGroup::setFlipX(bool flipX)
{
    cocos2d::Object* child;
    CCARRAY_FOREACH(this->getChildren(), child)
    {
        CCSWFNode* pNode = (CCSWFNode*) child;
        pNode->setFlipX(flipX);
    }
}
void CCSWFNodeGroup::setFlipY(bool flipY)
{
    cocos2d::Object* child;
    CCARRAY_FOREACH(this->getChildren(), child)
    {
        CCSWFNode* pNode = (CCSWFNode*) child;
        pNode->setFlipX(flipY);
    }
}
float CCSWFNodeGroup::scale()
{
    return 1.0f;
}

void CCSWFNodeGroup::setScale(float scale)
{
    cocos2d::Object* child;
    CCARRAY_FOREACH(this->getChildren(), child)
    {
        CCSWFNode* pNode = (CCSWFNode*) child;
        pNode->setScale(scale);
    }
}

float CCSWFNodeGroup::scaleX()
{
    return 1.0f;

}

void CCSWFNodeGroup::setScaleX(float scaleX)
{
    cocos2d::Object* child;
    CCARRAY_FOREACH(this->getChildren(), child)
    {
        CCSWFNode* pNode = (CCSWFNode*) child;
        pNode->setScaleX(scaleX);
    }
}

float CCSWFNodeGroup::scaleY()
{
    return 1.0f;
}

void CCSWFNodeGroup::setScaleY(float scaleY)
{
    cocos2d::Object* child;
    CCARRAY_FOREACH(this->getChildren(), child)
    {
        CCSWFNode* pNode = (CCSWFNode*) child;
        pNode->setScaleY(scaleY);
    }
}

void CCSWFNodeGroup::runAction(CCSWFNode* swf)
{
    if (isSingleton) {
        cocos2d::Object* child;
        CCARRAY_FOREACH(this->getChildren(), child)
        {
            CCSWFNode* pNode = (CCSWFNode*) child;
            if (pNode != swf) {
                pNode->stopAction();
            }
        }
    }
    
    swf->runAction(this);
}
void CCSWFNodeGroup::stopAction()
{
    cocos2d::Object* child;
    CCARRAY_FOREACH(this->getChildren(), child)
    {
        CCSWFNode* pNode = (CCSWFNode*) child;
        pNode->stopAction();
    }
}
bool CCSWFNodeGroup::Runing()
{
    cocos2d::Object* child;
    CCARRAY_FOREACH(this->getChildren(), child)
    {
        CCSWFNode* pNode = (CCSWFNode*) child;
        if(pNode->Runing()){
            return true;
        }
    }
    return false;
}


