//��������� ������� 
//������� ��������� 2102
//19.12.10 

#include <stdio.h>
#include <math.h>
#include <graphics.h>
#include "weight.h"

int Nx,Ny, DrawX,DrawY; //���-�� ������ � �������, ������� ������������ ����
float m, k, f; //����� ������, ��������� ������, ����������� ������(friction)
float X,Y, lx,ly, dl; //������� ������, ����� ������ � �����, ����������� ����������� ��������
float dt, T; //���������� �������, ���-�� ��������� ������
weight box[100][100]; //������ ������


void init() //���������� ������� ������ � ������������� ��������� ���������
{
  FILE *in;
  int Nd, x,y; //���-�� ����������, ������ ���������� ������
  float dx,dy; //�������� ����������
  int i,j;
  
  i=0;
  j=0;
  dt=0.1;
  in=fopen("input.txt","rt");
  fscanf(in,"������� ������������ ����: %i ",&DrawX);
  fscanf(in,"� %i \n",&DrawY);
  fscanf(in,"������� ������: %f ",&X);
  fscanf(in,"� %f \n",&Y);
  fscanf(in,"���-�� ������: %i ",&Nx);
  fscanf(in,"� %i \n",&Ny);
  fscanf(in,"�����: %f \n",&m);
  fscanf(in,"Ƹ�������: %f \n",&k);
  fscanf(in,"������: %f \n",&f);
  fscanf(in,"����������� ����������� ��������: %f \n",&dl);
  fscanf(in,"���-�� ��������: %i \n",&Nd);
  
  lx = X/(Nx+1); //������� ����� ������ � �����
  ly = Y/(Ny+1); 
  
  for (i=0;i<=Nx+1;i++) //������� ��������� ������ � ��������� ������� ���������
  {
    for (j=0;j<=Ny+1;j++)
    {
      box[i][j].X0=i*lx;
      box[i][j].Y0=j*ly;    
      box[i][j].X1=0;
      box[i][j].Y1=0;
      box[i][j].X2=0;
      box[i][j].Y2=0;
      box[i][j].X3=0;
      box[i][j].Y3=0;
      box[i][j].t=0; 
    } 
  }
  
  for (i=0;i<Nd;i++) //���������� � ������� ��������� ����������
  {
    fscanf(in,"%i",&x);
    fscanf(in," %i",&y);
    fscanf(in," %f",&dx);
    fscanf(in," %f \n",&dy);
    box[x][y].X2=dx;
    box[x][y].Y2=dy;
    box[x][y].X3=dx;
    box[x][y].Y3=dy;
  }
  
  initwindow(DrawX,DrawY); //������������� ������������ ����
  fclose(in);     
};


int G(float dr) //�������� G � ������ �������� ����� RGB, � ����������� �� ��������
{
  if (fmod(dr,(lx*lx+ly*ly))>0.5)
  {
    return 0;                                                  
  }
  if ((fmod(dr,(lx*lx+ly*ly))<=0.5)&&(fmod(dr,(lx*lx+ly*ly))>0.25))
  {
    return 69;                                                  
  }
  if ((fmod(dr,(lx*lx+ly*ly))<=0.25)&&(fmod(dr,(lx*lx+ly*ly))>0.125))
  {
    return 140;                                                  
  }
  if ((fmod(dr,(lx*lx+ly*ly))<=0.125)&&(fmod(dr,(lx*lx+ly*ly))>0.0625))
  {
    return 165;                                                  
  }
  if (fmod(dr,(lx*lx+ly*ly))<=0.0625)
  {
    return 255;                                                  
  }    
};

int B(float dr) //�������� B � ������ �������� ����� RGB, � ����������� �� ��������
{
  if (fmod(dr,(lx*lx+ly*ly))>0.03125)
  {
    return 0;                                                  
  }
  if (fmod(dr,(lx*lx+ly*ly))<=0.03125)
  {
    return 255;                                                  
  }
};


