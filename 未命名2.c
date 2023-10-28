#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<time.h>

#define U 1
#define D 2
#define L 3
#define R 4

typedef struct node                                 //�����ɻ��Ľڵ�
{
        int x;
        int y;
        struct node *next;
}node;

typedef struct Bullet
{
        int x;
        int y;
        int status;                                      //0  1 ��ʾ�˴��Ƿ����ӵ�
        struct Bullet *next;
}Bullet;                                           //�ӵ�ģ�ͽڵ�

typedef struct Enemy
{
        int x;
        int y;
        int status;                                     //0    1 ��ʾ�˽ڵ��Ƿ���ڷɻ�
}Enemy;                                                 //�л��ڵ�

// ȫ�ֱ���//
node *head,*p;
Enemy Ehead[30];
Bullet *HBullet,*q,*H;
int Status,score=0;
int BulletStatus,s=1;
int add_1=1,add_2=0;

//��������//
void Pos();                                               //�̶����
void CreateMap();                                   //������Ϸ����
void CreateAircraft();                              //�����Լ��ķɻ�
void printAircraft();                               //��ӡ�Լ��ķɻ�
void Aircraftmove();                                  //�Լ��ɻ����ƶ�
void CreateEnemyAircraftModel();                          //�����л���ģ�� �ṹ������
void CreateEnemyAircraft();                             //����һ�ܵз��ɻ�
void EnemyAircraftMove();                           //�л����ƶ�
void CreateBulletModel();                            //�����ӵ���ģ��
void Shoot();                                        //�����ӵ�
void Bulletmove();                                    //�ӵ����ƶ�
int GetBulletStatus();                                 //��ȡ�Ƿ����ӵ�����
void JudgeCollide();                                     //�ж��Ƿ���ел�
void JudgeGameOver();                                       //�ж��Ƿ���л���ײ  
                                           //Ϊ�˷�ֹ�����б�ĺ���ʱ��������޷�����,���Խ���Ƕ��ÿһ��ѭ���ĺ�����,��֤����©�κ����.
void Pause();                                              //��ͣ
void ControlGame();                                       //������Ϸ������
void GameStart();                                         //��Ϸ���ĵ�Ԥ�����к�������
void EndGame();                                             //������Ϸ
/*   
                  ��              �ɻ��Ļ���ģ��   
                                ������            ��һ��Ϊ��ͷ,         �л����趨ֻ���õ���ͷ,�������ֻ�Ǵ�ӡ����,��û��ʵ������
                                ������            ������Ϊ����

*/
void Pos(int x,int y)//���ù��λ��
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
        for(i=0;i<=67;i+=2)//���±�
        {
                Pos(i,0);
                printf("��");
                Pos(i,50);
                printf("��");
        }
        for(i=0;i<=50;i++)//���ұ�
        {
                Pos(0,i);
                printf("��");
                Pos(66,i);
                printf("��");

        }
        Pos(80,8);
        printf("���ĵ÷�%d",score);
        Pos(70,20);
        printf("Space:�����ӵ�");
        Pos(70,22);
        printf("ESC:�˳���Ϸ");
        Pos(70,24);
        printf("Enter:��ͣ");
        Pos(70,26);
        printf("�á�.��.��.�����Ʒɻ�");
        Pos(70,28);
        printf("F1:���ٵл��Ĳ���");
        Pos(70,30);
        printf("F2:�����л��Ĳ���");
        Pos(80,40);
        printf("�ر�˵��:");
        Pos(70,42);
        printf("�л��ӵ�ͼ�·ɳ���������Ϸ");
}

void CreateAircraft()
{
        node *q;
        int i;
        head=(node*)malloc(sizeof(node));//������ʽ�����ɻ�,x,y�ֱ�Ϊ��ʾ������
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
                printf("��");
                p=p->next;
        }
        p=head;

}

