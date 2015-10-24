#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#define MAXSTRINGS 128*1024*2024/2
//��������� ����� ��� ���������� � ��������� ������
int sort_func( const void *a, const void *b)
{
	return strcmp(*((char**)a),*((char**)b));
}
int main(int argc, char *argv[])
{
	char str[512];
	char *ptr=str;
	char *text=NULL;
	char **index=NULL;
	FILE *f;
	char **fnd,**key=&ptr;
	char *pch;
	size_t size,indsize;
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
	//��������� ������ ��� ����� �����+����������� 0
	text=(char*)malloc(size+1);
	if(!text)
	{
		printf("Not enough memory.\n");
		return 1;
	}
	//��������� ������ ��� ����������� ��������� ���-�� �������� �����
	index=(char**)malloc(sizeof(char*)*MAXSTRINGS);
	if(!text)
	{
		printf("Not enough memory.\n");
		return 1;
	}
	beg=clock();
	//������ ������ ����� ����� � ������
	fread(text,1,size,f);
	end=clock();
	printf("Reading: %d msec\n",(int)((end-beg)*1000/CLOCKS_PER_SEC));
	fclose(f);
	text[size]=0;
	indsize=0;
	beg=clock();
/*	pch = strtok (text,"\r\n");
	while(pch != NULL)
	{
		pch = strtok (NULL, "\r\n");
		index[indsize]=pch;
		indsize++;
	}
	*/
//����� ������� �������� ������� strtok
	for(pch=text;*pch;)	//������� ����
	{
		for(;*pch&&(*pch=='\r'||*pch=='\n');pch++)
			*pch=0;//����� ������ �����	
		if(*pch)		//���� �������
		{
			index[indsize]=pch;	//��������� ��������� �� ������ �����(������) 
			indsize++;
			for(;*pch&&*pch!='\r'&&*pch!='\n';pch++);	//����� ����� �����
		}
	}
	//������������� ������ ��� �������, ����� �� �������� ������
	//������������ ���� ��� �� ������� ���������
	index=(char**)realloc(index,sizeof(char*)*indsize);
	end=clock();
	printf("Parsing: %d msec\n",(int)((end-beg)*1000/CLOCKS_PER_SEC));
	beg=clock();
//���������� ������� ��������, ������ ������������ ���������� ������� �� �������
	qsort(index,indsize,sizeof(char*),sort_func);	//������������� ������ �����
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
		//�������� ����� ������
		fnd=(char**)bsearch(key,index,indsize,sizeof(char*),sort_func);
		end=clock();
		if(fnd)
			printf("YES\n");
		else
			printf("NO\n");
	}
	free(text);
	free(index);
	return 0;
}
