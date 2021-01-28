//Колебания решётки 
//Сташков Александр 2102
//19.12.10 

#include <stdio.h>
#include <math.h>
#include <graphics.h>
#include "weight.h"

int Nx,Ny, DrawX,DrawY; //Кол-во грузов в решётке, размеры графического окна
float m, k, f; //Масса грузов, жесткость пружин, коэффициент трения(friction)
float X,Y, lx,ly, dl; //Размеры камеры, длины пружин в покое, минимальное учитываемое смещение
float dt, T; //Промежуток времени, кол-во прошедших циклов
weight box[100][100]; //Массив грузов


void init() //считывание входных данных и приравнивание координат начальным
{
  FILE *in;
  int Nd, x,y; //Кол-во отклонений, номера отклонённых грузов
  float dx,dy; //Величины отклонения
  int i,j;
  
  i=0;
  j=0;
  dt=0.1;
  in=fopen("input.txt","rt");
  fscanf(in,"Размеры графического окна: %i ",&DrawX);
  fscanf(in,"х %i \n",&DrawY);
  fscanf(in,"Размеры камеры: %f ",&X);
  fscanf(in,"х %f \n",&Y);
  fscanf(in,"Кол-во грузов: %i ",&Nx);
  fscanf(in,"х %i \n",&Ny);
  fscanf(in,"Масса: %f \n",&m);
  fscanf(in,"Жёсткость: %f \n",&k);
  fscanf(in,"Трение: %f \n",&f);
  fscanf(in,"Минимальные учитываемые смещения: %f \n",&dl);
  fscanf(in,"Кол-во смещений: %i \n",&Nd);
  
  lx = X/(Nx+1); //рассчёт длины пружин в покое
  ly = Y/(Ny+1); 
  
  for (i=0;i<=Nx+1;i++) //задание координат грузов и обнуление времени затухания
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
  
  for (i=0;i<Nd;i++) //считывание и задание начальных отклонений
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
  
  initwindow(DrawX,DrawY); //инициализация графического окна
  fclose(in);     
};


int G(float dr) //знечение G в модели цветовой гаммы RGB, в зависимости от смещения
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

int B(float dr) //знечение B в модели цветовой гаммы RGB, в зависимости от смещения
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