void step(int& ph) //������� ��������� ������ � ��������� ������ �������, � ����������� �� ���� ����� � ���������� �������
{                 //���� ����� �������� ����� ���������� ������ �������� � ��� ���������� �������, � ����� ����� ���� ��������
  int i,j;
  float l1,l2,l3,l4, c1,c2,c3,c4,s1,s2,s3,s4; //����������� ������, �������� � ������ ������� ������ � ��� X
  
  i=0;
  j=0;   
     
  if (ph==1) //���� ����=1, �� �������� � ���������� ������ �������� � X2,Y2, � ����� ����� � X3,Y3, � ����� ������������ � X1,Y1
  {
    for (i=1;i<=Nx;i++)
    {
      for (j=1;j<=Ny;j++)
      { //������� ���������� �������� ���������� � ������ ������ 
        l1 = lx - sqrt((box[i][j].X2-box[i-1][j].X2+lx)*(box[i][j].X2-box[i-1][j].X2+lx) + (box[i][j].Y2-box[i-1][j].Y2)*(box[i][j].Y2-box[i-1][j].Y2));
        l2 = lx - sqrt((box[i][j].X2-box[i+1][j].X2-lx)*(box[i][j].X2-box[i+1][j].X2-lx) + (box[i][j].Y2-box[i+1][j].Y2)*(box[i][j].Y2-box[i+1][j].Y2));
        l3 = ly - sqrt((box[i][j].X2-box[i][j-1].X2)*(box[i][j].X2-box[i][j-1].X2) + (box[i][j].Y2-box[i][j-1].Y2+ly)*(box[i][j].Y2-box[i][j-1].Y2+ly));
        l4 = ly - sqrt((box[i][j].X2-box[i][j+1].X2)*(box[i][j].X2-box[i][j+1].X2) + (box[i][j].Y2-box[i][j+1].Y2-ly)*(box[i][j].Y2-box[i][j+1].Y2-ly));
        //������� ��������� � ������� ����� ������� �������� ������������ ��� X
        c1 = (box[i][j].X2-box[i-1][j].X2+lx)/(lx-l1);
        c2 = (box[i+1][j].X2-box[i][j].X2+lx)/(lx-l2);
        c3 = (box[i][j-1].X2-box[i][j].X2)/(ly-l3);
        c4 = (box[i][j+1].X2-box[i][j].X2)/(ly-l4);
        s1 = (box[i-1][j].Y2-box[i][j].Y2)/(lx-l1);
        s2 = (box[i+1][j].Y2-box[i][j].Y2)/(lx-l2);
        s3 = (box[i][j].Y2-box[i][j-1].Y2+ly)/(ly-l3);
        s4 = (box[i][j+1].Y2-box[i][j].Y2+ly)/(ly-l4);
        
        box[i][j].X1 = (k*dt*dt*(l1*c1-l2*c2-l3*c3-l4*c4)+box[i][j].X2*(f*dt+2*m)-box[i][j].X3*m)/(m+f*dt); //������� ����� ���������
        box[i][j].Y1 = (k*dt*dt*(l3*s3-l4*s4-l2*s2-l1*s1)+box[i][j].Y2*(f*dt+2*m)-box[i][j].Y3*m)/(m+f*dt);
        
        if (fabs(box[i][j].X1)<=dl) //��������� � ����������� ����������� �����������
        {
          box[i][j].X1=0;                        
        }
        if (fabs(box[i][j].Y1)<=dl)
        {
          box[i][j].Y1=0;                        
        }
        
        setfillstyle(1,COLOR(255,G(box[i][j].X1*box[i][j].X1+box[i][j].Y1*box[i][j].Y1),B(box[i][j].X1*box[i][j].X1+box[i][j].Y1*box[i][j].Y1))); //������� ������ ���������
        setcolor(COLOR(255,G(box[i][j].X1*box[i][j].X1+box[i][j].Y1*box[i][j].Y1),B(box[i][j].X1*box[i][j].X1+box[i][j].Y1*box[i][j].Y1)));
        box[i][j].draw(ph,DrawX/X,DrawY/Y); //��������� �����
        
        if (box[i][j].dev()==1) //������� ������� ���������
        {
          box[i][j].t=box[i][j].t+1;                   
        } 
      }  
    }
    
    ph=2; //��������� ���� �����     
  }
  else if (ph==2) //���� ����=2, �� �������� � ���������� ������ �������� � X1,Y1, � ����� ����� � X2,Y2, � ����� ������������ � X3,Y3
  {
    for (i=1;i<=Nx;i++)
    {
      for (j=1;j<=Ny;j++)
      { //������� ���������� �������� ���������� � ������ ������ 
        l1 = lx - sqrt((box[i][j].X1-box[i-1][j].X1+lx)*(box[i][j].X1-box[i-1][j].X1+lx) + (box[i][j].Y1-box[i-1][j].Y1)*(box[i][j].Y1-box[i-1][j].Y1));
        l2 = lx - sqrt((box[i][j].X1-box[i+1][j].X1-lx)*(box[i][j].X1-box[i+1][j].X1-lx) + (box[i][j].Y1-box[i+1][j].Y1)*(box[i][j].Y1-box[i+1][j].Y1));
        l3 = ly - sqrt((box[i][j].X1-box[i][j-1].X1)*(box[i][j].X1-box[i][j-1].X1) + (box[i][j].Y1-box[i][j-1].Y1+ly)*(box[i][j].Y1-box[i][j-1].Y1+ly));
        l4 = ly - sqrt((box[i][j].X1-box[i][j+1].X1)*(box[i][j].X1-box[i][j+1].X1) + (box[i][j].Y1-box[i][j+1].Y1-ly)*(box[i][j].Y1-box[i][j+1].Y1-ly));
        //������� ��������� � ������� ����� ������� �������� ������������ ��� X
        c1 = (box[i][j].X1-box[i-1][j].X1+lx)/(lx-l1);
        c2 = (box[i+1][j].X1-box[i][j].X1+lx)/(lx-l2);
        c3 = (box[i][j-1].X1-box[i][j].X1)/(ly-l3);
        c4 = (box[i][j+1].X1-box[i][j].X1)/(ly-l4);
        s1 = (box[i-1][j].Y1-box[i][j].Y1)/(lx-l1);
        s2 = (box[i+1][j].Y1-box[i][j].Y1)/(lx-l2);
        s3 = (box[i][j].Y1-box[i][j-1].Y1+ly)/(ly-l3);
        s4 = (box[i][j+1].Y1-box[i][j].Y1+ly)/(ly-l4);
        
        box[i][j].X3 = (k*dt*dt*(l1*c1-l2*c2-l3*c3-l4*c4)+box[i][j].X1*(f*dt+2*m)-box[i][j].X2*m)/(m+f*dt); //������� ����� ���������
        box[i][j].Y3 = (k*dt*dt*(l3*s3-l4*s4-l2*s2-l1*s1)+box[i][j].Y1*(f*dt+2*m)-box[i][j].Y2*m)/(m+f*dt);
        
        if (fabs(box[i][j].X3)<=dl) //��������� � ����������� ����������� �����������
        {
          box[i][j].X3=0;                        
        }
        if (fabs(box[i][j].Y3)<=dl)
        {
          box[i][j].Y3=0;                        
        }
        
        setfillstyle(1,COLOR(255,G(box[i][j].X1*box[i][j].X1+box[i][j].Y1*box[i][j].Y1),B(box[i][j].X1*box[i][j].X1+box[i][j].Y1*box[i][j].Y1))); //������� ������ ���������
        setcolor(COLOR(255,G(box[i][j].X1*box[i][j].X1+box[i][j].Y1*box[i][j].Y1),B(box[i][j].X1*box[i][j].X1+box[i][j].Y1*box[i][j].Y1)));
        box[i][j].draw(ph,DrawX/X,DrawY/Y); //��������� �����
        
        if (box[i][j].dev()==1) //������� ������� ���������
        {
          box[i][j].t=box[i][j].t+1;                   
        }  
      }  
    }
    
    ph=3; //��������� ���� �����   
  }
  else //���� ����=3, �� �������� � ���������� ������ �������� � X3,Y3, � ����� ����� � X1,Y1, � ����� ������������ � X2,Y2
  {
    for (i=1;i<=Nx;i++)
    {
      for (j=1;j<=Ny;j++)
      { //������� ���������� �������� ���������� � ������ ������ 
        l1 = lx - sqrt((box[i][j].X3-box[i-1][j].X3+lx)*(box[i][j].X3-box[i-1][j].X3+lx) + (box[i][j].Y3-box[i-1][j].Y3)*(box[i][j].Y3-box[i-1][j].Y3));
        l2 = lx - sqrt((box[i][j].X3-box[i+1][j].X3-lx)*(box[i][j].X3-box[i+1][j].X3-lx) + (box[i][j].Y3-box[i+1][j].Y3)*(box[i][j].Y3-box[i+1][j].Y3));
        l3 = ly - sqrt((box[i][j].X3-box[i][j-1].X3)*(box[i][j].X3-box[i][j-1].X3) + (box[i][j].Y3-box[i][j-1].Y3+ly)*(box[i][j].Y3-box[i][j-1].Y3+ly));
        l4 = ly - sqrt((box[i][j].X3-box[i][j+1].X3)*(box[i][j].X3-box[i][j+1].X3) + (box[i][j].Y3-box[i][j+1].Y3-ly)*(box[i][j].Y3-box[i][j+1].Y3-ly));
        //������� ��������� � ������� ����� ������� �������� ������������ ��� X
        c1 = (box[i][j].X3-box[i-1][j].X3+lx)/(lx-l1);
        c2 = (box[i+1][j].X3-box[i][j].X3+lx)/(lx-l2);
        c3 = (box[i][j-1].X3-box[i][j].X3)/(ly-l3);
        c4 = (box[i][j+1].X3-box[i][j].X3)/(ly-l4);
        s1 = (box[i-1][j].Y3-box[i][j].Y3)/(lx-l1);
        s2 = (box[i+1][j].Y3-box[i][j].Y3)/(lx-l2);
        s3 = (box[i][j].Y3-box[i][j-1].Y3+ly)/(ly-l3);
        s4 = (box[i][j+1].Y3-box[i][j].Y3+ly)/(ly-l4);
        
        box[i][j].X2 = (k*dt*dt*(l1*c1-l2*c2-l3*c3-l4*c4)+box[i][j].X3*(f*dt+2*m)-box[i][j].X1*m)/(m+f*dt); //������� ����� ���������
        box[i][j].Y2 = (k*dt*dt*(l3*s3-l4*s4-l2*s2-l1*s1)+box[i][j].Y3*(f*dt+2*m)-box[i][j].Y1*m)/(m+f*dt);
        
        if (fabs(box[i][j].X2)<=dl) //��������� � ����������� ����������� �����������
        {
          box[i][j].X2=0;                        
        }
        if (fabs(box[i][j].Y2)<=dl)
        {
          box[i][j].Y2=0;                        
        }
        
        setfillstyle(1,COLOR(255,G(box[i][j].X1*box[i][j].X1+box[i][j].Y1*box[i][j].Y1),B(box[i][j].X1*box[i][j].X1+box[i][j].Y1*box[i][j].Y1))); //������� ������ ���������
        setcolor(COLOR(255,G(box[i][j].X1*box[i][j].X1+box[i][j].Y1*box[i][j].Y1),B(box[i][j].X1*box[i][j].X1+box[i][j].Y1*box[i][j].Y1)));
        box[i][j].draw(ph,DrawX/X,DrawY/Y); //��������� �����
        
        if (box[i][j].dev()==1) //������� ������� ���������
        {
          box[i][j].t=box[i][j].t+1;                   
        } 
      }  
    }
    
    ph=1; //��������� ���� �����   
  }
  
  setcolor(COLOR(105,105,105)); //��������� ����� 
  for (i=0;i<=Nx+1;i++)
  {
    line(i*getmaxx()/(Nx+1),0,i*getmaxx()/(Nx+1),getmaxy());  
  }
  for (j=0;j<=Ny+1;j++)
  {
    line(0,j*getmaxy()/(Ny+1),getmaxx(),j*getmaxy()/(Ny+1));  
  }
  setcolor(15);
  
  T=T+1; //����������� � �������� ������ 
};


