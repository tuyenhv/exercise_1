#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "points.h"

int main()
{
  int number_points, i, number_points_polygon;
  struct point points[MAX_POINTS], points_polygon[MAX_POINTS];

  printf("The number of points in our space: \n");
  while (true){
    scanf("%d",&number_points);
    if ((number_points > 1) && (number_points < MAX_POINTS))
      break;
    else
      printf ("The number needs to be greater than 2 and less than %d, type again: \n", MAX_POINTS);
  }

  printf("Please input values for points \n");
  for (i = 0; i < number_points; i++){
    printf("Input coordinate for point number %d: \n",i);
    while(true){
      input_point(&points[i]);
      // Make sure that the point which you have just inputted is not duplicated.
      // The first element does not need to check_input_valid, from the second
      // element, they need to be check_input_valid to make sure they are not
      // duplicated with the other elements before.
      if ((i < 1) || check_input_valid(points, points[i],i))
        break;
      else
        printf("The input data is not valid, please try again \n");
    }
  }
  printf("\n===================================================\n");
  printf("The number of lines is: %d \n", count_lines_from_points(points, number_points));
  printf("\n===================================================\n");
  count_triangle_from_points(points, number_points);
  printf("\n===================================================\n");
  number_points_polygon = find_polygon(points, number_points, points_polygon);

  for (i = 0; i < number_points_polygon; i++){
    printf ("The coordinate of points on polygon is: x = %f, y = %f \n", points_polygon[i].x, points_polygon[i].y);
  }

  printf("\nThe area of polygon is: %f \n", polygon_area(points_polygon, number_points_polygon));

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

void swap(float *xp, float *yp){
    float temp = *xp;
    *xp = *yp;
    *yp = temp;
}

/* Bubble sort algorithm */
void bubble_sort(float list[], int n){
  int i, j;
  for (i = 0; i < n - 1; i++)
    for (j = 0; j < n - i - 1; j++)
      if (list[j] > list[j + 1])
        swap(&list[j],&list[j+1]);
}

float distance_points(struct point a, struct point b){
  return sqrt(((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y)));
}

/*
float caculate_area_triangle(struct point a, struct point b, struct point c){
  return abs((a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y)) / 2.0);
}
*/

float caculate_area_triangle(struct point a, struct point b, struct point c){
  float xa, xb, xc, p;
  xa = distance_points(b,c);
  xb = distance_points(a,c);
  xc = distance_points(a,b);
  p = (xa + xb + xc) / 2.0;
  if (xa + xb <= xc || xa + xc <= xb || xb + xc <= xa)
    return 0;
  else
    return sqrt(p * (p - xa) * (p - xb) * (p - xc));
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
  if (count_lines - (int)count_lines > 0.5)
    return (int)count_lines + 1;
  else
    return (int)count_lines;
}

int count_triangle_from_points(struct point points[], int number_points){
  if (number_points < 3){
    printf("Need more than two points \n");
    return -1;
  }
  int i, j, k, triangle_number = 0;
  float triangle_area;
  for (i = 0; i < number_points - 2; i++){
    for (j = i + 1; j < number_points - 1; j++){
      for (k = j + 1; k < number_points; k++){
        triangle_area = caculate_area_triangle(points[i], points[j], points[k]);
        if (triangle_area > 0){
          printf("The area of triangle from three points %d, %d, %d is: %f \n", i, j, k, triangle_area);
          triangle_number++;
        }
      }
    }
  }
  printf("The number of triangle is: %d \n", triangle_number);
  return triangle_number;
}

/* To find orientation of ordered triplet (p, q, r).
The function returns following values
0 --> p, q and r are colinear
1 --> Clockwise
2 --> Counterclockwise */
int orientation(struct point a, struct point b, struct point c){
  int val = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);
  if (val == 0) return 0; // colinear
  return (val > 0)? 1: 2; // Clockwise or Counterclockwise
}

/* Find the position point which has the minimum of abscissa */
int find_left(struct point points[], int number_points){
  int i, left_num = 0;
  for (i = 1; i < number_points; i++){
    if (points[i].x < points[left_num].x)
      left_num = i;
  }
  return left_num;
}

int find_polygon(struct point points[], int number_points, struct point points_polygon[]){
  if (number_points < 3){
    printf("Need more than two points \n");
    return -1;
  }
  int l = find_left(points, number_points), i;
  int p = l, q, count = 0;
  do {
    // Search for a point 'q' such that orientation(p, i, q) is Counterclockwise
    // for all points 'i'
    q = (p + 1) % number_points;
    for (i = 0; i < number_points; i++)
      if (orientation(points[p], points[i], points[q]) == 2)
        q = i;

    points_polygon[count] = points[q];
    count++;
    p = q; // Set p as q for next iteration
  }
  while (p != l);
  return count;
}

/* Caculate the area of a polygon with n points */
float polygon_area(struct point pts[], int n){
  if (n < 3){
    printf("Need more than two points \n");
    return -1;
  }
  float area = 0.0;
  int j = n - 1, i;
  for (i = 0; i < n; i++){
    area += (pts[j].x + pts[i].x) * (pts[j].y - pts[i].y);
    j = i;
  }
  return fabs(area / 2.0);
}
