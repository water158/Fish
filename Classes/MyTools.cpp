#include "MyTools.h"

float AverageSpeed(Vec2 begin_point, Vec2 end_point, float speed)
{
	float temp_time = 0.0f;
	float tempx = end_point.x - begin_point.x;
	float tempy = end_point.y - begin_point.y;
	float tempz = (float)(sqrt(tempx*tempx + tempy*tempy));
	//CCMessageBox("Not equal to Zero","error");
	temp_time = tempz / speed;
	return temp_time;
}

/*

            |0度
			|
			|
负90度-------------90度
或270度
            |
			|
			|180度

可以把起始点和终点同时移动，然后让起始点落在原点上，之后连接起点和终点，看直线与Y轴的夹角
*/
float PointRotatePoint(Vec2 begin_point, Vec2 end_point, bool isUp)
{
	float delta_x = end_point.x - begin_point.x;
	float delta_y = end_point.y - begin_point.y;
	float angle = (float)CC_RADIANS_TO_DEGREES(atanf(delta_x / delta_y));
	if (delta_y < 0)
	{
		if (delta_x < 0)
		{
			angle = 180 + fabs(angle);
		}
		else
		{
			angle = 180 - fabs(angle);
		}
	}

	if (isUp)
	{
		return angle;
	}
	else
	{
		return angle + 180.0f;
	}
}

float calcTwoPointLength(Vec2 p_begin, Vec2 p_end)
{
	//float f_mid_x = (p_end.x + p_begin.x)/2;
	//float f_mid_y = (p_end.y + p_begin.y)/2;
	//中心点
	//Point p_mid = Point( f_mid_x, f_mid_y);
    
	float f_temp_x = p_end.x - p_begin.x;
	float f_temp_y = p_end.y - p_begin.y;
    
	//两点距离的一半
	//float f_value = f_temp_x*f_temp_x + f_temp_y*f_temp_y;
	//float f_len = sqrt(f_value);
    
	return sqrt(f_temp_x * f_temp_x + f_temp_y * f_temp_y);
}

float calcTwoPointAngle(Vec2 begin_point, Vec2 end_point)
{
	float x = end_point.x - begin_point.x;
	float y = end_point.y - begin_point.y;
    
	// 开方
	float hypotenuse = sqrtf(x * x + y * y);
	// 获得弧度
	float cos = x / hypotenuse;
	// 求出弧度
	float radian = acosf(cos);
	// calculated angle in radians
	float angle = CC_RADIANS_TO_DEGREES(radian);
    
	// 根据coco2dx的坐标系进行修改
	if (y > 0.0f)
    {
		angle = 0.0f - angle;
	}
    
    if (angle > 0.0f)
    {
		angle -= 180.0f;
	}
    else if(angle < 0.0f)
    {
		angle += 180.0f;
	}
	
    return angle;
}

float calcAngleFromOriginToTarget(Vec2 origin, Vec2 target)
{
    float delta_x = target.x - origin.x;
	float delta_y = target.y - origin.y;
    
    return CC_RADIANS_TO_DEGREES(Point(delta_x, delta_y).getAngle());
}

Sprite* ccspmaskedSpriteWithSprite(Sprite* textureSprite, Sprite* maskSprite)
{
	// 1
	RenderTexture * rt = RenderTexture::create( maskSprite->getContentSize().width,
		maskSprite->getContentSize().height );

	// 2
	maskSprite->setPosition(maskSprite->getContentSize().width/2,
		maskSprite->getContentSize().height/2);
	textureSprite->setPosition(textureSprite->getContentSize().width/2,
		textureSprite->getContentSize().height/2);

	// 3
	BlendFunc blendFunc;
	blendFunc.src = GL_ONE;
	blendFunc.dst = GL_ZERO;
	maskSprite->setBlendFunc(blendFunc);

	blendFunc.src = GL_DST_ALPHA;
	blendFunc.dst = GL_ZERO;	
	textureSprite->setBlendFunc(blendFunc);

	// 4
	rt->begin();
	maskSprite->visit();
	textureSprite->visit();
	rt->end();

	// 5
	Sprite *retval = Sprite::createWithTexture(rt->getSprite()->getTexture());
	retval->setFlippedY(true);
	return retval;

}

