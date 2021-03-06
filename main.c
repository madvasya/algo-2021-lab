/*Задание:
Из входного потока вводится непрямоугольная матрица 
целых чисел [aij], i = 1, …, m, j = 1, …, n.  
Значения m и ni заранее не известны и вводятся 
из входного потока.

Сформировать новую матрицу, поместив в ее i-ую строку 
элементы из i-ой строки исходной матрицы, 
начиная с первого минимального элемента и до последнего элемента 
строки.

Исходную и полученную матрицы вывести в 
выходной поток с необходимыми комментариями.
*/

#include <stdio.h>
#include <malloc.h>
#include <locale.h>

//структура задающая строчку матрицы
typedef struct Line{
	int n;
	int *a;
}Line;

typedef struct Matrix{
	int lines;
	Line *matr;
}Matrix;

//прототипы функций
int getInt(int*);
//int getDouble(double *);
int input(Matrix *a);
void output(const char *msg, Matrix a);
void erase(Matrix *a);
void processMatrix(Matrix a, Matrix *b);
int min(int a[], int m);

int main(){
	setlocale(LC_ALL, "");
	printf("Здравствуйте! Вводим матричку.");
	Matrix matr = {0, NULL};
	Matrix res = {0, NULL};

	if(input(&matr) == 0){
		printf("EOF");
		return 1;
	}
	output("\nИсходная матрица:", matr);
	processMatrix(matr, &res);
	output("\nИзменённая матрица:", res);
	erase(&matr);
	erase(&res);
	return 0;
}

//Записываем целое число, введенное пользователем, по адресу a.
//Возвращает 0 если ввели мусор и 1, если ввод прошел успешно.
int getInt(int *a){
	int n;
	do{
		n = scanf("%d", a);
		if(n < 0){//EOF
			return 0;
		}
		if(n == 0){
			printf("Некорректный ввод! Ожидалось целое число.\n");			
			scanf("%*[^\n]");
			
			//scanf("%*s");	
			//scanf("%*c", 0);
		}	
	}while(n == 0);
	return 1;
}

int input(Matrix *rm){
	const char *pr = "";
	int m; //строки в матрице 
	int i, j; //индексы элементов
	int *p;

	//ввод кол-ва строк
	do{
		printf("%s\n", pr);
		printf("Введите число строк: ");
		pr = ("Ошибка! Ожидалось целое число больше нуля.");
		if(getInt(&m) == 0){
			return 0;//либо конец файла, либо ошибка по выделению памяти
		}
	}while(m < 1);
	//записываем в сруктуру (сруктура ыыыгыгыгы))) ) кол-во строк
	rm->lines = m;
	//выделяем память под наш массив структур
	rm->matr = (Line *)calloc(m, sizeof(Line));
	
	for(i = 0; i < rm->lines; ++i){
		//для каждой строки матрицы вводим кол-во столбцов
		pr = "";//система с объявлением ошибки гениальна, я в восторге
		do{
			printf("%s\n", pr);
			printf("Введите число элементов в строке %d:  ", i+1);
			pr = "Ошибка! Ожидалось целое число больше нуля.";
			if(getInt(&m) == 0){
				rm->lines = i;
				erase(rm);
				return 0;
			}
		}while (m < 1);
		//в i-тую ЛАЙН записываем число элементов в ней
		rm->matr[i].n = m;
		//затем выделяем в нее память и записываем указатели на выделенные блоки
		// в структурку каждой строки
		p = (int *)malloc(sizeof(int) * m);
		rm->matr[i].a = p;
		
		//ввод элементов каждой строчки
		printf("Введите элементы строки #%d:\n", i+1);
		for(j = 0; j < m; ++j, ++p){
			if(getInt(p) == 0){//если ВДРУГ вылетела ошибка...
				rm->lines = i+1;//поскольку мы выделили памяти на строчку больше, то нам необходимо очистить и её
				erase(rm);
				return 0;
			}
		}
	}
		return 1;
}

void processMatrix(Matrix orig_pm, Matrix *res_pm){
	int pmin;//позиция первого вхождения в строку минимального элемента
	int i, j, r_strl, o_strl;
	int *p_orig, *p_res;	

	//количество строк в новой матрице равно оригиналу:
	res_pm->lines = orig_pm.lines;
	/*выделяем память под массив из указателей на начала строк 
	для новой матрицы: */
	res_pm->matr = (Line *)calloc(orig_pm.lines, sizeof(Line));


	for(i = 0; i < orig_pm.lines; ++i){//инкремент по строчкам оригинала
		p_orig = orig_pm.matr[i].a;//берем указку на очередную строку оригинала
		o_strl = orig_pm.matr[i].n;

		pmin = min(p_orig, o_strl);//получаем позицию первого вхождения элемента в строку
		r_strl = o_strl - pmin;//кол-во элементов в каждой строчке новой матрицы
		res_pm->matr[i].n = r_strl;

		
		p_res = (int*)malloc(sizeof(int)*r_strl);//выделяем память под очередную строчку
		res_pm->matr[i].a = p_res;//указатель на новую строчку результирующей матрицы
		
		//printf("\nМинимум в %d-й строчке находится на позиции %d\n", i+1, pmin+1);
		int *p = p_orig;
		p_orig += pmin;
		
		for(j = 0; j < r_strl; ++j, ++p_res, ++p_orig){
			*p_res = *p_orig;
		}
		
	}
}

/*
Возвращает номер первого вхождения
минимального элемента в строку 
*/

int min(int a[], int m){
	int res = *a;
	int l = m;
	int mn = 0;
	for(; m-- > 0; ++a){
		if(*a < res){
			res = *a;
			mn = l-m-1;
		}
	}
	return mn;
}

void output(const char *msg, Matrix a){
	int i, j;
	int *p;
	printf("%s\n", msg);
	for(i = 0; i < a.lines; ++i){
		p = a.matr[i].a;
		for(j = 0; j < a.matr[i].n; ++j, ++p){
			printf("%d ", *p);
		}
		printf("\n");
	}
}

void erase(Matrix *a){
	int i;
	for (i = 0; i < a->lines; ++i){
		free(a->matr[i].a);//очищаем каждый элемент
	}
	free(a->matr);
	a->lines = 0;
	a->matr = NULL;
}

//Записываем вещественное число, введенное пользователем, по адресу a.
//Возвращает 0 если ввели мусор и 1, если ввод прошел успешно.
/*int getDouble(double *a){
	int n;
	do{
		n = scanf("%d", a);
		if(*a < 0){
			n = 0;
		}
		if(n == 0){
			printf("Ошибка! Необходимо ввести целое положительное число.\n");			
			scanf("%*[^\n]");
			
			//scanf("%*s");	
			//scanf("%*c", 0);
		}	
	}while(n == 0);
	return n < 0 ? 0 : 1;
}*/
