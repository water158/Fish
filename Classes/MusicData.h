//
//  MusicData.h
//  Qianpaobuyu
//
//  Created by etgame on 15-5-29.
//
//

#ifndef __Qianpaobuyu__MusicData__
#define __Qianpaobuyu__MusicData__

#include <iostream>
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
USING_NS_CC;
class UserData;
class MusicData
{

public:
    bool init();
    static MusicData* getInstance();
    void clean();
    static void destroyInstance();
    static MusicData* myMusic;
    
    void onMusic();
    void OnSound();
    void stopMusic(); 
    void stopSound();

	void pauseMusic();
	void pauseSound();
	void resumeMusic();
	void resumeSound();
    
    
    void loadMusic();
    void playSound(int kind);
    void playMusic(int kind);
    
	//预加载小游戏音乐
	void preloadSmallGameMusic();
	void unloadSmallGameMusic();

private:
    int music_ID;
    int sound_ID;
    
    int s_ID;
    UserData* u_data;
    ValueMap musicMap;
};
#endif
