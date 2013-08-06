#ifndef CCSWF_NODE_GROUP
#define CCSWF_NODE_GROUP

#include "cocos2d.h"
#include "CCSWFNode.h"

using namespace cocos2d;

class CCSWFNode;

class CCSWFNodeGroup : public cocos2d::Node
{
private:
    bool isSingleton;
    
public:
    CCSWFNodeGroup();
    ~CCSWFNodeGroup();
    void addChild(cocos2d::Node *child);
    void addChild(cocos2d::Node *child,int zOrder);
    
    bool getSingleton(){return isSingleton;};
    
    void setFlipX(bool flipX);
    void setFlipY(bool flipY);

    float scale();
    void setScale(float scale);
    float scaleX();
    void setScaleX(float scaleX);
    float scaleY();
    void setScaleY(float scaleY);
    
    void stopAction();
    void runAction(CCSWFNode* swf);
    bool Runing();

};


#endif
