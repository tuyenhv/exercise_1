#include <stdio.h>
#include <stdbool.h>
#include "points.h"

int main()
{
  int number_points, i;
  struct point points[MAX_POINTS];

  printf("The number of points in our space: \n");
  scanf("%d",&number_points);

  printf("Please input values for points \n");
  for (i = 0; i < number_points; i++){
    printf("Input coordinate for point number %d: \n",i);
    while(true){
      input_point(&points[i]);
      // Make sure that the point which you have just inputted is not duplicated.
      if ((i < 1) || check_input_valid(points, points[i],i))
        break;
      else
        printf("The input data is not valid, please try again \n");
    }
  }

  printf("The number of lines is: %d \n", count_lines_from_points(points, number_points));
  return 0;
}

/* Input the coordinate of point */
void input_point(struct point *pt){
  printf("Input X: ");
  scanf("%f", &pt->x);
  printf("Input Y: ");
  scanf("%f", &pt->y);
}

/* Check the two points are the same or not */
bool compare_point(struct point a, struct point b){
  if ((a.x == b.x) && (a.y == b.y))
    return true;
  return false;
}

bool check_input_valid(struct point pts[], struct point pt, int n){
  int i;
  for (i = 0; i < n; i++){
    if (compare_point(pts[i], pt))
      return false;
  }
  return true;
}

/* Display the coordinate of point */
void display_point(struct point pt){
  printf("The coordinate of the point is: (%f, %f) \n", pt.x, pt.y);
}

float scale_factor(struct point a, struct point b){
  /* Fix this:
  Currently, if two points have the same point.x, it will be return M_PI value.
  This can make a problem when the two points have scale_factor is M_PI acctually.
  Another solution is that we will get the maximum and minimum value of point.x
  and point.y, then get maximum value of scale_factor.
  The end, we will replace the M_PI value by maximum value of scale_factor + 1
  or something like that.
  */
  if ((a.x - b.x) == 0)
    return M_PI;
  return (a.y - b.y)/(a.x - b.x);
}

/* Bubble sort algorithm */
void bubble_sort(float list[], int n){
  int i, j, temp;
  for (i = 0; i < n - 1; i++){
    for (j = 0; j < n - i - 1; j++){
      if (list[j] > list[j + 1]){
        temp = list[j];
        list[j] = list[j + 1];
        list[j + 1] = temp;
      }
    }
  }
}

int count_lines_from_points(struct point points[], int number_points){
  int i, j, k, count, m;
  float scale_factors[number_points];
  float count_lines = number_points * (number_points - 1) / 2;
  for (i = 0; i < number_points; i++){
    k = 0;
    for (j = 0; j < number_points; j++){
      if (i != j){
        scale_factors[k] = scale_factor(points[i], points[j]);
        k++;
      }
    }
    bubble_sort(scale_factors, number_points - 1);

    /* Find the number of points which have the same line_number
    They will have the same scale_factor.
    Ex: A, B, C are in a line if scale_factor(A,B) = scale_factor(A,C)
    */
    count = 2;
    for (m = 0; m < number_points - 1; m++){
      if (scale_factors[m] == scale_factors[m + 1]){
        count++;
      }else if (count > 2){
        count_lines = count_lines - (float)((count - 1) / 2.0)  + (float)(1.0 / count);
        count = 2;
      }
    }
  }
  return (int)count_lines;
}
