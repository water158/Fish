//
//  NoticeBoss.h
//  Qianpaobuyu
//
//  Created by etgame on 15-6-9.
//
//

#ifndef __Qianpaobuyu__NoticeBoss__
#define __Qianpaobuyu__NoticeBoss__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;
class NoticeBoss : public Node
{
public:
    NoticeBoss();
    virtual ~NoticeBoss();
    static NoticeBoss* createNoticeBoss(int _kind, int level);
    virtual bool initNoticeBoss(int _kind, int level);
    
    void removeNotice();
    //CREATE_FUNC(CountdownLayer);
private:
    Size size;
    
    
};
#endif /* defined(__Qianpaobuyu__NoticeBoss__) */
