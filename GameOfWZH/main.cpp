/*
*
*		��VS�򿪸ô���һ��Ҫ�޸����ԣ�����
*		��VS�򿪸ô���һ��Ҫ�޸����ԣ�����
*		��VS�򿪸ô���һ��Ҫ�޸����ԣ�����
*		��VS�򿪸ô���һ��Ҫ�޸����ԣ�����
*		��VS�򿪸ô���һ��Ҫ�޸����ԣ�����
*		��VS�򿪸ô���һ��Ҫ�޸����ԣ�����
*		��VS�򿪸ô���һ��Ҫ�޸����ԣ�����
*
* 							�J��������ϵͳ����ϵͳ������ (/SUBSYSTEM:WINDOWS)
*	�޸ķ�ʽ���Ҽ���Ŀ���������ԡ��߼����ַ�����ʹ�ö��ֽ��ַ���
*							�KC/C++��SDL������
*/
/*
	�����������Ŀ��־
	1.������Ŀ
	2.��װeasyXͼ�ο�//�Ѱ�װ��
	3.������Ϸ����
	4.����ͼƬ��Դ��������Դ
	5.ʵ�ֽ�ɫ�����ƶ�
	6.ʵ��˫�������ƶ�//work���̫�����ˣ����˺þ�
*/
#include<stdio.h>
#include<graphics.h>
#include"tools.h"
#include"Check.h"
#include"checkfiles.h"
#include<conio.h>
#include<vector>
#include<Windows.h>
/*����ͷ�ļ�Ϊʱ�亯������*/
#include <sys/timeb.h>
#include <cstdio>
#include <time.h>
#include <string>
/*����ͷ�ļ�Ϊʱ�亯������*/

using namespace std;//���������ռ�	

#define WIN_WIDTH 600
#define WIN_HEIGHT 700
#define OBSTACLE_COUNT 10
#define RandomNumbersMax 101

IMAGE imgBackGs[4];
IMAGE imgHeros;//�����ɫ����ͼƬ
int heroX, heroY, heroHuY0, heroY0;//��ɫ����
bool heroLeft;//��ɫ�����ƶ�
bool heroRight;//��ɫ�����ƶ�

bool ifupdate;//�Ƿ���Ҫ����ˢ��ҳ��
bool ifPause;//�Ƿ���Ҫ��ͣ
bool keyPProcessed;  // �����������ж�P���Ƿ��Ѿ������
bool keyDProcessed = false;
bool keyAProcessed = false;
bool ifSpeeding = false; // �����Ƿ���Դ�������

int random[RandomNumbersMax];//�������
int randomIndex;

int HP;
int SCORE;
int maxScore;
int level;
bool ifAdded;
IMAGE imgFailures[100];
int EnemyFre0 = 50;
int EnemyFre1 = 51;
int ReadMax;
int dist;

typedef enum
{
	ENERMY1,//����������
	ENERMY2,//�½�������
	HT1,//����1
	XH,//���ͣ����ǻ�Ѫװ��
	OBSTACLE_TYPE_COUNT //����һ�ֱ߽��жϣ��������һ��������Ӧ�����պ����ϰ�������͸���
}obstacle_type;

//��������ϰ����ͼƬ
vector<vector<IMAGE>> obstacleImgs; //�൱��C���Ե� IMAGE obstacleImgs[][];�ŵ������пɱ�

typedef struct obstacle
{
	obstacle_type type;//�ϰ�������ͣ������ϰ���
	int imgIndex;//��ǰ��ʾ��ͼƬ���
	int x, y;//�ϰ��������
	int speed;//�ϰ�����ٶ�
	int power;//��һ���ϰ������ɶ����˺�
	int score;
	bool ifExist;//�Ƿ����
	bool ifhited;//�Ƕ���ײ
	bool ifpassed;//�Ƿ�ͨ��
}obstacle_t;

obstacle_t obstacles[OBSTACLE_COUNT];