bool isRoundContainsPoint(Node *aimnode, Vec2 aimpoint)
{
	if (!aimnode)
	{
		MessageBox("aimnode is null", "");
		return false;
	}
	Point myanchorpoint = aimnode->getAnchorPoint();

	float sca_x = aimnode->getScaleX();
	float sca_y = aimnode->getScaleY();
	//计算圆形的中心点位置坐标
	float mymidpoint_x = aimnode->getPositionX() - aimnode->getContentSize().width*myanchorpoint.x*sca_x + aimnode->getContentSize().width / 2 * sca_x;
	float mymidpoint_y = aimnode->getPositionY() - aimnode->getContentSize().height*myanchorpoint.y*sca_y + aimnode->getContentSize().height / 2 * sca_y;

	//计算半径
	float rlength = aimnode->getContentSize().width / 2 * sca_x;
	//计算两点之间的距离
	float tempx = aimpoint.x - mymidpoint_x;
	float tempy = aimpoint.y - mymidpoint_y;
	float tempz = (float)(sqrt(tempx*tempx + tempy*tempy));

	if (tempz > rlength)
		return false;
	else
		return true;
}

bool isRoundContainsNode(Node *aimroundnode, Node *aimnode)
{
	if (!aimnode || !aimroundnode)
	{
		MessageBox("ParamerNode is NULL", "");
		return false;
	}
	Point myanchorpoint = aimroundnode->getAnchorPoint();
	Point myanchorpoint1 = aimnode->getAnchorPoint();

	float sca_x = aimroundnode->getScaleX();
	float sca_y = aimroundnode->getScaleY();
	float sca_x1 = aimnode->getScaleX();
	float sca_y1 = aimnode->getScaleY();
	//计算圆形的中心点位置坐标
	float mymidpoint_x = aimroundnode->getPositionX() - aimroundnode->getContentSize().width*myanchorpoint.x*sca_x + aimroundnode->getContentSize().width / 2 * sca_x;
	float mymidpoint_y = aimroundnode->getPositionY() - aimroundnode->getContentSize().height*myanchorpoint.y*sca_y + aimroundnode->getContentSize().height / 2 * sca_y;
	float mymidpoint_x1 = aimnode->getPositionX() - aimnode->getContentSize().width*myanchorpoint1.x*sca_x1 + aimnode->getContentSize().width / 2 * sca_x1;
	float mymidpoint_y1 = aimnode->getPositionY() - aimnode->getContentSize().height*myanchorpoint1.y*sca_y1 + aimnode->getContentSize().height / 2 * sca_y1;

	//计算半径
	float rlength = aimroundnode->getContentSize().width / 2 * sca_x;
	float rlength1 = aimnode->getContentSize().width / 2 * sca_x1;


	//计算两点之间的距离
	float tempx = mymidpoint_x1 - mymidpoint_x;
	float tempy = mymidpoint_y1 - mymidpoint_y;
	float tempz = (float)(sqrt(tempx*tempx + tempy*tempy));

	if (tempz > rlength + rlength1)
		return false;
	else
		return true;
}

void copyData(const char* pFileURL, const char* pFileName)
{
	std::string strPath = FileUtils::getInstance()->fullPathForFilename(pFileURL);
	ssize_t len = 0;
	unsigned char *data = NULL;

	data = FileUtils::getInstance()->getFileData(strPath.c_str(), "r", &len);
	//CCAssert(data, "copyData is null");
	std::string destPath = FileUtils::getInstance()->getWritablePath();
	destPath += pFileName;


	FILE *fp = fopen(destPath.c_str(), "w+");

	if (fp)
	{
		fwrite(data, sizeof(char), len, fp);
		fclose(fp);
		delete[]data;
		data = NULL;
	}
    
    //std::string strContent((char *)data);
//    CCFileUtils::sharedFileUtils()->saveToFile(strContent, destPath);
}

