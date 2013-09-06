#include "CCSWFNode.h"
#include "CCSWFNodeGroup.h"
#include "CCGameSWF.h"
#include "cocos2d.h"
#include "gameswf/gameswf/gameswf.h"
#include "gameswf/gameswf/gameswf_player.h"
#include "gameswf/gameswf/gameswf_root.h"
#include "ccMacros.h"
#include "gameswf/gameswf/gameswf_types.h"
#include "gameswf/gameswf/gameswf_impl.h"

using namespace cocos2d;

class CCSWFNode_touchContainer : public cocos2d::Node
{
    cocos2d::Point m_position;
    int m_state;
    CCSWFNode_touchContainer(cocos2d::Point position,int state);
};

CCSWFNode_touchContainer::CCSWFNode_touchContainer(cocos2d::Point position, int state)
{
    m_position = position;
    m_state = state;
}

class CCSWFNode_imp : public cocos2d::Node
{
public:
    gameswf::gc_ptr<gameswf::player> m_player;
    gameswf::gc_ptr<gameswf::root>	m_movie;
    
    bool initWithSWFFile(const char *file);

};

bool CCSWFNode_imp::initWithSWFFile(const char *file)
{
    std::string pathKey = file;
    
    pathKey = CCFileUtils::getInstance()->fullPathForFilename(pathKey.c_str());
    if (pathKey.size() == 0)
    {
        return false;
    }

        GameSWF::sharedInstance();
        m_player = new gameswf::player();
        m_movie = m_player->load_file(pathKey.c_str());
        if (m_movie == NULL)
        {
            CCLOG("ERROR: Cannot open input file %s", file);

            return false;
        }
    return true;
}

SWFNode::SWFNode()
{
    
   m_image = new Image();
#if CC_ENABLE_CACHE_TEXTURE_DATA
    // Listen this event to save render texture before come to background.
    // Then it can be restored after coming to foreground on Android.
    NotificationCenter::getInstance()->addObserver(this,
                                                   callfuncO_selector(SWFNode::onToBackground),
                                                   EVENT_COME_TO_BACKGROUND,
                                                   NULL);
    
    NotificationCenter::getInstance()->addObserver(this,
                                                   callfuncO_selector(SWFNode::onToForeground),
                                                   EVNET_COME_TO_FOREGROUND, // this is misspelt
                                                   NULL);
#endif
}

SWFNode::~SWFNode()
{
    this->stopAction();
    this->removeFromParentAndCleanup(true);
    ((CCSWFNode_imp*)this->imp)->m_movie = NULL;
    ((CCSWFNode_imp*)this->imp)->m_player = NULL;
    delete imp;
#if CC_ENABLE_CACHE_TEXTURE_DATA
    NotificationCenter::getInstance()->removeObserver(this, EVENT_COME_TO_BACKGROUND);
    NotificationCenter::getInstance()->removeObserver(this, EVNET_COME_TO_FOREGROUND);
#endif
}


void SWFNode::setFrame(int frameNo){
    ((CCSWFNode_imp*)this->imp)->m_movie->goto_frame(frameNo);
}


int SWFNode::getFrame(){
    return ((CCSWFNode_imp*)this->imp)->m_movie->get_current_frame();
}

cocos2d::String* SWFNode::movieName()
{
    return cocos2d::String::createWithFormat("%s",  ((CCSWFNode_imp*)this->imp)->m_movie->m_movie->m_name.c_str());
}

void SWFNode::setMovieName(const char *movieName)
{
     ((CCSWFNode_imp*)this->imp)->m_movie->m_movie->m_name = movieName;// UTF8String];
}

SWFNode* SWFNode::create(const char* file)
{
    SWFNode* swf = new SWFNode();
    if (! swf->initWithSWFFile(file)) {
        swf->release();
        return NULL;
    }
    
    return swf;
}

bool SWFNode::initWithSWFFile(const char* file)
{
    imp = new CCSWFNode_imp();
    if (! ((CCSWFNode_imp*)this->imp)->initWithSWFFile(file))
    {
        imp->release();
        return false;
    }
    m_movieWidth =  ((CCSWFNode_imp*)this->imp)->m_movie->m_def->m_frame_size.m_x_max - ((CCSWFNode_imp*)this->imp)->m_movie->m_def->m_frame_size.m_x_min;
    m_movieHeight = ((CCSWFNode_imp*)this->imp)->m_movie->m_def->m_frame_size.m_y_max - ((CCSWFNode_imp*)this->imp)->m_movie->m_def->m_frame_size.m_y_min;
    m_localScaleX = (((CCSWFNode_imp*)this->imp)->m_movie->get_movie_width() / m_movieWidth);
    m_localScaleY = -(((CCSWFNode_imp*)this->imp)->m_movie->get_movie_height() / m_movieHeight);
    m_scaleX = 1.0;
    m_scaleY = 1.0;
    
    m_touchEvents = new Array();//[[NSMutableArray alloc] init];
    
    this->setContentSize(Size(m_movieWidth, m_movieHeight));
    this->setScale(1.0f);
    this->setAnchorPoint(Point(0.5,0.5));
    
    this->isGrouped = false;
    this->isRuning = false;
    this->m_listener = NULL;
    this->m_endListener = NULL;
    this->repeat = false;
    this->iFrameCount = ((CCSWFNode_imp*)this->imp)->m_movie->get_frame_count();

    return true;
}
void SWFNode::setFlipX(bool flipX)
{
    if (flipX) {
        setScaleX(m_localScaleX * -1);
        
    }
}
void SWFNode::setFlipY(bool flipY)
{
    if (flipY) {
        setScaleY(m_localScaleY * -1);
    }
}
float SWFNode::scale()
{
    CCAssert(m_scaleX == m_scaleY, "Node#scale. ScaleX != ScaleY. Don't know which one to return");
	return m_scaleX;
}

