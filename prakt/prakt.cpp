#include "stdafx.h"	//подключение библиотек
#include <stdio.h>
#include <conio.h> 
#include <stdlib.h>
#include <malloc.h>
#include <string>
#include <clocale>

using namespace std;
using namespace System;
using namespace System::IO;

#define ENTER 13	//обозначение кнопок
#define ESC 27
#define UP 72
#define DOWN 80
#define HOME 71
#define END 79




struct z {
	char name[25];	//Название авто
	char vid[20];	//вид
	char sost [10];	//состояние
	int power;		//мощность
	int summa;		//стоимость
	char data[11];	//дата производства 

};	//шаблон структуры для исходных данных

struct sp{
	char vid[20];
	int summa;
	struct sp* sled;
	struct sp* pred;
};	//шаблон, глобальное описание структуры двусторнонего списка


int menu (int);	 //указание шаблонов используемых функций
void maxim (struct z*, int);
void slojn (struct z*, int);
void first(struct z*, int);
void text_data(char *,int,char& );
void kolvo(struct z*,int);
void alfalist(struct z*,int, struct sp**);
void vstavka(struct z*,int,char*, struct sp**);
void listing(struct z*,int);
void diagram(struct z*,int, struct sp**);



int main (array<System::String ^> ^args)
{
	//на локальном уровне осуществляем открытие файла,
	//считываем данные из файла в выделенную память 
	int i,n,NC;
	FILE *in;
	struct z *cars;
		setlocale (LC_CTYPE,"Russian");	
		system("chcp 1251");
		system ("cls");
		Console::CursorVisible::set(false);	//отключаем видимость курсора 
		Console::BufferHeight=Console::WindowHeight;///установка размера буфера по ширине
		Console::BufferWidth=Console::BufferWidth;	//установка размера буфера по высоте
		if ((in=fopen("Car.txt","r"))==NULL){	//попытка открыть файл
			printf("\nфайл не найден!");		//при неудачном открытии выводим соообщение “Файл не открыт!”
			_getch ();exit(1);
		}
	fscanf(in,"%d",&NC);
	cars=(struct z*)malloc(NC*sizeof(struct z));//выделяем память
	
	for(i=0;i<NC;i++)	//считываем данные из списка
		fscanf (in,"%s%s%d%s%d%s", cars[i].name,
		cars[i].vid,&cars[i].power,cars[i].sost,
			&cars[i].summa,cars[i].data);
	printf ("Модель                  вид авто   мощность   состояние  стоимость(т.руб)       дата выпуска");
	printf ("\n");
	for(i=0;i<NC;i++)	//выводим данные из списка на экран 
		printf("\n%-25s %-10s %-10d %-10s %-8d %25s",
			cars[i].name,
			cars[i].vid,cars[i].power, cars[i].sost,
			cars[i].summa,cars[i].data);
	_getch ();
	struct sp* spisok = nullptr;
	while (1)
//в цикле создаем область для вопросов и окрашиваем еѐ в
//устанавливаем для букв цвет и подсветку для выбранного вопроса
	{	n=menu(8);	//выбор вопроса в меню 
		switch(n){
			case 1: maxim (cars,NC);	break;
			case 2: first (cars,NC);	break;
			case 3:	listing (cars,NC);	break;
			case 4: alfalist(cars,NC,&spisok);	break;
			case 5: kolvo (cars,NC);	break;
			case 6:	diagram (cars,NC,&spisok);	break;
			case 7: slojn (cars,NC);	break;
			case 8: exit(0);
		}
	}	// конец while(1)... 
	_getch ();
	return 0;
}	// конец main()
//---------------------------------------------------------------------
 
