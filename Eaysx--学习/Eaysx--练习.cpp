
//90̹�˴�ս
//��ʼ����(650*650����)
//ʤ�����
//�����ҿ��Ƶ�̹��
//���������
//�����ͼ���

#include<graphics.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<assert.h>

#pragma comment(lib,"Winmm.lib")
#include<mmsystem.h>

enum basic {
	width = 650,//��ʼ������
	length = 650,//��ʼ���泤��
	p_size = 25,//����ͼ��λ
	enemy_num = 8,//������,
	blood = 5,
	W_wall = 1,//��ǽ�����
	S_wall = 2,//��ǽ�����
	STRH_wall = 3,//��Ӫ���ʹ���
	P_wall = 4,//�������
	E_wall = 5,//��������
	Mag = 1,//�˺�
	P_fire_stop=4,//��ҿ����̹���ж���ȴʱ��
	G_width=800,//��Ϸ������
	G_length=650,//��Ϸ���泤��
};
#define PI 3.1415

//��ʼ����ľ�������
int R1[4] = { 104,279,264,339 };
int R2[4] = { 404,279,564,339 };
//��ʼ�����,��ӪѪ��
int P_Blood;
int PS_Blood;
int cont;//ȫ�ֵ�����
short G_Map[width / p_size][length / p_size] = { 0 };

struct Unity
{
	Unity* father;
	int xy[2];
	int S;//�ܼ�
	int H;//������·������
	int G;//�ƶ�����
};
struct QO1
{
	Unity* o[100];
	int max;
};
struct QO2
{
	Unity* o[200];
	int max;
};
class bullet
{
private:
	int z = 0;//�˺�
	int C = 0;
	IMAGE BN, BS, BW, BE, BZ;
	bool PD = 0;
public:
	int xpos = 0;
	int ypos = 0;
	int s = 0;//���о���
	void init(int x, int y, int c,bool source)
	{
		xpos = x, ypos = y;
		z = Mag;//�˺�����
		C = c;
		PD = source;
		loadimage(&BN, L"�ӵ�.png");
		loadimage(&BS, L"�ӵ�S.png");
		loadimage(&BW, L"�ӵ�L.png");
		loadimage(&BE, L"�ӵ�R.png");
	}
	int sports()//C��ʾ��Ͳ����
	{
		int i = 0;
		if (s < 6)
		{
			switch (C)
			{
			case 0:(i = display(xpos, ypos - 1)) == 0 ? 0 : s = 7; ypos -= 1; break;
			case 1:(i = display(xpos + 1, ypos)) == 0 ? 0 : s = 7; xpos += 1; break;
			case 2:(i = display(xpos, ypos + 1)) == 0 ? 0 : s = 7; ypos += 1; break;
			case 3:(i = display(xpos - 1, ypos)) == 0 ? 0 : s = 7; xpos -= 1; break;
			}
			s++;
		}
		else return 1;//��ʾ���Կ���
		return i;
	}

	int display(int var1, int var2)//x��y
	{
		if (!(var2 >= 0 && var2 < (width / p_size)) && !(var1 < (length / p_size) && var1 >= 0))
			return 1;//��������
		int i = IS_hunrt(var1, var2);//�жϵ�����״̬
		if (!i)//���������ڿյ�����ʱ
		{
			switch (C)
			{
			case 0:putimage(var1 * p_size, var2 * p_size, &BN); break;
			case 1:putimage(var1 * p_size, var2 * p_size, &BE); break;
			case 2:putimage(var1 * p_size, var2 * p_size, &BS); break;
			case 3:putimage(var1 * p_size, var2 * p_size, &BW); break;
			}
		}
		else
		{
			loadimage(&BZ, L"��ը.png");
			putimage(var1 * p_size, var2 * p_size, &BZ);
		}
		return i;//���ص�������Ϣ
	}
	int IS_hunrt(int var1, int var2)
	{
		switch (G_Map[var2][var1])
		{
		case 0:return 0;//�ڿյط��У��ڵ�δ����
		case 1:G_Map[var2][var1] = 0; return 1;//ײ����ǽ��������ǽ
		case 2:return 2;//ײ����ǽ
		case 3:P_Blood -= z; return 3;//���д�Ӫ(�����˴�ӪѪ��)(�����Լ����)
		case 4:P_Blood -= z; return 4;//�������
		case 5:if (PD)cont--; return 5;//���е���(ֻ�������Ч)
		}
		return -1;
	}
};

class Stronghold
{
private:
	IMAGE stronghold;
public:
	int xpos = 0;
	int ypos = 0;
	void init_xy(short** G_Map)
	{
		xpos = rand() % ((length / p_size) - 1);
		ypos = rand() % ((width / p_size) - 1);
		loadimage(&stronghold, L"��Ӫ.jpg");
		*((short*)G_Map + (length / p_size) * (ypos + 1) + xpos) = STRH_wall;
		*((short*)G_Map + (length / p_size) * ypos + xpos) = STRH_wall;
		*((short*)G_Map + (length / p_size) * ypos + (xpos + 1)) = STRH_wall;
		*((short*)G_Map + (length / p_size) * (ypos + 1) + (xpos + 1)) = STRH_wall;
	}
	void put()
	{
		putimage(xpos * p_size, ypos * p_size, &stronghold);
	}
};