bool isFileExist(const char* pFileName)
{
	if (!pFileName) return false;
	std::string filePath = pFileName;

	FILE *fp = fopen(filePath.c_str(), "r");
	if (fp)
	{
		fclose(fp);
		return true;
	}

	return false;
}

int getRandom(int begain_num, int end_num)
{
	if (begain_num >= end_num)
		return	 0;
	if (begain_num == 0 && end_num == 1)
		return (CCRANDOM_0_1() < 0.5f) ? 0 : 1;
	int temp = CCRANDOM_0_1()*begain_num + (end_num - begain_num);
	return temp;
}

int getIntFromS(const char *temp_string)
{
	if (temp_string)
		return atoi(temp_string);
	else
		return -1;
}
float getFloatFromS(const char *temp_string)
{
	if (temp_string)
		return atof(temp_string);
	else
		return -1;
}

std::string getStringFromInt(int temp_int)
{
	char temp_str[200];
	std::string format = "%d";
	sprintf(temp_str, format.c_str(), temp_int);

	return temp_str;
}

std::string getStringFromFloat(float temp_float)
{
	char temp_str[200];
	std::string format = "%f";
	sprintf(temp_str, format.c_str(), temp_float);

	return temp_str;
}

float calcRadius(float x, float y)
{
    return sqrtf(x * x + y * y);
}

float calcAngleDegrees(float x, float y)
{
    //CCLOG("calcAngleDegrees x=[%f] y=[%f] rad=[%f] deg=[%f]", x, y, atan2f(y, x), CC_RADIANS_TO_DEGREES(atan2f(y, x)));
    return CC_RADIANS_TO_DEGREES(atan2f(y, x));
}

Point getRandomPositionByOffset(Vec2 anchor_point, float offset_len_x, float offset_len_y)
{
	if(offset_len_x < 0)
	{
		offset_len_x = -offset_len_x;
	}
	if(offset_len_y < 0)
	{
		offset_len_y = -offset_len_y;
	}

	//范围内随机x、y值
	float temp_x = CCRANDOM_0_1()*offset_len_x;
	float temp_y = CCRANDOM_0_1()*offset_len_y;

	//随机正负
	temp_x *= (CCRANDOM_0_1() > 0.5f) ? -1 : 1;
	temp_y *= (CCRANDOM_0_1() > 0.5f) ? -1 : 1;

	return Point(anchor_point.x + temp_x, anchor_point.y + temp_y);
}

//范围随机点
Vec2 getRandomPositionByBound(Vec2 anchor_point, float min_x, float min_y, float max_x, float max_y)
{
	float dela_x = abs(max_x) - abs(min_x);
	float dela_y = abs(max_y) - abs(min_y);

	//范围内随机x、y值
	float temp_x = CCRANDOM_0_1()*dela_x + abs(min_x);
	float temp_y = CCRANDOM_0_1()*dela_y + abs(min_y);

	//随机正负
	temp_x *= (CCRANDOM_0_1() > 0.5f) ? -1 : 1;
	temp_y *= (CCRANDOM_0_1() > 0.5f) ? -1 : 1;

	//CCLog("-----rend_x----===  (%f)-----rend_y----===  (%f)", temp_x, temp_y);

	return anchor_point + Point(temp_x, temp_y);
}

bool compareString(const char* src, const char* dst)
{
	int n_len1 = strlen(src);
	int n_len2 = strlen(dst);

	if (n_len1 != n_len2)
	{
		return false;
	}
    return strncmp(src, dst, n_len1) == 0 ? true : false;
}

