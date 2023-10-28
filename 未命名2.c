#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<time.h>

#define U 1
#define D 2
#define L 3
#define R 4

typedef struct node                                 //己方飞机的节点
{
        int x;
        int y;
        struct node *next;
}node;

typedef struct Bullet
{
        int x;
        int y;
        int status;                                      //0  1 表示此处是否有子弹
        struct Bullet *next;
}Bullet;                                           //子弹模型节点

typedef struct Enemy
{
        int x;
        int y;
        int status;                                     //0    1 表示此节点是否存在飞机
}Enemy;                                                 //敌机节点

// 全局变量//
node *head,*p;
Enemy Ehead[30];
Bullet *HBullet,*q,*H;
int Status,score=0;
int BulletStatus,s=1;
int add_1=1,add_2=0;

//函数声明//
void Pos();                                               //固定光标
void CreateMap();                                   //创建游戏界面
void CreateAircraft();                              //创建自己的飞机
void printAircraft();                               //打印自己的飞机
void Aircraftmove();                                  //自己飞机的移动
void CreateEnemyAircraftModel();                          //创建敌机的模型 结构体数组
void CreateEnemyAircraft();                             //创建一架敌方飞机
void EnemyAircraftMove();                           //敌机的移动
void CreateBulletModel();                            //创建子弹的模型
void Shoot();                                        //发射子弹
void Bulletmove();                                    //子弹的移动
int GetBulletStatus();                                 //获取是否有子弹存在
void JudgeCollide();                                     //判定是否击中敌机
void JudgeGameOver();                                       //判定是否与敌机相撞  
                                           //为了防止在运行别的函数时这个函数无法运行,所以讲起嵌入每一个循环的函数中,保证不遗漏任何情况.
void Pause();                                              //暂停
void ControlGame();                                       //控制游戏主程序
void GameStart();                                         //游戏开的的预先运行函数集合
void EndGame();                                             //结束游戏
/*   
                  ■              飞机的基本模型   
                                ■■■            第一行为机头,         敌机的设定只需用到机头,机身仅仅只是打印出来,并没有实际内容
                                ■■■            下两行为机身

*/
void Pos(int x,int y)//设置光标位置
{
        COORD pos;
        pos.X=x;
        pos.Y=y;
        HANDLE hOutPut;
        hOutPut=GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleCursorPosition(hOutPut,pos);
}

void CreateMap()
{
        system("mode con cols=100 lines=55");
        int i;
        for(i=0;i<=67;i+=2)//上下边
        {
                Pos(i,0);
                printf("■");
                Pos(i,50);
                printf("■");
        }
        for(i=0;i<=50;i++)//左右边
        {
                Pos(0,i);
                printf("■");
                Pos(66,i);
                printf("■");

        }
        Pos(80,8);
        printf("您的得分%d",score);
        Pos(70,20);
        printf("Space:发射子弹");
        Pos(70,22);
        printf("ESC:退出游戏");
        Pos(70,24);
        printf("Enter:暂停");
        Pos(70,26);
        printf("用↑.↓.←.→控制飞机");
        Pos(70,28);
        printf("F1:加速敌机的产生");
        Pos(70,30);
        printf("F2:减缓敌机的产生");
        Pos(80,40);
        printf("特别说明:");
        Pos(70,42);
        printf("敌机从地图下飞出不结束游戏");
}

void CreateAircraft()
{
        node *q;
        int i;
        head=(node*)malloc(sizeof(node));//链表形式创建飞机,x,y分别为显示的坐标
        head->x=32;
        head->y=47;
        q=head;
        for(i=0;i<=4;i+=2)
        {
                p=(node*)malloc(sizeof(node));
                head->next=p;
                p->x=30+i;
                p->y=48;
                head=p;
        }
        for(i=0;i<=4;i+=2)
        {
                p=(node*)malloc(sizeof(node));
                head->next=p;
                p->x=30+i;
                p->y=49;
                head=p;
        }
        p->next=NULL;
        head=q;
        p=head;
}

void printAircraft()
{
        while(p!=NULL)
        {
                Pos(p->x,p->y);
                printf("■");
                p=p->next;
        }
        p=head;

}

