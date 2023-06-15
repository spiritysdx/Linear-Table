#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>//用于函数SetConsoleOutputCP(65001);更改cmd编码为utf8
#include<stdio.h>
#include<string.h>
#include<stdlib.h>


//动态数组结构体
struct dynamicArray
{
	void ** pAddr; //维护真实在堆区创建的数组的指针

	int m_capacity;  // 数组容量

	int m_size;   //数组大小
};


//初始化数组
struct dynamicArray * init_DynamicArray(int capacity)
{
	if (capacity <= 0)
	{
		return NULL;
	}

	//给数组分配空间

	struct dynamicArray * array  = (struct dynamicArray *)malloc(sizeof(struct dynamicArray));
	if (array == NULL)
	{
		return NULL;
	}

	//给数组初始化
	array->pAddr = (void **)malloc(sizeof(void *)* capacity);
	array->m_capacity = capacity;
	array->m_size = 0;
	
	return array;
}

//插入数组
void insert_DynamicArray(struct dynamicArray * array , int pos  , void * data)
{
	if ( array == NULL)
	{
		return;
	}
	if ( data == NULL)
	{
		return;
	}

	//无效位置  尾插
	if (pos < 0 || pos > array->m_size)
	{
		pos = array->m_size;
	}

	//判断是否满了，如果满动态扩展
	if (array->m_size == array->m_capacity)
	{
		//1、计算新空间大小
		int newCapacity = array->m_capacity * 2;

		//2、创建新空间
		void ** newSpace = (void **)malloc(sizeof(void *)* newCapacity);

		//3、将原有数据拷贝到新空间下
		memcpy(newSpace, array->pAddr, sizeof(void *)* array->m_capacity);

		//4、释放原有内存空间
		free(array->pAddr);

		//5、更新新空间指向
		array->pAddr = newSpace;

		//6、更新新容量
		array->m_capacity = newCapacity;
	}

	//插入新元素

	//移动元素 进行插入新元素
	for (int i = array->m_size - 1; i >= pos; i--)
	{
		//数据向后移动
		array->pAddr[i + 1] = array->pAddr[i];
	}

	//将新元素 插入到指定位置上
	array->pAddr[pos] = data;
	//更新大小
	array->m_size++;
}


//遍历数组
void foreach_DynamicArray(struct dynamicArray * array, void(*myPrint)(void*))
{
	if (array == NULL)
	{
		return;
	}
	if (myPrint == NULL)
	{
		return;
	}

	for (int i = 0; i < array->m_size;i++)
	{
		myPrint(array->pAddr[i]);
	}
}


//删除数组  按位置删除
void removeByPos_DynamicArray(struct dynamicArray * array , int pos)
{
	if ( NULL == array)
	{
		return;
	}

	if (pos < 0  || pos > array->m_size - 1)
	{
		return;
	}

	//数据前移
	for (int i = pos; i < array->m_size - 1;i++)
	{
		array->pAddr[i] = array->pAddr[i + 1];
	}

	//更新数组大小
	array->m_size--;

}

//按值删除数据
void removeByValue_DynamicArray(struct dynamicArray * array , void * data , int (* myCompare)(void * ,void *))
{
	if ( array == NULL)
	{
		return;
	}
	if ( data == NULL)
	{
		return;
	}

	for (int i = 0; i < array->m_size;i++)
	{
		if (myCompare(array->pAddr[i], data))
		{
				//如果找到要删除的数据，i就是要删除的具体位置
			removeByPos_DynamicArray(array, i);
			break;
		}
	}

}

//销毁数组
void destroy_DynamicArray(struct dynamicArray* array)
{
	if (array == NULL)
	{
		return;
	}

	if (array->pAddr != NULL)
	{
		free(array->pAddr);
		array->pAddr = NULL;
	}


	free(array);
	array = NULL;
}








//测试
struct Person
{
	char name[64];
	int age;
};


void myPrintPerson(void *data)
{
	struct Person * p = (struct Person *)data;
	printf("姓名： %s 年龄：%d\n", p->name, p->age);

}

int myComparePerson(void * data1, void *data2)
{
	struct Person *  p1 = (struct Person *)data1;
	struct Person *  p2 = (struct Person *)data2;

	return  strcmp(p1->name, p2->name) == 0 && p1->age == p2->age;
}

int main(){
	SetConsoleOutputCP(65001);

	//初始化动态数组
	struct dynamicArray * array = init_DynamicArray(5);

	//准备数据
	struct Person p1 = { "亚瑟", 18 };
	struct Person p2 = { "妲己", 20 };
	struct Person p3 = { "安琪拉", 19 };
	struct Person p4 = { "凯", 21 };
	struct Person p5 = { "孙悟空", 999 };
	struct Person p6 = { "李白", 999};

	printf("插入数据前： 容量：%d  大小：%d\n", array->m_capacity, array->m_size);

	//插入数据
	insert_DynamicArray(array, 0, &p1);
	insert_DynamicArray(array, 0, &p2);
	insert_DynamicArray(array, 1, &p3);
	insert_DynamicArray(array, 0, &p4);
	insert_DynamicArray(array, -1, &p5);
	insert_DynamicArray(array, 2, &p6);

	// 凯 妲己 李白 安琪拉  亚瑟  孙悟空

	//遍历数据
	foreach_DynamicArray(array, myPrintPerson);


	printf("插入数据后： 容量：%d  大小：%d\n", array->m_capacity, array->m_size);


	//测试删除 按位置删除
	removeByPos_DynamicArray(array, 2);
	printf("---------------------\n");
	foreach_DynamicArray(array, myPrintPerson);
	printf("删除数据后： 容量：%d  大小：%d\n", array->m_capacity, array->m_size);

	struct Person  p = { "亚瑟", 18 };
	removeByValue_DynamicArray(array, &p, myComparePerson);
	printf("---------------------\n");
	foreach_DynamicArray(array, myPrintPerson);
	printf("删除数据后： 容量：%d  大小：%d\n", array->m_capacity, array->m_size);


	//array->pAddr = NULL;
	//array->m_capacity = 0;
	//array->m_size = 0;

	//销毁数组
	destroy_DynamicArray(array);
	array = NULL;

	system("pause");
	return EXIT_SUCCESS;
}