clock_t lastDPressTime = 0; // ���ڼ�¼��һ�ΰ���D����ʱ��
clock_t lastAPressTime = 0; // ���ڼ�¼��һ�ΰ���D����ʱ��
const clock_t doublePressInterval = CLOCKS_PER_SEC / 3; // �趨˫�����ʱ�䣬����300ms
//�����Ƕ��岿��-----------------------------------------------------------------------------------------------------------------------------------
//�������߼�����-----------------------------------------------------------------------------------------------------------------------------------

//ʱ�亯������ȡʱ��������λ�����뼶����������������
//int time_interval()
//{
//	struct timeb tv {};
//	struct tm* t;
//
//	ftime(&tv);
//
//	time_t now = tv.time;
//	t = localtime(&now);
//	if (t == nullptr) {
//		perror("localtime");
//		return -1;
//	}
//
//	//printf("time_now: %d-%d-%d %d:%d:%d.%d\n", 1900+t->tm_year, 1+t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, tv.millitm);
//	return tv.millitm % 100;
//}

//�����ϰ��д���������ʱ����������붼�Ѿ��Ƚ���Ϥ�ˣ�û��ôдע��
//void createObstacle()
//{
//	int i;
//	for (i = 0; i < OBSTACLE_COUNT; i++)
//	{
//		if (obstacles[i].ifExist == false)
//		{
//			break;
//		}
//	}
//	if (i >= OBSTACLE_COUNT) return;
//
//	obstacles[i].ifExist = true;
//	obstacles[i].ifhited = false;
//	obstacles[i].ifpassed = false;
//	obstacles[i].imgIndex = 0;
//	if (randomIndex >= RandomNumbersMax) randomIndex = 0;
//	random[randomIndex] = time_interval();
//	obstacles[i].type = (obstacle_type)(random[randomIndex++] % (OBSTACLE_TYPE_COUNT - 0));//�Ѷ�����ӷŵ�һ�𣬲�Ȼ���ӵĳ��ָ��ʻ����
//	obstacles[i].x = WIN_WIDTH;
//	obstacles[i].y = 360 - obstacleImgs[obstacles[i].type][0].getheight();
//	if (obstacles[i].type == TORTOISE)
//	{
//		obstacles[i].speed = 1;
//		obstacles[i].power = 5 + level * 5;
//		obstacles[i].score = 5;
//	}
//	else if (obstacles[i].type == LION)
//	{
//		obstacles[i].speed = 4;
//		obstacles[i].power = 15 + level * 5;
//		obstacles[i].score = 11;
//	}
//	else if (obstacles[i].type == ROLLINGTORT)
//	{
//		obstacles[i].speed = 2;
//		obstacles[i].power = 6 + level * 5;
//		obstacles[i].score = 6;
//	}
//	else if (obstacles[i].type >= HOOK1 && obstacles[i].type <= HOOK2)
//	{
//		obstacles[i].speed = 0;
//		obstacles[i].power = 21 + level * 5;
//		obstacles[i].y = 0;
//		obstacles[i].score = 12;
//	}
//}

