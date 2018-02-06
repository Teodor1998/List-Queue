#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct nod {
    char nume[21];
    int lifetime;
    int contor;
    //contor va fi folosit doar la Round Robin si la Planificarea cu prioritati
    int prioritate;
    int good;
    int bad;
    //good si bad sunt campuri pentru Bonus
    //good -> creste cand e trimis in wait
    //bad -> creste cand e trimis in ready
    struct nod *next;
} *Nod;

Nod initList(char *nume, int lifetime, int prioritate)
{
    //Creeaza un nod cu elementele date
    Nod l = (Nod)malloc(sizeof(struct nod));
    strcpy(l->nume,nume);
    l->lifetime = lifetime;
    l->prioritate = prioritate;
    l->next = NULL;
    return l;
}

Nod addLast(Nod l,char *nume, int lifetime, int prioritate)
{
    //Adauga un element la sfarsitul listei
    Nod aux,tmp;
    aux = initList(nume,lifetime,prioritate);
    if (l == NULL)
        return aux;
    tmp = l;
    while(tmp->next != NULL)
        tmp = tmp->next;
    tmp->next = aux;
    return l;
}

void freeList(Nod *l)
{
    //Eliberarea memoriei alocate unei liste
    if ((*l) != NULL)
    {
        Nod tmp;
        while ((*l) != NULL)
        {
            tmp = (*l);
            (*l) = (*l)->next;
            free(tmp);
        }
    }
}

Nod takeFirst(Nod *l)
{
    //Returneaza primul nod din lista, extrangandu-l din aceasta
    if ((*l) == NULL)
        return NULL;
    Nod tmp = (*l);
    (*l) = (*l)->next;
    tmp->next = NULL;
    return tmp;
}

void tick_FCFS (Nod *Ready, Nod *Running)
{
    //Functia tick pentru planificarea
    //First Come First Served
    if ((*Running) != NULL)
    {
        (*Running)->lifetime--;
        if ((*Running)->lifetime == 0)
        {
            free(*Running);
            (*Running) = NULL;
            (*Running) = takeFirst(Ready);
        }
    }
    else
        (*Running) = takeFirst(Ready);
}

Nod shortest_time(Nod *Ready)
{
    //Returneaza procesul cu cel mai mic timp de executie
    int time = (*Ready)->lifetime;
    if ((*Ready)->next == NULL)
    {
        return takeFirst(Ready);
    }
    else
    {
        Nod tmp = (*Ready);
        while (tmp != NULL)
        {
            if (tmp->lifetime < time)
                time = tmp->lifetime;
            tmp = tmp->next;
        }
        tmp = (*Ready);
        if(tmp->lifetime == time)
        {
            return takeFirst(Ready);
        }
        else
        {
            while(tmp->next->lifetime != time)
                tmp = tmp->next;
            Nod tmp2 = tmp->next;
            tmp->next = tmp2->next;
            tmp2->next = NULL;
            return tmp2;
        }
    }
}

void tick_SJF (Nod *Ready, Nod *Running)
{
    //Functie tick pentru planificarea
    //Shortest Job First
    if ((*Running) != NULL)
    {
        (*Running)->lifetime--;
        if((*Running)->lifetime == 0)
        {
            free(*Running);
            (*Running) = NULL;
            if ((*Ready) != NULL)
                (*Running) = shortest_time(Ready);
        }
    }
    else
    {
        if ((*Ready) != NULL)
            (*Running) = shortest_time(Ready);
    }
}

void tick_RR (Nod *Ready, Nod *Running, int cuanta)
{
    //Functia tick pentru planificarea
    //Round Robin
    if ((*Running) != NULL)
    {
        (*Running)->lifetime--;
        (*Running)->contor++;
        if ((*Running)->lifetime == 0)
        {
            free(*Running);
            (*Running) = NULL;
            (*Running) = takeFirst(Ready);
            if ((*Running) != NULL)
                (*Running)->contor = 0;
        }
        else
        {
            if ((*Running)->contor == cuanta)
            {
                (*Ready) = addLast((*Ready),(*Running)->nume,(*Running)->lifetime,(*Running)->prioritate);
                free(*Running);
                (*Running) = NULL;
                (*Running) = takeFirst(Ready);
                if ((*Running) != NULL)
                    (*Running)->contor = 0;
            }
        }

    }
    else
    {
        (*Running) = takeFirst(Ready);
        if ((*Running) != NULL)
            (*Running)->contor = 0;
    }
}