void Aircraftmove()
{
        JudgeGameOver();
        JudgeCollide();
        Sleep(50);
        if(Status==U)                //���Ϸ���
        {
                while (p!=NULL)
                {
                        p->y=p->y-1;
                        Pos(p->x,p->y);
                        printf("��");
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
        if(Status==D)              //���·���
        {
                while (p!=NULL)
                {
                        p->y=p->y+1;
                        Pos(p->x,p->y);
                        printf("��");
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
        if(Status==L)               //�������
        {
                while (p!=NULL)
                {
                        p->x=p->x-2;
                        Pos(p->x,p->y);
                        printf("��");
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
        if(Status==R)                  //���ҷ���
        {
                while (p!=NULL)
                {
                        p->x=p->x+2;
                        Pos(p->x,p->y);
                        printf("��");
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
        for(j=1;j<=30;j++)              //�ṹ�������ʾ�о��ɻ����ͬʱ����30��,Ҳ����������ô��,�����õ��Դ�.
        {                               //ÿһ��ֵ��ʾһ�ܷɻ�,�л�ֻ��֪�����ͷ����
                Ehead[j].x=-1;
                Ehead[j].y=1;
                Ehead[j].status=0;
        }
}

void CreateEnemyAircraft()//������һ���µķɻ�
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
                        while(Ehead[j].x%2!=0)  //��֤��ͷxΪż��,�������.
                        {
                                Ehead[j].x=rand()%62+3;
                        }
                        Ehead[j].y=1;
                        Ehead[j].status=1;
                        Pos((Ehead[j].x),(Ehead[j].y));
                        printf("��");
                        break;
                }
        }
}

void EnemyAircraftMove()                          //ģ��л����·��еĺ���
{
        Sleep(100);
        if(s>=15)                  //���Ƶл��ɻ��ĳ���Ƶ��
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
                if(Ehead[j].status==1 && Ehead[j].y==1)      //�������
                {
                        Pos(Ehead[j].x,Ehead[j].y);
                        printf("  ");
                }
                if(Ehead[j].status==1 && Ehead[j].y==2)   //�������
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
                if(Ehead[j].status==1 && Ehead[j].y==50)//�������
                {
                        Pos(Ehead[j].x-2,Ehead[j].y-1);
                        printf("      ");
                        Pos(Ehead[j].x-2,Ehead[j].y-2);
                        printf("      ");
                }
                if(Ehead[j].status==1 && Ehead[j].y==51)//�������
                {
                        Pos(Ehead[j].x-2,Ehead[j].y-2);
                        printf("      ");
                }
        }
        for(j=1;j<=20;j++)                      //��ÿһ���л���ͷ�����ƶ�һλ
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
                if(Ehead[j].status==1 && Ehead[j].y==2)//�������
                {
                        Pos(Ehead[j].x,Ehead[j].y);
                        printf("��");
                        Pos(Ehead[j].x-2,Ehead[j].y-1);
                        printf("������");
                }
                if(Ehead[j].status==1 && 3<=Ehead[j].y && Ehead[j].y<=50)
                {
                        Pos(Ehead[j].x,Ehead[j].y);
                        printf("��");
                        Pos(Ehead[j].x-2,Ehead[j].y-1);
                        printf("������");
                        Pos(Ehead[j].x-2,Ehead[j].y-2);
                        printf("������");
                }
                if(Ehead[j].status==1 && Ehead[j].y==51)//�������
                {
                        Pos(Ehead[j].x-2,Ehead[j].y-1);
                        printf("������");
                        Pos(Ehead[j].x-2,Ehead[j].y-2);
                        printf("������");
                }
                if(Ehead[j].status==1 && Ehead[j].y==52)//�������
                {
                        Pos(Ehead[j].x-2,Ehead[j].y-2);
                        printf("������");
                }
        }
        JudgeGameOver();
}

void CreateBulletModel()                       //�����ӵ���ģ��
{
        int i;
        q=(Bullet*)malloc(sizeof(Bullet));            
        q->x=0;
        q->y=1;
        q->status=0;
        H=q;
        for(i=2;i<=46;i++)                       //ÿ��������һ���ӵ�,�ӵ��������ֵΪ��ͼ���±߿���ȷ����ֵ.����һ���±߿�ȷ��һ����������.
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

void Shoot()                                         //�����ӵ�
{
        JudgeGameOver();
        JudgeCollide();
        while(HBullet->next->y!=head->y)               
        {
                HBullet=HBullet->next;
        }
        HBullet->x=head->x;
        HBullet->status=1;                                  //1��ʾ�˴����ӵ�
        Pos(HBullet->x,HBullet->y);
        printf("��");
        HBullet=H;
}

void BulletMove()                 //�ӵ��ķ���
{                                  //��о��ɻ�����ͬ��
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
                        printf("��");
                }
                HBullet=HBullet->next;
        }
        HBullet=H;
        JudgeGameOver();
}

int GetBulletStatus()        //��ȡ��ͼ���Ƿ�����ӵ�,�������ӵ���ʱ��BulletMove()��������
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

void JudgeCollide()         //�ж��Ƿ��ел�
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
                                        printf("���ĵ÷�%d",score);
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
                                        printf("���ĵ÷�%d",score);
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
                                        printf("���ĵ÷�%d",score);
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

void JudgeGameOver()   //�ж��Ƿ�ײ��
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

void Pause()//��ͣ
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

void ControlGame()                                          // ����������
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


