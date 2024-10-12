#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct nod
{
    int val, cost;
    struct nod *next;
} nod, *lista;
// adaug un nod in lista de adiacenta a nodului x
void addedge(lista *a, int x, int y, int z)
{
    lista l;
    int ok = 0;
    l = a[x];
    while (l != NULL)
    {
        if (l->val == y)
            ok = 1;
        l = l->next;
    }
    if (ok == 0)
    {
        lista nou = (lista)malloc(sizeof(nod));
        nou->val = y;
        nou->cost = z;
        nou->next = (a)[x];
        (a)[x] = nou;
    }
    free(l);
}
void dfs(lista *a, int x, int *con, int k)
{
    lista l;
    l = a[x];
    con[x] = k;
    while (l != NULL)
    {
        if (con[l->val] == 0)
            dfs(a, l->val, con, k);
        l = l->next;
    }
    free(l);
}
// functie pentru algoritmul lui Prim, adaptat pentru liste
void Prim(lista *a, int i, int k, int n, int *prim)
{
    int *ok, ok2, ok3, mini, p, s = 0;
    lista l;
    ok = calloc(n + 1, sizeof(int));
    ok[i] = 1;
    ok2 = 0;
    while (ok2 == 0)
    {
        ok3 = 0;
        mini = 400;
        for (i = 1; i <= n; i++)
        {
            l = a[i];
            while (l != NULL)
            {
                if (ok[i] == 1 && ok[l->val] == 0 && l->cost < mini)
                {
                    mini = l->cost;
                    p = l->val;
                    ok3 = 1;
                }
                l = l->next;
            }
        }
        if (ok3 == 1)
        {
            s = s + mini;
            ok[p] = 1;
        }
        else
            ok2 = 1;
    }
    prim[k] = s;
    free(ok);
    free(l);
}
// functie pentru algoritmul lui dijkstra
// costul arcelor a fost inlocuit in algoritm cu scorul acestora,
// definit in enuntul problemei. Adancimea fiecarui nod este pastrata
// in vectorul adancime
void dijkstra(lista *a, int start, int *adancime, int *traseu, float *dist, int n)
{
    int *vizitat;
    vizitat = calloc(n + 2, sizeof(int));
    int c, p, next, i;
    float distmin;
    lista l;
    l = a[start];
    while (l != NULL)
    {
        dist[l->val] = (float)l->cost / adancime[l->val];
        traseu[l->val] = start;
        l = l->next;
    }
    for (i = 1; i <= n; i++)
        if (dist[i] == 0)
            dist[i] = 500;
    dist[start] = 0;
    vizitat[start] = 1;
    traseu[start] = -1;
    c = 1;
    p = start;
    next = start;
    int ok = 0;
    while (c < n)
    {
        distmin = 500;
        for (i = 1; i <= n; i++)
            if (dist[i] < distmin && vizitat[i] == 0)
            {

                distmin = dist[i];
                next = i;
            }
        if (p == start)
            traseu[next] = p;
        vizitat[next] = 1;
        l = a[next];
        while (l != NULL)
        {
            ok = 1;
            if (distmin + ((float)l->cost / adancime[l->val]) < dist[l->val])
            {
                traseu[l->val] = next;
                dist[l->val] = distmin + ((float)l->cost / adancime[l->val]);
            }
            l = l->next;
            p = next;
        }
        if (ok == 0)
            next = p;
        c++;
    }
    free(vizitat);
}
// functie care afiseaza drumul de la insula la corabie
// si calculeaza in acelasi timp si suma arcelor prin care se trece
// si adancimea minima
void printdrum(int i, int *traseu, int n, int *s, int *mini, int *adancime, lista *a, FILE *g)
{
    if (i == -1)
        return;
    lista l;
    if (traseu[i] > 0)
    {
        l = a[traseu[i]];
        while (l != NULL)
        {
            if (l->val == i)
                *s = *s + l->cost;
            l = l->next;
        }
    }
    printdrum(traseu[i], traseu, n, s, mini, adancime, a, g);
    if (i == 1)
        fprintf(g, "Insula ");
    else if (i == n)
        fprintf(g, "Corabie");
    else
    {
        fprintf(g, "Nod_%d ", i);
        if (adancime[i] < *mini)
            *mini = adancime[i];
    }
}
// functie pentru dezalocarea memoriei grafului
void elib(lista *l)
{
    lista aux;
    while (*l != NULL)
    {
        aux = *l;
        *l = (*l)->next;
        free(aux);
    }
}
int main(int argc, char *argv[])
{
    FILE *f, *g;
    f = fopen("tema3.in", "r");
    g = fopen("tema3.out", "w");
    int n, m, i, c = 0, p, q, j, ok, z, s, mini;
    char x[30], y[30], nr[2];
    if (strcmp(argv[1], "1") == 0)
    {
        fscanf(f, "%d%d", &n, &m);
        lista *a;
        a = (lista *)malloc((n + 2) * sizeof(lista));
        for (i = 0; i <= n + 1; i++)
            a[i] = NULL;
        // vectorul cod restine codificarile nodurilor
        char **cod;
        cod = (char **)malloc((n + 2) * sizeof(char *));
        for (i = 0; i <= n + 1; i++)
            cod[i] = (char *)malloc(30 * sizeof(char));
        for (i = 1; i <= m; i++)
        {
            fscanf(f, "%s%s%d", x, y, &z);
            p = 0, q = 0;
            ok = 0;
            for (j = 1; j <= c; j++)
                if (strcmp(cod[j], x) == 0)
                {
                    p = j;
                    ok = 1;
                }
            if (ok == 0)
            {
                strcpy(cod[c + 1], x);
                c++;
                p = c;
            }
            ok = 0;
            for (j = 1; j <= c; j++)
                if (strcmp(cod[j], y) == 0)
                {
                    q = j;
                    ok = 1;
                }
            if (ok == 0)
            {
                strcpy(cod[c + 1], y);
                c++;
                q = c;
            }
            addedge(a, p, q, z);
            addedge(a, q, p, z);
        }
        int *con;
        con = calloc(n + 1, sizeof(int));
        // determinam numarul de componente conexe cu dfs
        int k = 1;
        for (i = 1; i <= n; i++)
            if (con[i] == 0)
            {
                dfs(a, i, con, k);
                k++;
            }
        fprintf(g, "%d\n", k - 1);
        // aplicam algoritmul lui Prim pe fiecare componenta conexa
        // si retinem sumele arborilor rezultati in vectorul prim
        // pe care il vom sorta
        int *prim;
        prim = calloc(k, sizeof(int));
        j = 1;
        i = 1;
        while (j <= k - 1)
        {
            if (con[i] == j)
            {
                Prim(a, i, j, n, prim);
                j++;
            }
            i++;
        }
        int aux;
        for (i = 1; i < k - 1; i++)
            for (j = i + 1; j <= k - 1; j++)
                if (prim[i] > prim[j])
                {
                    aux = prim[i];
                    prim[i] = prim[j];
                    prim[j] = aux;
                }
        for (i = 1; i <= k - 1; i++)
            fprintf(g, "%d\n", prim[i]);
        for (i = 0; i <= n + 1; i++)
            free(cod[i]);
        free(cod);
        free(con);
        free(prim);
        for (i = 1; i <= n; i++)
            elib(&a[i]);
        free(a);
    }
    else if (strcmp(argv[1], "2") == 0)
    {
        fscanf(f, "%d%d", &n, &m);
        lista *a;
        a = (lista *)malloc((n + 2) * sizeof(lista));
        for (i = 0; i <= n + 1; i++)
            a[i] = NULL;
        // crearea vecorului de liste de adiacenta
        for (i = 1; i <= m; i++)
        {
            fscanf(f, "%s%s%d", x, y, &z);
            p = 0;
            q = 0;
            ok = 0;
            if (strcmp(x, "Insula") == 0)
            {
                p = 1;
                ok = 1;
            }
            else if (strcmp(x, "Corabie") == 0)
            {
                p = n;
                ok = 1;
            }
            else
            {
                if (isdigit(x[strlen(x) - 2]) != 0)
                {
                    int t = 0, h;
                    while (x[t] != '_')
                        t++;
                    t++;
                    h = 0;
                    while (t < strlen(x))
                    {
                        nr[h] = x[t];
                        h++;
                        t++;
                    }
                }
                else
                {
                    p = x[strlen(x) - 1];
                    p = p - '0';
                    ok = 1;
                }
            }
            if (ok == 0)
                p = atoi(nr);
            ok = 0;
            if (strcmp(y, "Insula") == 0)
            {
                q = 1;
                ok = 1;
            }
            else if (strcmp(y, "Corabie") == 0)
            {
                q = n;
                ok = 1;
            }
            else
            {
                if (isdigit(y[strlen(y) - 2]) != 0)
                {
                    int t = 0, h;
                    while (y[t] != '_')
                        t++;
                    t++;
                    h = 0;
                    while (t < strlen(y))
                    {
                        nr[h] = y[t];
                        h++;
                        t++;
                    }
                }
                else
                {
                    q = y[strlen(y) - 1];
                    q = q - '0';
                    ok = 1;
                }
            }
            if (ok == 0)
                q = atoi(nr);
            addedge(a, p, q, z);
        }
        // citirea adancimii
        int *adancime;
        adancime = calloc(n + 1, sizeof(int));
        for (i = 1; i <= n; i++)
        {
            fscanf(f, "%s%d", x, &z);
            p = 0;
            ok = 0;
            if (strcmp(x, "Insula") == 0)
            {
                adancime[1] = z;
                ok = 1;
            }
            else if (strcmp(x, "Corabie") == 0)
            {
                adancime[n] = z;
                ok = 1;
            }
            else
            {
                if (isdigit(x[strlen(x) - 2]) != 0)
                {
                    int t = 0, h;
                    while (x[t] != '_')
                        t++;
                    t++;
                    h = 0;
                    while (t < strlen(y))
                    {
                        nr[h] = x[t];
                        h++;
                        t++;
                    }
                }
                else
                {
                    p = x[strlen(x) - 1];
                    p = p - '0';
                    ok = 1;
                }
                if (ok == 0)
                    p = atoi(nr);
                adancime[p] = z;
            }
        }
        int *traseu;
        float *dist;
        // vectorul traseu retine drumul de la insula la corabie
        // si dist scorul
        traseu = calloc(n + 1, sizeof(int));
        dist = calloc(n + 1, sizeof(float));
        adancime[0] = 0;
        traseu[0] = 0;
        dist[0] = 0;
        dijkstra(a, n, adancime, traseu, dist, n);
        if (dist[1] == 500)
            fprintf(g, "Echipajul nu poate ajunge la insula");
        else
        {
            for (i = 1; i <= n; i++)
            {
                traseu[i] = 0;
                dist[i] = 0;
            }
            dijkstra(a, 1, adancime, traseu, dist, n);
            if (dist[n] == 500)
                fprintf(g, "Echipajul nu poate transporta comoara inapoi la corabie");
            else
            {
                s = 0;
                mini = 500;
                printdrum(n, traseu, n, &s, &mini, adancime, a, g);
                fprintf(g, "\n%d\n%d\n", s, mini);
                fscanf(f, "%d", &z);
                j = z % mini;
                c = z / mini;
                if (j == 0)
                    fprintf(g, "%d", c);
                else
                    fprintf(g, "%d", c + 1);
            }
        }
        free(adancime);
        free(traseu);
        free(dist);
        for (i = 1; i <= n; i++)
            elib(&a[i]);
        free(a);
    }
    fclose(f);
    fclose(g);
    return 0;
}