void Aircraftmove()
{
        JudgeGameOver();
        JudgeCollide();
        Sleep(50);
        if(Status==U)                //向上飞行
        {
                while (p!=NULL)
                {
                        p->y=p->y-1;
                        Pos(p->x,p->y);
                        printf("■");
                        p=p->next;
                }
                p=head;
                Pos(p->x-2,p->y+3);
                printf("  ");
                Pos(p->x,p->y+3);
                printf("  ");
                Pos(p->x+2,p->y+3);
                printf("  ");
        }
        if(Status==D)              //向下飞行
        {
                while (p!=NULL)
                {
                        p->y=p->y+1;
                        Pos(p->x,p->y);
                        printf("■");
                        p=p->next;
                }
                p=head;
                Pos(p->x,p->y-1);
                printf("  ");
                Pos(p->x-2,p->y);
                printf("  ");
                Pos(p->x+2,p->y);
                printf("  ");
        }
        if(Status==L)               //向左飞行
        {
                while (p!=NULL)
                {
                        p->x=p->x-2;
                        Pos(p->x,p->y);
                        printf("■");
                        p=p->next;
                }
                p=head;
                Pos(p->x+2,p->y);
                printf("  ");
                Pos(p->x+4,p->y+1);
                printf("  ");
                Pos(p->x+4,p->y+2);
                printf("  ");
        }
        if(Status==R)                  //向右飞行
        {
                while (p!=NULL)
                {
                        p->x=p->x+2;
                        Pos(p->x,p->y);
                        printf("■");
                        p=p->next;
                }
                p=head;
                Pos(p->x-2,p->y);
                printf("  ");
                Pos(p->x-4,p->y+1);
                printf("  ");
                Pos(p->x-4,p->y+2);
                printf("  ");
        }

}

void CreateEnemyAircraftModel()
{                              
        int j;
        for(j=1;j<=30;j++)              //结构体数组表示敌军飞机最多同时存在30架,也许不可能有那么多,数字用的稍大.
        {                               //每一个值表示一架飞机,敌机只需知道其机头坐标
                Ehead[j].x=-1;
                Ehead[j].y=1;
                Ehead[j].status=0;
        }
}

void CreateEnemyAircraft()//创建出一架新的飞机
{
        JudgeCollide();
        JudgeGameOver();
        srand((unsigned)time(NULL));
        int j;
        for(j=1;j<=20;j++)
        {
                if(Ehead[j].status==0)
                {
                        JudgeGameOver();
                        while(Ehead[j].x%2!=0)  //保证机头x为偶数,对其格子.
                        {
                                Ehead[j].x=rand()%62+3;
                        }
                        Ehead[j].y=1;
                        Ehead[j].status=1;
                        Pos((Ehead[j].x),(Ehead[j].y));
                        printf("■");
                        break;
                }
        }
}