Nod nod_prioritar_extract(Nod *Ready, int prioritate)
{
    //Functia extrage nodul cu cea mai mare prioritate din coada
    //Ready
    if ((*Ready) == NULL)
        return NULL;
    if ((*Ready)->next == NULL)
        return takeFirst(Ready);
    Nod tmp = (*Ready);
    Nod tmp2;
    int i,gasit = 0;
    for (i=prioritate;i>=1;i--)
    {
        //Pornesc de la nivelul maxim si verific daca exista nod in lista cu
        //acest nivel si apoi scad treptat cu cate un nivel pana gasesc un nod
        if ((*Ready)->prioritate == i)
        {
            tmp = (*Ready);
            (*Ready) = (*Ready)->next;
            tmp->next = NULL;
            return tmp;
        }
        tmp = (*Ready);
        while (tmp->next != NULL)
        {
            if (tmp->next->prioritate == i)
            {
                gasit = 1;
                break;
            }
            tmp = tmp->next;
        }
        if (gasit == 1)
        {
            tmp2 = tmp->next;
            tmp->next = tmp2->next;
            tmp2->next = NULL;
            return tmp2;
        }
    }
    return NULL;
}

int highest_priority(Nod Ready, int prioritate)
{
    //Functia returneaza campul prioritate al nodului cu cea mai mare
    //prioritate ce se afla in Ready
    
    //Pe acelasi principiu cu functia de mai sus
    if (Ready == NULL)
        return 0;
    if (Ready->next == NULL)
        return Ready->prioritate;
    Nod tmp=Ready;
    int i,gasit = 0;
    for (i=prioritate;i>=1;i--)
    {
        if (Ready->prioritate == i)
            return Ready->prioritate;
        tmp = Ready;
        while(tmp->next != NULL)
        {
            if (tmp->next->prioritate == i)
            {
                gasit = 1;
                break;
            }
            tmp = tmp->next;
        }
        if (gasit == 1)
            return tmp->next->prioritate;
    }
    return 0;
}

void tick_PP (Nod *Ready, Nod *Running, int cuanta, int prioritate)
{
    //Functia tick pentru
    //Planificarea pe Prioritati
    int high_p = highest_priority((*Ready),prioritate);
    //Aflu prioritatea procesului cu cea mai mare prioritate din coada Ready
    if ((*Running) != NULL)
    {
        (*Running)->lifetime--;
        (*Running)->contor++;
        if ((*Running)->lifetime == 0)
        {
            free(*Running);
            (*Running) = NULL;
            (*Running) = nod_prioritar_extract(Ready,prioritate);
            if ((*Running) != NULL)
                (*Running)->contor = 0;
        }
        else
        {
            if ((*Running)->contor == cuanta)
            {
                //daca a depasit cuanta il trecem in Ready
                (*Ready) = addLast((*Ready), (*Running)->nume, (*Running)->lifetime, (*Running)->prioritate);
                free(*Running);
                (*Running) = NULL;
                (*Running) = nod_prioritar_extract(Ready,prioritate);
                if (*Running != NULL)
                    (*Running)->contor = 0;
            }
            else
                if ((*Running)->prioritate < high_p)
                {
                    //Daca exista in Ready un proces cu prioritate mai mare il trecem pe cel curent in Ready si
                    //pe celalalt in Running
                    (*Ready) = addLast((*Ready),(*Running)->nume,(*Running)->lifetime,(*Running)->prioritate);
                    free(*Running);
                    (*Running) = NULL;
                    (*Running) = nod_prioritar_extract(Ready, prioritate);
                    if ((*Running) != NULL)
                        (*Running)->contor = 0;
                }
        }
    }
    else
    {
        (*Running) = nod_prioritar_extract(Ready,prioritate);
        if ((*Running) != NULL)
            (*Running)->contor = 0;
    }
}

