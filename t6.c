#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#pragma()
char str1[100];
char str2[100];

int k=0;
/* 定义WORD为两个字节的类型 */
typedef unsigned short WORD;
/* 定义DWORD为四个字节的类型 */
typedef unsigned int DWORD;
typedef struct BMP_FILE_HEADER
{
	WORD bType;      // 文件标识符 
	DWORD bSize;     // 文件的大小 
	WORD bReserved1; // 保留值,必须设置为0  
	WORD bReserved2; // 保留值,必须设置为0 
	DWORD bOffset;   // 文件头的最后到图像数据位开始的偏移量 
} __attribute__((packed)) BMPFILEHEADER;     // 14 字节
 
/* 位图信息头 */
typedef struct BMP_INFO
{
	DWORD bInfoSize;            // 信息头的大小   
	DWORD bWidth;               // 图像的宽度     
	DWORD bHeight;              // 图像的高度     
	WORD bPlanes;               // 图像的位面数   
	WORD bBitCount;             // 每个像素的位数 
	DWORD bCompression;         // 压缩类型 
	DWORD bmpImageSize;         // 图像的大小,以字节为单位 
	DWORD bXPelsPerMeter;       // 水平分辨率 
	DWORD bYPelsPerMeter;       // 垂直分辨率 
	DWORD bClrUsed;             // 使用的色彩数 
	DWORD bClrImportant;        // 重要的颜色数 
} __attribute__((packed))  BMPINF;            // 40 字节
 
/* 彩色表:调色板 */
typedef struct RGB_QUAD 
{
	char rgbBlue;     // 蓝色强度 
	char rgbGreen;    // 绿色强度 
	char rgbRed;      // 红色强度 
	char rgbReversed; // 保留值 
} __attribute__((packed)) RGBQUAD;

typedef struct list
{
    char name[50];
    BMPFILEHEADER head1;
    BMPINF info;
    char * rgb;
    unsigned char  *point;
} list1;

list1 arr[100];