Vec2 getMappingPoint(Vec2 pCenter, Vec2 pOne, float angle)
{
	//先算出运来的与水平角度的夹角
	float fAngle = calcAngleFromOriginToTarget(pCenter, pOne);

	//两点之间的距离就是半径
	float fRadius = calcTwoPointLength(pCenter, pOne);

	float temp_x = fRadius * cos(CC_DEGREES_TO_RADIANS(fAngle + angle));
	float temp_y = fRadius * sin(CC_DEGREES_TO_RADIANS(fAngle + angle));

	return Point(pCenter.x + temp_x, pCenter.y + temp_y);
}

//获取系统当前时间（单位：毫秒数）
long GetTime() 
{ 
	struct tm *tm; 
	time_t timep; 
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
	time(&timep); 
#else  
	struct timeval now;
	gettimeofday(&now, NULL);
	timep = now.tv_sec; 
#endif  


	tm = localtime(&timep); 
	int year = tm->tm_year + 1900; 
	int month = tm->tm_mon + 1; 
	int day = tm->tm_mday; 
	int hour=tm->tm_hour; 
	int min=tm->tm_min; 
	int second=tm->tm_sec; 

	//换算成秒来存储
	long temp = second+min*60+hour*3600+day*3600*24+month*3600*24*31+year*3600*24*31*365;
	return temp;
	return 0;
}

tm* GetTime1()
{
	struct tm *tm; 
	time_t timep; 
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
	time(&timep); 
#else  
	struct timeval now;
	gettimeofday(&now, NULL);  
	timep = now.tv_sec; 
#endif  

	tm = localtime(&timep); 
	return tm;
}

