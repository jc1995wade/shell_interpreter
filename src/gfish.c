/*******************************************************
 * @file        gfish.c
 * @author      Dwade
 * @verson      v1.0
 * @date        2017-9-21
 * @brief       逐行解释.def文件,并运行指定命令
 *******************************************************/
#include "common.h"
#include "gfish.h"
#include <stdlib.h>
void error(char* str);
void lineParser(char * bufffer);

struct Node{
	char* vname;    // 变量名指针
	int   val;      // 保存变量值
	struct Node* next;     // 下一节点指针
}; 
typedef struct Node ValTypeDef;  // 定义存储变量名、变量值结构体

/********************************************************
 * FunctionName:      getNameMemory(char* str);
 * Description:       动态申请内存,并初始化内容为str指向
 *                    的变量名
 * Input:             str  变量名指针 
 * Output:            NULL
 *******************************************************/
char* getNameMemory(char* str)
{
	char* p = NULL;
	if((p = (char*)malloc(sizeof(str))) == NULL){
		error("get memory ERROR");
		return NULL;
	}
	memset(p, 0, sizeof(p));
	strcpy(p, str);
	return p;
}


/********************************************************
 * FunctionName:      linkNameMemory();
 * Description:       添加链表节点、并初始化节点数据
 * Input:             head 链表头节点
 *                    str  变量名字地址
 * Output:            NULL　
 *******************************************************/
void linkGetMemory(ValTypeDef* head, char* str)
{
	ValTypeDef* vp = NULL;
	ValTypeDef* tmp = NULL;
	if((vp = (ValTypeDef*)malloc(sizeof(ValTypeDef))) == NULL){
		error("get mem error");
		return ;
	}
	//　添加变量名
	char* name = NULL;
	if((name = getNameMemory(str)) == NULL){
		error("get name mem error");
		return ;
	}
	// 初始化变量名
    vp->vname = name;

	vp->val = 0;
	vp->next = NULL;
	if(head->next == NULL){
		head->next = vp;
	}
	else{
		tmp = head->next;
		head->next = vp;
		vp->next = tmp;
	}

}

/********************************************************
 * FunctionName:      linkNameSearch();
 * Description:       查找已有变量
 * Input:             hd 链表头节点
 *                    name 变量名指针
 * Output:            NULL　
 *******************************************************/
ValTypeDef * linkNameSearch(ValTypeDef* hd, char*name)
{
	while(hd->next != NULL){
		hd = hd->next;
		if(!strcmp(name, hd->vname)){
			return hd;
		}
	}
	return NULL;
}


/********************************************************
 * FunctionName:      linkAddData();
 * Description:       添加节点数据
 * Input:             head 链表头节点
 *                    name 变量名指针
 *                    data 数据指针
 * Output:            NULL　
 *******************************************************/
void linkAddData(ValTypeDef* hd, char* name, char* data)
{
	int value = atoi(data);
	ValTypeDef *p;
	p = linkNameSearch(hd, name);
	if(!p){
		printf("变量不存在\n");
	}
	else{
		p->val = value;
	}
}

// 打印变量值
void linkPrintData(ValTypeDef* hd, char* name)
{
	ValTypeDef *p;
	p = linkNameSearch(hd, name);
	if(!p){
		printf("变量不存在\n");
	}
	else{
		printf("%d\n",p->val);
	}
}

// 删除节点
void linkDelete(ValTypeDef* hd, char* name)
{
	ValTypeDef *p1;
	p1 = hd;
	while(hd->next != NULL){
		p1 = hd;
		hd = hd->next;
		if(!strcmp(name, hd->vname)){
			p1->next = hd->next;
			free(hd);
			return ;
		}
	}
	printf("变量不存在\n");

}


//删除链表
void linkDistory(ValTypeDef* hd)
{
	ValTypeDef* p1;
	ValTypeDef* p2;
	p1 = hd->next;
	if(p1 == NULL){
		return ;
	}
	while(p1->next != NULL){
		p2 = p1;
		p1 = p1->next;
		free(p2->vname);
		free(p2);
	}
	free(p1);
	hd->next = NULL;
}


/*=========出错处理函数============*/
void error(char* str)
{
	fprintf(stderr,"%s:%s\n",str, strerror(errno));
}



/********************************************************
 * FunctionName:      cutSpace();
 * Description:       截断字符串头尾空格
 * Input:             str  待截断字符串指针 
 * Output:            返回截断后的字符指针
 *******************************************************/
char *cutSpace(char *str)
{
	char *start = str;
	char *end = str + strlen(str);
	while(*start == ' ') start++;
	while(*end-- == ' ');
	*(end+1) = '\0';
	return start;
}

//判断字符串是否为数字
int isNumber(char* sec)
{
	while((*sec++ != '\0') && ((*sec < '0') || (*sec > '9')) ){
		return 1;
	}
	return 0;
}



/********************************************************
 * FunctionName:      gfishCmdAdd();
 * Description:       加法指令
 * Input:             hd      链表头结点
 *                    first   第一个加数
 *                    second  第二个加数
 * Output:            NULL
 *******************************************************/
int gADD(ValTypeDef* hd, char *first, char * second)
{
	ValTypeDef* p1;
	ValTypeDef* p2;
	p1 = linkNameSearch(hd, first);
	p2 = linkNameSearch(hd, second);
	if(!p1){
		printf("gfish:变量\"%s\"不存在\n", first);
		return 0;
	}
	else if( !isNumber(second) ){
	     p1->val += atoi(second);
		return 0;
	}
	else if(!p2){
		printf("gfish:变量\"%s\"不存在\n",second);
		return 0;
	}
	else{
		p1->val += p2->val;
	}
	return 0;
}