void tick_Bonus (Nod *Ready, Nod *Running, int cuanta, int prioritate)
{
    //Functia tick pentru
    //Planificarea pe Prioritati de la Bonus
    
    //Functia tick este identica cu cea de mai sus doar ca difera prin
    //cateva adaugari in ceea ce tine de campurile good si bad.
    int high_p = highest_priority((*Ready),prioritate);
    if ((*Running) != NULL)
    {
        (*Running)->lifetime--;
        (*Running)->contor++;
        if ((*Running)->lifetime == 0)
        {
            free(*Running);
            (*Running) = NULL;
            (*Running) = nod_prioritar_extract(Ready,prioritate);
            if ((*Running) != NULL)
                (*Running)->contor = 0;
        }
        else
        {
            if ((*Running)->contor == cuanta)
            {
                (*Ready) = addLast((*Ready), (*Running)->nume, (*Running)->lifetime, (*Running)->prioritate);
                //addLast nu modifica si campurile good si bad, motiv pentru care le voi modifica mai jos,
                Nod tmp = (*Ready);
                while (tmp->next != NULL)
                    tmp = tmp->next;
                tmp->bad = (*Running)->bad+1;
                tmp->good = 0;
                if(tmp->bad == 2)
                {
                    tmp->prioritate--;
                    tmp->bad = 0;
                    tmp->good = 0;
                }
                free(*Running);
                (*Running) = NULL;
                (*Running) = nod_prioritar_extract(Ready,prioritate);
                if (*Running != NULL)
                    (*Running)->contor = 0;
            }
            else
                if ((*Running)->prioritate < high_p)
                {
                    (*Ready) = addLast((*Ready),(*Running)->nume,(*Running)->lifetime,(*Running)->prioritate);
                    Nod tmp = (*Ready);
                    while(tmp->next != NULL)
                        tmp = tmp->next;
                    tmp->bad = (*Running)->bad+1;
                    tmp->good = 0;
                    if(tmp->bad == 2)
                    {
                        tmp->prioritate--;
                        tmp->bad = 0;
                        tmp->good = 0;
                    }
                    free(*Running);
                    (*Running) = NULL;
                    (*Running) = nod_prioritar_extract(Ready, prioritate);
                    if ((*Running) != NULL)
                        (*Running)->contor = 0;
                }
        }
    }
    else
    {
        (*Running) = nod_prioritar_extract(Ready,prioritate);
        if ((*Running) != NULL)
        {
            (*Running)->contor = 0;
        }
    }
}

void add(Nod *Ready, Nod *Running, char *buffer, int plan, int cuanta, int priority)
{
    //Functia add
    Nod aux1,aux2,aux3;
    int i,k = 0;
    aux1 = (Nod)malloc(sizeof(struct nod));
    aux2 = (Nod)malloc(sizeof(struct nod));
    aux3 = (Nod)malloc(sizeof(struct nod));
    //In numele fiecarei structuri salvez datele primite la add
    int lifetime,prioritate;
    i = 1;
    int j = 0;
    for (i=i+1;i<strlen(buffer);i++)
    {
        if (buffer[i] == ' ')
            break;
        else
        {
            aux1->nume[j] = buffer[i];
            j++;
            aux1->nume[j] = 0;
        }
    }
    j = 0;
    for (i=i+1;i<strlen(buffer);i++)
    {
        if (buffer[i] == ' ')
            break;
        else
        {
            aux2->nume[j] = buffer[i];
            j++;
            aux2->nume[j] = 0;
        }
    }
    j = 0;
    for (i=i+1;i<strlen(buffer);i++)
    {
        if (buffer[i] == ' ' || buffer[i] == '\n' || buffer[i] == EOF)
            break;
        else
        {
            aux3->nume[j] = buffer[i];
            j++;
            aux3->nume[j] = 0;
        }
    }
    lifetime = atoi(aux2->nume);
    prioritate = atoi(aux3->nume);
    (*Ready) = addLast((*Ready),aux1->nume,lifetime,prioritate);
    //Apoi eliberez memoria alocata structurilor auxiliare
    free(aux1);
    free(aux2);
    free(aux3);
    if (plan == 5)
    {
        //Initializez cele doua campuri pentru planificarea de la Bonus
        Nod tmp = (*Ready);
        while (tmp->next != NULL)
            tmp = tmp->next;
        tmp->good = 0;
        tmp->bad = 0;
    }
    switch(plan)
    {
        case 1:
            {
                tick_FCFS(Ready,Running);
                break;
            }
        case 2:
            {
                tick_SJF(Ready,Running);
                break;
            }
        case 3:
            {
                tick_RR(Ready,Running,cuanta);
                break;
            }
        case 4:
            {
                tick_PP(Ready,Running,cuanta,priority);
                break;
            }
        case 5:
            {
                tick_Bonus(Ready,Running,cuanta,priority);
                break;
            }
    }
}