int menu (int n)	//n=8
{
	char dan[8][60]={
	"Какая машина в автосалоне стоит дороже всех?               ",
	"Модель самой старой машины?                                ",
	"Список машин, относящихся к споркарам дороже 2 млн.руб     ",
	"Алфавитный список моделей по виду авто                     ",
	"Количество машин, относящихся к типу машин:                ",
	"Диаграмма.Процентное соотношение стоимости авто            ",
	"Машины с разными состояниями и одной ценой                 ",
	"Выход                                                      "
};		//задание списка вопросов - пунктов меню
		
		Console::ForegroundColor=ConsoleColor::White;
		Console::BackgroundColor=ConsoleColor::DarkCyan;
		Console::Clear();
		Console::CursorLeft=10;
		Console::CursorTop=4;//первая точка, откуда будем закрашивать область меню
		printf ("\n");

		for (int i=0;i<7;i++)	//размещение вопросов 
		{
			Console::CursorLeft=10;
			Console::CursorTop=i+5;
			printf("%s", dan[i]);
		}
		Console::CursorLeft=10;
		Console::CursorTop=12;//последняя точка,где будет заканчиваться выделенная область под меню
		printf("\n");
	int y1=0,y2=n-1;//устанавливаем выделение для строчки 
	char c=1;
	while (c!=ESC)	//при нажатии кнопки ESC, меню закроется 
	{
		switch (c){
			case DOWN:	y2=y1;y1++;break;//программирование кнопки вниз
			case UP:	y2=y1;y1--;break;//программирование кнопки вверх
			case ENTER: return y1+1;break;//программирование кнопки выбор
			case HOME:	if (y1!=0)y2=y1;y1=0;break;//программирование кнопки начало
			case END:	if (y1!=n-1)y2=y1;y1=n-1;break;//программирование кнопки конец
		}
		if (y1>n-1){y2=n-1;y1=0;}//условие,когда срабатывает кнопка внихз
		if (y1<0){y2=0;y1=n-1;}//условие,когда срабатывает кнопка вверх 
		Console::ForegroundColor=ConsoleColor::DarkCyan;//привыборе вопроса,буквы окрашиваются в цвет тёмно-голубой
		Console::BackgroundColor=ConsoleColor::White;	//привыборе вопроса,план за буквами подсвечивается белым
		Console::CursorLeft=10;
		Console::CursorTop=y1+5;
		printf("%s",dan[y1]);
		Console::ForegroundColor=ConsoleColor::White;
		Console::BackgroundColor=ConsoleColor::DarkCyan;
		Console::CursorLeft=10;
		Console::CursorTop=y2+5;
		printf("%s",dan[y2]);
		c=getch();
	}	// конец while(c!=ESC)... 
	exit(0);
}
//---------------------------------------------------------------------
//функция поиска наибольшей стоимости машины
void maxim (struct z* cars, int NC){
	int i=0; struct z ecars;
	strcpy (ecars.name, cars[0].name);
	ecars.summa = cars[0].summa;
	for (i=1; i<NC; i++)
		if (cars[i].summa>ecars.summa){
			strcpy (ecars.name,cars[i].name);
			ecars.summa = cars[i].summa;
		}
		Console::ForegroundColor=ConsoleColor::DarkCyan;
		Console::BackgroundColor=ConsoleColor::White;
		Console::CursorLeft =10;
		Console::CursorTop =15;
		printf ("\n");
		Console::CursorLeft = 10;
		printf ("Максимальный стоимость авто %d рублей",ecars.summa);
		Console::CursorLeft =10;
		Console::CursorTop =16;
		printf ("\n");
		Console::CursorLeft =10;
		printf ("Модель авто %s",ecars.name);
		_getch ();
}
//---------------------------------------------------------------------
//функция перевода преобразования даты 
void text_data (char *s, char *sd)
{
	char s0[3],mounth[12][9]={"января","февраля","марта","апреля","мая","июня",
"июля","августа","сентября","октября","ноября","декабря"};
	strcpy (s,sd+8);
	strcat(s," ");
	strncpy (s0,sd+5,2); s0[2]=0;
	strcat (s, mounth[ atoi(s0)-1]);
	strcat (s," ");
	strncat(s,sd,4);
	return;
}
//---------------------------------------------------------------------
//функция поиска самой старой машины
void first (struct z* cars, int NC)
{
	int i;
	char s[17];
	struct z* best = cars;
	for (i=1;i<NC;i++)
		if (strcmp(cars[i].data,best->data)<0)
			best=&cars[i];
	text_data (s, best->data);
	Console::ForegroundColor=ConsoleColor::DarkCyan;
	Console::BackgroundColor=ConsoleColor::White;
	Console::CursorTop=15;
	Console::CursorLeft=10;
	printf("Самая сарая машина %s  %s года выпуска",best->name,s);
	getch ();
}
//---------------------------------------------------------------------
//функция счёта машин, относящихся к задаваемому типу
void kolvo (struct z* cars, int NC)
{
	char s1[20]="";
	int i,k=0;
	Console::ForegroundColor=ConsoleColor::DarkCyan;
	Console::BackgroundColor=ConsoleColor::White;
	Console::CursorLeft=54;
	Console::CursorTop=9;
	Console::CursorVisible::set(true);
	scanf("%s",s1);
	Console::CursorVisible::set(false);
	for(i=0;i<NC;i++)
		if (strcmp(cars[i].vid,s1)==0)
			k++;
	
	
	Console::ForegroundColor=ConsoleColor::DarkCyan;
	Console::BackgroundColor=ConsoleColor::White;
	Console::CursorLeft=10;
	Console::CursorTop=15;
	
	printf("Машины \"%s\" ",s1);
	printf("всего : %d шт",k);
	getch();

}
//---------------------------------------------------------------------
//сортировка по типу авто
void alfalist (struct z* cars, int NC,struct sp** spisok)
{
	int i;
	struct sp* nt;
	Console::ForegroundColor=ConsoleColor::DarkCyan;
	Console::BackgroundColor=ConsoleColor::White;
	Console::Clear();
	if (!(*spisok))
		for (i=0;i<NC;i++)
			vstavka(cars,NC, cars[i].vid,spisok);
	Console::Clear();
	printf (" Алфавитный список типов машин\n");
	printf ("\nТип авто       Суммарная стоимость (т.р.)");
	printf("\n===========================================");

	for (nt=*spisok; nt!=0;nt=nt->sled)
		printf ("\n%-25s %d", nt->vid,nt->summa);

	Console::CursorLeft=50;
	Console::CursorTop=0;
	printf (" Обратный список\n\n");
	Console::CursorLeft=50;
	printf ("Тип авто       Суммарная стоимость (т.р.)\n");
	Console::CursorLeft=50;
	printf("===========================================\n");
 
	for (nt=*spisok;nt->sled!=0 ; nt=nt->sled);
		for(;nt!=0;nt=nt->pred){
			Console::CursorLeft=50;
			printf("%-25s %d\n", nt->vid,nt->summa);
		}

	getch ();
}
//---------------------------------------------------------------------
//функция заполнения двустороннего списка
void vstavka (struct z* cars,int NC,char* type,struct sp** spisok)
{
	int i;
	struct sp *now, *nt, *z=0;
	for (nt=*spisok; nt!=0 && strcmp (nt->vid,type)<0; z=nt,nt=nt->sled);
	if (nt && strcmp(nt->vid, type)==0) return;
	now=(struct sp*)malloc(sizeof(struct sp));
	strcpy (now->vid, type);
	now->pred=z;
	now->sled=nt;
	now->summa=0;
	for (i=0;i<NC; i++)
		if (strcmp (cars[i].vid,type)==0)
			now->summa+=cars[i].summa;
	if (!z)	*spisok=now;
	if (z)	z->sled=now;
	if (nt) nt->pred=now;
}
//---------------------------------------------------------------------
//функция, выводящая список машин относящихся к спорткарам дороже 2 млн.руб
void listing (struct z* cars, int NC)
{
	int i;
	struct z* nt;
	Console::ForegroundColor=ConsoleColor::DarkCyan;
	Console::BackgroundColor=ConsoleColor::White;
	Console::Clear();
	printf ("\n\rСписок машин относящихся к спорткарам дороже 2 млн.руб.");
	printf ("\n\r==================================================================\n\r");
	for (i=0,nt=cars;i<NC;nt++,i++)
		if (nt->summa > 2000 && strcmp(nt->vid,"спорткар")==0)
			printf ("\n\r %-20s %ld",nt->name,nt->summa);
	getch();
}
//---------------------------------------------------------------------
//построение и вывод диаграммы
void diagram (struct z* cars, int NC,struct sp** spisok) 
{
	struct sp *nt;
	int len,i,NColor;
	int sum = 0;
	char st1[20];
	char st2[20];
	System::ConsoleColor Color;
	Console::ForegroundColor=ConsoleColor::DarkCyan;
	Console::BackgroundColor=ConsoleColor::Black;
	Console::Clear();
	for (i=0;i<NC;i++)sum +=cars[i].summa;
	if (!(*spisok))
		for (i=0;i<NC;i++)
			vstavka (cars,NC,cars[i].vid,spisok);
	Color=ConsoleColor::Black; NColor=0;
	for (nt=*spisok,i=0;nt!=0;nt=nt->sled,i++)
	{
		sprintf(st1,"%s",nt->vid);
		sprintf(st2,"%3.1f%%",(nt->summa*100./sum));
		Console::ForegroundColor=ConsoleColor::DarkCyan;
		Console::BackgroundColor=ConsoleColor::Black;
		Console::CursorLeft=5; Console::CursorTop=i+1;
		printf(st1);
		Console::CursorLeft=40;
		printf("%s",st2);
		Console::BackgroundColor=++Color;NColor++;
		Console::CursorLeft=50;
		for (len=0;len<nt->summa*100/sum;len++)printf(" ");
		if(NColor==14)
		{Color=ConsoleColor::Black;NColor=0;		}
	}
		getch ();
		return;
}
//---------------------------------------------------------------------
//функция поиска машин с разными состояниями и одной ценой
void slojn (struct z* cars,int NC)
{
	struct z* prev = cars;
	Console::BackgroundColor=ConsoleColor::White;
	Console::ForegroundColor=ConsoleColor::DarkCyan;
	Console::CursorTop=15;
	Console::CursorLeft=10;
	for (int i=0;i<NC;i++,prev++)
		for (int z=i+1;z<NC;z++)
			if (strcmp(prev->sost,cars[z].sost)!=0 && prev->summa==cars[z].summa){
				printf ("%-21s %-10s %d\n",prev->name,prev->sost,prev->summa);
				Console::CursorLeft=10;
				printf ("%-21s %-10s %d\n",cars[z].name,cars[z].sost,cars[z].summa);
				getch ();
				return;
			}
	printf ("Нет машин в разных состояних, с однинаковой стоимостью");
	getch();
}
