#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<inttypes.h>
#include<math.h>

typedef struct pixel_rgb
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} pixel_rgb;

typedef struct tree
{
    pixel_rgb pixel;
    long long poz;
    uint32_t area;
    struct tree *top_left,*top_right,*bottom_left,*bottom_right;
}tree;

typedef struct QuadtreeNode
{
	unsigned char blue, green ,red;
	uint32_t area;
	int32_t top_left,top_right;
	int32_t bottom_left, bottom_right;

 }__attribute__((packed)) QuadtreeNode;

 pixel_rgb** init_matrix(int height, int width)
 {
     pixel_rgb **m=(pixel_rgb**)malloc(sizeof(pixel_rgb*)*height);
     int i;
     for(i=0;i<height;i++)
        m[i]=(pixel_rgb*)malloc(sizeof(pixel_rgb)*width);
    return m;
 }


 void compresie(pixel_rgb** m, tree** node,int a, int b,int size, int factor)
 {
     int i,j;
     unsigned long long red=0,green=0,blue=0,mean=0;
     for(i=a;i<a+size;i++)
        for(j=b;j<b+size;j++)
            {
                red+=m[i][j].red;
                green+=m[i][j].green;
                blue+=m[i][j].blue;
            }
    //media aritmetica
    red=red/(size*size);
    green=green/(size*size);
    blue=blue/(size*size);
    (*node)=malloc(sizeof(tree));
    (*node)->area=size*size;
    (*node)->pixel.red=red;
    (*node)->pixel.green=green;
    (*node)->pixel.blue=blue;
    for(i=a;i<a+size;i++)
        for(j=b;j<b+size;j++)
                mean+=((red-m[i][j].red)*(red-m[i][j].red)+(green-m[i][j].green)*(green-m[i][j].green)+(blue-m[i][j].blue)*(blue-m[i][j].blue));
    mean/=(3*size*size);
    if(mean>factor)
    {
        compresie(m,&(*node)->top_left,a,b,size/2,factor);//stanga sus
        compresie(m,&(*node)->top_right,a,b+size/2,size/2,factor);//dreapta sus
        compresie(m,&(*node)->bottom_right,a+size/2,b+size/2,size/2,factor); //dreapta jos
        compresie(m,&(*node)->bottom_left,a+size/2,b,size/2,factor);//stanga jos
        return;
    }
    else
    {
        (*node)->top_left=(*node)->top_right=(*node)->bottom_left=(*node)->bottom_right=NULL;
        return;
    }
 }

void count(tree *node,uint32_t *nr_noduri, int *index)
{
    if(node==NULL)
        return;
    else
    {
        node->poz=(*index);
        (*nr_noduri)++;
        (*index)++;
        count(node->top_left,nr_noduri,index);
        count(node->top_right,nr_noduri,index);
        count(node->bottom_right,nr_noduri,index);
        count(node->bottom_left,nr_noduri,index);
    }
}
void make_array_of_quadtree(tree* node, QuadtreeNode** vq, int i)
{
        (*vq)[i].area=node->area;
        (*vq)[i].red=node->pixel.red;
        (*vq)[i].green=node->pixel.green;
        (*vq)[i].blue=node->pixel.blue;
        if(node->top_left==NULL)
            {
                (*vq)[i].top_left=-1;
                (*vq)[i].top_right=-1;
                (*vq)[i].bottom_right=-1;
                (*vq)[i].bottom_left=-1;
                return;
            }
        else
        {
            (*vq)[i].top_left=node->top_left->poz;
            (*vq)[i].top_right=node->top_right->poz;
            (*vq)[i].bottom_right=node->bottom_right->poz;
            (*vq)[i].bottom_left=node->bottom_left->poz;
            make_array_of_quadtree(node->top_left,vq,node->top_left->poz);
            make_array_of_quadtree(node->top_right,vq,node->top_right->poz);
            make_array_of_quadtree(node->bottom_right,vq,node->bottom_right->poz);
            make_array_of_quadtree(node->bottom_left,vq,node->bottom_left->poz);
        }
}
void print_compress(QuadtreeNode *vq, char *f_out, uint32_t nr_noduri, int index)
{
    FILE *out=fopen(f_out,"wb");
    uint32_t nr_frunze=0;
    unsigned int i;
    for(i=0;i<index;i++)
    {
        if(vq[i].top_left==-1)
            nr_frunze++;
    }
    fwrite(&nr_frunze,sizeof(uint32_t),1,out);
    fwrite(&nr_noduri,sizeof(uint32_t),1,out);
    for(i=0;i<index;i++)
        fwrite(&vq[i],sizeof(QuadtreeNode),1,out);
    fclose(out);
}

