//
//  SWFNodeGroup.cpp
//  TangooBaby
//
//  Created by chenee on 13-5-20.
//  Copyright chenee543216@gmail.com 2013. All rights reserved.
//

#include "CCSWFNodeGroup.h"
#include "CCSWFNode.h"

SWFNodeGroup::SWFNodeGroup()
{
    isSingleton = true;
}
SWFNodeGroup::~SWFNodeGroup()
{
    CCLOG("delete group");
   
    stopAction();
    
    this->removeAllChildrenWithCleanup(true);
    this->removeFromParentAndCleanup(true);
}
void SWFNodeGroup::addChild(cocos2d::Node *child,int zOrder)
{
    Node::addChild(child,zOrder);
    ((SWFNode*)child)->setGrouped(true);
}
void SWFNodeGroup::addChild(cocos2d::Node *child)
{
    this->addChild(child, 0);
}

void SWFNodeGroup::setFlipX(bool flipX)
{
    cocos2d::Object* child;
    CCARRAY_FOREACH(this->getChildren(), child)
    {
        SWFNode* pNode = (SWFNode*) child;
        pNode->setFlipX(flipX);
    }
}
void SWFNodeGroup::setFlipY(bool flipY)
{
    cocos2d::Object* child;
    CCARRAY_FOREACH(this->getChildren(), child)
    {
        SWFNode* pNode = (SWFNode*) child;
        pNode->setFlipX(flipY);
    }
}
float SWFNodeGroup::scale()
{
    return 1.0f;
}

void SWFNodeGroup::setScale(float scale)
{
    cocos2d::Object* child;
    CCARRAY_FOREACH(this->getChildren(), child)
    {
        SWFNode* pNode = (SWFNode*) child;
        pNode->setScale(scale);
    }
}

float SWFNodeGroup::scaleX()
{
    return 1.0f;

}

void SWFNodeGroup::setScaleX(float scaleX)
{
    cocos2d::Object* child;
    CCARRAY_FOREACH(this->getChildren(), child)
    {
        SWFNode* pNode = (SWFNode*) child;
        pNode->setScaleX(scaleX);
    }
}

float SWFNodeGroup::scaleY()
{
    return 1.0f;
}

void SWFNodeGroup::setScaleY(float scaleY)
{
    cocos2d::Object* child;
    CCARRAY_FOREACH(this->getChildren(), child)
    {
        SWFNode* pNode = (SWFNode*) child;
        pNode->setScaleY(scaleY);
    }
}

void SWFNodeGroup::runAction(Node* swf)
{
    if (isSingleton) {
        cocos2d::Object* child;
        CCARRAY_FOREACH(this->getChildren(), child)
        {
            SWFNode* pNode = (SWFNode*) child;
            if (pNode != swf) {
                pNode->stopAction();
            }
        }
    }
    
    ((SWFNode*)swf)->runAction(this);
}
void SWFNodeGroup::stopAction()
{
    cocos2d::Object* child;
    CCARRAY_FOREACH(this->getChildren(), child)
    {
        SWFNode* pNode = (SWFNode*) child;
        pNode->stopAction();
    }
}
bool SWFNodeGroup::Runing()
{
    cocos2d::Object* child;
    CCARRAY_FOREACH(this->getChildren(), child)
    {
        SWFNode* pNode = (SWFNode*) child;
        if(pNode->Runing()){
            return true;
        }
    }
    return false;
}


