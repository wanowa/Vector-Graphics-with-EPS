#ifndef kutuphane
#define kutuphane

typedef struct Color{
	double red,green,blue;
}Color;

typedef struct Point2D{
	double x,y,centre,width,height,append;
	struct Point2D *next;
}Point2D;

typedef struct Figure{
	double resolution,thickness,llx,lly,urx,ury,width,height;
	Color color;
	Point2D *point;
}Figure;

typedef struct Tree{
	int data;
	struct Tree *left,*right;
}Tree;

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

#endif