void orizontal(pixel_rgb **m, tree **node,int a, int b, int size, int factor)
{
    int i,j;
    unsigned long long red=0,green=0,blue=0,mean=0;
    for(i=a;i<a+size;i++)
        for(j=b;j<b+size;j++)
            {
                red+=m[i][j].red;
                green+=m[i][j].green;
                blue+=m[i][j].blue;
            }
    //media aritmetica
    red=red/(size*size);
    green=green/(size*size);
    blue=blue/(size*size);
    (*node)=malloc(sizeof(tree));
    (*node)->area=size*size;
    (*node)->pixel.red=red;
    (*node)->pixel.green=green;
    (*node)->pixel.blue=blue;
    for(i=a;i<a+size;i++)
        for(j=b;j<b+size;j++)
            mean+=((red-m[i][j].red)*(red-m[i][j].red)+(green-m[i][j].green)*(green-m[i][j].green)+(blue-m[i][j].blue)*(blue-m[i][j].blue));
    mean/=(3*size*size);
    if(mean>factor)
    {
        orizontal(m,&(*node)->top_left,a,b+size/2,size/2,factor);
        orizontal(m,&(*node)->top_right,a,b,size/2,factor);
        orizontal(m,&(*node)->bottom_right,a+size/2,b,size/2,factor);
        orizontal(m,&(*node)->bottom_left,a+size/2,b+size/2,size/2,factor);
        return;
    }
    else
    {
        (*node)->top_left=(*node)->top_right=(*node)->bottom_left=(*node)->bottom_right=NULL;
        return;
    }
}

void vertical(pixel_rgb** m, tree** node,int a, int b,int size, int factor)
 {
     int i,j;
     unsigned long long red=0,green=0,blue=0,mean=0;
     for(i=a;i<a+size;i++)
        for(j=b;j<b+size;j++)
            {
                red+=m[i][j].red;
                green+=m[i][j].green;
                blue+=m[i][j].blue;
            }
    //media aritmetica
    red=red/(size*size);
    green=green/(size*size);
    blue=blue/(size*size);
    (*node)=malloc(sizeof(tree));
    (*node)->area=size*size;
    (*node)->pixel.red=red;
    (*node)->pixel.green=green;
    (*node)->pixel.blue=blue;
    for(i=a;i<a+size;i++)
        for(j=b;j<b+size;j++)
                mean+=((red-m[i][j].red)*(red-m[i][j].red)+(green-m[i][j].green)*(green-m[i][j].green)+(blue-m[i][j].blue)*(blue-m[i][j].blue));
    mean/=(3*size*size);
    if(mean>factor)
    {
        vertical(m,&(*node)->top_left,a+size/2,b,size/2,factor);
        vertical(m,&(*node)->top_right,a+size/2,b+size/2,size/2,factor);
        vertical(m,&(*node)->bottom_right,a,b+size/2,size/2,factor);
        vertical(m,&(*node)->bottom_left,a,b,size/2,factor);
        return;
    }
    else
    {
        (*node)->top_left=(*node)->top_right=(*node)->bottom_left=(*node)->bottom_right=NULL;
        return;
    }
 }


void make_tree_from_quadtree(tree **node, QuadtreeNode *vq,int poz)
{
    (*node)=(tree*)malloc(sizeof(tree));
    (*node)->area=vq[poz].area;
    (*node)->pixel.red=vq[poz].red;
    (*node)->pixel.green=vq[poz].green;
    (*node)->pixel.blue=vq[poz].blue;
    (*node)->poz=poz;
    if(vq[poz].top_left!=-1)
    {
        make_tree_from_quadtree(&(*node)->top_left,vq,vq[poz].top_left);
        make_tree_from_quadtree(&(*node)->top_right,vq,vq[poz].top_right);
        make_tree_from_quadtree(&(*node)->bottom_right,vq,vq[poz].bottom_right);
        make_tree_from_quadtree(&(*node)->bottom_left,vq,vq[poz].bottom_left);
    }
    else
    {
        (*node)->top_left=NULL;
        (*node)->top_right=NULL;
        (*node)->bottom_right=NULL;
        (*node)->bottom_left=NULL;
    }
}

void decompresie(pixel_rgb ***m, tree* node, int a, int b, int size)
{
    if(node->top_left!=NULL)
    {
        decompresie(m,node->top_left,a,b,size/2);
        decompresie(m,node->top_right,a,b+size/2,size/2);
        decompresie(m,node->bottom_right,a+size/2,b+size/2,size/2);
        decompresie(m,node->bottom_left,a+size/2,b,size/2);
    }
    else
    {
        int i,j;
        for(i=a;i<a+size;i++)
            for(j=b;j<b+size;j++)
                {
                    (*m)[i][j].red=node->pixel.red;
                    (*m)[i][j].green=node->pixel.green;
                    (*m)[i][j].blue=node->pixel.blue;
                }
    }
}