//��ײ���
//void checkHit()
//{
//	for (int i = 0; i < OBSTACLE_COUNT; i++)
//	{
//		if (obstacles[i].ifExist && !obstacles[i].ifhited)
//		{
//			MyTriangle HeroLegs;
//			MyRectangle HeroBody, ObstacleBody;
//			int a1x, a1y, a2x, a2y;//��ɫ���������������Ϣ
//			if (!ifHeroSquating && !isHeroJumping)//���ܵļ��
//			{
//				HeroBody.x = heroX + 119;
//				HeroBody.y = heroY + 38;
//				a2x = heroX + imgHeros[i].getwidth() - 50;
//				a2y = heroY + imgHeros[i].getheight() - 75;
//				HeroBody.width = a2x - HeroBody.x;
//				HeroBody.height = a2y - HeroBody.y;
//				//���������岿�ֵľ��μ���
//				//�������ǵ��������ֱ��ʹ��heroX��heroY�����λ�ã�����ֱ���þͺ�
//
//				HeroLegs.x1 = heroX + heroLegsL[heroIndex][0];
//				HeroLegs.y1 = heroY + heroLegsL[heroIndex][1];
//				HeroLegs.x2 = heroX + heroLegsL[heroIndex][2];
//				HeroLegs.y2 = heroY + heroLegsL[heroIndex][3];
//				HeroLegs.x3 = heroX + heroLegsL[heroIndex][4];
//				HeroLegs.y3 = heroY + heroLegsL[heroIndex][5];
//
//			}
//			if ((isHeroJumping || !isHeroGrounded) && !ifHeroSquating)//����
//			{
//				HeroBody.x = heroX + 91;
//				HeroBody.y = heroY + 26;
//				a2x = heroX + imgHeros[i].getwidth() - 41;
//				a2y = heroY + imgHeros[i].getheight() - 74;
//				HeroBody.width = a2x - HeroBody.x;
//				HeroBody.height = a2y - HeroBody.y;
//
//				HeroLegs.x1 = heroX + heroLegsL[heroIndex][0];
//				HeroLegs.y1 = heroY + heroLegsL[heroIndex][1];
//				HeroLegs.x2 = heroX + heroLegsL[heroIndex][2];
//				HeroLegs.y2 = heroY + heroLegsL[heroIndex][3];
//				HeroLegs.x3 = heroX + heroLegsL[heroIndex][4];
//				HeroLegs.y3 = heroY + heroLegsL[heroIndex][5];
//			}
//			if (ifHeroSquating)//����S��
//			{
//				HeroBody.x = squatX + 5;
//				HeroBody.y = squatY + 3;
//				a2x = squatX + imgHeroSquat[squatImgNumbers - 1].getwidth() - 10;
//				a2y = squatY + imgHeroSquat[squatImgNumbers - 1].getheight() - 6;
//				HeroBody.width = a2x - HeroBody.x;
//				HeroBody.height = a2y - HeroBody.y;
//			}
//			//�ϰ���ľ���������Ϣ
//			IMAGE img = obstacleImgs[obstacles[i].type][obstacles[i].imgIndex];
//			int b1x = obstacles[i].x + 5;
//			int b1y = obstacles[i].y + 5;
//			int b2x = obstacles[i].x + img.getwidth() - 5;
//			int b2y = obstacles[i].y + img.getheight();
//			ObstacleBody.x = b1x;
//			ObstacleBody.y = b1y;
//			ObstacleBody.width = b2x - b1x;
//			ObstacleBody.height = b2y - b1y;
//
//			if (checkRectangleCollision(HeroBody, ObstacleBody))//�ǵ���ӡ������Ȳ���ײ��
//			{
//				HP -= obstacles[i].power;
//				obstacles[i].ifhited = true;
//				if (HP > 0)
//				{
//					mciSendString("play res/hit.mp3", 0, 0, 0);
//				}
//			}
//			if (!ifHeroSquating && checkTriangleRectangleCollision(HeroLegs, ObstacleBody))
//			{
//				HP -= obstacles[i].power;
//				obstacles[i].ifhited = true;
//				if (HP > 0)
//				{
//					mciSendString("play res/hit.mp3", 0, 0, 0);
//				}
//			}
//		}
//		/*if (obstacles[i].ifExist && obstacles[i].ifhited)
//		{
//			putimagePNG2()
//		}*/
//	}
//}

//�ӷּ��
//void checkScore()
//{
//	for (int i = 0; i < OBSTACLE_COUNT; i++)
//	{
//		if (obstacles[i].ifExist && !obstacles[i].ifpassed && !obstacles[i].ifhited)
//		{
//			if (obstacles[i].x + obstacleImgs[obstacles[i].type][0].getwidth() < heroX)
//			{
//				SCORE += obstacles[i].score;
//				maxScore += obstacles[i].score;
//				obstacles[i].ifpassed = true;
//				//printf("%d\n", SCORE);
//			}
//		}
//		if (SCORE >= 100)
//		{
//			SCORE = 0;
//			level++;
//			ifAdded = false;
//		}
//		if (level >= 0 && !ifAdded)
//		{
//			for (int j = 0; j < 3; j++)
//			{
//				EnemyFre0 -= (level * 3);
//				if (EnemyFre0 <= 25) EnemyFre0 = 25;
//				EnemyFre1 -= (level * 2);
//				if (EnemyFre1 < 30) EnemyFre1 = 30;
//				bgflySpeed[j] += level;
//			}
//			ifAdded = true;
//		}
//	}
//}