class Tank_P
{
private:
	//yunyong
	int xpos = 0;//x����
	int ypos = 0;//y����
	IMAGE TN, TE, TW, TS;
	ExMessage msg = { 0 };
	bullet D;
	
public:
	int Towards = 0;//0��ʾ���ߣ�1��ʾ���ߣ�2��ʾ�ϱߣ�3��ʾ����
	int stop;//Ϩ��
	//�ƶ�����
	void put_image_p()//��ӡͼƬ
	{
		switch (Towards)
		{
		case 0:putimage(xpos * p_size, ypos * p_size, &TN); break;//��
		case 1:putimage(xpos * p_size, ypos * p_size, &TE); break;//��
		case 2:putimage(xpos * p_size, ypos * p_size, &TS); break;//��
		case 3:putimage(xpos * p_size, ypos * p_size, &TW); break;//��
		default: break;
		}
	}

	void set_blocks(short** map)
	{
		*((short*)map + (length / p_size) * ypos + xpos) = P_wall;
	}
	void clear_blocks(short** map)
	{
		*((short*)map + (length / p_size) * ypos + xpos) = 0;
	}

	void IF_H_ob(char I, short** map, int* pos)//�ϰ����
	{
		if (*((short*)map + (length / p_size) * ypos + xpos) != 0)
		{
			switch (I)
			{
			case'+':(*pos) = (*pos) - 1; break;
			case'-':(*pos) = (*pos) + 1; break;
			default:break;
			}
		}
	}
	void move(short** map)//�����ά��ͼ
	{
		clear_blocks(map);
		//��������
		if (((GetKeyState('w') & 0x8000) || (GetKeyState('W') & 0x8000)) && stop == 0)
		{
			ypos -= ypos - 1 < 0 ? 0 : 1;
			Towards = 0; IF_H_ob('-', map, &ypos);
		}
		else if (((GetKeyState('s') & 0x8000) || (GetKeyState('S') & 0x8000)) && stop == 0)
		{
			ypos += ypos + 1 >= (width / p_size) ? 0 : 1, Towards = 2;
			IF_H_ob('+', map, &ypos);
		}
		else if ((GetKeyState('d') & 0x8000) || (GetKeyState('D') & 0x8000) && stop == 0)
		{
			xpos += xpos + 1 >= (length / p_size) ? 0 : 1, Towards = 1;
			IF_H_ob('+', map, &xpos);
		}
		else if ((GetKeyState('a') & 0x8000) || (GetKeyState('A') & 0x8000) && stop == 0)
		{
			xpos -= xpos - 1 < 0 ? 0 : 1; Towards = 3;
			IF_H_ob('-', map, &xpos);
		}
		set_blocks(map);
	}

	int fire(short** map)
	{
		if ((GetKeyState('j') & 0x8000 || GetKeyState('J') & 0x8000) && D.s > 5)
		{
			D.s = 0;
			D.init(xpos, ypos, Towards,1);
			stop = P_fire_stop;
		}
		return D.sports();
	}

	void init_xy(int x, int y)
	{
		xpos = x, ypos = y;
		stop = 0;
		D.s = 8;//����7��ʾ���Կ��𣬵���0��ʾ�տ�����
		loadimage(&TN, L"���.jpg");
		rotateimage(&TE, &TN, -PI / 2);
		rotateimage(&TS, &TN, PI);
		rotateimage(&TW, &TN, (PI / 2));
	}
	int get_D_xpos()
	{
		return D.xpos;
	}
	int get_D_ypos()
	{
		return D.ypos;
	}
	int getxpos()
	{
		return xpos;
	}
	int getypos()
	{
		return ypos;
	}
};

class Tank_C
{
private:
	bullet D;
	IMAGE CN, CE, CS, CW;
	QO1* open;
	int path[150];
	int T = 0;//����path��
public:
	int Towards = 0;//0��ʾ���ߣ�1��ʾ���ߣ�2��ʾ�ϱߣ�3��ʾ����
	int xpos = 0, ypos = 0;
	int stop = 0;
	short key_map[width / p_size][length / p_size] = { 0 };
	QO2* close;

	void init(short** map, int x, int y)
	{
		T = 0; cont++;
		xpos = rand() % (length / p_size), ypos = rand() % (width / p_size);
		D.s = 8;//��������Ϊδ����(s>6)
		Towards = 0;//��ʼ����
		stop = rand() % 5;//��stopΪ0ʱ���ܿ�ʼ�ж�

		//ʹ��ǰ��Ҫ����ʼ������
		loadimage(&CS, L"����.jpg");
		rotateimage(&CN, &CS, PI);
		rotateimage(&CE, &CN, -(PI / 2));
		rotateimage(&CW, &CN, (PI / 2));
		//���������----�����ʹʷϳ�
		//while (G_Map[ypos][xpos]) { xpos = rand() % (length / p_size), ypos = rand() % (width / p_size); }
		//��һ������������
		while (G_Map[ypos][xpos] || !(abs(xpos - x) > 8 && abs(ypos - y) > 8))
		{
			xpos = rand() % (length / p_size), ypos = rand() % (width / p_size);
		}
		//���ý��·���
		set_blocks();
		//��ʼ������
		open = (QO1*)malloc(sizeof(QO1));
		close = (QO2*)malloc(sizeof(QO2));
		if (close) memset(close, 0, sizeof(QO2));
		else assert(close);
		if (open) memset(open, 0, sizeof(QO1));
		else assert(open);
		A_move(x, y);
	}

	void set_blocks()
	{
		G_Map[ypos][xpos] = E_wall;
	}

	void clear_blocks()
	{
		G_Map[ypos][xpos] = 0;
	}

	void put_image()
	{
		switch (Towards)
		{
		case 0:putimage(xpos * p_size, ypos * p_size, &CN); break;//��
		case 1:putimage(xpos * p_size, ypos * p_size, &CE); break;//��
		case 2:putimage(xpos * p_size, ypos * p_size, &CS); break;//��
		case 3:putimage(xpos * p_size, ypos * p_size, &CW); break;//��
		default: break;
		}
	}
	bool IS_out(int i, int j)
	{
		if (xpos + j<0 || xpos + j>(length / p_size));
		if (ypos + i<0 || ypos + i>(width / p_size));
		if ((i + j) == 1 || (i + j) == -1)
			return 1;
		return 0;
	}