//根据图片和指定的的色相和饱和度，创建新的图片
Sprite* getGrayLightWithSprite(Sprite* oldsprite, int is_plus, int saturation)
{
	Sprite * new_sprite = NULL;
	if(!oldsprite)
	{
		return new_sprite;
	}

	Point posOldAnchorPoint = oldsprite->getAnchorPoint();
	oldsprite->setAnchorPoint(Point(0.5f, 0.5f));
	//Sprite转成CCimage
	RenderTexture *out_texture = RenderTexture::create(Director::getInstance()->getWinSize().width, 
		Director::getInstance()->getWinSize().height);

	if(!out_texture)
	{
		return new_sprite;
	}

	const Point pos = Point(100, 0);
	out_texture->getSprite()->getTexture()->drawAtPoint(pos);
	out_texture->begin();
	oldsprite->visit();
	out_texture->end();
	oldsprite->setAnchorPoint(posOldAnchorPoint);

	Image* final_image = out_texture->newImage();
	if(!final_image)
	{
		return new_sprite;
	}
	unsigned char * pData = final_image->getData();
	int iIndex = 0;

	if(is_plus != 0) 
	{
		for (int i = 0; i < final_image->getHeight(); i ++) 
		{
			for (int j = 0; j < final_image->getWidth(); j ++) 
			{
				// highlight
				int iBPos = iIndex;
				int iB = pData[iIndex];
				iIndex ++;
				int iG = pData[iIndex];
				iIndex ++;
				int iR = pData[iIndex];
				iIndex ++;
				//                unsigned int o = pData[iIndex];
				iIndex ++;

				SetHueAndSaturation(&iR, &iG, &iB, 0 + 136, saturation);

				pData[iBPos] = (unsigned char)iR;
				pData[iBPos + 1] = (unsigned char)iG;
				pData[iBPos + 2] = (unsigned char)iB;
			}
		}
	}
	else
	{
		for (int i = 0; i < final_image->getHeight(); i ++) 
		{
			for (int j = 0; j < final_image->getWidth(); j ++) 
			{
				// gray
				int iBPos = iIndex;
				unsigned int iB = pData[iIndex];
				iIndex ++;
				unsigned int iG = pData[iIndex];
				iIndex ++;
				unsigned int iR = pData[iIndex];
				iIndex ++;
				//                unsigned int o = pData[iIndex];
				iIndex ++;
				unsigned int iGray = 0.3 * iR + 0.4 * iG + 0.2 * iB;
				pData[iBPos] = pData[iBPos + 1] = pData[iBPos + 2] = (unsigned char)iGray;
			}
		}
	}

	Texture2D *texture = new Texture2D();
	if(!texture)
	{
		return new_sprite; 
	}

	texture->initWithImage(final_image);

	Vec2 posPosition = oldsprite->getParent()->convertToWorldSpace(oldsprite->getPosition());
	Sprite* newSprite = Sprite::createWithTexture(texture);
	oldsprite->setContentSize(oldsprite->getContentSize());
	newSprite->setTextureRect(Rect(posPosition.x - oldsprite->getAnchorPoint().x * oldsprite->getContentSize().width, 
		Director::getInstance()->getWinSize().height - posPosition.y - oldsprite->getAnchorPoint().y * oldsprite->getContentSize().height, 
		oldsprite->getContentSize().width, 
		oldsprite->getContentSize().height));
	newSprite->setAnchorPoint(oldsprite->getAnchorPoint());
	delete final_image;
	texture->release();

	return newSprite;
}
//设置色相和饱和度
void SetHueAndSaturation(int *R, int *G, int *B, int hValue, int sValue)
{
	int intR = *R;
	int intG = *G;
	int intB = *B;

	if (intR < intG)
		SwapRGB(&intR, &intG);
	if (intR < intB)
		SwapRGB(&intR, &intB);
	if (intB > intG)
		SwapRGB(&intB, &intG);

	int delta = intR - intB;
	if (!delta) return;

	int entire = intR + intB;
	int H, S, L = entire >> 1;  //右移一位其实就是除以2（很巧妙）
	if (L < 128)
		S = delta * 255 / entire;
	else
		S = delta * 255 / (510 - entire);

	if (hValue)
	{
		if (intR == *R)
			H = (*G - *B) * 60 / delta;
		else if (intR == *G)
			H = (*B - *R) * 60 / delta + 120;
		else
			H = (*R - *G) * 60 / delta + 240;
		H += hValue;
		if (H < 0) H += 360;
		else if (H > 360) H -= 360;
		int index = H / 60;
		int extra = H % 60;
		if (index & 1) extra = 60 - extra;
		extra = (extra * 255 + 30) / 60;
		intG = extra - (extra - 128) * (255 - S) / 255;
		int Lum = L - 128;
		if (Lum > 0)
			intG += (((255 - intG) * Lum + 64) / 128);
		else if (Lum < 0)
			intG += (intG * Lum / 128);
		CheckRGB(&intG);
		switch (index)
		{
		case 1:
			SwapRGB(&intR, &intG);
			break;
		case 2:
			SwapRGB(&intR, &intB);
			SwapRGB(&intG, &intB);
			break;
		case 3:
			SwapRGB(&intR, &intB);
			break;
		case 4:
			SwapRGB(&intR, &intG);
			SwapRGB(&intG, &intB);
			break;
		case 5:
			SwapRGB(&intG, &intB);
			break;
		}
	}
	else
	{
		intR = *R;
		intG = *G;
		intB = *B;
	}
	if (sValue)
	{
		if (sValue > 0)
		{
			sValue = sValue + S >= 255? S: 255 - sValue;
			sValue = 65025 / sValue - 255;
		}
		intR += ((intR - L) * sValue / 255);
		intG += ((intG - L) * sValue / 255);
		intB += ((intB - L) * sValue / 255);
		CheckRGB(&intR);
		CheckRGB(&intG);
		CheckRGB(&intB);
	}
	AssignRGB(R, G, B, intR, intG, intB);
}
void SwapRGB(int *a, int *b)
{
	*a+=*b;
	*b=*a-*b;
	*a-=*b;
}
//范围
void CheckRGB(int *Value)
{
	if (*Value < 0) *Value = 0;
	else if (*Value > 255) *Value = 255;
}
//赋值
void AssignRGB(int *R, int *G, int *B, int intR, int intG, int intB)
{
	*R = intR;
	*G = intG;
	*B = intB;
}
//设置明亮
void SetBright(int *R, int *G, int *B, int bValue)
{
	int intR = *R;
	int intG = *G;
	int intB = *B;
	if (bValue > 0)
	{
		intR = intR + (255 - intR) * bValue / 255;
		intG = intG + (255 - intG) * bValue / 255;
		intB = intB + (255 - intB) * bValue / 255;
	}
	else if (bValue < 0)
	{
		intR = intR + intR * bValue / 255;
		intG = intG + intG * bValue / 255;
		intB = intB + intB * bValue / 255;
	}
	CheckRGB(&intR);
	CheckRGB(&intG);
	CheckRGB(&intB);
	AssignRGB(R, G, B, intR, intG, intB);
}