//��Ⱦ��Ϸ����
void updateBackGroud()
{
	putimagePNG2(0, 0, WIN_WIDTH, &imgBackGs[0]);
}

//�������Ƿ���Ҫ��;�˳�
void ifWannaExit()
{
	if (HP > 0)
	{
		if (IDYES == MessageBoxA(nullptr, "��Ϸδ���棬�Ƿ��˳���", "ע��", MB_YESNO))
		{
			exit(0);
		}
	}
	else
	{
		if (IDYES == MessageBoxA(nullptr, "�����ѱ��棬�Ƿ������˳���", "DailyRunningNG", MB_YESNO))
		{
			exit(0);
		}
	}
}

void speedUpHero() 
{
	if (ifSpeeding) 
	{
		dist *= 2; // Double the speed
	}
	else dist = 5;
}

//��������
void keyEvent()
{
	//���һ�İ����¼�
	bool keyAPressed = (GetAsyncKeyState('A') & 0x8000) != 0;  // ���һ����
	bool keyDPressed = (GetAsyncKeyState('D') & 0x8000) != 0;  // ���һ����

	//��������
	bool keyEscPressed = (GetAsyncKeyState(VK_ESCAPE) & 0x8000) != 0;//Esc�˳���
	bool keyPPressed = (GetAsyncKeyState('P') & 0x8000) != 0;

	if (keyEscPressed)
	{
		ifWannaExit();
	}

	//�����Ǽ��P���İ��£�����GetAsyncKeyState��������ⵥ�ΰ���״̬�����԰��º͵�����true������Ҫ��һ��keyProcessed���жϰ��º͵���
	if (keyPPressed && !keyPProcessed)
	{
		// ��P����������֮ǰδ����ʱ���л���ͣ״̬
		ifPause = !ifPause;
		keyPProcessed = true;  // ���Ϊ�Ѵ���
	}
	else if (!keyPPressed && keyPProcessed)
	{
		// ��P�����ͷ���֮ǰ�Ѵ���ʱ�����ô����־
		keyPProcessed = false;
	}

	//A��D��������Pһ��
	if (keyAPressed)
	{
		heroLeft = true;
	}
	if (keyDPressed)
	{
		heroRight = true;
	}

	// ����D��
	if (keyDPressed && !keyDProcessed)
	{
		clock_t currentTime = clock();
		clock_t interval = currentTime - lastDPressTime;

		if (lastDPressTime != 0 && interval < doublePressInterval)
		{
			ifSpeeding = true;
			lastDPressTime = 0; // ��ֹ�ظ���������
		}
		else
		{
			lastDPressTime = currentTime;
		}
		speedUpHero();
		keyDProcessed = true;
	}
	else if (!keyDPressed && keyDProcessed)
	{
		ifSpeeding = false;
		speedUpHero();
		keyDProcessed = false;
	}

	// ����A��
	if (keyAPressed && !keyAProcessed)
	{
		clock_t currentTime = clock();
		clock_t interval = currentTime - lastAPressTime;

		if (lastAPressTime != 0 && interval < doublePressInterval)
		{
			ifSpeeding = true;
			lastAPressTime = 0; // ��ֹ�ظ���������
		}
		else
		{
			lastAPressTime = currentTime;
		}
		speedUpHero();
		keyAProcessed = true;
	}
	else if (!keyAPressed && keyAProcessed)
	{
		ifSpeeding = false;
		speedUpHero();
		keyAProcessed = false;
	}
}

//���µз�
//void updateEnemy()
//{
//	//�����ϰ���
//	for (int i = 0; i < OBSTACLE_COUNT; i++)
//	{
//		if (obstacles[i].ifExist)
//		{
//			putimagePNG2(obstacles[i].x, obstacles[i].y, WIN_WIDTH,
//				&obstacleImgs[obstacles[i].type][obstacles[i].imgIndex]);
//		}
//	}
//}

