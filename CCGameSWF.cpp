#include "CCGameSWF.h"
#include "gameswf/gameswf/gameswf.h"
#include "gameswf/gameswf/gameswf_character.h"
#include "gameswf/base/tu_file.h"

using namespace cocos2d;

#pragma mark - Callback declarations
// handler functions //
static void     CCGameSWF_fscommand_handler     (gameswf::character* movie, const char* command, const char* arg);  // fscommand handler //
static tu_file* CCGameSWF_file_opener           (const char* url_or_path);                                          // file opener //
static void     CCGameSWF_log_handler           (bool error, const char* message);                                  // log handler //

#pragma mark - GameSWF implementation

static GameSWF *s_ccGameSWF_sharedInstance = NULL;

GameSWF* GameSWF::sharedInstance()
{

    if (!s_ccGameSWF_sharedInstance)
    {
        s_ccGameSWF_sharedInstance = new GameSWF();
        s_ccGameSWF_sharedInstance->init();
    }
    return s_ccGameSWF_sharedInstance;
}

bool GameSWF::init()
{
        m_fscommandListeners  = new cocos2d::Array();
        gameswf::register_file_opener_callback(&CCGameSWF_file_opener);
        gameswf::register_fscommand_callback(&CCGameSWF_fscommand_handler);
#ifdef DEBUG
        gameswf::register_log_callback(&CCGameSWF_log_handler);
#endif
    gameswf::set_sound_handler(0);
        
        gameswf::render_handler* render = gameswf::create_render_handler_ogles();
        gameswf::set_render_handler(render);
        
        gameswf::set_glyph_provider(gameswf::create_glyph_provider_tu());
        return true;
}

void GameSWF::dealloc()
{
    m_fscommandListeners->release();
}

#pragma mark - fscommand handlers



#pragma mark - Callback implementations

void CCGameSWF_fscommand_handler (gameswf::character* movie, const char* command, const char* arg)
{
    
}

tu_file* CCGameSWF_file_opener (const char* url_or_path)
{
    assert(url_or_path);
	if (strncasecmp(url_or_path, "http://", 7) == 0)
	{
//		const char* slash = strchr(url_or_path + 7, '/');
//		if (slash == NULL)
//		{
//			return NULL;
//		}
//		
//		char host[256];
//		memset(host, 0, 256);
//		strncpy(host, url_or_path + 7, slash - url_or_path - 7);
//		
//		int len = 0;
//		const void* data = NULL;
//		tu_string swfile;
//		tu_string hisfile;
//		
//		NSString* swfFile = [NSString stringWithFormat: @"%@/Documents/my.swf", NSHomeDirectory()];
//		swfile = [swfFile UTF8String];
//		NSString* hisFile = [NSString stringWithFormat: @"%@/Documents/his.txt", NSHomeDirectory()];
//		hisfile = [hisFile UTF8String];
//		NSString* hostName = [NSString stringWithUTF8String: host];
//		NSString* pathToFile= [NSString stringWithUTF8String: slash];
//		NSURL* fileURL = [[NSURL alloc] initWithScheme:@"http" host:hostName path:pathToFile];
//		if (fileURL)
//		{
//			NSData* fileContents = [NSData dataWithContentsOfURL:fileURL];
//			if (fileContents)
//			{
//				len = [fileContents length];
//				data = [fileContents bytes];
//			}
//			else
//			{
//				return NULL;
//			}
//		}
//		else
//		{
//			return NULL;
//		}
//		
//		// write swf
//		{
//			tu_file fi(swfile.c_str(), "wb");
//			fi.write_bytes(data, len);
//		}
//		
//		// update history
//		
//		// read history
//		array<tu_string> his;
//		{
//			tu_file fi(hisfile.c_str(), "r");
//			if (fi.get_error() == TU_FILE_NO_ERROR)
//			{
//				char buf[1024];
//				while (fi.read_string(buf, 1024) > 1)	// !!!!
//				{
//					his.push_back(buf);
//				}
//			}
//		}
//		
//		// test for same
//		bool is_new = true;
//		for (int i = 0; i < his.size(); i++)
//		{
//			if (his[i] == url_or_path + 7)
//			{
//				is_new = false;
//				break;
//			}
//		}
//		
//		// append new item
//		if (is_new)
//		{
//			his.push_back(url_or_path + 7);
//		}
//		
//		// truncate to 20
//		while (his.size() > 20)
//		{
//			his.remove(0);
//		}
//		
//		// save history
//		{
//			tu_file fi(hisfile.c_str(), "w");
//			for (int i = 0; i < his.size(); i++)
//			{
//				fi.write_string(his[i].c_str());
//				fi.write_string("\n");
//			}
//		}
//        //	printf("%s as %s\n", url, swfile.c_str());
//		return new tu_file(swfile.c_str(), "rb");
	}
    //CCLOG("swf file name :%s",url_or_path);
    //return new tu_file(  fopen( url_or_path, "rb")    , true);
	return new tu_file(url_or_path, "rb");
}

static void CCGameSWF_log_handler (bool error, const char* message)
{
    if (error) {
        CCLOG("::ERROR:: %s",message);
    }else{
        CCLOG(":::: %s",message);
    }
}



