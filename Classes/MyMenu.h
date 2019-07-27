//
//  MyMenu.h
//  Qianpaobuyu
//
//  Created by etgame on 15-5-19.
//
//

#ifndef __Qianpaobuyu__MyMenu__
#define __Qianpaobuyu__MyMenu__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;
class MyMenu : public Menu
{
public:
    MyMenu();
    virtual ~MyMenu();
    static MyMenu* createMyMenu();
    virtual bool initMyMenu();
    
    /** initializes a Menu with a NSArray of MenuItem objects */
    bool initWithArray(const Vector<MenuItem*>& arrayOfItems);
    static MyMenu* createWithArray(const Vector<MenuItem*>& arrayOfItems);
    static MyMenu* createWithItem(MenuItem* item);
    static MyMenu* create(MenuItem* item, ...);
    static MyMenu* createWithItems(MenuItem *firstItem, va_list args);
    CREATE_FUNC(MyMenu);
    
private:
    CC_SYNTHESIZE(bool, isChosed, IsChosed);
    
};
#endif /* defined(__Qianpaobuyu__MyMenu__) */