void multiple_add(Nod *Ready, Nod *Running, char *buffer, int plan, int cuanta, int priority)
{
    //Functia Multiple Add
    int n,i,contor=0,lifetime,i1,j,prioritate;
    n = strlen(buffer);
    for (i=0;i<n;i++)
        if (buffer[i]==' ')
            contor++;
    i = 2;//la urmatoarea parcurgere scapam de "ma"
    contor/=3;
    //Vor fi 3 spatii pentru fiecare proces
    //de forma _P1_5_4
    //obtin numarul de procese adaugate
    Nod aux1 = (Nod)malloc(sizeof(struct nod));
    Nod aux2 = (Nod)malloc(sizeof(struct nod));
    Nod aux3 = (Nod)malloc(sizeof(struct nod));
    for (i1=0;i1<contor;i1++)
    {
        j = 0;
        for (i=i+1;i<n;i++)
        {
            if (buffer[i]==' ')
                break;
            else
            {
                aux1->nume[j] = buffer[i];
                j++;
                aux1->nume[j] = 0;
            }
        }
        j = 0;
        for (i=i+1;i<n;i++)
        {
            if (buffer[i]==' ')
                break;
            else
            {
                aux2->nume[j] = buffer[i];
                j++;
                aux2->nume[j] = 0;
            }
        }
        j = 0;
        for (i=i+1;i<n;i++)
        {
            if (buffer[i]==' ' || buffer[i]=='\n' || buffer[i] == EOF)
                break;
            else
            {
                aux3->nume[j] = buffer[i];
                j++;
                aux3->nume[j] = 0;
            }
        }
        lifetime = atoi(aux2->nume);
        prioritate = atoi(aux3->nume);
        (*Ready) = addLast((*Ready),aux1->nume,lifetime,prioritate);
        if (plan == 5)
        {
            //Ca la add, initializez cele doua campuri good si bad pentru Bonus
            Nod tmp = (*Ready);
            while(tmp->next != NULL)
                tmp = tmp->next;
            tmp->good = 0;
            tmp->bad = 0;
        }
    }
    free(aux1);
    free(aux2);
    free(aux3);
    switch(plan)
    {
        case 1:
            {
                tick_FCFS(Ready,Running);
                break;
            }
        case 2:
            {
                tick_SJF(Ready,Running);
                break;
            }
        case 3:
            {
                tick_RR(Ready,Running,cuanta);
                break;
            }
        case 4:
            {
                tick_PP(Ready,Running,cuanta,priority);
                break;
            }
        case 5:
            {
                tick_Bonus(Ready,Running,cuanta,priority);
                break;
            }
    }
}