void graySprite(Node* sprite)  
{  
	if(sprite)
	{
		GLProgram * p = new GLProgram();
		p->initWithFilenames("shader/gray.vsh", "shader/gray.fsh");
		p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
		p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
		p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
		p->link();
		p->updateUniforms();
		sprite->setShaderProgram(p);
	}  
}

std::string switchIntToString(int num)
{
	if(num == 0)
	{
		return "a";
	}
	int nNum = num;
	std::string str = "";
	while (nNum)
	{
		int n = nNum%10;
		if(n == 0)
			str += "!";
		else if(n == 1)
			str += "@";
		else if(n == 2)
			str += "#";
		else if(n == 3)
			str += "$";
		else if(n == 4)
			str += "%";
		else if(n == 5)
			str += "^";
		else if(n == 6)
			str += "&";
		else if(n == 7)
			str += "*";
		else if(n == 8)
			str += "(";
		else if(n == 9)
			str += ")";

		nNum /= 10;
	}
	return str.c_str();
}
int switchStringToInt(std::string p)
{
	int nNum = 0;
	int nRate = 1;
	for (int i = 0; i < p.length(); i++)
	{
		char c = p[i];
		int nInt = 0;
		if(c == '!')
			nInt = 0;
		else if(c == '@')
			nInt = 1;
		else if(c == '#')
			nInt = 2;
		else if(c == '$')
			nInt = 3;
		else if(c == '%')
			nInt = 4;
		else if(c == '^')
			nInt = 5;
		else if(c == '&')
			nInt = 6;
		else if(c == '*')
			nInt = 7;
		else if(c == '(')
			nInt = 8;
		else if(c == ')')
			nInt = 9;
		else
			return 0;


		nNum += (nInt * pow(10, i));
	}
	return nNum;
}

// 获取友盟渠道名
std::string getUmengChannelString()
{
	switch (CURRENT_CHANNEL) {
		CHANNEL_NAME(QingNingZhuShou);
		CHANNEL_NAME(WanDouJia);
		CHANNEL_NAME(WiFiWanNengYaoShi);
		CHANNEL_NAME(SouYing);
		CHANNEL_NAME(YouKu);
		CHANNEL_NAME(FengWang);
		CHANNEL_NAME(ChengLongXinXi);
		CHANNEL_NAME(ZhangXingLiYi);
		CHANNEL_NAME(NanJingYouTong);
		CHANNEL_NAME(XuhangWangLuo);
		CHANNEL_NAME(ZongHengTianYa);
		CHANNEL_NAME(FengHuaHeRui);
		CHANNEL_NAME(HuoCheZhanWiFi);
		CHANNEL_NAME(QiFan);
		CHANNEL_NAME(WanJiangPing);
		CHANNEL_NAME(MuMaYi);
		CHANNEL_NAME(IOS);
		CHANNEL_NAME(XY);
        CHANNEL_NAME(GooglePlay);
	case C4399:
		return "4399";
	default:
		return "";
	}
}