void SWFNode::setScale(float scale)
{
    m_scaleX = m_scaleY = scale;
    setScaleX(m_localScaleX*m_scaleX);
    setScaleY(m_localScaleY*m_scaleY);
}

float SWFNode::scaleX()
{
    return m_scaleX;
}

void SWFNode::setScaleX(float scaleX)
{
    m_scaleX = scaleX;
    cocos2d::Sprite::setScaleX(m_scaleX);
}

float SWFNode::scaleY()
{
    return m_scaleY;
}

void SWFNode::setScaleY(float scaleY)
{
    m_scaleY = scaleY;
    Sprite::setScaleY(m_scaleY);
    
}

void SWFNode::onEnterTransitionDidFinish()
{
}

void SWFNode::onExit()
{
    if(! isRuning)
        return;
    
    unschedule(schedule_selector(SWFNode::update));
    this->isRuning = false;
}
void SWFNode::runAction(Object* psender)
{
    if(isRuning)
        return;
    
    //if in singleton group we stop other sibling.
    if (isGrouped) {
       SWFNodeGroup* g = (SWFNodeGroup*)this->getParent();
        if (g->getSingleton()) {
            g->stopAction();
        }
    }
    
    this->isRuning = true;
    //reset 
    ((CCSWFNode_imp*)this->imp)->m_movie->goto_frame(0);
    ((CCSWFNode_imp*)this->imp)->m_movie->set_play_run();
    
    schedule(schedule_selector(SWFNode::update),1/2);
}
void SWFNode::stopAction()
{
    if(! isRuning)
        return;
    unschedule(schedule_selector(SWFNode::update));
    this->isRuning = false;
}

void SWFNode::addFrameListener(int iFrame, cocos2d::Object* target,SEL_MenuHandler selector)
{
    CCAssert(target != NULL, "swf action should not be NULL");

    if(iFrame == -1){
        m_endListener = target;
        m_pfnEndSelector = selector;
    }else if(iFrame > 0){
        m_listener = target;
        m_pfnSelector = selector;
        iListenFrame = iFrame;
    }
}
void SWFNode::setNextAction(SWFNode* target)
{
    CCAssert(target != NULL, "next swf action should not be NULL");
    
    m_endListener = target;
    m_pfnEndSelector = menu_selector(SWFNode::runAction);
}
void SWFNode::setRepeat(bool flag)
{
    this->repeat = flag;
}
bool SWFNode::getRepeat()
{
    return repeat;
}


void SWFNode::update(float dt)
{
    bool tobestop = false;
    int iFrame = ((CCSWFNode_imp*)this->imp)->m_movie->get_current_frame();
    if (m_listener && iFrame == iListenFrame) {
        (m_listener->*m_pfnSelector)(this);
    }
    if(! repeat){
        if (iFrame == iFrameCount - 1) {
            tobestop = true;
        }
    }
    ((CCSWFNode_imp*)this->imp)->m_movie->advance(dt);
    
    if(tobestop){
        this->stopAction();
        if(m_endListener){
            (this->m_endListener->*m_pfnEndSelector)(this);
        }
    }
}

void SWFNode::draw()
{
    if (! isRuning) {
        return;
    }
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    
    kmMat4 matrixP;
	kmMat4 matrixMV;
	kmMat4 matrixMVP;
	
	kmGLGetMatrix(KM_GL_PROJECTION, &matrixP );
	kmGLGetMatrix(KM_GL_MODELVIEW, &matrixMV );
    kmMat4Multiply(&matrixMVP, &matrixP, &matrixMV);
    
    GL::useProgram(0);

	((CCSWFNode_imp*)this->imp)->m_movie->display(&matrixMVP.mat[0]);
    CHECK_GL_ERROR_DEBUG();

    //render();
    
    glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);

}

void SWFNode::onToBackground(cocos2d::Object *obj)
{
#if CC_ENABLE_CACHE_TEXTURE_DATA
gameswf::get_render_handler()->m_reload_shader = true;
#endif
}

void SWFNode::onToForeground(cocos2d::Object *obj)
{
#if CC_ENABLE_CACHE_TEXTURE_DATA

#endif
}

//@end