void wait(Nod *Waiting, Nod *Ready, Nod *Running, int plan, int cuanta, int prioritate)
{
    //Functia Wait
    if ((*Running) != NULL)
    {
        (*Waiting) = addLast((*Waiting),(*Running)->nume,(*Running)->lifetime,(*Running)->prioritate);
        if (plan == 5)
        {
            //In acest if (pentru planificarea de la Bonus) adaug +1 la campul good al procesului ce
            //tocmai a trecut in waiting
            Nod tmp = (*Waiting);
            while(tmp->next)
                tmp = tmp->next;
            tmp->good = (*Running)->good +1;
            //Modific valoarea inainte ca Running sa fie eliberat.
            tmp->bad = 0;
            if(tmp->good == 2)
                tmp->prioritate++;
        }
        free(*Running);
        (*Running) = NULL;
    }
    switch(plan)
    {
        case 1:
            {
                tick_FCFS(Ready,Running);
                break;
            }
        case 2:
            {
                tick_SJF(Ready,Running);
                break;
            }
        case 3:
            {
                tick_RR(Ready,Running,cuanta);
                break;
            }
        case 4:
            {
                tick_PP(Ready,Running,cuanta,prioritate);
                break;
            }
        case 5:
            {
                tick_Bonus(Ready,Running,cuanta,prioritate);
                break;
            }
    }
}

void event(Nod *Waiting, Nod *Ready, Nod *Running, char *buffer, int plan, int cuanta, int prioritate)
{
    //Functia Event
    Nod aux = (Nod)malloc(sizeof(struct nod));
    int i,j=0;
    for (i=2;i<strlen(buffer);i++)
    {
        if (buffer[i] == " ")
            break;
        else
        {
            aux->nume[j] = buffer[i];
            j++;
            aux->nume[j] = 0;
        }
    }
    if ((*Waiting) != NULL)
    {
        Nod tmp = (*Waiting);
        if (strcmp(aux->nume,tmp->nume) == 0)
        {
            (*Ready) = addLast((*Ready),tmp->nume,tmp->lifetime,tmp->prioritate);
            if (plan == 5)
            {
                Nod tmp2 = (*Ready);
                while (tmp2->next != NULL)
                    tmp2 = tmp2->next;
                tmp2->bad = tmp->bad;
                tmp2->good = tmp->good;
            }
            (*Waiting) = (*Waiting)->next;
            free(tmp);
        }
        else
        {
            while (tmp->next != NULL)
            {
                if (strcmp(aux->nume,tmp->next->nume) == 0)
                {
                    Nod tmp2 = tmp->next;
                    (*Ready) = addLast((*Ready),tmp2->nume,tmp2->lifetime,tmp2->prioritate);
                    if (plan == 5)
                    {
                        Nod tmp3 = (*Ready);
                        while (tmp3 != NULL)
                            tmp3 = tmp3->next;
                        tmp3->bad = tmp2->bad;
                        tmp3->good = tmp2->good;
                        //addLast nu modifica si campurile bad si good, de aceea le modific manual
                        //prin intermediul lui tmp3.
                    }
                    tmp->next = tmp2->next;
                    free(tmp2);
                    break;
                }
                tmp=tmp->next;
            }
        }
    }
    free(aux);
    switch(plan)
    {
        case 1:
            {
                tick_FCFS(Ready,Running);
                break;
            }
        case 2:
            {
                tick_SJF(Ready,Running);
                break;
            }
        case 3:
            {
                tick_RR(Ready,Running,cuanta);
                break;
            }
        case 4:
            {
                tick_PP(Ready,Running,cuanta,prioritate);
                break;
            }
        case 5:
            {
                tick_Bonus(Ready,Running,cuanta,prioritate);
                break;
            }
    }
}

void show (Nod Running, FILE* output)
{
    //Functie Show
    if (Running == NULL)
        fprintf(output,"\n");
    else
        fprintf(output,"%s %d\n", Running->nume, Running->lifetime);
}

