//
//  MusicData.cpp
//  Qianpaobuyu
//
//  Created by etgame on 15-5-29.
//
//

#include "MusicData.h"
#include "UserData.h"
#include "Manager.h"

const int mainBackGroudMusic[] = {
    4,
    3, //第一关背景音乐
    5,
    6,//Boss1
    7,
    8,
    47,
    
};
const int mainBackGroudSound[31] = {
    2,//0 爆炸音效
    9, //1 特殊玩法 爆炸附带音效
    10, //2 “天呐，你发大财啦！”
    11, //3 鱼潮来临，转换背景 海潮音效
    12, //4 点击菜单 音效
    13, //5 换炮及点击菜单内 menu的音效
    14,  //6
    15, //7 场景随机模拟音效（）
    16,
    23, //8 -----奖励金币（待议）
    26, //9 签到领取成功 音效
    28, //10 子弹发射
    37, //11 鱼死亡奖励金币
    38, //12 任务过关 领取金币
    55, //13 ”这你都能逮到我“
    56, //14 “这才是开始”
    57,  //15 “不会的，不可能”
    58,  //16 “谁，谁， 不要啊！”
    59, //17 "这，这，不是真的吧"
    60, //18 "噢， No"
    61, //19 “啊， 怎么会这样”
    62, //20 “耶，我不甘心”
    63, //21 "我很丑，可是我很温柔"
    65, //22 “恩哼”
    66, //23 "" 类似小孩哭
    67, //24 "哦呃"
    68, //25 ""小孩叫
    69, //26 胜利音效
    85, //27 打中Boss（待议）
    29, //点击生效
    31, //胜利 音效
};
//押注小游戏的音乐
const int m_aSmallGameSound[7] = {
	44,  //押注的声音
	45,  //赔了
	46,  //刚进小游戏
	47,  //背景音乐
	53,  //旋转过程中的音乐声
	69,  //赚到了
	71};  //开始旋转的声音
bool MusicData::init()
{
    u_data = UserData::getInstance();
    u_data->load();
    music_ID = 1;
	sound_ID = 1;
    return true;
}
MusicData* MusicData::myMusic = nullptr;
MusicData* MusicData::getInstance()
{
    if (myMusic == NULL) 
	{
        myMusic = new MusicData();
        myMusic->init();
    }
    
    return myMusic;
}
void MusicData::clean()
{
    
    
}

void MusicData::destroyInstance()
{
    if (myMusic != NULL) 
	{
		log("delete myMusic;");
        //delete myMusic;
		CC_SAFE_DELETE(myMusic);
        myMusic = NULL;
    }
}

void MusicData::loadMusic()
{
    for (int i = 0; i< 6; i++) 
	{
        //加载音乐
        __String* music_str = __String::createWithFormat("music/%d.mp3", mainBackGroudMusic[i]);
        SimpleAudioEngine::getInstance()->preloadBackgroundMusic(music_str->getCString());
    }
    for (int i = 0; i < 29; i++) 
	{
        __String* sound_str = __String::createWithFormat("music/%d.mp3", mainBackGroudSound[i]);
        SimpleAudioEngine::getInstance()->preloadEffect(sound_str->getCString());
    }
    
    //设置默认音量
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1);
    SimpleAudioEngine::getInstance()->setEffectsVolume(1);
}
void MusicData::playMusic(int kind)
{
    if (u_data->getMusic() == 1) 
	{
        music_ID = kind;
        __String* chose_music = __String::createWithFormat("music/%d.mp3", mainBackGroudMusic[music_ID]);
        SimpleAudioEngine::getInstance()->playBackgroundMusic(chose_music->getCString(),true);
    }
  
}
void MusicData::playSound(int kind)
{
    if (u_data->getSound() == 1) 
	{
        sound_ID = kind;

        __String* chose_sound = __String::createWithFormat("music/%d.mp3", sound_ID);
		if(!Manager::getInstance()->getInSmallGame())
			SimpleAudioEngine::getInstance()->preloadEffect(chose_sound->getCString());
        SimpleAudioEngine::getInstance()->playEffect(chose_sound->getCString());
    }
}

void MusicData::onMusic()
{
    myMusic->playMusic(music_ID);
}
void MusicData::OnSound()
{
    myMusic->playSound(sound_ID);
}
void MusicData::stopMusic()
{
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}
void MusicData::stopSound()
{
    SimpleAudioEngine::getInstance()->stopAllEffects();
}

void MusicData::pauseMusic()
{
	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}
void MusicData::pauseSound()
{
	SimpleAudioEngine::getInstance()->pauseAllEffects();
}
void MusicData::resumeMusic()
{
	SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
void MusicData::resumeSound()
{
	SimpleAudioEngine::getInstance()->resumeAllEffects();
}

//预加载小游戏音乐
void MusicData::preloadSmallGameMusic()
{
	for (int i = 0; i < 7; i++) 
	{
		//加载音乐
		__String* music_str = __String::createWithFormat("music/%d.mp3", m_aSmallGameSound[i]);
		if(i == 3)
		{
			SimpleAudioEngine::getInstance()->preloadBackgroundMusic(music_str->getCString());
		}
		else
			SimpleAudioEngine::getInstance()->preloadEffect(music_str->getCString());
	}
}
void MusicData::unloadSmallGameMusic()
{
	for (int i = 0; i < 7; i++) 
	{
		__String* music_str = __String::createWithFormat("music/%d.mp3", m_aSmallGameSound[i]);
		SimpleAudioEngine::getInstance()->unloadEffect(music_str->getCString());
	}
}