	//A*�㷨-----ʵ��AiѰ·
	bool A_move(int x, int y)//����0��ʾ��·��
	{
		memset(path, 0, sizeof(int) * 150);
		memset(key_map, 0, sizeof(short) * (length / p_size) * (width / p_size));
		//(1)�������
		Unity* now = (Unity*)malloc(sizeof(Unity));//���ٿռ�洢����㡯����
		if (!now)assert(now);//�������ʧ�ܱ���
		
		now->xy[0] = xpos, now->xy[1] = ypos, now->G = 0;
		now->father = NULL;
		(open->o[(open->max)++]) = now;//���뵽open���У���ʱmax+1

		bool is_continue = true;
		int  i = 0, index = 0;//i���ڸ���ͳ��(����)��index��������λ��		
		while (is_continue)
		{

			//�ж�open���Ƿ�Ϊ�գ����Ϊ�ձ�ʾ��·��
			if (open->max == 0)return 0;

			//Ѱ�ҵ�S��С�ĸ���
			for (i = 1, now = open->o[0], index = 0; i < open->max; i++)
			{
				if (open->o[i]->S < now->S)
				{
					now = open->o[i];
					index = i;
				}
				else if (open->o[i]->S == now->S)//���ܴ���һ��ʱ,�Ƚ������پ���
				{
					if (open->o[i]->H < now->H)
					{
						now = open->o[i];
						index = i;
					}
				}
			}
			//����ʱ��д��
			//for (i = 1, now = open->o[0], index = 0; i < open->max; i++) { if (open->o[i]->S < now->S) { now = open->o[i]; index = i; } else if (open->o[i]->S == now->S) { if (open->o[i]->H < now->H) { now = open->o[i]; index = i;}}}
			
			//��Ϊȡ�������Ľڵ�Ϊ����Ҫ��open�е�max����������-1
			open->max -= 1;
			//�������ʽڵ����close��,������Է��ʣ���open����ɾ��
			open->o[index] = open->o[open->max];//����βֵ���ǵķ���
			key_map[now->xy[1]][now->xy[0]] = 1;
			close->o[close->max] = now; close->max += 1;

			//������Χ���ĸ���λ	
			for (int H = -1; H < 2; H++)//����չ��
			{
				for (int L = -1; L < 2; L++)
				{
					//�ж��Ƿ�Ϊ�����ϣ���������������
					if (abs(H + L) == 1)
					{
						//�ж��Ƿ���߽�
						if (now->xy[0] + L < (length / p_size) && now->xy[1] + H < (width / p_size) && (now->xy[0] + L >= 0) && (now->xy[1] + H >= 0))
						{
							//�ж��Ƿ�Ϊ�ѷ���
							if (key_map[now->xy[1] + H][now->xy[0] + L] == 0 && is_not_in_open(now->xy[0] + L, now->xy[1] + H))
							{
								open->o[open->max] = (Unity*)malloc(sizeof(Unity));
								open->o[open->max]->father = now;//ָ����һ��
								open->o[open->max]->xy[0] = now->xy[0] + L;
								open->o[open->max]->xy[1] = now->xy[1] + H;
								open->o[open->max]->S = -1;//Ԥ���Ϊ��Ч·��(����õ�����Ч����֮�����串��)

								if (G_Map[now->xy[1] + H][now->xy[0] + L] == 0)//���Ϊ�յ�
								{
									open->o[open->max]->G = now->G + 1;
									open->o[open->max]->H = abs(now->xy[0] + L - x) + abs(now->xy[1] + H - y);
									open->o[open->max]->S = (open->o[open->max]->G) + (open->o[open->max]->H);
									open->max += 1;
								}
								else if (G_Map[now->xy[1] + H][now->xy[0] + L] == 1)//���Ϊ��ǽ����Ҫͨ��������Ҫ������3�����
								{
									open->o[open->max]->G = now->G + 1;
									open->o[open->max]->H = abs(now->xy[0] + L - x) + abs(now->xy[1] + H - y) + 3;
									open->o[open->max]->S = (open->o[open->max]->G) + (open->o[open->max]->H);
									open->max += 1;
								}
								//������յ������Ѱ��
								else if (G_Map[now->xy[1] + H][now->xy[0] + L] == G_Map[y][x])
								{
									H = 2, L + 2;//��ֹѰ�����ܷ���
									is_continue = false;//��ֹ��������
								}
								//��Ȼ��������������õ㱻����̹��ռ��/�õ�Ϊ���ɴݻٸ�ǽ/����Ŀ�����ͣ�\
								  �������Բ�����,�����뵽open���У���ȻҲ�Ͳ���������·��\
								  �������ڲ������max+=1������������һ�β����Ὣ����Ч���񸲸�
							}
						}
					}
				}
			}
		}
		//һ��Ѱ�ҵ��յ㣬��˿̵�open���е����һλ��Ϊ�յ�
		Unity* F = open->o[open->max];
		path[F->father->G + 1] = -1;
		for (; F->father != NULL; F = F->father)
		{
			//0��ʾ���ߣ�1��ʾ���ߣ�2��ʾ�ϱߣ�3��ʾ����
			switch (F->father->xy[0] - F->xy[0])
			{
			case -1:path[F->father->G] = 1; break;
			case 0://��ʾ�ϱ�����
				if (F->father->xy[1] - F->xy[1] == -1)path[F->father->G ] = 2;
				else path[F->father->G] = 0;
				break;
			case 1:path[F->father->G] = 3; break;
			}
		}

		//�ͷŶ�̬�ڴ�
		for (i = 0; i <= open->max; i++)
		{
			free(open->o[i]);
			open->o[i] = NULL;
		}
		open->max = 0;
		for (i = 0; i <= close->max; i++)
		{
			free(close->o[i]);
			close->o[i] = NULL;
		}
		close->max = 0;
		now = NULL; F = NULL; 
		T = 0;
		return 1;
	}