void FCFS (FILE *input, FILE *output)
{
    Nod Ready = NULL;
    Nod Running = NULL;
    Nod Waiting = NULL;
    int i,n=0;
    char buffer[100] = "",c;
    c = fgetc(input);
    c = fgetc(input);
    while(1)
    {
        if (c != '\n' && c != EOF)
        {
            buffer[n] = c;
            n++;
        }
        else
        {
            switch(buffer[0])
            {
                case 'a':
                    {
                        add(&Ready, &Running, buffer, 1, 0, 0);
                        break;
                    }
                case 's':
                    {
                        show(Running,output);
                        if (Running != NULL)
                        break;
                    }
                case 't':
                    {
                        tick_FCFS(&Ready,&Running);
                        break;
                    }
                case 'm':
                    {
                        multiple_add(&Ready, &Running, buffer, 1, 0, 0);
                        break;
                    }
                case 'w':
                    {
                        wait(&Waiting, &Ready, &Running, 1, 0, 0);
                        break;
                    }
                case 'e':
                    {
                        event(&Waiting, &Ready, &Running, buffer, 1, 0, 0);
                        break;
                    }
                //La comenzile cu tick implicit am adaugat doua argumente in plus:
                // - 1: ce reprezinta planificarea
                // - 2: ce reprezinta cuanta (in cazul FCFS nu va fi folosita si ii trimitem o valoare oarecare
                // eu am ales 0)
            }
            for (i=0;i<n;i++)
                buffer[i]=0;
            n=0;
        }
        if (c == EOF)
            break;
        c = fgetc(input);
    }
    freeList(&Running);
    freeList(&Ready);
    freeList(&Waiting);
}

void SJF(FILE *input, FILE *output)
{
    Nod Ready = NULL;
    Nod Running = NULL;
    Nod Waiting = NULL;
    int i,n=0;
    char buffer[100] = "",c;
    c = fgetc(input);
    c = fgetc(input);
    while(1)
    {
        if (c != '\n' && c != EOF)
        {
            buffer[n] = c;
            n++;
        }
        else
        {
            switch(buffer[0])
            {
                case 'a':
                    {
                        add(&Ready, &Running, buffer, 2, 0, 0);
                        break;
                    }
                case 's':
                    {
                        show(Running,output);
                        if (Running != NULL)
                        break;
                    }
                case 't':
                    {
                        tick_SJF(&Ready,&Running);
                        break;
                    }
                case 'm':
                    {
                        multiple_add(&Ready, &Running, buffer, 2, 0, 0);
                        break;
                    }
                case 'w':
                    {
                        wait(&Waiting, &Ready, &Running, 2, 0, 0);
                        break;
                    }
                case 'e':
                    {
                        event(&Waiting, &Ready, &Running, buffer, 2, 0, 0);
                        break;
                    }
            }
            for (i=0;i<n;i++)
                buffer[i]=0;
            n=0;
        }
        if (c == EOF)
            break;
        c = fgetc(input);
    }
    freeList(&Running);
    freeList(&Ready);
    freeList(&Waiting);
}

void RR(FILE *input, FILE *output, int cuanta)
{
    Nod Ready = NULL;
    Nod Running = NULL;
    Nod Waiting = NULL;
    int i,n=0;
    char buffer[100] = "",c;
    c = fgetc(input);
    c = fgetc(input);
    while(1)
    {
        if (c != '\n' && c != EOF)
        {
            buffer[n] = c;
            n++;
        }
        else
        {
            switch(buffer[0])
            {
                case 'a':
                    {
                        add(&Ready, &Running, buffer, 3, cuanta, 0);
                        break;
                    }
                case 's':
                    {
                        show(Running,output);
                        if (Running != NULL)
                        break;
                    }
                case 't':
                    {
                        tick_RR(&Ready,&Running,cuanta);
                        break;
                    }
                case 'm':
                    {
                        multiple_add(&Ready, &Running, buffer, 3, cuanta, 0);
                        break;
                    }
                case 'w':
                    {
                        wait(&Waiting, &Ready, &Running, 3, cuanta, 0);
                        break;
                    }
                case 'e':
                    {
                        event(&Waiting, &Ready, &Running, buffer, 3, cuanta, 0);
                        break;
                    }
            }
            for (i=0;i<n;i++)
                buffer[i]=0;
            n=0;
        }
        if (c == EOF)
            break;
        c = fgetc(input);
    }
    freeList(&Running);
    freeList(&Ready);
    freeList(&Waiting);
}

