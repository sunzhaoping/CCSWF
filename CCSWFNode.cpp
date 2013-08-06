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
            printf("ERROR: Cannot open input file %s", file);

            return false;
        }
    return true;
}

SWFNode::SWFNode()
{

}
SWFNode::~SWFNode()
{
    this->stopAction();
    this->removeFromParentAndCleanup(true);
    ((CCSWFNode_imp*)this->imp)->m_movie = NULL;
     ((CCSWFNode_imp*)this->imp)->m_player = NULL;
    
    delete imp;
}

cocos2d::String* SWFNode::movieName()
{
    return cocos2d::String::createWithFormat("%s",  ((CCSWFNode_imp*)this->imp)->m_movie->m_movie->m_name.c_str());
}

void SWFNode::setMovieName(char *movieName)
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
#define STRINGIFY(A)  #A
const char* ColorVertexShader1 = STRINGIFY(
attribute vec4 a_position;
uniform mat4 Projection;
uniform mat4 u_MVPMatrix;
void main(void)
{
    vec4 tmp;
    tmp =  Projection * u_MVPMatrix * a_position;
//    tmp = u_MVPMatrix * a_position;
//    tmp = a_position;
    
    //    tmp.x *= -1.0;
//    tmp.y *= -1.0;
    gl_Position = tmp;
    
}
);

const char* ColorFragmentShader1 = STRINGIFY(
uniform lowp vec4 u_color;
                                            
void main(void)
{
    gl_FragColor = u_color;
}
);
unsigned int m_currentProgram = 0;

void ApplyOrtho(float maxX, float maxY)
{
    
    //        maxX = 1024*20/2;
    //        maxY = 768*20/2;
    float a = 1.0f / maxX;
    float b = 1.0f / maxY;
    float ortho[16] = {
        a, 0,  0, 0,
        0, b,  0, 0,
        0, 0, -1, 0,
        0, 0,  0, 1
    };
    
    GLint projectionUniform = glGetUniformLocation(m_currentProgram, "Projection");
    glUniformMatrix4fv(projectionUniform, 1, 0, &ortho[0]);
    CHECK_GL_ERROR_DEBUG();
}
void ApplyMatrix()
{
    float	mat[16]={
//        1.0f/512,    0,              0,              0,
//        0,              1.0f/384,    0,              0,
//        0,0,-0.000976562,   0,
//        -0.273438,-0.234375,0,1,
        
        1/512,0,0,0,
        0,-1/384,0,0,
        0,0,-1/1024,0,
//        0,0,0,1,
        -372,-474,0,1,
    };

    GLint modelviewUniform = glGetUniformLocation(m_currentProgram, "u_MVPMatrix");
    glUniformMatrix4fv(modelviewUniform, 1, 0, mat);
    CHECK_GL_ERROR_DEBUG();
}
static void ApplyColor()
{
    GLfloat color[4];
    {
        color[0] = 0.5;
        color[1] = 0.5;
        color[2] = 1.0;
        color[3] = 1.0;
    }
    
    GLint lineColorSlot = glGetUniformLocation(m_currentProgram, "u_color");
    glUniform4fv(lineColorSlot, 1, color);
    CHECK_GL_ERROR_DEBUG();
    
}
GLuint BuildShader(const char* source, GLenum shaderType)
{
    GLuint shaderHandle = glCreateShader(shaderType);
    glShaderSource(shaderHandle, 1, &source, 0);
    glCompileShader(shaderHandle);
    
    GLint compileSuccess;
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compileSuccess);
    
    if (compileSuccess == GL_FALSE) {
        GLchar messages[256];
        glGetShaderInfoLog(shaderHandle, sizeof(messages), 0, &messages[0]);
        printf("%s", messages);
        exit(1);
    }
    
    return shaderHandle;
}

GLuint BuildProgram(const char* vertexShaderSource,
                    const char* fragmentShaderSource) 
{
    GLuint vertexShader = BuildShader(vertexShaderSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = BuildShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    
    GLuint programHandle = glCreateProgram();
    glAttachShader(programHandle, vertexShader);
    glAttachShader(programHandle, fragmentShader);
    glLinkProgram(programHandle);
    
    GLint linkSuccess;
    glGetProgramiv(programHandle, GL_LINK_STATUS, &linkSuccess);
    if (linkSuccess == GL_FALSE) {
        GLchar messages[256];
        glGetProgramInfoLog(programHandle, sizeof(messages), 0, &messages[0]);
        printf("%s", messages);
        
        exit(1);
    }
    
    return programHandle;
}
void render()
{
    m_currentProgram = BuildProgram(ColorVertexShader1, ColorFragmentShader1);
    CHECK_GL_ERROR_DEBUG();
    
    kmMat4 matrixP;
	kmMat4 matrixMV;
	kmMat4 matrixMVP;
	
	kmGLGetMatrix(KM_GL_PROJECTION, &matrixP );
	kmGLGetMatrix(KM_GL_MODELVIEW, &matrixMV );
	
	kmMat4Multiply(&matrixMVP, &matrixP, &matrixMV);
    
#if CC_ENABLE_GL_STATE_CACHE
    GL::useProgram(0);
#endif
    ApplyColor();
//    ApplyOrtho(512, 384);
//    ApplyMatrix();
    
    GLint projectionUniform = glGetUniformLocation(m_currentProgram, "Projection");
    glUniformMatrix4fv(projectionUniform, 1, 0, &matrixP.mat[0]);
    CHECK_GL_ERROR_DEBUG();

    GLint modelviewUniform = glGetUniformLocation(m_currentProgram, "u_MVPMatrix");
    glUniformMatrix4fv(modelviewUniform, 1, 0, &matrixMV.mat[0]);
    CHECK_GL_ERROR_DEBUG();
    
    GLuint positionSlot = glGetAttribLocation(m_currentProgram, "a_position");
    glEnableVertexAttribArray(positionSlot);
    float fcoords[8] ={
        80,-60,
        5680,-60,
        80,3540,
        5680,3540,
    };
    glVertexAttribPointer(positionSlot,2, GL_FLOAT,GL_FALSE, 0, fcoords);
    CHECK_GL_ERROR_DEBUG();
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    CHECK_GL_ERROR_DEBUG();
}



void SWFNode::draw()
{
    if (! isRuning) {
        return;
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    
    kmMat4 matrixP;
	kmMat4 matrixMV;
	kmMat4 matrixMVP;
	
	kmGLGetMatrix(KM_GL_PROJECTION, &matrixP );
	kmGLGetMatrix(KM_GL_MODELVIEW, &matrixMV );
    kmMat4Multiply(&matrixMVP, &matrixP, &matrixMV);
    
#if CC_ENABLE_GL_STATE_CACHE
    GL::useProgram(0);//valid program is NON_ZERO unsigned int
#endif
	((CCSWFNode_imp*)this->imp)->m_movie->display(&matrixMVP.mat[0]);
    CHECK_GL_ERROR_DEBUG();

//    render();
    
    glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);

}

//@end
