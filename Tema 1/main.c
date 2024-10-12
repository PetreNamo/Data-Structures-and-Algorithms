#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//structura pentru banda si deget
typedef struct nod
{
    char a;
    struct nod *next, *prev;
} nod, *lista;
typedef struct coada
{
    char sir[20];
    struct coada *urm;
} coada, *cod;
typedef struct stiva
{
    lista p;
    struct stiva *urm;
} stiva, *sti;
//adauga un nou nod la finalul benzii
void adaugaf(lista l, char c)
{
    lista nou=(lista)malloc(sizeof(nod));
    nou->a=c;
    nou->prev=l->prev;
    nou->next=NULL;
    nou->prev->next=nou;
    l->prev=nou;
}
//adauga un nou nod la mijlocul benzii
void adaugamij(lista *deget, char c)
{
    lista nou=(lista)malloc(sizeof(nod));
    nou->a=c;
    nou->next=(*deget)->next;
    nou->prev=*deget;
    (*deget)->next=nou;
    if((*deget)->next!=NULL)
        (*deget)->next->prev=(*deget);
}
//adauga un nou nod la finalul cozii
void adaugarefinal(cod l, char a[20])
{
    cod nou,s=l;
    nou=(cod)malloc(sizeof(coada));
    strcpy(nou->sir,a);
    nou->urm=NULL;
    while(s->urm!=NULL)
        s=s->urm;
    s->urm=nou;
}
//adauga un nou nod la inceputul stivei
void adaugareinceput(sti *l, lista q)
{
    sti nou;
    nou=(sti)malloc(sizeof(stiva));
    nou->p=q;
    nou->urm=*l;
    *l=nou;
}
//elimina primul nod din coada
void eliminaprimulc(cod *l)
{
    cod aux=(*l)->urm;
    (*l)->urm=(*l)->urm->urm;
    free(aux);
}
//elimina primul nod din stiva
void eliminaprimuls(sti *l)
{
    sti aux=(*l)->urm;
    (*l)->urm=(*l)->urm->urm;
    free(aux);
}
//subprogram pentru afisarea benzii si a pozitiei degetului
void SHOW(lista l, FILE *g, lista deget)
{
    lista s=l;
    s=s->next;
    while(s!=NULL)
    {
        if(s==deget)
        {
            fprintf(g,"|");
            fprintf(g,"%c",s->a);
            fprintf(g,"|");
        }
        else
            fprintf(g,"%c",s->a);
        s=s->next;
    }
    fprintf(g,"\n");
}
//muta degetul cu o poz la stanga
void MOVE_LEFT(lista *deget)
{
    if((*deget)->prev!=NULL)
        *deget=(*deget)->prev;
}
//muta degetul cu o poz la dreapta
void MOVE_RIGHT(lista l, lista *deget)
{
    if(*deget!=l->prev)
        *deget=(*deget)->next;
    else
    {
        adaugaf(l,'#');
        *deget=(*deget)->next;
    }
}
//muta degetul la stanga pe prima aparitie a caracterului c
void MOVE_LEFT_C(lista l, lista *deget, char c, FILE *g)
{
    lista s=*deget,deget2;
    int ok=0;
    while(s!=NULL)
    {
        if(s->a==c && ok==0)
        {
            ok=1;
            deget2=s;
        }
        s=s->prev;
    }
    if(ok==1)
        *deget=deget2;
    else
        fprintf(g,"ERROR\n");
}
//muta degetul la dreapta pe prima aparitie a caracterului c
void MOVE_RIGHT_C(lista l, lista *deget, char c)
{
    lista s=*deget,deget2;
    int ok=0;
    while(s!=NULL)
    {
        if(s->a==c && ok==0)
        {
            ok=1;
            deget2=s;
        }
        s=s->next;
    }
    if(ok==1)
        *deget=deget2;
    else
    {
        adaugaf(l,'#');
        *deget=l->prev;
    }
}
//scrie caracterul c pe pozitia degetului
void WRITE(lista *deget, char c)
{
    (*deget)->a=c;
}
//insereaza un caracter in stanga degetului
void INSERT_LEFT(lista *deget, char c, FILE *g)
{
    if((*deget)->prev==NULL)
        fprintf(g,"ERROR\n");
    else
    {
        *deget=(*deget)->prev;
        adaugamij(deget,c);
        *deget=(*deget)->next;
    }
}
//insereaza un caracter in dreapta degetului
void INSERT_RIGHT(lista l, lista *deget, char c)
{
    if((*deget)->next!=NULL)
    {
        adaugamij(deget,c);
        *deget=(*deget)->next;
    }
    else
    {
        adaugaf(l,c);
        *deget=l->prev;
    }
}
//afiseaza caracterul de pe pozita degetului
void SHOW_CURRENT(lista deget, FILE *g)
{
    fprintf(g,"%c\n",deget->a);
}
/*subprogram pentru toate actiunile care nu se executa pe loc
si se introduc in coada pana la intalnirea comenzii EXECUTE*/
void EXECUTE(lista l, lista *deget, cod C, FILE *g, sti U, sti R)
{
    char x[20];
    lista temp=*deget;
    strcpy(x,C->urm->sir);
    if(strcmp(x,"MOVE_LEFT")==0)
    {
        MOVE_LEFT(deget);
        if(temp!=*deget)
            adaugareinceput(&U->urm,temp);
    }
    else if(strcmp(x,"MOVE_RIGHT")==0)
    {
        MOVE_RIGHT(l, deget);
        if(temp!=*deget)
            adaugareinceput(&U->urm,temp);
    }
    else if(strstr(x,"MOVE_LEFT_CHAR")!=NULL)
    {
        char caracter;
        caracter=x[strlen(x)-1];
        MOVE_LEFT_C(l, deget, caracter,g);
    }
    else if(strstr(x,"MOVE_RIGHT_CHAR")!=NULL)
    {
        char caracter;
        caracter=x[strlen(x)-1];
        MOVE_RIGHT_C(l, deget, caracter);
    }
    else if(strstr(x,"WRITE")!=NULL)
    {
        char caracter;
        caracter=x[strlen(x)-1];
        WRITE(deget,caracter);
        while(U->urm!=NULL)
            eliminaprimuls(&U);
        while(R->urm!=NULL)
            eliminaprimuls(&R);
    }
    else if(strstr(x,"INSERT_LEFT")!=NULL)
    {
        char caracter;
        caracter=x[strlen(x)-1];
        INSERT_LEFT(deget, caracter,g);
    }
    else if(strstr(x,"INSERT_RIGHT")!=NULL)
    {
        char caracter;
        caracter=x[strlen(x)-1];
        INSERT_RIGHT(l,deget, caracter);
    }
    eliminaprimulc(&C);
}
/*executa operatia UNDO, schiband pozitia
degetului cu cea dinainte*/
void UNDO(lista *deget, sti U, sti R)
{
    adaugareinceput(&R->urm,*deget);
    *deget=U->urm->p;
    eliminaprimuls(&U);
}
/*executa operatia REDO, schiband pozitia
degetului cu cea dinainte*/
void REDO(lista *deget, sti U, sti R)
{
    adaugareinceput(&U->urm,*deget);
    *deget=R->urm->p;
    eliminaprimuls(&R);
}
//subprogram pentru eliberarea memoriei utilizate de coada
void free1(cod *l)
{
    cod aux;
    while(*l!=NULL)
    {
        aux=*l;
        *l=(*l)->urm;
        free(aux);
    }
}
//subprogram pentru eliberarea memoriei utilizate de stiva
void free2(sti *l)
{
    sti aux;
    while(*l!=NULL)
    {
        aux=*l;
        *l=(*l)->urm;
        free(aux);
    }
}
//subprogram pentru eliberarea memoriei utilizate de banda
void free3(lista *l)
{
    lista aux;
    while(*l!=NULL)
    {
        aux=*l;
        *l=(*l)->next;
        free(aux);
    }
}
int main()
{
    //construiesc banda la inceput
    lista l=(lista)malloc(sizeof(nod));
    l->next=NULL;
    l->prev=NULL;
    l->next=(lista)malloc(sizeof(nod));
    l->prev=l->next;
    l->next->next=NULL;
    l->next->prev=NULL;
    l->next->a='#';
    lista deget=l->next;
    //initializez stivele si coada
    cod C=(cod)malloc(sizeof(coada));
    C->urm=NULL;
    sti U=(sti)malloc(sizeof(stiva));
    U->urm=NULL;
    sti R=(sti)malloc(sizeof(stiva));
    R->urm=NULL;
    FILE *f, *g;
    f=fopen("tema1.in","r");
    g=fopen("tema1.out","w");
    int n,i;
    char s[20];
    fscanf(f,"%d",&n);
    fgets(s,20,f);
    for(i=0; i<n; i++)
    {
        fgets(s,20,f);
        strcpy(s+strlen(s)-1,s+strlen(s));
        if(strcmp(s,"EXECUTE")!=0 && strcmp(s,"UNDO")!=0 && strcmp(s,"REDO")!=0 && strcmp(s,"SHOW")!=0 && strcmp(s,"SHOW_CURRENT")!=0)
            adaugarefinal(C,s);
        else if(strcmp(s,"EXECUTE")==0)
            EXECUTE(l,&deget,C,g,U,R);
        else if(strcmp(s,"UNDO")==0)
            UNDO(&deget,U,R);
        else if(strcmp(s,"REDO")==0)
            REDO(&deget,U,R);
        else if(strcmp(s,"SHOW")==0)
            SHOW(l,g,deget);
        else if(strcmp(s,"SHOW_CURRENT")==0)
            SHOW_CURRENT(deget,g);
    }
    free1(&C);
    free2(&U);
    free2(&R);
    free3(&l);
    fclose(f);
    fclose(g);
    return 0;
}