int gSUB(ValTypeDef* hd, char *first, char * second)
{
	ValTypeDef* p1;
	ValTypeDef* p2;
	p1 = linkNameSearch(hd, first);
	p2 = linkNameSearch(hd, second);
	if(!p1){
		printf("gfish:变量\"%s\"不存在\n", first);
		return 0;
	}
	else if( !isNumber(second) ){
	     p1->val -= atoi(second);
		return 0;
	}
	else if(!p2){
		printf("gifsh:变量\"%s\"不存在\n",second);
		return 0;
	}
	else{
		p1->val -= p2->val;
	}
	return 0;
}


int gMUL(ValTypeDef* hd, char *first, char * second)
{
	ValTypeDef* p1;
	ValTypeDef* p2;
	p1 = linkNameSearch(hd, first);
	p2 = linkNameSearch(hd, second);
	if(!p1){
		printf("gfish:变量\"%s\"不存在\n", first);
		return 0;
	}
	else if( !isNumber(second) ){
	     p1->val *= atoi(second);
		return 0;
	}
	else if(!p2){
		printf("gifsh:变量\"%s\"不存在\n",second);
		return 0;
	}
	else{
		p1->val *= p2->val;
	}
	return 0;
}

int gDIV(ValTypeDef* hd, char *first, char * second)
{
	ValTypeDef* p1;
	ValTypeDef* p2;
	p1 = linkNameSearch(hd, first);
	p2 = linkNameSearch(hd, second);
	if(!p1){
		printf("gfish:变量\"%s\"不存在\n", first);
		return 0;
	}
	else if( !isNumber(second)){
		if(atoi(second))
			p1->val /= atoi(second);
		return 0;
	}
	else if(!p2){
		printf("gifsh:变量\"%s\"不存在\n",second);
		return 0;
	}
	else{
		if(p2->val)
			p1->val /= p2->val;
	}
	return 0;
}

/*=============声明函数指针数组====================*/
int (*oper_func[])(ValTypeDef*, char*, char*) = { 
	gADD, gSUB, gMUL, gDIV  // ... 添加函数名 
};


/********************************************************
 * FunctionName:      gfish(char* str);
 * Description:       执行gfish 解释器
 * Input:             str  需要解析的文件名 
 * Output:            NULL
 *******************************************************/
void gfish(char *str)
{
	char buffer[1024];
	int i = 0;
	int start = 1;
	int rd = 0;    // 文件是否读完标志

    /*--------------指针数组用来指向操作符--------------*/

	char* op[] = { "add", "sub", "mul", "div", NULL};  // 添加操作命令
	
		

	/*============创建变量链表============*/
	ValTypeDef head;
	ValTypeDef *hd;
	hd = &head;
	head.next = NULL;

	/*============打开文件==============*/
	int fd = open(str, O_RDONLY);
	if(-1 == fd){
		error("open error");
	}


	/*============读取行数据============*/
	while(1){
		i = 0;
		memset(buffer, 0, sizeof(buffer));
		do{
			rd = read(fd, &buffer[i], 1);
		}while(buffer[i++] != '\n' && rd != 0);
		if(0 == rd){                            //读到文件末尾退出循环
			printf("file end\n");
			return ;  	
		}

		if(strlen(buffer) <= 1){           // 每行字符少于一个　跳过
			continue;
		} 
		buffer[i-1] = '\0';                // 去回车符
	

		/*-----------解析指令---------*/
		char * buf = cutSpace(buffer);   
		if(0 == strlen(buf)){               // 如果当行全是空格 跳过
			continue;
		}
		char * splitptr = " ";             // 拆分字符串
		char * oc = strtok(buf, splitptr);
		if(!strncmp(oc, "#", 1)){          // # 开头为注释
			continue;
		}
		char * sc = strtok(NULL, splitptr);
		char * tc = strtok(NULL, splitptr);


		if( (1 == start) && (!strcmp(oc, "start")) ){      // 解释开始
			printf("this is start\n");
			start = 0;
		}
		else{
			if(!strcmp(oc, "end")){             // 解释结束
				printf("end\n");
				while(hd->next != NULL){
					hd = hd->next;
					printf("link=%5s, ", hd->vname );
					printf("val =%5d\n", hd->val);
				}
				return ;
			}
			else if(!strcmp(oc, "def")){        // def ...
				if( (sc[0] >= '0' && sc[0] <= '9') ){
					printf("变量 \"%s\" 不合法\n", sc);
					continue;
				}
				else{
					linkGetMemory(&head, sc);  // def i
				}
			}
			else if(!strcmp(oc, "set")){        // set ...
				linkAddData(&head, sc, tc);
			}
			else if(!strcmp(oc, "print")){      // print ...
				linkPrintData(&head, sc);	
			}
			else if(!strcmp(oc, "undef")){
				linkDelete(&head, sc);
			}
			else{                              //　add sub ...
				int i = 0;
				while(op[i]){
					if(!strcmp(oc, op[i])){
						oper_func[i](&head, sc, tc);
					}
					i++;
				}
			}
		}
	}	
	linkDistory(&head);
	close(fd);	
}