void PP(FILE *input, FILE *output, int cuanta, int nivele_prioritate)
{
    Nod Ready = NULL;
    Nod Running = NULL;
    Nod Waiting = NULL;
    int i,n=0;
    char buffer[100] = "",c;
    c = fgetc(input);
    c = fgetc(input);
    while(1)
    {
        if (c != '\n' && c != EOF)
        {
            buffer[n] = c;
            n++;
        }
        else
        {
            switch(buffer[0])
            {
                case 'a':
                    {
                        add(&Ready, &Running, buffer, 4, cuanta, nivele_prioritate);
                        break;
                    }
                case 's':
                    {
                        show(Running,output);
                        if (Running != NULL)
                        break;
                    }
                case 't':
                    {
                        tick_PP(&Ready,&Running,cuanta,nivele_prioritate);
                        break;
                    }
                case 'm':
                    {
                        multiple_add(&Ready, &Running, buffer, 4, cuanta, nivele_prioritate);
                        break;
                    }
                case 'w':
                    {
                        wait(&Waiting, &Ready, &Running, 4, cuanta, nivele_prioritate);
                        break;
                    }
                case 'e':
                    {
                        event(&Waiting, &Ready, &Running, buffer, 4, cuanta, nivele_prioritate);
                        break;
                    }
                }
            for (i=0;i<n;i++)
                buffer[i]=0;
            n=0;
        }
        if (c == EOF)
            break;
        c = fgetc(input);
    }
    freeList(&Running);
    freeList(&Ready);
    freeList(&Waiting);
}

void Bonus(FILE *input, FILE *output, int cuanta, int nivele_prioritate)
{
    Nod Ready = NULL;
    Nod Running = NULL;
    Nod Waiting = NULL;
    int i,n=0;
    char buffer[100] = "",c;
    c = fgetc(input);
    c = fgetc(input);
    while(1)
    {
        if (c != '\n' && c != EOF)
        {
            buffer[n] = c;
            n++;
        }
        else
        {
            switch(buffer[0])
            {
                case 'a':
                    {
                        add(&Ready, &Running, buffer, 5, cuanta, nivele_prioritate);
                        break;
                    }
                case 's':
                    {
                        show(Running,output);
                        if (Running != NULL)
                        break;
                    }
                case 't':
                    {
                        tick_Bonus(&Ready,&Running,cuanta,nivele_prioritate);
                        break;
                    }
                case 'm':
                    {
                        multiple_add(&Ready, &Running, buffer, 5, cuanta, nivele_prioritate);
                        break;
                    }
                case 'w':
                    {
                        wait(&Waiting, &Ready, &Running, 5, cuanta, nivele_prioritate);
                        break;
                    }
                case 'e':
                    {
                        event(&Waiting, &Ready, &Running, buffer, 5, cuanta, nivele_prioritate);
                        break;
                    }
                }
            for (i=0;i<n;i++)
                buffer[i]=0;
            n=0;
        }
        if (c == EOF)
            break;
        c = fgetc(input);
    }
    freeList(&Running);
    freeList(&Ready);
    freeList(&Waiting);
}

int main (int argc, char *argv[])
{
    if (argc!=3)
    {
        printf("Numarul de argumente trebuie sa fie 2:\n1)fisierul de intrare\n2)fisierul de iesire!\n");
        return 0;
    }
    FILE *input;
    FILE *output;
    //fisierul din care se citeste:
    input = fopen(argv[1],"r");
    //fisierul in care se scrie:
    output = fopen(argv[2],"w");

    int plan;
    fscanf(input,"%d",&plan);
    switch (plan)
    {
        //In acest switch se alege planificarea ce va fi utilizata
        case 1:
            {
                FCFS(input, output);
                break;
            }
        case 2:
            {
                SJF(input, output);
                break;
            }
        case 3:
            {
                int cuanta;
                fscanf(input,"%d",&cuanta);
                RR(input, output, cuanta);
                break;
            }
        case 4:
            {
                int cuanta, nivele_prioritate;
                fscanf(input,"%d%d",&cuanta,&nivele_prioritate);
                PP(input, output, cuanta,nivele_prioritate);
                break;
            }
        case 5:
            {
                int cuanta, nivele_prioritate;
                fscanf(input,"%d%d",&cuanta,&nivele_prioritate);
                Bonus(input,output,cuanta,nivele_prioritate);
                break;
            }
    }
    fclose(input);
    fclose(output);
    return 0;
}