void print_decompress(pixel_rgb **m, char *f_out, int size)
{
    FILE *out=fopen(f_out,"wb");
    fprintf(out,"P6\n");
    fprintf(out,"%d %d\n", size,size);
    fprintf(out,"255\n");
    int i,j;
    for(i=0;i<size;i++)
        for(j=0;j<size;j++)
            fwrite(&m[i][j],sizeof(pixel_rgb),1,out);
    fclose(out);
}

void free_tree(tree **node)
{
    if(*node==NULL)
        return;
    free_tree(&(*node)->top_left);
    free_tree(&(*node)->top_right);
    free_tree(&(*node)->bottom_right);
    free_tree(&(*node)->bottom_left);
    free(*node);
}

 int main(int argc, char *argv[])
 {
     if(strcmp(argv[1],"-c")==0)
     {
         int width,height,valmax,i,factor,index=0;
         pixel_rgb **m;
         tree *node=NULL;
         char tip[3],nimic;
         FILE *in;
         in= fopen(argv[3],"rb");
         fscanf(in,"%s", tip);
         fscanf(in,"%d ", &width);
         fscanf(in,"%d\n", &height);
         fscanf(in,"%d", &valmax);
         //initializam matricea de pixeli
         m=init_matrix(height,width);
        fread(&nimic,sizeof(char),1,in);
         for(i=0;i<height;i++)
            fread(m[i],sizeof(pixel_rgb),width,in);
         fclose(in);
         factor=atoi(argv[2]);
          uint32_t  nr_noduri=0;
         compresie(m,&node,0,0,width,factor);
         count(node,&nr_noduri,&index);
         QuadtreeNode *vq=(QuadtreeNode*)malloc(sizeof(QuadtreeNode)*index);
         //facem vectorul cerut
        make_array_of_quadtree(node,&vq,0);
         print_compress(vq,argv[4],nr_noduri,index);
         //eliberam memorie
         free(vq);
         for(i=0;i<height;i++)
            free(m[i]);
        free(m);
        free_tree(&node);
     }
     if(strcmp(argv[1],"-d")==0)
     {
         uint32_t nr_frunze,nr_noduri;
         int i;
         FILE *in;
         in=fopen(argv[2],"rb");
         fread(&nr_frunze,sizeof(uint32_t),1,in);
         fread(&nr_noduri,sizeof(uint32_t),1,in);
         QuadtreeNode *vq=(QuadtreeNode*)malloc(sizeof(QuadtreeNode)*nr_noduri);
        for(i=0;i<nr_noduri;i++)
            fread(&vq[i],sizeof(QuadtreeNode),1,in);
        tree *node=NULL;
        //facem vector de tip tree
        make_tree_from_quadtree(&node,vq,0);
        int size=sqrt(node->area);
        pixel_rgb **m;
        m=init_matrix(size,size);
        //realizam matricea de pixeli
        decompresie(&m,node,0,0,size);
        free(node);
        print_decompress(m,argv[3],size);
        for(i=0;i<size;i++)
            free(m[i]);
        free(m);
        free(vq);
     }
     if(strcmp(argv[1],"-m")==0)
     {
         int width,height,valmax,i,factor;
        pixel_rgb **m;
        tree *node=NULL;
        char tip[3],nimic;
        FILE *in;
        in= fopen(argv[4],"rb");
        fscanf(in,"%s", tip);
        fscanf(in,"%d ", &width);
        fscanf(in,"%d\n", &height);
        fscanf(in,"%d", &valmax);
        m=init_matrix(height,width);
        fread(&nimic,sizeof(char),1,in);
        for(i=0;i<height;i++)
            fread(m[i],sizeof(pixel_rgb),width,in);
        factor=atoi(argv[3]);
        fclose(in);
        if(strcmp(argv[2],"h")==0)
            {
                orizontal(m,&node,0,0,width,factor);
                decompresie(&m,node,0,0,width);
                free(node);
                print_decompress(m,argv[5],width);
                for(i=0;i<height;i++)
                    free(m[i]);
                free(m);
            }
        if(strcmp(argv[2],"o")==0)
        {
            vertical(m,&node,0,0,width,factor);
            decompresie(&m,node,0,0,width);
            free(node);
            print_decompress(m,argv[5],width);
            for(i=0;i<height;i++)
                free(m[i]);
            free(m);
        }
     }
    return 0;
 }
