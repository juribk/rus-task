#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define SWAP(a, b) {a = a + b; b = a - b; a = a - b;} 
#define MIN(a,b) (((a) < (b)) ? (a):(b))
#define MAX(a,b) (((a) > (b)) ? (a):(b))

typedef struct point {
    int x;
    int y; 
} pnt;

// Перемножаем знаки
int Sign (int x, int y) {
    return ((x >= 0) ? 1 : -1) * ((y >= 0) ? 1 : -1);
}

// Вычисляем ориентированную площадь треугольника
int Area (pnt a, pnt b, pnt c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

// Проверка на пересечение отрезков по осям
bool Intersect (int a, int b, int c, int d) {
	if (a > b)  SWAP (a, b);
	if (c > d)  SWAP (c, d);
	return MAX(a, c) <= MIN(b, d);
}

int Intersection (pnt a1, pnt a2, pnt b1, pnt b2) {
    return Intersect (a1.x, a2.x, b1.x, b2.x)               // Пересечение в плоскости x 
		&& Intersect (a1.y, a2.y, b1.y, b2.y)               // Пересечение в плоскости y 
		&& Sign(Area(a1, a2, b1), Area(a1, a2, b2)) <= 0    // b1 и b2 по разные стороны отрезка (a1, a2)
		&& Sign(Area(b1, b2, a1), Area(b1, b2, a2)) <= 0;   // a1 и a2 по разные стороны отрезка (b1, b2)
}

int main (void) {
    pnt a1, a2, b1, b2;
    int ret;
    a1.x = 1; a1.y = 6;
    a2.x = 5; a2.y = 2;
    b1.x = 2; b1.y = 3;
    b2.x = 7; b2.y = 8;
    ret = Intersection(a1, a2, b1, b2);
    printf("ret = %d\n", ret);

    a1.x = 1; a1.y = 6;
    a2.x = 5; a2.y = 2;
    b1.x = 4; b1.y = 5;
    b2.x = 7; b2.y = 8;

    ret = Intersection(a1, a2, b1, b2);
    printf("ret = %d\n", ret);
}