//���½�ɫ
void updateHeros()
{
		putimagePNG2(heroX, heroY, &imgHeros);//�����ɫ�ı��ܺ���Ծ
	//}
	//if (ifHeroSquating)//���ӵ�������㣬�����������¾Ͳ���Ҫ������������
	//{
	//	if (heroIndex < squatImgNumbers - 6)
	//	{
	//		putimagePNG2(heroX, heroY, &imgHeros[heroIndex]);
	//		putimagePNG2(heroX, heroY, &imgHeroSquat[heroIndex]);
	//	}
	//	else if (heroIndex < squatImgNumbers - 4)
	//	{
	//		putimagePNG2(heroX, heroY, &imgHeroSquat[heroIndex]);
	//	}
	//	else if (heroIndex < squatImgNumbers - 1)
	//	{
	//		int y = heroY + imgHeros[heroIndex].getheight() - imgHeroSquat[heroIndex].getheight();
	//		int x = heroX + imgHeros[0].getwidth() * 0.5 - imgHeroSquat[squatImgNumbers - 1].getwidth() * 0.5;
	//		int xChannel = heroX + imgHeros[0].getwidth() * 0.5 - imgHeroSquat[heroIndex].getwidth() * 0.5;
	//		int yChannel = heroY + imgHeros[heroIndex].getheight() - imgHeroSquat[heroIndex].getheight();
	//		putimagePNG2(x, y, &imgHeroSquat[squatImgNumbers - 1]);
	//		putimagePNG2(xChannel, yChannel, &imgHeroSquat[heroIndex]);
	//	}
	//	else
	//	{
	//		int y = heroY + imgHeros[heroIndex].getheight() - imgHeroSquat[heroIndex].getheight();
	//		int x = heroX + imgHeros[0].getwidth() * 0.5 - imgHeroSquat[heroIndex].getwidth() * 0.5;
	//		putimagePNG2(x, y, &imgHeroSquat[heroIndex]);
	//		squatX = x;
	//		squatY = y;
	//	}
	//}
}

//����Ѫ���ͷ�����
//void updateBloodBar()
//{
//	drawBloodBar(10, 10, 200, 10, 2, BLUE, DARKGRAY, RED, HP / 100.0);//Ѫ��
//	drawBloodBar(10, 30, 200, 10, 2, BLUE, DARKGRAY, YELLOW, SCORE / 100.0);//������
//
//	//���·�����ʾ
//	setbkmode(TRANSPARENT);
//	settextcolor(BLACK);
//
//	//��ǰ�ܷ�
//	TCHAR strPresentScore[] = _T("��ǰ�ܷ֣�");
//	outtextxy(WIN_WIDTH + 10, 120, strPresentScore);
//	TCHAR strScore[500];
//	_stprintf(strScore, _T("%d"), maxScore);
//	outtextxy(WIN_WIDTH + 90, 121, strScore);
//
//	//��ʷ��߷�
//	TCHAR strHistoryMaxScore[] = _T("��ʷ��߷֣�");
//	outtextxy(WIN_WIDTH + 10, 139, strHistoryMaxScore);
//	TCHAR strScoreRead[500];
//	_stprintf(strScoreRead, _T("%d"), ReadMax);
//	outtextxy(WIN_WIDTH + 102, 140, strScoreRead);
//
//}

//�������
//void checkHP()
//{
//	if (HP <= 0)
//	{
//		int grade = (maxScore >= ReadMax ? maxScore : ReadMax);
//		WriteGrade(grade);
//		mciSendString("stop res/traveling.mp3", 0, 0, 0);
//
//		char name[64] = { 0 };
//		int i = 0, j = 0;
//		int timers = 0;
//		int Tmax = 15;
//		mciSendString("play res/failure.mp3", 0, 0, 0);
//		while (i < 5446)
//		{
//			keyEvent();//�������뺯��
//			timers += getDelay();//���ϵײ��߼�������λʱ��
//			if (timers >= Tmax)
//			{
//				timers = 0;
//				sprintf(name, "res/failure/%04d.jpg", i + 1);
//				j = i % 10;//j��0��9
//				loadimage(&imgFailures[j], name);
//				putimage(0, 0, &imgFailures[j]);
//				i++;
//			}
//		}
//
//		mciSendString("stop res/failure.mp3", 0, 0, 0);
//		//putimage(0, 0, &imgFailures[0]);
//		ifWannaExit();
//	}
//}