void EnemyAircraftMove()                          //模拟敌机向下飞行的函数
{
        Sleep(100);
        if(s>=15)                  //控制敌机飞机的出现频率
        {
                CreateEnemyAircraft();   
                s=1;
        }
        s=s+add_1;
        JudgeCollide();
        JudgeGameOver();
        int j;
        for(j=1;j<=30;j++)
        {
                if(Ehead[j].status==1 && Ehead[j].y==1)      //特殊情况
                {
                        Pos(Ehead[j].x,Ehead[j].y);
                        printf("  ");
                }
                if(Ehead[j].status==1 && Ehead[j].y==2)   //特殊情况
                {
                        Pos(Ehead[j].x,Ehead[j].y);
                        printf("  ");
                        Pos(Ehead[j].x-2,Ehead[j].y-1);
                        printf("      ");
                }
                if(Ehead[j].status==1 && 3<=Ehead[j].y && Ehead[j].y<=49)
                {
                        Pos(Ehead[j].x,Ehead[j].y);
                        printf("  ");
                        Pos(Ehead[j].x-2,Ehead[j].y-1);
                        printf("      ");
                        Pos(Ehead[j].x-2,Ehead[j].y-2);
                        printf("      ");
                }
                if(Ehead[j].status==1 && Ehead[j].y==50)//特殊情况
                {
                        Pos(Ehead[j].x-2,Ehead[j].y-1);
                        printf("      ");
                        Pos(Ehead[j].x-2,Ehead[j].y-2);
                        printf("      ");
                }
                if(Ehead[j].status==1 && Ehead[j].y==51)//特殊情况
                {
                        Pos(Ehead[j].x-2,Ehead[j].y-2);
                        printf("      ");
                }
        }
        for(j=1;j<=20;j++)                      //将每一个敌机机头向下移动一位
        {
                if(Ehead[j].status==1)
                {
                        if(Ehead[j].y<=52)
                        {
                                Ehead[j].y=Ehead[j].y+1;
                        }
                        if(Ehead[j].y>=53)
                        {
                                Ehead[j].status=0;
                        }
                }
        }
        for(j=1;j<=20;j++)
        {
                if(Ehead[j].status==1 && Ehead[j].y==2)//特殊情况
                {
                        Pos(Ehead[j].x,Ehead[j].y);
                        printf("■");
                        Pos(Ehead[j].x-2,Ehead[j].y-1);
                        printf("■■■");
                }
                if(Ehead[j].status==1 && 3<=Ehead[j].y && Ehead[j].y<=50)
                {
                        Pos(Ehead[j].x,Ehead[j].y);
                        printf("■");
                        Pos(Ehead[j].x-2,Ehead[j].y-1);
                        printf("■■■");
                        Pos(Ehead[j].x-2,Ehead[j].y-2);
                        printf("■■■");
                }
                if(Ehead[j].status==1 && Ehead[j].y==51)//特殊情况
                {
                        Pos(Ehead[j].x-2,Ehead[j].y-1);
                        printf("■■■");
                        Pos(Ehead[j].x-2,Ehead[j].y-2);
                        printf("■■■");
                }
                if(Ehead[j].status==1 && Ehead[j].y==52)//特殊情况
                {
                        Pos(Ehead[j].x-2,Ehead[j].y-2);
                        printf("■■■");
                }
        }
        JudgeGameOver();
}

void CreateBulletModel()                       //创建子弹的模型
{
        int i;
        q=(Bullet*)malloc(sizeof(Bullet));            
        q->x=0;
        q->y=1;
        q->status=0;
        H=q;
        for(i=2;i<=46;i++)                       //每行最多存在一个子弹,子弹的最大数值为地图上下边框所确定的值.所以一上下边框确定一个最大的链表.
        {
                HBullet=(Bullet*)malloc(sizeof(Bullet));
                HBullet->y=i;
                HBullet->x=-1;
                HBullet->status=0;
                q->next=HBullet;
                q=HBullet;
        }
        q->next=H;
        HBullet=H;
}

void Shoot()                                         //发射子弹
{
        JudgeGameOver();
        JudgeCollide();
        while(HBullet->next->y!=head->y)               
        {
                HBullet=HBullet->next;
        }
        HBullet->x=head->x;
        HBullet->status=1;                                  //1表示此处有子弹
        Pos(HBullet->x,HBullet->y);
        printf("■");
        HBullet=H;
}

void BulletMove()                 //子弹的飞行
{                                  //与敌军飞机飞行同理
        JudgeGameOver();
        JudgeCollide();
        while(HBullet!=q)
        {
                if(HBullet->status==1)
                {
                        Pos(HBullet->x,HBullet->y);
                        printf("  ");
                }
                HBullet=HBullet->next;
        }
        HBullet=H->next;
        while(HBullet!=q)
        {
                H->x=HBullet->x;
                H->status=HBullet->status;
                H=H->next;
                HBullet=HBullet->next;
        }

        HBullet=HBullet->next;
        H=HBullet;
        while(HBullet!=q)
        {
                if(HBullet->status==1)
                {
                        Pos(HBullet->x,HBullet->y);
                        printf("■");
                }
                HBullet=HBullet->next;
        }
        HBullet=H;
        JudgeGameOver();
}

int GetBulletStatus()        //获取地图中是否存在子弹,不存在子弹的时候BulletMove()将不运行
{
        JudgeGameOver();
        JudgeCollide();
        while(HBullet!=q)
        {
                if(HBullet->status==1)
                {
                        BulletStatus=1;
                        break;
                }
                HBullet=HBullet->next;
        }
        HBullet=H;
        return 0;
}

