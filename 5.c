#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#define MAXSTRINGS 128*1024*2024/2
//сравнение строк для сортировки и двоичного поиска
int sort_func( const void *a, const void *b)
{
	return strcmp(*((char**)a),*((char**)b));
}
int main(int argc, char *argv[])
{
	char str[512];
	char *ptr=str;
	char *text=NULL;
	char **pointers=NULL;
	FILE *f;
	char **fnd,**key=&ptr;
	char *pch;
	size_t size,ptrsize;
	clock_t beg,end;
	if(argc!=2)
	{
		printf("Parameter not found.\n");
		return 1;
	}
	f=fopen(argv[1],"rb");
	if(!f)
	{
		printf("Can't open file.\n");
		return 1;
	}
	fseek(f,0,SEEK_END);
	size=ftell(f);
	fseek(f,0,SEEK_SET);
	//Выделение памяти под объем файла+завершающий 0
	text=(char*)malloc(size+1);
	if(!text)
	{
		printf("Not enough memory.\n");
		return 1;
	}
	//Выделение памяти под максимально возможное кол-во указателей на строки
	pointers=(char**)malloc(sizeof(char*)*MAXSTRINGS);
	if(!text)
	{
		printf("Not enough memory.\n");
		return 1;
	}
	beg=clock();
	//Просто чтение всего файла в память
	fread(text,1,size,f);
	end=clock();
	printf("Reading: %d msec\n",(int)((end-beg)*1000/CLOCKS_PER_SEC));
	fclose(f);
	text[size]=0;
	ptrsize=0;
	beg=clock();
/*	pch = strtok (text,"\r\n");
	while(pch != NULL)
	{
		pch = strtok (NULL, "\r\n");
		pointers[ptrsize]=pch;
		ptrsize++;
	}
	*/
//такой парсинг оказался быстрее strtok
	for(pch=text;*pch;)	//перебор слов
	{
		for(;*pch&&(*pch=='\r'||*pch=='\n');pch++)
			*pch=0;//поиск начала слова	
		if(*pch)		//если найдено
		{
			pointers[ptrsize]=pch;	//запомнить указатель на начало слова(строки) 
			ptrsize++;
			for(;*pch&&*pch!='\r'&&*pch!='\n';pch++);	//поиск конца слова
		}
	}
	//Перевыделение памяти под указатели, чтобы не занимать лишнюю
	//Динамический рост был бы слишком медленным
	pointers=(char**)realloc(pointers,sizeof(char*)*ptrsize);
	end=clock();
	printf("Parsing: %d msec\n",(int)((end-beg)*1000/CLOCKS_PER_SEC));
	beg=clock();
//сортировка массива указателей, быстее библиотечной реализации сделать не удалось
	qsort(pointers,ptrsize,sizeof(char*),sort_func);	//Отсортировать массив указателей на строки
	end=clock();
	printf("Sorting: %d msec\n",(int)((end-beg)*1000/CLOCKS_PER_SEC));
	printf("Enter strings:\n");
	while(1)
	{
		fgets(str,512,stdin);
		if(10==str[strlen(str)-1])
			str[strlen(str)-1]=0;
		if(0==strcmp("exit",str))
			break;
		beg=clock();
		//Двоичный поиск строки
		fnd=(char**)bsearch(key,pointers,ptrsize,sizeof(char*),sort_func);
		end=clock();
		if(fnd)
			printf("YES\n");
		else
			printf("NO\n");
	}
	free(text);
	free(pointers);
	return 0;
}