//������һ֡ͼƬ������
void fly()
{
	//��ײ���
	//checkHit();

	//��ɫ���ұ仯
	if (heroLeft && !heroRight && heroX > 0)
	{
		heroX -= dist;
		if (heroX < 0)
		{
			heroX += dist;
		}
		heroLeft = false;
	}
	if (!heroLeft && heroRight)
	{
		heroX += dist;
		if (heroX >= WIN_WIDTH - imgHeros.getwidth())
		{
			heroX = WIN_WIDTH - imgHeros.getwidth();
		}
		heroRight = false;
	}
	if (heroLeft && heroRight)
	{
		heroLeft = false;
		heroRight = false;
	}
	
	//��ʾ�ϰ���ı仯
	//static int frameCount = 0;//���徲̬���������ϰ������
	//static int EnemyFre = 70;//ÿ������֡

	//frameCount++;//��һ�����ǵڼ�֡
	//if (frameCount > EnemyFre)
	//{
	//	frameCount = 0;
	//	if (randomIndex >= RandomNumbersMax) randomIndex = 0;
	//	random[randomIndex] = time_interval();
	//	EnemyFre = EnemyFre0 + random[randomIndex++] % EnemyFre1;
	//	createObstacle();
	//}
	////�����ϰ��������
	//for (int i = 0; i < OBSTACLE_COUNT; i++)
	//{
	//	if (obstacles[i].ifExist)
	//	{
	//		obstacles[i].x -= (bgflySpeed[2] + obstacles[i].speed);
	//		if (obstacles[i].x < -obstacleImgs[obstacles[i].type][obstacles[i].imgIndex].getwidth() * 2)
	//		{
	//			obstacles[i].ifExist = false;
	//		}
	//		int len = obstacleImgs[obstacles[i].type].size();
	//		obstacles[i].imgIndex = (obstacles[i].imgIndex + 1) % len;
	//	}
	//}
}