void JudgeCollide()         //判断是否集中敌机
{       
        int j;
        while(HBullet!=q)
        {
                for(j=1;j<=30;j++)
                {
                        JudgeGameOver();
                        if(HBullet->status==1 && Ehead[j].status==1)
                        {
                                if((HBullet->x==Ehead[j].x && HBullet->y==Ehead[j].y))
                                {
                                        HBullet->status=0;
                                        Ehead[j].status=0;
                                        score=score+10+add_2;
                                        Pos(80,8);
                                        printf("您的得分%d",score);
                                        Pos(HBullet->x,HBullet->y);
                                        printf("  ");
                                        Pos(HBullet->x-2,HBullet->y-1);
                                        printf("      ");
                                        Pos(HBullet->x-2,HBullet->y-2);
                                        printf("      ");
                                }
                                if(HBullet->x+2==Ehead[j].x && HBullet->y==Ehead[j].y)
                                {
                                        HBullet->status=0;
                                        Ehead[j].status=0;
                                        score=score+10+add_2;
                                        Pos(80,8);
                                        printf("您的得分%d",score);
                                        Pos(HBullet->x,HBullet->y);
                                        printf("  ");
                                        Pos(HBullet->x+2,HBullet->y);
                                        printf("  ");
                                        Pos(HBullet->x,HBullet->y-1);
                                        printf("      ");
                                        Pos(HBullet->x,HBullet->y-2);
                                        printf("      ");
                                }
                                if(HBullet->x-2==Ehead[j].x && HBullet->y==Ehead[j].y)
                                {
                                        HBullet->status=0;
                                        Ehead[j].status=0;
                                        score=score+10+add_2;
                                        Pos(80,8);
                                        printf("您的得分%d",score);
                                        Pos(HBullet->x,HBullet->y);
                                        printf("  ");
                                        Pos(HBullet->x-2,HBullet->y);
                                        printf("  ");
                                        Pos(HBullet->x-4,HBullet->y-1);
                                        printf("      ");
                                        Pos(HBullet->x-4,HBullet->y-2);
                                        printf("      ");
                                }
                        }

                }
                HBullet=HBullet->next;
        }
        HBullet=H;
}

void JudgeGameOver()   //判断是否撞机
{
        int j;
        for(j=1;j<=30;j++)
        {
                while(p!=NULL)
                {
                        if(Ehead[j].x==p->x && Ehead[j].y==p->y)
                        {
                                EndGame();
                        }
                        p=p->next;
                }
        }
        p=head;
}

void Pause()//暂停
{
        while(1)
        {
                Sleep(300);
                if(GetAsyncKeyState(VK_RETURN))
                {   
                        break;
                }
        }
}

void ControlGame()                                          // 控制主程序
{
        while(1)
        {               
                JudgeGameOver();
                JudgeCollide();
                if(GetAsyncKeyState(VK_UP) && head->y!=1)
                {
                        Status=U;
                        Aircraftmove();
                }
                else if(GetAsyncKeyState(VK_DOWN) && head->y+2!=49)
                {

                        Status=D;
                        Aircraftmove();
                }
                else if(GetAsyncKeyState(VK_LEFT) && head->x-2!=2)
                {
                        Status=L;
                        Aircraftmove();
                }
                else if(GetAsyncKeyState(VK_RIGHT) && head->x+2!=64)
                {
                        Status=R;
                        Aircraftmove();
                }
                if(GetAsyncKeyState(VK_F1) && head->y!=1)
                {
                        if(add_1<5)
                        {
                        add_1=add_1+1;
                        add_2=add_2+2;
                        }
                }
                if(GetAsyncKeyState(VK_F2) && head->y!=1)
                {
                        if(add_1>2 &&add_1 <=5)
                        {
                        add_1=add_1-1;
                        add_1=add_2-2;
                        }
                }
                else if(GetAsyncKeyState(VK_RETURN))
                {
                        Pause();
                }
                else if(GetAsyncKeyState(VK_SPACE))
                {
                        Shoot();
                }
                else if(GetAsyncKeyState(VK_ESCAPE))
                {
                        EndGame();
                }
                GetBulletStatus();
                if(BulletStatus==1)
                {
                        BulletMove();
                }
                EnemyAircraftMove();
                JudgeGameOver();
        }
}

void GameStart()
{
        CreateMap();
        CreateAircraft();
        printAircraft();
        CreateEnemyAircraftModel();
        CreateBulletModel();
}

void EndGame()
{
        system("cls");
        Pos(35,25);
        printf("Game Over.");
        Pos(1,50);
        exit(0);
}

int main()
{
        GameStart();
        ControlGame();
}


