#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "kutuphane.h"

//----------------FUNCTION DEFINITION----------------------
Figure *start_figure(double width, double height);
void set_thickness_resolution(Figure *fig, double thickness,double resolution);
void set_color(Figure *fig, Color c);
void draw_fx(Figure *fig, double f(double x), double start_x,double end_x, double step_size);
void draw_polyline(Figure *fig, Point2D *poly_line, int n);
void draw_polygon(Figure *fig, Point2D *poly_line);
void draw_ellipse(Figure *fig, Point2D *centre, Point2D *width_height);
void draw_binary_tree(Figure *fig,Tree *root);
void scale_figure(Figure *fig, double scale_x, double scale_y);
void resize_figure(Figure *fig, Point2D *start_roi, Point2D *end_roi);
void append_figures(Figure *fig1, Figure *fig2);
void export_eps(Figure *fig, char *file_name);
double f(double x);
void add_to_tail(Point2D *coordinate, double x, double y);
double upper_ellipse(double x,double coor_x,double coor_y,double a,double b);
double lower_ellipse(double x,double coor_x,double coor_y,double a,double b);
//----------------FUNCTION DEFINITION----------------------

Figure *start_figure(double width, double height){
	Figure *fig=(Figure*)malloc(sizeof(Figure));
	fig->width=width;
	fig->height=height;
	fig->llx=-(width/2);
	fig->lly=-(height/2);
	fig->urx=width/2;
	fig->ury=height/2;
	return fig;
}
 
void set_thickness_resolution(Figure *fig, double thickness,double resolution){
	fig->resolution=resolution;
	fig->thickness=thickness;
}

void set_color(Figure *fig, Color c){
	fig->color.red=c.red;
	fig->color.green=c.green;
	fig->color.blue=c.blue;
}

void draw_fx(Figure *fig, double f(double x), double start_x,double end_x, double step_size){
	double x,y;
	Point2D *coordinate=(Point2D*)malloc(sizeof(Point2D));
	y=f(start_x);
	coordinate->x=start_x;
	coordinate->y=y;
	for(x=start_x;x<end_x;x+=step_size){
		y=f(x);
		add_to_tail(coordinate,x,y);
	}
	y=f(end_x);
	add_to_tail(coordinate,end_x,y);
	fig->point=coordinate;
}

void add_to_tail(Point2D *coordinate, double x, double y){
	Point2D *iter;
	iter=coordinate;
	while(iter->next!=NULL){
		iter=iter->next;
	}
	iter->next=(Point2D*)malloc(sizeof(Point2D));
	iter=iter->next;
	iter->x=x;
	iter->y=y;
	iter->next=NULL;
}

void draw_polyline(Figure *fig, Point2D *poly_line, int n){
	int i=0;
	Point2D *coordinate;
	coordinate=(Point2D*)malloc(sizeof(Point2D));
	coordinate->x=poly_line[0].x;
	coordinate->y=poly_line[0].y;
	for(i=0;i<n;i++){
		add_to_tail(coordinate,poly_line[i].x,poly_line[i].y);
	}
	fig->point=coordinate;
}

void draw_polygon(Figure *fig, Point2D *poly_line){
	int i=1;
	Point2D *coordinate;
	double x,y;
	coordinate=(Point2D*)malloc(sizeof(Point2D));
	coordinate->x=poly_line[0].x;
	coordinate->y=poly_line[0].y;
	x=poly_line[0].x;
	y=poly_line[0].y;
	while(1){
		if(poly_line[i].x==x&&poly_line[i].y==y){
			add_to_tail(coordinate,poly_line[i].x,poly_line[i].y);
			break;
		}
		add_to_tail(coordinate,poly_line[i].x,poly_line[i].y);
		i++;
	}
	fig->point=coordinate;
}

void draw_ellipse(Figure *fig, Point2D *centre, Point2D *width_height){
	Point2D *iter,*coordinate=(Point2D*)malloc(sizeof(Point2D));
	iter=coordinate;
	double i,resolution=fig->resolution;
	double radius_x=width_height->x/2,radius_y=width_height->y/2;
	double start_x=centre->x-radius_x;
	i=start_x;
	double end_x=centre->x+radius_x;
	for(i;i<end_x;i+=1/resolution){
		iter->x=i;
		iter->y=upper_ellipse(i,centre->x,centre->y,radius_x,radius_y);
		iter->next=(Point2D*)malloc(sizeof(Point2D));
		iter=iter->next;
	}
	iter->x=end_x;
	iter->y=upper_ellipse(end_x,centre->x,centre->y,radius_x,radius_y);
	iter->next=(Point2D*)malloc(sizeof(Point2D));
	iter=iter->next;
	i=end_x;
	for(i;i>start_x;i-=1/resolution){
		iter->x=i;
		iter->y=lower_ellipse(i,centre->x,centre->y,radius_x,radius_y);
		iter->next=(Point2D*)malloc(sizeof(Point2D));
		iter=iter->next;
	}
	iter->x=start_x;
	iter->y=lower_ellipse(start_x,centre->x,centre->y,radius_x,radius_y);
	iter->next=NULL;
	fig->point=coordinate;
}

double upper_ellipse(double x,double h,double k,double a,double b){
	double result=sqrt((1-((x-h)*(x-h))/(a*a))*(b*b))+k;
	return result;
}

double lower_ellipse(double x,double h,double k,double a,double b){
	double result=-(sqrt((1-((x-h)*(x-h))/(a*a))*(b*b))+k);
	return result;
}

void draw_binary_tree(Figure *fig,Tree *root){

}

void scale_figure(Figure *fig, double scale_x, double scale_y){
	Point2D *iter=fig->point;
	while(iter!=NULL){
		iter->x=iter->x*scale_x;
		iter->y=iter->y*scale_y;
		iter=iter->next;
	}
}

void resize_figure(Figure *fig, Point2D *start_roi, Point2D *end_roi){
	fig->llx=start_roi->x;
	fig->lly=start_roi->y;
	fig->urx=end_roi->x;
	fig->ury=end_roi->y;
}

void append_figures(Figure *fig1, Figure *fig2){
	Point2D *iter=fig1->point;
	while(iter->next!=NULL){
		iter=iter->next;
	}
	iter->next=fig2->point;
	iter->next->append=1;
}

void export_eps(Figure *fig, char *file_name){
	int check=0;
	Point2D *temp=fig->point;
	FILE *fp=fopen(file_name,"w");
	fclose(fp);
	fp=fopen(file_name,"a+");
	fprintf(fp,"%%!PS-Adobe-3.0 EPSF-3.0");
 	fprintf(fp,"\n%%%%BoundingBox: %d %d %d %d",(int)fig->llx,(int)fig->lly,(int)fig->urx,(int)fig->ury);
	fprintf(fp,"\n%d setlinewidth\n%.1lf %.1lf %.1lf setrgbcolor",(int)fig->thickness,fig->color.red,fig->color.green,fig->color.blue);
	while(temp!=NULL){
		if(check==0){
			fprintf(fp,"\n%.1lf %.1lf moveto",temp->x,temp->y);
			temp=temp->next;
			check=1;
		}
		else{
			if(temp->append==1){
				fprintf(fp,"\n%.3lf %.3lf moveto",temp->x,temp->y);
				temp=temp->next;
			}
			else{
				fprintf(fp,"\n%.3lf %.3lf lineto",temp->x,temp->y);
				temp=temp->next;
			}

		}
	}
	fprintf(fp,"\nstroke\n");
}

double f(double x){
	return x*x/4;
}