void step(int& ph) //рассчёт координат грузов в следующий момент времени, в зависимости от фазы цикла и промежутка времени
{                 //Фаза цикла сообщает какие переменные хранят смещение в два предыдущих момента, а какие могут быть изменены
  int i,j;
  float l1,l2,l3,l4, c1,c2,c3,c4,s1,s2,s3,s4; //Расстяжение пружин, косинусы и синусы наплона пружин к оси X
  
  i=0;
  j=0;   
     
  if (ph==1) //Если фаза=1, то смещение в предыдущий момент хранится в X2,Y2, в более раний в X3,Y3, а новые записываются в X1,Y1
  {
    for (i=1;i<=Nx;i++)
    {
      for (j=1;j<=Ny;j++)
      { //рассчёт деформации пружинок соединённых с данным грузом 
        l1 = lx - sqrt((box[i][j].X2-box[i-1][j].X2+lx)*(box[i][j].X2-box[i-1][j].X2+lx) + (box[i][j].Y2-box[i-1][j].Y2)*(box[i][j].Y2-box[i-1][j].Y2));
        l2 = lx - sqrt((box[i][j].X2-box[i+1][j].X2-lx)*(box[i][j].X2-box[i+1][j].X2-lx) + (box[i][j].Y2-box[i+1][j].Y2)*(box[i][j].Y2-box[i+1][j].Y2));
        l3 = ly - sqrt((box[i][j].X2-box[i][j-1].X2)*(box[i][j].X2-box[i][j-1].X2) + (box[i][j].Y2-box[i][j-1].Y2+ly)*(box[i][j].Y2-box[i][j-1].Y2+ly));
        l4 = ly - sqrt((box[i][j].X2-box[i][j+1].X2)*(box[i][j].X2-box[i][j+1].X2) + (box[i][j].Y2-box[i][j+1].Y2-ly)*(box[i][j].Y2-box[i][j+1].Y2-ly));
        //рассчёт косинусов и синусов углов наклона пружинок относительно оси X
        c1 = (box[i][j].X2-box[i-1][j].X2+lx)/(lx-l1);
        c2 = (box[i+1][j].X2-box[i][j].X2+lx)/(lx-l2);
        c3 = (box[i][j-1].X2-box[i][j].X2)/(ly-l3);
        c4 = (box[i][j+1].X2-box[i][j].X2)/(ly-l4);
        s1 = (box[i-1][j].Y2-box[i][j].Y2)/(lx-l1);
        s2 = (box[i+1][j].Y2-box[i][j].Y2)/(lx-l2);
        s3 = (box[i][j].Y2-box[i][j-1].Y2+ly)/(ly-l3);
        s4 = (box[i][j+1].Y2-box[i][j].Y2+ly)/(ly-l4);
        
        box[i][j].X1 = (k*dt*dt*(l1*c1-l2*c2-l3*c3-l4*c4)+box[i][j].X2*(f*dt+2*m)-box[i][j].X3*m)/(m+f*dt); //рассчёт новых координат
        box[i][j].Y1 = (k*dt*dt*(l3*s3-l4*s4-l2*s2-l1*s1)+box[i][j].Y2*(f*dt+2*m)-box[i][j].Y3*m)/(m+f*dt);
        
        if (fabs(box[i][j].X1)<=dl) //сравнение с минимальным учитываемым отклонением
        {
          box[i][j].X1=0;                        
        }
        if (fabs(box[i][j].Y1)<=dl)
        {
          box[i][j].Y1=0;                        
        }
        
        setfillstyle(1,COLOR(255,G(box[i][j].X1*box[i][j].X1+box[i][j].Y1*box[i][j].Y1),B(box[i][j].X1*box[i][j].X1+box[i][j].Y1*box[i][j].Y1))); //задание цветов рисования
        setcolor(COLOR(255,G(box[i][j].X1*box[i][j].X1+box[i][j].Y1*box[i][j].Y1),B(box[i][j].X1*box[i][j].X1+box[i][j].Y1*box[i][j].Y1)));
        box[i][j].draw(ph,DrawX/X,DrawY/Y); //рисование груза
        
        if (box[i][j].dev()==1) //счётчик времени колебаний
        {
          box[i][j].t=box[i][j].t+1;                   
        } 
      }  
    }
    
    ph=2; //изменение фазы цикла     
  }
  else if (ph==2) //Если фаза=2, то смещение в предыдущий момент хранится в X1,Y1, в более раний в X2,Y2, а новые записываются в X3,Y3
  {
    for (i=1;i<=Nx;i++)
    {
      for (j=1;j<=Ny;j++)
      { //рассчёт деформации пружинок соединённых с данным грузом 
        l1 = lx - sqrt((box[i][j].X1-box[i-1][j].X1+lx)*(box[i][j].X1-box[i-1][j].X1+lx) + (box[i][j].Y1-box[i-1][j].Y1)*(box[i][j].Y1-box[i-1][j].Y1));
        l2 = lx - sqrt((box[i][j].X1-box[i+1][j].X1-lx)*(box[i][j].X1-box[i+1][j].X1-lx) + (box[i][j].Y1-box[i+1][j].Y1)*(box[i][j].Y1-box[i+1][j].Y1));
        l3 = ly - sqrt((box[i][j].X1-box[i][j-1].X1)*(box[i][j].X1-box[i][j-1].X1) + (box[i][j].Y1-box[i][j-1].Y1+ly)*(box[i][j].Y1-box[i][j-1].Y1+ly));
        l4 = ly - sqrt((box[i][j].X1-box[i][j+1].X1)*(box[i][j].X1-box[i][j+1].X1) + (box[i][j].Y1-box[i][j+1].Y1-ly)*(box[i][j].Y1-box[i][j+1].Y1-ly));
        //рассчёт косинусов и синусов углов наклона пружинок относительно оси X
        c1 = (box[i][j].X1-box[i-1][j].X1+lx)/(lx-l1);
        c2 = (box[i+1][j].X1-box[i][j].X1+lx)/(lx-l2);
        c3 = (box[i][j-1].X1-box[i][j].X1)/(ly-l3);
        c4 = (box[i][j+1].X1-box[i][j].X1)/(ly-l4);
        s1 = (box[i-1][j].Y1-box[i][j].Y1)/(lx-l1);
        s2 = (box[i+1][j].Y1-box[i][j].Y1)/(lx-l2);
        s3 = (box[i][j].Y1-box[i][j-1].Y1+ly)/(ly-l3);
        s4 = (box[i][j+1].Y1-box[i][j].Y1+ly)/(ly-l4);
        
        box[i][j].X3 = (k*dt*dt*(l1*c1-l2*c2-l3*c3-l4*c4)+box[i][j].X1*(f*dt+2*m)-box[i][j].X2*m)/(m+f*dt); //рассчёт новых координат
        box[i][j].Y3 = (k*dt*dt*(l3*s3-l4*s4-l2*s2-l1*s1)+box[i][j].Y1*(f*dt+2*m)-box[i][j].Y2*m)/(m+f*dt);
        
        if (fabs(box[i][j].X3)<=dl) //сравнение с минимальным учитываемым отклонением
        {
          box[i][j].X3=0;                        
        }
        if (fabs(box[i][j].Y3)<=dl)
        {
          box[i][j].Y3=0;                        
        }
        
        setfillstyle(1,COLOR(255,G(box[i][j].X1*box[i][j].X1+box[i][j].Y1*box[i][j].Y1),B(box[i][j].X1*box[i][j].X1+box[i][j].Y1*box[i][j].Y1))); //задание цветов рисования
        setcolor(COLOR(255,G(box[i][j].X1*box[i][j].X1+box[i][j].Y1*box[i][j].Y1),B(box[i][j].X1*box[i][j].X1+box[i][j].Y1*box[i][j].Y1)));
        box[i][j].draw(ph,DrawX/X,DrawY/Y); //рисование груза
        
        if (box[i][j].dev()==1) //счётчик времени колебаний
        {
          box[i][j].t=box[i][j].t+1;                   
        }  
      }  
    }
    
    ph=3; //изменение фазы цикла   
  }
  else //Если фаза=3, то смещение в предыдущий момент хранится в X3,Y3, в более раний в X1,Y1, а новые записываются в X2,Y2
  {
    for (i=1;i<=Nx;i++)
    {
      for (j=1;j<=Ny;j++)
      { //рассчёт деформации пружинок соединённых с данным грузом 
        l1 = lx - sqrt((box[i][j].X3-box[i-1][j].X3+lx)*(box[i][j].X3-box[i-1][j].X3+lx) + (box[i][j].Y3-box[i-1][j].Y3)*(box[i][j].Y3-box[i-1][j].Y3));
        l2 = lx - sqrt((box[i][j].X3-box[i+1][j].X3-lx)*(box[i][j].X3-box[i+1][j].X3-lx) + (box[i][j].Y3-box[i+1][j].Y3)*(box[i][j].Y3-box[i+1][j].Y3));
        l3 = ly - sqrt((box[i][j].X3-box[i][j-1].X3)*(box[i][j].X3-box[i][j-1].X3) + (box[i][j].Y3-box[i][j-1].Y3+ly)*(box[i][j].Y3-box[i][j-1].Y3+ly));
        l4 = ly - sqrt((box[i][j].X3-box[i][j+1].X3)*(box[i][j].X3-box[i][j+1].X3) + (box[i][j].Y3-box[i][j+1].Y3-ly)*(box[i][j].Y3-box[i][j+1].Y3-ly));
        //рассчёт косинусов и синусов углов наклона пружинок относительно оси X
        c1 = (box[i][j].X3-box[i-1][j].X3+lx)/(lx-l1);
        c2 = (box[i+1][j].X3-box[i][j].X3+lx)/(lx-l2);
        c3 = (box[i][j-1].X3-box[i][j].X3)/(ly-l3);
        c4 = (box[i][j+1].X3-box[i][j].X3)/(ly-l4);
        s1 = (box[i-1][j].Y3-box[i][j].Y3)/(lx-l1);
        s2 = (box[i+1][j].Y3-box[i][j].Y3)/(lx-l2);
        s3 = (box[i][j].Y3-box[i][j-1].Y3+ly)/(ly-l3);
        s4 = (box[i][j+1].Y3-box[i][j].Y3+ly)/(ly-l4);
        
        box[i][j].X2 = (k*dt*dt*(l1*c1-l2*c2-l3*c3-l4*c4)+box[i][j].X3*(f*dt+2*m)-box[i][j].X1*m)/(m+f*dt); //рассчёт новых координат
        box[i][j].Y2 = (k*dt*dt*(l3*s3-l4*s4-l2*s2-l1*s1)+box[i][j].Y3*(f*dt+2*m)-box[i][j].Y1*m)/(m+f*dt);
        
        if (fabs(box[i][j].X2)<=dl) //сравнение с минимальным учитываемым отклонением
        {
          box[i][j].X2=0;                        
        }
        if (fabs(box[i][j].Y2)<=dl)
        {
          box[i][j].Y2=0;                        
        }
        
        setfillstyle(1,COLOR(255,G(box[i][j].X1*box[i][j].X1+box[i][j].Y1*box[i][j].Y1),B(box[i][j].X1*box[i][j].X1+box[i][j].Y1*box[i][j].Y1))); //задание цветов рисования
        setcolor(COLOR(255,G(box[i][j].X1*box[i][j].X1+box[i][j].Y1*box[i][j].Y1),B(box[i][j].X1*box[i][j].X1+box[i][j].Y1*box[i][j].Y1)));
        box[i][j].draw(ph,DrawX/X,DrawY/Y); //рисование груза
        
        if (box[i][j].dev()==1) //счётчик времени колебаний
        {
          box[i][j].t=box[i][j].t+1;                   
        } 
      }  
    }
    
    ph=1; //изменение фазы цикла   
  }
  
  setcolor(COLOR(105,105,105)); //рисование сетки 
  for (i=0;i<=Nx+1;i++)
  {
    line(i*getmaxx()/(Nx+1),0,i*getmaxx()/(Nx+1),getmaxy());  
  }
  for (j=0;j<=Ny+1;j++)
  {
    line(0,j*getmaxy()/(Ny+1),getmaxx(),j*getmaxy()/(Ny+1));  
  }
  setcolor(15);
  
  T=T+1; //прибавление к счётчику циклов 
};


