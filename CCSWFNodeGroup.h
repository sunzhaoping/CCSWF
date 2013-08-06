#ifndef CCSWF_NODE_GROUP
#define CCSWF_NODE_GROUP

#include "cocos2d.h"

using namespace cocos2d;

class SWFNodeGroup : public cocos2d::Node
{
private:
    bool isSingleton;
    
public:
    SWFNodeGroup();
    ~SWFNodeGroup();
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
    void runAction(Node* swf);
    bool Runing();

};


#endif