int main()
{
	char *pp;
    int fd[2];
    pipe(fd);
	if(pipe(fd)==-1)
	{
		printf("管道创建失败");
	}
	fcntl(fd[0], F_GETFL);
	fcntl(fd[1], F_GETFL);
	while(1)
	{
		printf("请输入要处理的图片路径\n");
		scanf("%s",str1);
		printf("请输入图片处理后的输出路径\n");
		scanf("%s",str2);
	
		pid_t pid =fork();
		if(pid==-1)
		{
			printf("无法创建进程");
		}
		else if(pid==0)
		{
			//子进程运行图片处理
			printf("当前子进程pid%d\n",getpid());
			int xunhuan=0;
			int x=-1;
			for(int i=0;i<100;i++)
			{
   				if (strcmp(str1, arr[i].name) == 0)
   				{
					xunhuan =1;
					x=i;
					break;
   				}
			}
			if(xunhuan==1)
			{
    			printf("缓存处理\n");
	
    			BMPFILEHEADER bmpFileHeader;  // 定义一个 BMP 文件头的结构体
				bmpFileHeader = arr[x].head1;
				BMPINF bmpInfo;               // 定义一个 BMP 文件信息结构体
				bmpInfo =arr[x].info ;
    


   
 
 
	// 输出BMP文件的位图文件头的所有信息
    //int arr[bmpInfo.bWidth][bmpInfo.bHeight];
    unsigned char  *p;
    p = arr[x].point;
        unsigned char  *p1;
    p1 = arr[x].rgb;

	unsigned char * ppt = (unsigned char *)malloc(sizeof(unsigned char )* bmpInfo.bHeight*bmpInfo.bWidth);
	 memcpy(ppt,arr[x].point,sizeof(unsigned char )* bmpInfo.bHeight*bmpInfo.bWidth);
    
	// for(int i=0;i<bmpInfo.bHeight*bmpInfo.bWidth;i++)
	// {
	// 	if(ppt[i]>80)
	// 	{
	// 		ppt[i]=255;
	// 	}
	// 	else{
	// 		ppt[i]=0;
	// 	}
	// }

	printf("位图的高度 = %d \n", bmpInfo.bHeight);
	printf("位图的宽度 = %d \n", bmpInfo.bWidth);
	printf("每个像素的位数 = %d 位\n", bmpInfo.bBitCount);


	FILE *fwp;

if((fwp = fopen(str2,"wb")) == NULL)
{
		printf("Cann't create the file!\n");
		return 0;
	}
    fseek(fwp, 0, SEEK_SET);   
    fwrite(&bmpFileHeader, 14, 1, fwp);
    fwrite(&bmpInfo, 40, 1, fwp);
    
    fwrite(p1, 1024, 1, fwp);

    fwrite(ppt, bmpInfo.bHeight*bmpInfo.bWidth, 1, fwp);

     free(p);
    free(ppt);
    fclose(fwp);
}
else{
    printf("正常处理\n");
    
	FILE *fp;                     // 定义一个文件指针
	BMPFILEHEADER bmpFileHeader;  // 定义一个 BMP 文件头的结构体
	BMPINF bmpInfo;               // 定义一个 BMP 文件信息结构体 
    RGBQUAD bgr;
	if((fp = fopen(str1, "rb")) == NULL) // fp = 0x00426aa0
	{
		printf("Cann't open the file!\n");
		return 0;
	}
    printf("fp是什么-%p\n",*fp);
    printf("fp大小%d\n",sizeof(*fp));
	fseek(fp, 0, SEEK_SET);                            // fp = 0000
	// 读取文件信息
	/*
		参数1: 读出数据的存放缓冲区
		参数2: 读取多少个字节的数据
		参数3: 读取几包这样的数据
		参数4: 文件指针（源数据）
	*/
    
	fread(&bmpFileHeader, 14, 1, fp); 
	fread(&bmpInfo, 40, 1, fp);
    unsigned char  *p;
    p = (unsigned char *)malloc(sizeof(unsigned char )* bmpInfo.bHeight*bmpInfo.bWidth);
        unsigned char  *p1;
    p1 = (unsigned char *)malloc(sizeof(unsigned char )*1024);
   	fread(p1,1024,1,fp);
	fread(p, bmpInfo.bHeight*bmpInfo.bWidth,1,fp);
	
	printf("hello1 \n");
	close(fd[0]);
	write(fd[1],str1,18);
	
	write(fd[1],&bmpFileHeader,14);
	write(fd[1],&bmpInfo,40);
	write(fd[1],p1,1024);
	/*
	write(fd[1],p,sizeof(unsigned char )* bmpInfo.bHeight*bmpInfo.bWidth);
	
	*/
  


	close(fd[1]);
	//二值化处理
	for(int i=0;i<bmpInfo.bHeight*bmpInfo.bWidth;i++)
	{
		if(p[i]>80)
		{
			p[i]=255;
		}
		else{
			p[i]=0;
		}
	}
    printf("文件头大小%ld \n",sizeof(BMPFILEHEADER));
    printf("文件信息大小%ld \n",sizeof(BMPINF));
    printf("调色板信息大小%ld \n",sizeof(RGBQUAD));
    printf("BMP 文件大小 = %d 字节\n", bmpFileHeader.bSize);
	printf("psinyi = 0X%X\n", bmpFileHeader.bOffset);
	printf("信息头的大小 = %d 字节\n", bmpInfo.bInfoSize);
	printf("位图的高度 = %d \n", bmpInfo.bHeight);
	printf("位图的宽度 = %d \n", bmpInfo.bWidth);
	printf("每个像素的位数 = %d 位\n", bmpInfo.bBitCount);
	printf("图像的大小 = %d 字节\n", bmpInfo.bmpImageSize);

    FILE *fwp;

    if((fwp = fopen(str2,"wb")) == NULL)
    {
		printf("Cann't create the file!\n");
		return 0;
	}
    fseek(fwp, 0, SEEK_SET);   
    fwrite(&bmpFileHeader, 14, 1, fwp);
    fwrite(&bmpInfo, 40, 1, fwp);
    fwrite(p1, 1024, 1, fwp);
    fwrite(p, bmpInfo.bHeight*bmpInfo.bWidth, 1, fwp);

    free(p);
    free(p1);
	fclose(fp);
    fclose(fwp);

	
}
    
    
}
else
{
	close(fd[1]);

	read(fd[0],arr[k].name,18);
	printf("%s \n",arr[k].name);
	
	read(fd[0],&arr[k].head1,54);
	arr[k].rgb = (char *)malloc(1024);
	read(fd[0],arr[k].rgb,1024);
	/*
	arr[k].point  = (unsigned char *)malloc(sizeof(unsigned char )* arr[k].info.bHeight* arr[k].info.bWidth);
	read(fd[0],arr[k].point,sizeof(unsigned char )*  arr[k].info.bHeight* arr[k].info.bWidth);
	*/
	close(fd[0]);

    k++;
}
 }


    return 0;
}