void shutdown() //����� ������ � ���������� ���������
{
  FILE *out;
  int i,j;
  
  i=0;
  j=0; 
     
  out=fopen("output.txt","wt");
  fprintf(out,"%f \n",T*dt); //����� ������� ��������� �������
  for (i=1;i<=Nx;i++) //����� ������� ��������� ��� ������� �����
  {
    for(j=1;j<=Ny;j++)
    {
      fprintf(out,"%i ",i);
      fprintf(out,"%i ",j);
      fprintf(out,"%f \n",box[i][j].t*dt);                
    }  
  }
  
  closegraph(); //�������� ������������ ����
  fclose(out);           
};


main()
{
  int ph, sumdev; //���� �����, ��������� ���������� 
  int i,j;
  
  ph=1; //������� ��������� ��������
  T=0;
  sumdev=0;
  i=0;
  j=0;  
      
  init(); //�������������
  for (i=1;i<=Nx;i++) //������� ��������� ���������� ����������
  {
    for (j=1;j<=Ny;j++)
    {
      sumdev=sumdev+box[i][j].dev();  
    }  
  }
  
  while (sumdev != 0) //������� ���� � �������� ��������� ��������� �������
  {
    step(ph); //������� ����� ��������� � ���������� ����
    
    sumdev=0; //������� ���������� ����������
    for (i=1;i<=Nx;i++)
    {
      for (j=1;j<=Ny;j++)
      {
        sumdev=sumdev+box[i][j].dev(); 
      }  
    }
    
    Sleep(10); //�������� ��� ���������
    cleardevice(); //������� ������������ ����
  }
  
  shutdown(); //���������� ���������
};