void shutdown() //Вывод данных и завершение программы
{
  FILE *out;
  int i,j;
  
  i=0;
  j=0; 
     
  out=fopen("output.txt","wt");
  fprintf(out,"%f \n",T*dt); //вывод времени затухания системы
  for (i=1;i<=Nx;i++) //вывод времени затухания для каждого груза
  {
    for(j=1;j<=Ny;j++)
    {
      fprintf(out,"%i ",i);
      fprintf(out,"%i ",j);
      fprintf(out,"%f \n",box[i][j].t*dt);                
    }  
  }
  
  closegraph(); //закрытие графического окна
  fclose(out);           
};


main()
{
  int ph, sumdev; //Фаза цикла, суммарное отклонение 
  int i,j;
  
  ph=1; //задание начальных значений
  T=0;
  sumdev=0;
  i=0;
  j=0;  
      
  init(); //Инициализация
  for (i=1;i<=Nx;i++) //рассчёт исходного суммарного отклонения
  {
    for (j=1;j<=Ny;j++)
    {
      sumdev=sumdev+box[i][j].dev();  
    }  
  }
  
  while (sumdev != 0) //рабочий цикл с условием затухания колебаний системы
  {
    step(ph); //рассчёт новых координат и измениение фазы
    
    sumdev=0; //рассчёт суммарного отклонения
    for (i=1;i<=Nx;i++)
    {
      for (j=1;j<=Ny;j++)
      {
        sumdev=sumdev+box[i][j].dev(); 
      }  
    }
    
    Sleep(10); //задержка для рисования
    cleardevice(); //очистка графического окна
  }
  
  shutdown(); //завершение программы
};
