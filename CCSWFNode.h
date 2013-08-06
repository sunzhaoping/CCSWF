#ifndef CCSWF_NODE
#define CCSWF_NODE

#import "cocos2d.h"
#include "gameswf.h"
#include "CCSWFNodeGroup.h"

using namespace cocos2d;

class CCSWFNode_imp;
class CCSWFNodeGroup;

class CCSWFNode : public cocos2d::Sprite
{
private:
    bool isGrouped;
    bool isRuning;
    CCSWFNode_imp *imp;
    GLfloat m_movieWidth;
    GLfloat m_movieHeight;
    GLfloat m_localScaleX;
    GLfloat m_localScaleY;
    GLfloat m_scaleX;
    GLfloat m_scaleY;
    Array* m_touchEvents;
    
    cocos2d::Object* m_listener;
    SEL_MenuHandler    m_pfnSelector;
    int iListenFrame;
    
    cocos2d::Object* m_endListener;
    SEL_MenuHandler m_pfnEndSelector;
    
    bool repeat;
    int iFrameCount;
public:
    CCSWFNode();
    ~CCSWFNode();

    void setGrouped(bool flag){ isGrouped = flag;};
    bool getGrouped(){ return isGrouped;};
    
    
    
    cocos2d::String* movieName();
    void setMovieName(char *movieName);
//    CCSWFNode(char* file);
    static CCSWFNode* create(const char* file);
    bool initWithSWFFile(const char* file);
    void setFlipX(bool flipX);
    void setFlipY(bool flipY);

    float scale();
    void setScale(float scale);
    float scaleX();
    void setScaleX(float scaleX);
    float scaleY();
    void setScaleY(float scaleY);
//    void dealloc();
    void onEnterTransitionDidFinish();
    void onExit();
    void update(float dt);
    void draw();
    
    void stopAction();
    void runAction(Object* psender = NULL);
    bool Runing(){return isRuning;};
    void addFrameListener(int iFrame, cocos2d::Object* target,SEL_MenuHandler selector);
    void setNextAction(CCSWFNode* target);

    void setRepeat(bool flag);
    bool getRepeat();



};


#endif