	void move()
	{
		clear_blocks();
		if (path[T] == -1)
			fire(1);
		else if (stop == 0)
		{
			switch (path[T++])
			{
			case 0:Towards = 0;
				if (G_Map[ypos - 1][xpos] == 0) {ypos -= 1; stop = 5;}
				else { fire(1); T -= 1; }
				break;
			case 1:Towards = 1;
				if (G_Map[ypos][xpos + 1] == 0) { xpos += 1; stop = 5;}
				else { fire(1); T -= 1; }
				break;
			case 2:Towards = 2;
				if (G_Map[ypos + 1][xpos] == 0) { ypos += 1; stop = 5; }
				else { fire(1); T -= 1; }
				break;
			case 3:Towards = 3;
				if (G_Map[ypos][xpos - 1] == 0) { xpos -= 1; stop = 5; }
				else { fire(1); T -= 1; }
				break;
			}
		}
		set_blocks();
		put_image(); 
	}

	bool is_not_in_open(int X,int Y)
	{
		int i = 0;
		for (; i < open->max&&!(open->o[i]->xy[0] == X && open->o[i]->xy[1] == Y); i++);
		if (i == open->max)return 1;
		else return 0;
	}

	int fire(bool F)
	{
		if (F && D.s > 6 && stop == 0)
		{
			D.s = 0;
			D.init(xpos, ypos, Towards, 0);//����Ϊx��y���꣬�ڿڳ����ڵ���Դ�Ƿ������
			stop = 8;
		}
		return D.sports();
	}
};