//��Ϸ��ʼ��
void init()
{
	MessageBoxA(0, "���ɳ�����Ϸ������Ϸ����һ������д�ģ�������п��ܻ�����쳣��", "�Ҹ�", MB_OK);

	//������Ϸ����
	initgraph(WIN_WIDTH, WIN_HEIGHT, EX_DBLCLKS);

	char name[64] = { 0 };

	//���޳�ʼ����
	////��ʼ����
	//preLoadSound("res/beginning.mp3");
	//mciSendString("play res/beginning.mp3", 0, 0, 0);

	//��ʱ�������Դ
	////�����Դ������
	char tempName[64] = { 0 };

	//mciSendString("stop res/beginning.mp3", 0, 0, 0);

	//���ر�����Դ
	sprintf(name, "res/bg.png");
	loadimage(&imgBackGs[0], name, WIN_WIDTH, WIN_HEIGHT);

	//���ؽ�ɫ
	sprintf(name, "res/zl.png");
	loadimage(&imgHeros, name);
	char relativePath[100] = ".\\res";  // ���·��
	char filename[64] = { 0 };  // Ҫ�����ļ���
	sprintf(filename, "zl.png");
	if (!isFileExists(relativePath, filename))
	{
		MessageBoxA(nullptr, "��Դ���ش�������ϵ����Ա�����Դ�����ԣ������룺0x1110Failure", "����", MB_OK);
		exit(0);
	}

	//��ʼ����ɫλ����Ϣ
	heroX = WIN_WIDTH * 0.5 - imgHeros.getwidth() * 0.5;
	heroY = WIN_HEIGHT * 0.5 - imgHeros.getheight() * 0.5;

	ifupdate = true;//һ��ʼ����Ҫˢ�µ�

	//��װ�����ϰ���
	////�����ϰ���1-�ڹ�1
	//IMAGE imgTort;
	//loadimage(&imgTort, "res/t1.png");
	//vector<IMAGE> imgTortArray;
	//imgTortArray.push_back(imgTort);
	//obstacleImgs.push_back(imgTortArray);
	////�����ϰ���2-ʨ��
	//IMAGE imgLion;
	//vector<IMAGE> imgLionArray;
	//for (int i = 0; i < 6; i++)
	//{
	//	sprintf(name, "res/p%d.png", i + 1);
	//	loadimage(&imgLion, name);
	//	imgLionArray.push_back(imgLion);
	//}
	//obstacleImgs.push_back(imgLionArray);
	////�����ϰ���3-��ת���ڹ�
	//IMAGE imgRollingTort;
	//vector<IMAGE> imgRollingTortArray;
	//for (int i = 0; i < 6; i++)
	//{
	//	sprintf(name, "res/t%d.png", i + 2);
	//	loadimage(&imgRollingTort, name);
	//	imgRollingTortArray.push_back(imgRollingTort);
	//}
	//obstacleImgs.push_back(imgRollingTortArray);
	////�����ϰ���4-����
	//IMAGE imgHOOK;
	//for (int i = 0; i < 4; i++)
	//{
	//	vector<IMAGE> imgHookArray;
	//	sprintf(name, "res/h%d.png", i + 1);
	//	loadimage(&imgHOOK, name, 63, 260, true);
	//	imgHookArray.push_back(imgHOOK);
	//	obstacleImgs.push_back(imgHookArray);
	//}
	////��ʼ���ϰ����
	//for (int i = 0; i < OBSTACLE_COUNT; i++)
	//{
	//	obstacles[i].ifExist = false;
	//	obstacles[i].ifhited = false;
	//}

	ifPause = false;
	keyPProcessed = false;

	for (int i = 0; i < 100; i++) random[i] = 0;
	randomIndex = 0;

	HP = 100;
	SCORE = 0;
	level = 0;
	maxScore = 0;
	heroLeft = false;
	heroRight = false;
	ifAdded = true;
	ReadMax = ReadGrade();
	dist = 5;

	//Ԥ������Ч
	/*preLoadSound("res/hit.mp3");
	preLoadSound("res/failure.mp3");*/
	cleardevice(); // �����Ļ����
	//��ʼ��Ϸ����ʵ����loading�ļ�ͼ��Ŀ�������������ϷҪ��ʼ�ˣ�����2�����ҵķ�Ӧʱ��
	//sprintf(tempName, "res/start.png");
	//loadimage(&imgCHECKING, tempName);
	//for (int i = 0; i < 200; i++)
	//{
	//	BeginBatchDraw();//��ʼ��Ⱦ
	//	putimagePNG2((WIN_WIDTH + 196) / 2.0 - imgCHECKING.getwidth() / 2.0, WIN_HEIGHT - 100, &imgCHECKING);
	//	drawBloodBar((WIN_WIDTH + 196) / 2.0 - 200, WIN_HEIGHT - 70, 400, 5, 2, BLACK, DARKGRAY, WHITE, 1);
	//	EndBatchDraw();//����������Ⱦ
	//	Sleep(10);
	//}
	/*preLoadSound("res/traveling.mp3");
	mciSendString("play res/traveling.mp3 repeat", 0, 0, 0);*/
}

//������
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
{
	init();//����������

	int timer = 0;//ʹ�ü�ʱ���Ż�֡
	while (1)//ÿ��ѭ����һ֡
	{
		keyEvent();//�������뺯��
		timer += getDelay();//���ϵײ��߼�������λʱ��
		if ((timer > 30 || ifupdate) && !ifPause)
		{
			timer = 0;
			ifupdate = false;

			BeginBatchDraw();//�滭����
			updateBackGroud();//���±���
			//system("pause");//��������
			updateHeros();
			//updateEnemy();
			//updateBloodBar();
			//printf("%d\n", heroIndex);//��������
			EndBatchDraw();//����������Ⱦ
			//checkHP();
			//checkScore();
			fly();//�����ƶ���Ϊ��һ����Ծ��׼����
		}
	}
	return 0;
}