void start_window()
{
	cleardevice();
	//�����ӡ
	settextstyle(100, 0, L"����", 0, 0, FW_BLACK, true, 0, 0);
	settextcolor(YELLOW);
	outtextxy(40, 100, L"̹ �� �� ս");
	//���ζԻ���
	RECT r1[4] = { R1[0],R1[1],R1[2],R1[3] };
	RECT r2[4] = { R2[0],R2[1],R2[2],R2[3] };
	setfillcolor(RGB(105, 217, 78));
	solidrectangle(R1[0], R1[1], R1[2], R1[3]);
	solidrectangle(R2[0], R2[1], R2[2], R2[3]);
	settextstyle(25, 0, L"����");
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	drawtext(L"����˵��", r1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	drawtext(L"��ʼ��Ϸ", r2, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

}

bool LBC_cope(ExMessage msg)
{
	//����������˵��
	if (msg.x >= R1[0] && msg.x <= R1[2] && msg.y >= R1[1] && msg.y <= R1[3])
	{
		IMAGE OPIN;
		loadimage(&OPIN, L"������ť˵��.png");
		//Resize(&OPIN, 300, 211);
		putimage(68, 359, &OPIN);
	}
	else
	{
		//������뿪ʱ��ո�����
		clearrectangle(68, 359, 650, 650);
	}
	return true;
}

bool  LBS_cope(ExMessage msg)
{
	if ((msg.x >= R2[0] && msg.y >= R2[1]) && (msg.x <= R2[2] && msg.y <= R2[3]))
		return 0;
	return 1;
}
//�������
bool W_button()
{
	ExMessage msg;
	bool i = 1;//�ж��Ƿ�ѭ���ڿ�ʼʱ����
	while (i)
	{
		getmessage(&msg, EX_MOUSE | EX_KEY);
		switch (msg.message)
		{
		case WM_KEYDOWN://ESC������
			if (msg.vkcode == VK_ESCAPE)
				return 0;
		case WM_LBUTTONDOWN://����Ҽ�����
			i = LBS_cope(msg);
			break;
		case WM_MOUSEMOVE://����ƶ�����
			i = LBC_cope(msg);
			break;
		default:continue;
		}
		flushmessage();//��Ϊ���������ٶ��ر�죬������1���ڻ���n���������ݴ���������Ҫ���
		Sleep(50);//����50���루windows���Ժ���Ϊ��λ��
	}
	return 1;
}

void map(int L, IMAGE* BJ)
{

	if (L)//�ж��Ƿ���Ҫ�����ϰ���
	{
		int x = 0, y = 0;
		for (int i = 0; i < 350; i++)
		{
			x = rand() % (length / p_size);
			y = rand() % (width / p_size);
			if (G_Map[y][x])//�ж��Ƿ��Ѿ����ϰ���
				i--;
			else
			{
				G_Map[y][x] = (rand() % 10 < 7) ? 1 : 2;//1/7�ĸ����Ǹ�ǽ��6/7�ĸ�������ǽ
			}
		}
	}
	else
	{
		putimage(0, 0, BJ);
		IMAGE S_W, T_W;//�����ǽ����ǽ��ͼƬ�ṹ��ʵ������
		loadimage(&S_W, L"��ǽ.png");
		loadimage(&T_W, L"��ǽ.png");
		for (int x = 0; x < length / p_size; x++)
		{
			for (int y = 0; y < width / p_size; y++)
			{
				if (G_Map[y][x])
				{
					if (G_Map[y][x] == S_wall)
					{
						putimage(x * p_size, y * p_size, &S_W);
					}
					else if (G_Map[y][x] == W_wall)
					{
						putimage(x * p_size, y * p_size, &T_W);
					}
				}
			}
		}
	}
}

bool is_near_player(int x, int y, int C)
{
	int i = 1;
	switch (C)
	{
	case 0:for (; i < 6 && G_Map[y - i][x] != 2; i++);
		if (i == 6)return 1; break;
	case 1:for (; i < 6 && G_Map[y][x + i] != 2; i++);
		if (i == 6)return 1; break;
	case 2:for (; i < 6 && G_Map[y + i][x] != 2; i++);
		if (i == 6)return 1; break;
	case 3:for (; i < 6 && G_Map[y][x - i] != 2; i++);
		if (i == 6)return 1; break;
	}
	return 0;

}


void set_enemy(Tank_C** List, short** G_Map, int x, int y)
{
	for (int i = 0; i < enemy_num; i++)
	{
		List[i] = new Tank_C;
		Tank_C* A = *(List + i);
		A->init(G_Map, x, y);
		A->put_image();
	}
}

int IS_over()
{
	if (cont == 0) { return 0; }
	else if (P_Blood == 0) { return 1; }
	return 2;
}


void enemy_move(Tank_C* base[enemy_num],Tank_P player)
{
	int p = 0;
	for (p = 0; p < enemy_num; p++)
	{
		if (base[p] != NULL)
		{
			if (base[p]->stop == 0)
			{
				//�ȴ�����
				if ((p % 2) == 0) { base[p]->move(); }//��Ӫ
				else { base[p]->A_move(player.getxpos(), player.getypos()); base[p]->move(); }//���
			}
			else { base[p]->stop -= 1; base[p]->put_image(); }

			if (player.getxpos() == base[p]->xpos)
			{

				base[p]->Towards = base[p]->ypos - player.getypos() < 0 ? 2 : 0;
				if (is_near_player(base[p]->xpos, base[p]->ypos, base[p]->Towards))
				{
					base[p]->put_image();
					base[p]->fire(1);
				}
			}
			else if (player.getypos() == base[p]->ypos)
			{
				base[p]->Towards = base[p]->xpos - player.getxpos() < 0 ? 1 : 3;
				if (is_near_player(base[p]->xpos, base[p]->ypos, base[p]->Towards))
				{
					base[p]->put_image();
					base[p]->fire(1);
				}
			}
		}
	}

}

void Health_Points(IMAGE* B)
{
	int i = 0;
	for (; i < P_Blood; i++)
	{
		putimage(35 + i * 65, 680, B, SRCPAINT);
	}
}

int play( IMAGE* BJ,IMAGE* B,IMAGE* XL)
{
	int x, y, k;
	//��ʼ����Ӫλ��
	Stronghold S;
	S.init_xy((short**)G_Map);
	x = S.xpos, y = S.ypos;
	//��ʼ�����,��ӪѪ��
	P_Blood = blood;
	//PS_Blood = blood;
	//���ɵ���
	int  p = 0;
	Tank_C* base[enemy_num] = { NULL };
	cont = 0;
	set_enemy(base,(short**) G_Map, S.xpos, S.ypos);
	//��ʼ��̹��λ��
	while ((abs(x - S.xpos) > 7 || abs(y - S.ypos) > 7) || G_Map[y][x]>=2)
	{
		x = rand() % (length / p_size);
		y = rand() % (width / p_size);
	}
	Tank_P player;
	player.init_xy(x, y);
	player.set_blocks((short**)G_Map);
	while ((k = IS_over()) == 2)
	{
		BeginBatchDraw();
		cleardevice();
		//���Ƶ�ͼ
		map(0, BJ);
		S.put();
		player.move((short**)G_Map);
		player.put_image_p();
		player.stop > 0 ? player.stop -= 1 : 0;
		putimage(0, 650, XL);
		Health_Points(B);
		//�Ƿ���е���
		if (player.fire((short**)G_Map) == 5)
		{
			for (p = 0; p < enemy_num; p++)
			{
				if (base[p] != NULL && base[p]->xpos == player.get_D_xpos() && base[p]->ypos == player.get_D_ypos())
				{
					base[p]->clear_blocks();
					free(base[p]);
					base[p] = NULL;
				}
			}
		}
		//�����ƶ�
		enemy_move(base,player);
		FlushBatchDraw();
		//system("pause");
		Sleep(100);
	}
	return k;
}

bool NEXT()
{

	ExMessage M;
	while (true)
	{
		getmessage(&M,EX_MOUSE);
		if (M.lbutton && (M.x > 54 && M.x < 250) && (M.y > 490 && M.y < 580))
		{
			return 0;
		}
		else if (M.lbutton && (M.x > 388 && M.x < 600) && (M.y > 499 && M.y < 564))
		{
			return 1;
		}
	}


}

int main()
{
	
	bool IS_NEXT = true;
	//��ʼ��2023/06/19
	IMAGE BJ,XL;
	IMAGE pw, cw, B;
	loadimage(&pw, L"���ʤ��.png");
	loadimage(&cw, L"���ʧ��.jpg");
	loadimage(&BJ, L"����.jpg");
	loadimage(&XL, L"��Ϣ������.png");
	loadimage(&B, L"��.jpg");
	srand((unsigned)time(NULL));//�������������
	//mciSendString(L"open ��ը��.mp3 alias ��ը��", NULL, 0, NULL);//�������ã�����Ȥ�ٿ�����
	//mciSendString(L"close ��ը��", NULL, 0, NULL);
	while (IS_NEXT)
	{
		initgraph(width, length, EX_SHOWCONSOLE | EW_DBLCLKS);
		start_window();
		memset(G_Map, 0, sizeof(G_Map));
		if (W_button())
		{
			closegraph();
			initgraph(G_length,G_width, EX_SHOWCONSOLE | EW_DBLCLKS);
			//�����ʼ����
			map(1, &BJ);
			//��ʽ��ʼ
			IS_NEXT = play(&BJ,&B,&XL);
			EndBatchDraw();
		}
		closegraph();
		initgraph(width, length, EX_SHOWCONSOLE | EW_DBLCLKS);
		//����
		switch (IS_NEXT)
		{
		case 0:putimage(0, 0, &pw); break;
		case 1:putimage(0, 0, &cw); break;
		}
		IS_NEXT = NEXT();
		closegraph();
	}

	return 0;
	//��һ�������2023/6/26
}



//void original_route(int x, int y)
//{
//	int num[4];
//	int X=xpos, Y=ypos;
//	int min = 0;
//	while (true)
//	{
//		min = 0;
//		memset(num, 1111, sizeof(int) * 4);
//		G_Map[Y + 1][X] < 2 ? num[0] = abs(y - Y - 1) + abs(x - X) : 0;//'s'---0
//		G_Map[Y][X + 1] < 2 ? num[1] = abs(y - Y) + abs(x - X - 1) : 0;//'d'---1
//		G_Map[Y - 1][X] < 2 ? num[2] = abs(y - Y + 1) + abs(x - X) : 0;//'w'---2
//		G_Map[Y][X - 1] < 2 ? num[3] = abs(y - Y) + abs(x - X + 1) : 0;//'a'---3
//		MOVE.M[MOVE.W] = 0;
//		for (int i = 0; i < 4; i++)
//		{
//			if (num[min] > num[i])
//			{
//				min = i;
//				MOVE.M[MOVE.W] = i;
//			}
//		}
//		switch (min)
//		{
//		case 0:Y += 1; break;
//		case 1:X += 1; break;
//		case 2:Y -= 1; break;
//		case 3:X -= 1; break;
//		}
//		MOVE.W++;
//		if (G_Map[Y + 1][X] == 3 || G_Map[Y][X + 1] == 3 || G_Map[Y - 1][X] == 3 || G_Map[Y][X - 1] == 3)
//		{
//			break;
//		}
//	}
//}
//void move(int x, int y)
//{
//	if (!stop)
//	{
//		/*int k = 0;
//		int next[3] = { 0 }, cont_p = 0, cont = 1000000;
//		for(int i=-1;i<2;i++)
//			for (int j = -1; j < 2; j++)
//			{
//				if (IS_out(i, j))
//				{
//					k = *((short*)map + (length / p_size) * (ypos + i) + xpos + j);
//					if (k == W_wall)
//					{
//						cont_p =3+ ((y - (ypos + i)) < 0 ? ypos + i - y : y - ypos - i) + ((x - xpos - j) < 0 ? xpos + j - x : x - xpos - j);
//						if (cont > cont_p)
//						{
//							next[0] = i, next[1] = j, next[2] = 1;
//							cont = cont_p;
//						}
//					}
//					else if (k == 0)
//					{
//						if (S[1] == ypos + i && S[0] == xpos + j)
//							cont_p =7+((y - (ypos + i)) < 0 ? ypos + i - y : y - ypos - i) + ((x - xpos - j) < 0 ? xpos + j - x : x - xpos - j);
//						else
//							cont_p = ((y - (ypos + i)) < 0 ? ypos + i - y : y - ypos - i) + ((x - xpos - j) < 0 ? xpos + j - x : x - xpos - j);
//						if (cont > cont_p)
//						{
//							next[0] = i, next[1] = j, next[2] = 0;
//							cont = cont_p;
//						}
//					}
//				}
//			}
//		if (next[2])
//			fire(map);
//		S[0] = xpos, S[1] = ypos;
//		xpos += next[1];
//		ypos += next[0];*/
//		clear_blocks();
//		//����ֻ�ƿ���ǽ��·��
//		
//		int r = rand() % 100;
//		if (MOVE.R != MOVE.W)
//		switch (MOVE.M[MOVE.R++])
//		{
//		case 0:G_Map[ypos + 1][xpos] == 1 && r < 50 ? fire(true) : stop = rand() % 3 + 1; if (!G_Map[ypos + 1][xpos])ypos += 1; break;
//		case 1:G_Map[ypos][xpos + 1] == 1 && r < 50 ? fire(true) : stop = rand() % 3 + 1; if (!G_Map[ypos][xpos + 1])xpos += 1; break;
//		case 2:G_Map[ypos - 1][xpos] == 1 && r < 50 ? fire(true) : stop = rand() % 3 + 1; if (!G_Map[ypos - 1][xpos])ypos -= 1; break;
//		case 3:G_Map[ypos][xpos - 1] == 1 && r < 50 ? fire(true) : stop = rand() % 3 + 1; if (!G_Map[ypos][xpos - 1])xpos -= 1; break;
//		}
//		set_blocks();
//	}
//	else
//	{
//		fire(false);
//		stop--;
//	}
//}
//void move()
//{
//	if (!stop)
//	{
//		clear_blocks();
//		//����ֻ�ƿ���ǽ��·��
//		int r = rand() % 100;
//		switch (MOVE.M[MOVE.R++])
//		{
//		case 0:G_Map[ypos + 1][xpos] == 1 && r < 50 ? fire(true) : stop = rand() % 3 + 1; if (!G_Map[ypos + 1][xpos])ypos += 1; break;
//		case 1:G_Map[ypos][xpos + 1] == 1 && r < 50 ? fire(true) : stop = rand() % 3 + 1; if (!G_Map[ypos][xpos + 1])xpos += 1; break;
//		case 2:G_Map[ypos - 1][xpos] == 1 && r < 50 ? fire(true) : stop = rand() % 3 + 1; if (!G_Map[ypos - 1][xpos])ypos -= 1; break;
//		case 3:G_Map[ypos][xpos - 1] == 1 && r < 50 ? fire(true) : stop = rand() % 3 + 1; if (!G_Map[ypos][xpos - 1])xpos -= 1; break;
//		}
//		set_blocks();
//	}
//	else
//	{
//		stop--;
//	}
//	fire(false);
//}
	////����ѷ���
	////�������ܽڵ�
	//if (G_Map[L->xy[1]][L->xy[0] + 1] < 2)//���ǲ����ƻ�����
	//{
	//	if (!key_map[L->xy[1]][L->xy[0] + 1])//δ���ʹ�
	//	{
	//		now = (Unity*)malloc(sizeof(Unity));
	//		now->xy[0] = X + 1; now->xy[1] = Y;
	//		now->H = abs(x - X + 1) + abs(Y - y);
	//		now->G = abs(X + 1 - xpos) + abs(y - ypos);
	//		now->S = now->H + now->G;
	//		now->father = L;
	//		(open->o[(open->max)]) = now;
	//		//�ϸ�����
	//		i = open->max;
	//		while ((open->o[(i - 1) / 2])->S > now->S)
	//		{
	//			open->o[i] = open->o[(i - 1) / 2];
	//			open->o[(i - 1) / 2] = now;
	//			i = (i - 1) / 2;
	//		}
	//		key_map[open->o[0]->xy[1]][open->o[0]->xy[0] + 1] = 1;//����Ѿ����ʹ�
	//	}
	//}
//#include<graphics.h>
//#include<stdio.h>
//#include<math.h>
//#define S(x,y) 0.5*x*y*y 
//
//int main()
//{
//	initgraph(600, 500, SHOWCONSOLE);
//	setbkcolor(WHITE);
//	cleardevice();
//	IMAGE view;
//	loadimage(&view, L"ҹ�µĹ�����.png", 600, 500, true);
//	putimage(0, 0, &view);
//
//	setlinecolor(GREEN);
//	setfillcolor(GREEN);
//	fillcircle(200, 250, 50);
//	fillcircle(400, 250, 50);
//
//	setlinestyle(PS_SOLID, 5);
//	setfillcolor(YELLOW);
//	fillrectangle(200, 200, 400, 300);
//
//
//	settextstyle(40, 0, L"����");
//	setbkcolor(YELLOW);
//	settextcolor(RED);
//	outtextxy(220, 230, L"��ʼ��Ϸ");
//
//	Sleep(1000);
//	setbkcolor(BLACK);
//	cleardevice();
//
//	////׹��С��1
//	//int a = 10; float x = 45, v = 0;
//	//while (TRUE)
//	//{
//	//	v += a;
//	//	x += v;
//	//	if (x > 485) { v = -(0.95 * v); x = 485; }
//	//	solidcircle(300, x, 15);
//	//	Sleep(100);
//	//	cleardevice();
//	//}
//
//	////��תʱ��
//	//float x =0;
//	//setorigin(200, 300);//�����߼�����
//	//setaspectratio(1, -1);//��ת���꣨x�᲻�䣬y��ת��
//	//for (; 1; x += 3.14/18)
//	//{
//	//	circle(0, 0, 50);
//	//	line(0, 0, (int)50*cos(x), (int)50*sin(x));
//	//	Sleep(100);
//	//	cleardevice();
//	//}
//
//	//���ƶ�С��
//	int R[4] = { 400,400,475,450 };
//	ExMessage msg;
//	setbkcolor(BLACK);
//	cleardevice();
//	setfillcolor(YELLOW);
//	int x = 20, y = 20;
//	HWND Dive = GetHWnd();
//	while (true)
//	{
//		if(peekmessage(&msg,EM_MOUSE))
//		if (msg.message == WM_LBUTTONDOWN && msg.x >= 400 && msg.x <= 475 && msg.y >= 400 && msg.y <= 450)
//		{
//			int x = MessageBox(Dive, L"�Ƿ��˳���Ϸ����", L"��ʾ", MB_OKCANCEL);
//			if (x == IDOK)
//				break;
//		}
//		flushmessage(EM_MOUSE);//������п쵼�µ���Ϣ��
//
//		BeginBatchDraw();
//		setfillcolor(YELLOW);
//		rectangle(R[0], R[1], R[2], R[3]);
//		settextstyle(50, 0, L"����");
//		setbkcolor(WHITE);
//		settextcolor(GREEN);
//		outtextxy(400, 400, L"ESC");
//		setbkcolor(BLACK);
//		solidcircle(x, y, 20);
//		if (GetAsyncKeyState(VK_UP))//�ϼ�
//		{
//			y -= 10;
//		}
//		if (GetAsyncKeyState(VK_DOWN))//�¼�
//		{
//			y += 10;
//		}
//		if (GetAsyncKeyState(VK_LEFT))//���
//		{
//			x -= 10;
//		}
//		if (GetAsyncKeyState(VK_RIGHT))//�Ҽ�
//		{
//			x += 10;
//		}
//		// ����һ��������ͼ��
//		setlinecolor(RED);
//		setfillcolor(YELLOW);
//		moveto(100, 100);
//		lineto(200, 100);
//		lineto(200, 200);
//		lineto(150, 250);
//		lineto(100, 200);
//		lineto(100, 100);
//		
//
//		// ��䲻����ͼ��
//		setfillcolor(BLUE);
//		floodfill(140, 150, RED);
//		FlushBatchDraw();
// 
//		Sleep(100);
//		cleardevice();
//
//	}
//	wchar_t name[10];
//	InputBox(name, 12, L"����������", L"�û���Ϣ�Ǽ�", L"����");
//	settextcolor(YELLOW);
//	settextstyle(50, 0, L"����");
//	outtextxy(100, 200,name);
//	EndBatchDraw();
//	getchar();
//	closegraph();
//	return 0;
//}
//����һ�������ã�
		//flushmessage();
		//if (peekmessage(&msg, EX_KEY))
		//{
		//	getmessage(&msg, EX_KEY);
		//	clearrectangle(xpos * p_size, ypos * p_size, (xpos+1) * p_size, (1+ypos) * p_size);
		//	switch (msg.vkcode)
		//	{
		//	case 0x57:
		//	case 'w':
		//		ypos -= ypos - 1 < 0 ? 0 : 1; Towards = 0; IF_H_ob('-', map, &ypos); break;//�ƶ���������ײ���,�߽���
		//	case 0x53:
		//	case 's':
		//		ypos += ypos + 1 > (width / p_size) ? 0 : 1, Towards = 2; IF_H_ob('+', map, &ypos); break;
		//	case 0x44:
		//	case 'd':
		//		xpos += xpos + 1 > (length / p_size) ? 0 : 1, Towards = 1; IF_H_ob('+', map, &xpos); break;
		//	case 0x41:
		//	case 'a':
		//		xpos -= xpos - 1 < 0 ? 0 : 1; Towards = 3; IF_H_ob('-', map, &xpos); break;
		//	default:break;
		//	}
		//}
//		//�ж��Ƿ��Ѿ������ʹ�
		//	if (now->xy[0] + 1 <length/p_size &&key_map[now->xy[1]][now->xy[0] + 1] == 0)//��
		//	{
		//		if (G_Map[now->xy[1]][now->xy[0] + 1] == G_Map[y][x])break;
		//		//�ж��ǲ��ǲ��ɴݻ��谭
		//		if (G_Map[now->xy[1]][now->xy[0] + 1] < 2)
		//		{
		//			//��������,����open����
		//			open->o[index] = (Unity*)malloc(sizeof(Unity));
		//			open->o[index]->father = now;
		//			open->o[index]->xy[0] = now->xy[0] + 1; open->o[index]->xy[1] = now->xy[1];
		//			open->o[index]->G = now->G + 1;
		//			open->o[index]->H = abs(x - open->o[index]->xy[0]) + abs(open->o[index]->xy[1] - y);
		//			open->o[index]->S = open->o[index]->H + open->o[index]->G;
		//			open->max += 1;
		//			index = open->max;
		//		}
		//	}
		//	if (now->xy[0] - 1 >= 0 && key_map[now->xy[1]][now->xy[0] - 1] == 0)//��
		//	{
		//		if (G_Map[now->xy[1]][now->xy[0] - 1] == G_Map[y][x])break;
		//		if (G_Map[now->xy[1]][now->xy[0] - 1] < 2)
		//		{
		//			open->o[index] = (Unity*)malloc(sizeof(Unity));
		//			open->o[index]->father = now;
		//			open->o[index]->xy[0] = now->xy[0] - 1; open->o[index]->xy[1] = now->xy[1];
		//			open->o[index]->G = now->G + 1;
		//			open->o[index]->H = abs(x - open->o[index]->xy[0]) + abs(open->o[index]->xy[1] - y);
		//			open->o[index]->S = open->o[index]->H + open->o[index]->G;
		//			open->max += 1;
		//			index = open->max;
		//		}
		//	}
		//	if (now->xy[1] + 1 < width / p_size && key_map[now->xy[1] + 1][now->xy[0]] == 0)//��
		//	{
		//		if (G_Map[now->xy[1] + 1][now->xy[0]] == G_Map[y][x])break;
		//		if (G_Map[now->xy[1] + 1][now->xy[0]] < 2)
		//		{
		//			open->o[index] = (Unity*)malloc(sizeof(Unity));
		//			open->o[index]->father = now;
		//			open->o[index]->xy[0] = now->xy[0]; open->o[index]->xy[1] = now->xy[1] + 1;
		//			open->o[index]->G = now->G + 1;
		//			open->o[index]->H = abs(x - open->o[index]->xy[0]) + abs(open->o[index]->xy[1] - y);
		//			open->o[index]->S = open->o[index]->H + open->o[index]->G;
		//			open->max += 1;
		//			index = open->max;
		//		}
		//	}
		//	if (now->xy[1] - 1 >= 0 && key_map[now->xy[1] - 1][now->xy[0]] == 0)//��
		//	{
		//		if (G_Map[now->xy[1] - 1][now->xy[0]] == G_Map[y][x])break;
		//		if (G_Map[now->xy[1] - 1][now->xy[0]] < 2)
		//		{
		//			open->o[index] = (Unity*)malloc(sizeof(Unity));
		//			open->o[index]->father = now;
		//			open->o[index]->xy[0] = now->xy[0]; open->o[index]->xy[1] = now->xy[1] - 1;
		//			open->o[index]->G = now->G + 1;	
		//			open->o[index]->H = abs(x - open->o[index]->xy[0]) + abs(open->o[index]->xy[1] - y);
		//			open->o[index]->S = open->o[index]->H + open->o[index]->G;
		//			open->max += 1;
		//			index = open->max;
		//		}
		//	}
		//	if (now == open->o[index])
		//	{
		//		open->o[index] = open->o[open->max];
		//		open->o[open->max] = NULL;
		//	}
		//}
		//��¼·��

