#include <iostream>
#include <graphics.h>
#include <math.h>
#include <bits/stdc++.h>
#define MAX 100
#define MAXIM 100
using namespace std;
char SirIntrodus[MAXIM],c;
string copie,s[MAX];
int input_pos = 0,j,okay,scene=1,variabile,n;
bool isRunning = true;
char coada[MAX][MAX],*p,v[MAX][MAX];
void rezultatExpresie();
void introducere()
{
    setbkcolor(57650676);
    readimagefile("exp.jpg", 0, 0,1000,600);
    settextstyle(8,0,5);
    settextstyle(9,HORIZ_DIR, 3);
    setfillstyle(SOLID_FILL,COLOR(206,11,84));
    int the_end = 0;
    outtextxy (0,0, "Introduceti expresia: ");
    do
    {
        outtextxy (100,100, SirIntrodus);
        c = getch();
        switch (c)
        {
        case 8: // backspace /
            if (input_pos)
            {
                input_pos--;
                SirIntrodus[input_pos] = 0;
            }
            break;
        case 13: // return /
            the_end = 1;
            break;
        case 27: // Escape = /
            SirIntrodus[0] = 0;
            the_end = 1;
            break;
        default:
            if (input_pos < MAXIM-1 && c >= ' ' && c <= '~')
            {
                SirIntrodus[input_pos] = c;
                input_pos++;
                SirIntrodus[input_pos] = 0;
            }
        }
    }
    while (!the_end);
}
struct nod
{
    string info;
    nod *st;
    nod *dr;
    int coloana = 0;
    int linie;
    int centrux;
    int centruy;
};
struct matrice
{
    int i1,j1,i2,j2;
} m[101][101];
int noduri;
int indicei=1,indicej=1;
int lungime,latime;
int imax,jmax;
int l = 0;
nod *root;
int col = 0;
int calculinaltime(nod*&n)
{
    if(n == NULL)
        return 0;
    return 1 + max(calculinaltime(n->st),calculinaltime(n->dr));
}
void numarnoduri(nod *&n)
{
    if(n != NULL)
    {
        noduri++;
        numarnoduri(n->st);
        numarnoduri(n->dr);
    }
}
bool eOperatorUnar(char c)
{
    if (c== 's' || c=='c' || c=='r' || c=='t' || c=='l')
        return true;
    return false;
}
void desen_nod(string element,int x,int y)
{
    setbkcolor(57650676);
    settextstyle(9,HORIZ_DIR, 1);
    int n = element.length();
    char sir[n + 1];
    strcpy(sir, element.c_str());
    outtextxy(x-5, y-5, sir);
    outstreamxy(x,y);
    circle(x,y,25);
}
void afisare(nod *n,bool existast, bool existadr)
{
    settextstyle(9,HORIZ_DIR, 2);
    if(n != NULL)
    {
        //cout << n->info << ' ' << n->linie << ' ' << n->coloana << endl;
        n->centrux = (m[n->linie][n->coloana].i1 + m[n->linie][n->coloana].i2)/2+200;
        n->centruy = (m[n->linie][n->coloana].j1 + m[n->linie][n->coloana].j2)/2+20;
        int x = n->centrux;
        int y = n->centruy;
        desen_nod(n->info,n->centrux,n->centruy);
        if(n->dr != NULL)
            existadr = true;
        if(n->st != NULL)
            existast = true;
        if(existast)
        {
            int distanta = n->coloana - n->st->coloana;// distanta dintre 2 casute
            int finalx = x-distanta * lungime - 7; // scadeam 7 pentru a face linia pe marginea nodului
            int finaly = y+latime - 24;
            line(x-20,y+20,finalx,finaly);// linia dintre noduri
            x -= lungime;
            y += latime;
            afisare(n->st,false,false);
            x += lungime;
            y -= latime;
        }
        if(existadr)
        {
            int distanta = n->dr->coloana - n->coloana;
            int finalx = x+distanta * lungime + 1;
            int finaly = y+latime - 25;
            line(x+20,y+20,finalx,finaly);
            x += lungime;
            y += latime;
            afisare(n->dr,false,false);
            x -= lungime;
            y -= latime;
        }
    }
}
void coordonatematrice()
{
    int coordi1 = 0;
    int coordi2 = lungime;
    int coordj1 = 0;
    int coordj2 = latime;
    for(int i = 1; i <=imax; ++i)
    {
        coordi1 = 0;
        coordi2 = lungime;
        for(int j = 1; j <= jmax; ++j)
        {
            m[i][j] = {coordi1,coordj1,coordi2,coordj2};
            coordi1 += lungime;
            coordi2 += lungime;
        }
        coordj1 += latime;
        coordj2 += latime;
    }
    /* cout << endl;
    for(int i = 1; i <= imax; ++i)
     {
         for(int j = 1; j <= jmax; ++j)
             cout << m[i][j].i1 << " " << m[i][j].j1 << " " << m[i][j].i2 << " " << m[i][j].j2 << endl;
         cout << endl;
     }*/
}
void parcurgere_linii(nod*&n,int l)
{
    if(n != NULL)
    {
        n->linie = l;
        parcurgere_linii(n->st,l + 1);
        parcurgere_linii(n->dr,l + 1);
    }
}
void parcurgere_coloane(nod*&n)
{
    if(n != NULL)
    {
        parcurgere_coloane(n->st);
        n->coloana = ++col;
        parcurgere_coloane(n->dr);
    }
}
int prioritate(char c)
{
    if(c=='^')
        return 3;
    else if(c=='*'||c=='/')
        return 2;
    else if(c=='+'||c=='-')
        return 1;
    else return -1;
}
void infixtoPostfix(char s[MAX][MAX], int n, char coada[MAX][MAX])
{
    stack<char>st;
    int ceva=0;
    int ok=0;
    for(int i=0; i<n; i++)
    {
        if(strcmp(s[i],"sin")==0 || strcmp(s[i],"cos")==0 || strcmp(s[i],"tg")==0 || strcmp(s[i],"ctg")==0 || strcmp(s[i],"rad")==0 || strcmp(s[i],"log")==0)
        {
            st.push('s');
            char cp[MAX]="";
            strcpy(cp,s[i]);
            i++;
            int ok=1;//e paranteza deschisa // 2+sin(4+5) // 2 4 5 + sin  +
            st.push(s[i][0]);
            i++;
            while(i<n && ok!=0)
            {
                if(strcmp(s[i],"sin")==0 || strcmp(s[i],"cos")==0 || strcmp(s[i],"tg")==0 || strcmp(s[i],"ctg")==0 || strcmp(s[i],"rad")==0 || strcmp(s[i],"log")==0)
                {
                    st.push('s');
                    char cp[MAX]="";
                    strcpy(cp,s[i]);
                    i++;
                    int ok=1;//e paranteza deschisa // 2+sin(4+5) // 2 4 5 + sin  +
                    st.push(s[i][0]);
                    i++;
                    while(i<n && ok!=0)
                    {
                        if(strlen(s[i])==1)//poate fi litera/cifra/( /)/operatori
                        {
                            if(strstr("0123456789x",s[i]))
                                strcat(coada[j++],s[i]);
                            else if(strcmp(s[i],"(")==0 )
                            {
                                ok++;
                                if (ceva==0)
                                {
                                    st.push(s[i][0]);
                                    n++;
                                    ceva=1;
                                }
                                else
                                    st.push(s[i][0]);
                            }
                            else if(strcmp(s[i],")")==0)
                            {
                                // cout<<s[i]<<"  PARANTEZA ";
                                ok--;
                                while(!st.empty()&&st.top()!='(')
                                {
                                    char aux[5];
                                    aux[0]=st.top();
                                    aux[1]=0;
                                    strcat(coada[j++],aux);
                                    st.pop();
                                }
                                if(!st.empty())
                                {
                                    //cout<<st.top()<<"PARANTEZA ";
                                    st.pop();//elim paranteaza
                                }
                            }
                            else//operanzi
                            {
                                while(!st.empty()&& prioritate(st.top())>=prioritate(s[i][0]))
                                {
                                    char aux[5];
                                    aux[0]=st.top();
                                    aux[1]=0;
                                    strcat(coada[j++],aux);
                                    st.pop();
                                }
                                st.push(s[i][0]);//dupa ce a scos toate cu priorit mai mare il adauga pe s[i] in stiva/ daca stiva e goala la inceput il pune direct
                            }
                        }
                        else//mai multe cifre, nr>9
                        {
                            strcat(coada[j++],s[i]);
                        }
                        i++;
                    }
                    while(!st.empty() && st.top()!='s')
                    {
                        char aux[5];
                        aux[0]=st.top();
                        aux[1]=0;
                        strcat(coada[j++],aux);
                        st.pop();
                    }
                    st.pop();
                    strcat(coada[j++],cp);
                    i--;
                }
                if(strlen(s[i])==1)//poate fi litera/cifra/( /)/operatori
                {
                    if(strstr("0123456789x",s[i]))
                        strcat(coada[j++],s[i]);
                    else if(strcmp(s[i],"(")==0 )
                    {
                        ok++;
                        if (ceva==0)
                        {
                            st.push(s[i][0]);
                            n++;
                            ceva=1;
                        }
                        else
                            st.push(s[i][0]);
                    }
                    else if(strcmp(s[i],")")==0)
                    {
                        ok--;
                        while(!st.empty()&&st.top()!='(')
                        {
                            char aux[5];
                            aux[0]=st.top();
                            aux[1]=0;
                            strcat(coada[j++],aux);
                            st.pop();
                        }
                        if(!st.empty())
                        {
                            cout<<st.top()<<"PARANTEZA ";
                            st.pop();//elim paranteaza
                        }
                    }
                    else//operanzi
                    {
                        while(!st.empty()&& prioritate(st.top())>=prioritate(s[i][0]))
                        {
                            char aux[5];
                            aux[0]=st.top();
                            aux[1]=0;
                            strcat(coada[j++],aux);
                            st.pop();
                        }
                        st.push(s[i][0]);//dupa ce a scos toate cu priorit mai mare il adauga pe s[i] in stiva/ daca stiva e goala la inceput il pune direct
                    }
                }
                else//mai multe cifre, nr>9
                {
                    strcat(coada[j++],s[i]);
                }
                i++;
            }
            while(!st.empty() && st.top()!='s')
            {
                char aux[5];
                aux[0]=st.top();
                aux[1]=0;
                strcat(coada[j++],aux);
                st.pop();
            }
            st.pop();
            strcat(coada[j++],cp);
            i--;
        }
        else if(strlen(s[i])==1) //poate fi litera/cifra/( /)/operatori
        {
            if(strstr("0123456789x",s[i]))
                strcat(coada[j++],s[i]);
            else if(strcmp(s[i],"(")==0 )
            {
                if (ceva==0)
                {
                    st.push(s[i][0]);
                    n++;
                    ceva=1;
                }
                else
                    st.push(s[i][0]);
            }
            else if(strcmp(s[i],")")==0)
            {
                while(!st.empty()&&st.top()!='(')
                {
                    char aux[5];
                    aux[0]=st.top();
                    aux[1]=0;
                    strcat(coada[j++],aux);
                    st.pop();
                }
                if(!st.empty())
                {
                    st.pop();//elim paranteaza
                }
            }
            else//operanzi
            {
                while(!st.empty()&& prioritate(st.top())>=prioritate(s[i][0]))
                {
                    char aux[5];
                    aux[0]=st.top();
                    aux[1]=0;
                    strcat(coada[j++],aux);
                    st.pop();
                }
                st.push(s[i][0]);//dupa ce a scos toate cu priorit mai mare il adauga pe s[i] in stiva/ daca stiva e goala la inceput il pune direct
            }
        }
        else//mai multe cifre, nr>9
        {
            strcat(coada[j++],s[i]);
        }
    }
    while(!st.empty())
    {
        char aux[5];
        aux[0]=st.top();
        aux[1]=0;
        strcat(coada[j++],aux);
        st.pop();
    }
}
bool OperatorCaracter(char c)
{
    return (c == '+' || c== '-' || c== '*' || c == '/' || c== '^' || c=='s' || c=='c' || c=='r' || c=='t' || c=='l');
}
nod* construct(string postfix[MAX], int n)
{
    if(n!=0)
    {
        stack<nod*> s;// 3 sin
        for (int i=0; i<n; i++) // parcurgem expresia postfixata
        {
            if(postfix[i].length()!=0)
            {
                char c=postfix[i][0];
                if (OperatorCaracter(c)) // daca e operator
                {
                    if(eOperatorUnar(c)) // scoatem un nod din stiva
                    {
                        nod* x = s.top();
                        s.pop();
                        nod *r=new nod;
                        int l=postfix[i].length();
                        r->info.replace(0,l,postfix[i]);
                        r->st=NULL;
                        r->dr=x;
                        s.push(r);//punem nodul in stiva
                    }
                    else    // scoatem 2 noduri din stiva
                    {
                        nod* x = s.top();
                        s.pop();
                        nod* y = s.top();
                        s.pop();
                        // operatorul e radacina iar x si y fii
                        nod *r=new nod;
                        int l=postfix[i].length();
                        r->info.replace(0,l,postfix[i]);
                        r->info, postfix[i];
                        r->st=y;
                        r->dr=x;
                        s.push(r);//punem nodul in stiva
                    }
                }
                else if(!OperatorCaracter(c))
                {
                    nod *r=new nod;
                    int l=postfix[i].length();
                    r->info.replace(0,l,postfix[i]);
                    r->info, postfix[i];
                    r->st=NULL;
                    r->dr=NULL;
                    s.push(r);//punem nodul in stiva
                }
            }
        }
        return s.top();
    }


}
string convertToString(char* a)
{
    string s = a;
    return s;
}
void incarca()
{
    setbkcolor(COLOR(206,11,84));
    int i,j,r=10;
    for(i=10; i<1000; i++)
    {
        setcolor(j);
        circle(10,i,r);// stanga lateral
        circle(1000-r,i,r);//dreapta
        setcolor(5);
        circle(i,10,r); //sus
        circle(i,600-r,r);
        delay(0.1);
    }
    readimagefile("loading.jpg",345,180, 660, 370);  //
    rectangle(370,420,630,400);
    settextstyle(8,HORIZ_DIR,7);
    setcolor(15);
    setbkcolor(BLACK);
    outtextxy(363,80,"PROIECT");
    for(int i=1; i<240; i++)
    {
        line(370+i,400,370+i,420);
        delay(1);
    }
    cleardevice();
}
void paginaDescriere()
{
    cleardevice();
    setbkcolor(61463804);
    setcolor(BLACK);
    settextstyle(9,0,8);
    outtextxy(270,70,"DESCRIERE");
    readimagefile("reg.jpg", 0, 0,1000,600);
    readimagefile("sageata.jpg", 0, 0,80,60);
    setfillstyle(SOLID_FILL, WHITE);
    settextstyle(8,0,2);
    setcolor(BLACK);
    outtextxy(100,90,"Programul poate interpreta o formula matematica");
    outtextxy(100,110,"si o afiseaza grafic.");
    outtextxy(100,130,"In sectiunea <arbore> este reprezentat ");
    outtextxy(100,150,"arborele expresiei.");
    outtextxy(100,170,"In sectiunea <evaluator> este reprezentata");
    outtextxy(100,190,"expresia intodusa.");
    settextstyle(9,0,5);
    int x,y;
    getmouseclick(WM_LBUTTONDOWN, x, y);
    if(x>=0 && x<=100 && y>=0 && y<=100)
        scene=1; // inapoi la pagina principala
}
void paginaArbore()
{
    cleardevice();
    setbkcolor(50483883);
    readimagefile("game.jpg", 0, 0,1000,600);
    readimagefile("sageata.jpg", 0, 0,100,80);
    settextstyle(8,HORIZ_DIR,4);
    outtextxy(310,42,"ARBORE");
    int x,y;
    getmouseclick(WM_LBUTTONDOWN, x, y);
    if(x>=0 && x<=100 && y>=0 && y<=80)
    {
        scene=1;
    }
}
bool eNumar(string c)
{
    if (c.back()=='0' || c.back()=='1' || c.back()=='2' || c.back()=='3' ||  c.back()=='4' ||  c.back()=='5' ||  c.back()=='6' || c.back()=='7' || c.back()=='8' ||  c.back()=='9')
    {
        return true;
    }
    return false;
}
bool eVariabila(string c) // daca ultimul caracter e cifra => avem un numar// sau daca e variabila
{
    if (c.back()=='0' || c.back()=='1' || c.back()=='2' || c.back()=='3' ||  c.back()=='4' ||  c.back()=='5' ||  c.back()=='6' || c.back()=='7' || c.back()=='8' ||  c.back()=='9')
    {
        return true;
    }
    else if (c=="a" || c=="b" || c=="d" || c=="e"|| c=="f" || c=="g" || c=="h" || c=="i" || c=="j" || c=="l" || c=="m" || c=="n" || c=="o" || c=="p"|| c=="q"  || c=="w" || c=="x" || c=="y" || c=="v" || c=="z"  )
    {
        return true;
    }
    return false;
}
int nrcaracterenod(nod*t)
{
    return t->info.length();
}
int nrnoduri(nod *n)
{
    if(n == NULL) return 0;
    else
    {
        return 1+nrnoduri(n->st)+nrnoduri(n->dr);
    }
}
int NR(nod *r)
{
    if(r == NULL) return 0;
    else
    {
        if(r->info=="^" || r->info=="/")
            return 1+NR(r->st)+NR(r->dr);
        else if(r->info=="+"||r->info=="*"||r->info=="-")
            return max(NR(r->st),NR(r->dr));
        else return 0;
    }
}
int af;
int suprafatazona;
void Desen(nod *r,int LungimeZonaStart,int LungimeZonaStop, int LatimeZonaStart, int LatimeZonaStop)
{
    settextstyle(5,HORIZ_DIR, 1);
    if(r->info=="/")
    {
         int hst=NR(r->st);
         int hdr=NR(r->dr);
        hst++;
        hdr++;
        int lst=nrnoduri(r->st);
        int ldr=nrnoduri(r->dr);
        int lmax=max(lst,ldr);
        lmax*=40;
        line((LungimeZonaStart+LungimeZonaStop)/2-lmax/2,(LatimeZonaStart+LatimeZonaStop)/2,(LungimeZonaStart+LungimeZonaStop)/2+lmax/2,(LatimeZonaStart+LatimeZonaStop)/2);
        int hstart=max(LatimeZonaStart,(LatimeZonaStart+LatimeZonaStop)/2-30*hst);
        int hstop=min(LatimeZonaStop,(LatimeZonaStart+LatimeZonaStop)/2+30*hdr-10);
        Desen(r->st,(LungimeZonaStart+LungimeZonaStop)/2-lmax/2-10,(LungimeZonaStart+LungimeZonaStop)/2+lmax/2+10,hstart,(LatimeZonaStart+LatimeZonaStop)/2-10);
        Desen(r->dr,(LungimeZonaStart+LungimeZonaStop)/2-lmax/2-10,(LungimeZonaStart+LungimeZonaStop)/2+lmax/2+10,(LatimeZonaStart+LatimeZonaStop)/2,hstop);

    }
    else if(r->info=="+" || r->info=="*" || r->info=="-")
    {
        if(af==0){af=1;suprafatazona=(LungimeZonaStop-LungimeZonaStart)/nrnoduri(r);}
        int nrnodst=nrnoduri(r->st);
        int nrnoddr=nrnoduri(r->dr);
        int centruNodX=LungimeZonaStart+nrnodst*suprafatazona+suprafatazona/2+10;
        int centruNodY=(LatimeZonaStart+LatimeZonaStop)/2;
        bgiout<<r->info;
        outstreamxy(centruNodX,centruNodY);
        if(r->info=="*" && nrnoduri(r)>3)
        {
                    settextstyle(5,HORIZ_DIR,6);
        bgiout<<"(";
        outstreamxy(centruNodX+20,centruNodY-20);
        bgiout<<")";
        outstreamxy(LungimeZonaStop-6,centruNodY-20);
        }
       // cout<<"chestii..."<<LungimeZonaStart+suprafatazona*(nrnodst+1)<<' '<<LungimeZonaStop;
      //cout<<endl;
        Desen(r->st,LungimeZonaStart,LungimeZonaStart+suprafatazona*nrnodst,LatimeZonaStart,LatimeZonaStop);
        Desen(r->dr,LungimeZonaStart+suprafatazona*(nrnodst+1),LungimeZonaStop,LatimeZonaStart,LatimeZonaStop);
    }
    else if (r->info=="sin"|| r->info=="cos" || r->info=="tg" || r->info=="ctg" || r->info=="log")
    {
        int nrnoddr=nrnoduri(r->dr);
                if(af==0){af=1;suprafatazona=(LungimeZonaStop-LungimeZonaStart)/nrnoduri(r);}
        int centruNodX=LungimeZonaStart+20;
        int centruNodY=(LatimeZonaStart+LatimeZonaStop)/2;
        bgiout<<r->info;
        outstreamxy(centruNodX,centruNodY);
        settextstyle(5,HORIZ_DIR,6);
        bgiout<<"(";
        outstreamxy(centruNodX+20,centruNodY-20);
         bgiout<<")";
       outstreamxy(LungimeZonaStop+7,centruNodY-20);
        Desen(r->dr,centruNodX+45,LungimeZonaStop,LatimeZonaStart+50,LatimeZonaStop-50);//lungimestop=start+suprafata
    }
    else if ( r->info=="^")
    {
        int nrnodst=nrnoduri(r->st);
        int nrnoddr=nrnoduri(r->dr);
        int hst=NR(r->st);
        int hdr=NR(r->dr);
        hst++;
        hdr++;
         if(af==0){af=1;suprafatazona=(LungimeZonaStop-LungimeZonaStart)/nrnoduri(r);}
       /*if( r->st->info=="+" || r->st->info=="-" ||r->dr->info=="+" || r->dr->info=="-" ||r->dr->info=="*" )
         {
        outtextxy(LungimeZonaStart,(LatimeZonaStop+LatimeZonaStart)/2,"(");
        outtextxy(LungimeZonaStart+nrnodst*suprafatazona,(LatimeZonaStop+LatimeZonaStart)/2,")");
          }
          */
        int maxi=max((LatimeZonaStop+LatimeZonaStart)/2-hdr*30,LatimeZonaStart);
        Desen(r->st,LungimeZonaStart,LungimeZonaStart+nrnodst*suprafatazona,LatimeZonaStart,LatimeZonaStop);
        Desen(r->dr,LungimeZonaStart+nrnodst*suprafatazona,LungimeZonaStart+nrnodst*suprafatazona,maxi,(LatimeZonaStop+LatimeZonaStart)/2);

    }
    else if ( r->info=="rad")
    {
        int h=NR(r->dr);
        if(r->dr->info=="^"||r->dr->info=="/")h++;
        h++;
        //rcout<<h;
        if(af==0){af=1;suprafatazona=(LungimeZonaStop-LungimeZonaStart)/nrnoduri(r);}
        line(LungimeZonaStart+suprafatazona/4,(LatimeZonaStart+LatimeZonaStop)/2-25*h,LungimeZonaStop-10,(LatimeZonaStart+LatimeZonaStop)/2-25*h);//in loc de 26 cv cu noduri sus jos
        line(LungimeZonaStart+suprafatazona/4,(LatimeZonaStart+LatimeZonaStop)/2-25*h,LungimeZonaStart+suprafatazona/4,(LatimeZonaStart+LatimeZonaStop)/2+25*h);
        line(LungimeZonaStart+suprafatazona/4,(LatimeZonaStart+LatimeZonaStop)/2+25*h,LungimeZonaStart,(LatimeZonaStart+LatimeZonaStop)/2-25*h);
        Desen(r->dr,LungimeZonaStart+suprafatazona/4,LungimeZonaStop-10,(LatimeZonaStart+LatimeZonaStop)/2-25*h+20,(LatimeZonaStart+LatimeZonaStop)/2+25*h);
    }
    else if (eVariabila(r->info))
    {

        bgiout<<r->info;
        int centruNodX=(LungimeZonaStart+LungimeZonaStop)/2;
        int centruNodY=(LatimeZonaStart+LatimeZonaStop)/2;
        outstreamxy(centruNodX,centruNodY);
    }
}
void paginaEvaluator()
{
    cleardevice();
    readimagefile("exp.jpg", 0, 0,1000,600);
    setbkcolor(57650676);
    readimagefile("sageata.jpg", 0, 0,100,80);
    readimagefile("raspuns.jpg", 0, 81,100,150);
    setcolor(BLACK);
    Desen(root,0,900,0,600);
    int x,y;
    getmouseclick(WM_LBUTTONDOWN, x, y);
    if(x>=0 && x<=100 && y>=81 && y<=150)
        scene=5;
    else if(x>=0 && x<=100 && y>=0 && y<=80)
        scene=1;
}
void paginaPrincipala()
{
    cleardevice();
    setfillstyle(SOLID_FILL,59543514);
    bar(0,0,1000,90);
    setbkcolor(59543514);
    settextstyle(9,0,5);
    setcolor(WHITE);
    outtextxy(135,15,"EVALUATOR EXPRESII");
    line(0,90,1500,90);
    settextstyle(9,0,6);
    setfillstyle(SOLID_FILL,COLOR(0,174,239));
    setcolor(BLACK);
    bar(50,110,950,250);
    outtextxy(270,137,"DESCRIERE");
    setfillstyle(SOLID_FILL,57650750);
    bar(50,250,950,390);
    outtextxy(340,277,"ARBORE");
    setfillstyle(SOLID_FILL,57650678);
    bar(50,390,950,540);
    outtextxy(270,420,"EVALUATOR");
    setfillstyle(SOLID_FILL,COLOR(245,22,106));
    int x,y;
    getmouseclick(WM_LBUTTONDOWN, x, y);
    if(x>=50 && x<=950 && y>=110 && y<=250)
        scene=2;
    else  if(x>=50 && x<=950 && y>=250 && y<=390)
        scene=3;
    else  if(x>=50 && x<=950 && y>=390 && y<=540)
        scene=4;

}
void variante()
{
    switch (scene)
    {
    case 1:
        paginaPrincipala();
        break;
    case 2:
        paginaDescriere();
        break;
    case 3:
        paginaArbore();
        break;
    case 4:
        paginaEvaluator();
        break;
    case 5:
        rezultatExpresie();
        break;
    }
}
string ParantezaCoresp(string c)
{
    if (c.back() == ')')
    {
        return "(";
    }

}
bool ParantezaDeschisa(string c)
{
    if (c.back() == '(')
    {
        return true;
    }
    return false;
}
bool ParantezaInchisa(string c)
{
    if (c.back() == ')')
    {
        return true;
    }
    return false;
}
bool SirOperator(string c)
{
    if (c == "+" || c== "-" || c== "*" ||  c== "sin"  || c== "cos"  || c== "rad" || c == "^" || c== "/" || c == "tg" || c== "ctg" || c== "log")//log=log in baza e
    {
        return true;
    }
    return false;
}
bool Unar(string c)
{
    if (c== "sin"  || c== "cos"  || c== "rad" || c == "tg" || c== "ctg" || c== "log")
    {
        return true;
    }
    return false;
}
int tip(char p)
{
    if(strchr("0123456789",p))
        return 1;
    if(strchr("sincosthctgradlog",p))
        return 2;
    if(strchr("(",p))
        return 3;
    if(strchr(")",p))
        return 4;
    if(strchr("*",p))
        return 5;
    if(strchr("/",p))
        return 6;
    if(strchr("^",p))
        return 7;
    if(strchr("x",p))
        return 8;
    if(strchr("+",p))
        return 9;
    if(strchr("-",p))
        return 10;
}
void verificareExpresie(char ex[MAX][MAX],int k,int &dc,bool &rez)
{
    int pd=0,pi=0,pr=1,i=0;
    for( i=0; i<k ; i++) //2 + 3 )
    {
        int t=tip(ex[i][0]);
        if(i==k-1)
        {
            if(t==4) pi++;
            else if(t==3) pd++;
            if(t==2 || t==3 || t==5 || t==6 || t==7 || t==9 || t==10 )
            {
                dc=1;// nu putem avea operatori sau paranteze deschise la finalul exprsiei
                rez=false;
            }
        }
        else
        {

            if(t==1)
            {
                if(tip(ex[i+1][0])==3 || tip(ex[i+1][0])==8 || tip(ex[i+1][0])==2)
                {
                    dc=2;//dupa numere putem avea doar operatori sau paranteza inchisa
                    rez= false;
                }
            }
            else if(t==2)
            {
                if(tip(ex[i+1][0])!=3 )
                {
                    dc=3;// dupa sin putem avea doar paranteza deschisa
                    rez= false;
                }
            }
            else if(t==3)
            {
                if(tip(ex[i+1][0])==4 ||tip(ex[i+1][0])==5 ||tip(ex[i+1][0])==6  ||tip(ex[i+1][0])==7   ||tip(ex[i+1][0])==9 ||tip(ex[i+1][0])==10  )
                {
                    dc=4;//dupa paranteza deschisa putem avea doar un numar sau ceva cu sin/cos
                    rez= false;
                }
            }
            else if(t==4)
            {
                if(tip(ex[i+1][0])==1 || tip(ex[i+1][0])==2 || tip(ex[i+1][0])==3 || tip(ex[i+1][0])==8)
                {
                    dc=5;//dupa paranteza inchisa putem avea doar operanzi sau alte paranteze inchise
                    rez= false;
                }
            }
            else if(t==5 || t==6 || t==7 || t==9 || t==10)
            {
                if(tip(ex[i+1][0])==1 || tip(ex[i+1][0])==2 || tip(ex[i+1][0])==3 || tip(ex[i+1][0])==8)
                {

                }
                else
                {
                    dc=6;//dupa operatori nu putem avea decat numere sau paranteza deschisa
                    rez= false;
                }
            }
            else  if(t==8)
            {
                if(tip(ex[i+1][0])==5 || tip(ex[i+1][0])==6 ||tip(ex[i+1][0])==7 || tip(ex[i+1][0])==9 || tip(ex[i+1][0])==10 || tip(ex[i+1][0])==4)
                {

                }else
                {
                    dc=7;//dupa x nu putem avea decat operatori sau paranteza inchisa
                    rez= false;
                }
            }
            if(strcmp(ex[i],"(")==0)
            {
                pd++;
            }
            else if(strcmp(ex[i],")")==0)
            {
                pi++;
            }
            if(pd<pi)
            {
                dc=8;//nu am pus parantezele corect
                rez= false;
            }
        }
    }
    if(pd!=pi)
    {
        rez=false;
        dc=9;
    }
}
float ConvertireInNr(string element)
{
    float num_float = stof(element);
    return num_float;
}
float Operatie(float a, float b, char op)
{
    if(op == '+')
        return b+a;
    else if(op == '-')
        return b-a;
    else if(op == '*')
        return b*a;
    else if(op == '/')
        return b/a;
    else if(op == '^')
        return pow(b,a);
    else
        return INT_MIN;
}
float trig(float a, string operatie)
{
    if(operatie=="cos")
        return cos(a);
    else  if(operatie=="sin")
        return sin(a);
    else if(operatie=="tg")
        return tan(a);
    else  if(operatie=="rad")
        return sqrt(a);
    else if (operatie=="ctg")
        return cos(a)/sin(a);
    else if (operatie=="log")
        return log(a);
}
float postfixEval(string s[])
{
    float a, b;
    stack<float> stk;
    for(int i=0; i<n; i++)// sin(3)+rad(4)// 3 sin 4 rad +
    {
        if(SirOperator(s[i]))
        {
            if(Unar(s[i]))
            {
                a = stk.top();
                stk.pop();
                stk.push(trig(a,s[i]));
            }
            else
            {
                a = stk.top();
                stk.pop();
                b = stk.top();
                stk.pop();
                char it=s[i].back();
                stk.push(Operatie(a, b, it));
            }
        }
        else if(eNumar(s[i]))
        {
            stk.push(ConvertireInNr(s[i]));
        }
    }
    return stk.top();
}
void rezultatExpresie()
{
    cleardevice();
    readimagefile("exp.jpg", 0, 0,1000,600);
    setbkcolor(57650676);
    readimagefile("sageata.jpg", 0, 0,100,80);
    setcolor(BLACK);
    settextstyle(8,HORIZ_DIR,7);
    float raspuns=postfixEval(s);
    bgiout<<raspuns;
    outstreamxy(300,300);
    int x,y;
    getmouseclick(WM_LBUTTONDOWN, x, y);
    if(x>=0 && x<=100 && y>=0 && y<=80)
        scene=4;
}
int main()
{
    initwindow(1000,600,"grafica arbore binar");
    //incarca();
    introducere();
    int k=0;
    int t;
    char s2[2];
    s2[1]='\0';
    for (int i=0; i<strlen(SirIntrodus); i++)
    {
        int t=tip(SirIntrodus[i]);
        if (t==1 || t==2)
        {
            strcpy(v[k],"");
            while (tip(SirIntrodus[i])==t && i<strlen(SirIntrodus))
            {
                s2[0]=SirIntrodus[i];
                s2[1]='\0';
                strcat(v[k],s2);
                i++;
            }
            i--;
        }
        else
        {
            s2[0]=SirIntrodus[i];
            s2[1]='\0';
            strcpy(v[k],s2);
        }
        k++;
    }
    for(int i=0; i<k; i++)
        cout<<v[i]<<' ';
    n=k;
    bool rez=true;
    int dc;
    verificareExpresie(v,k,dc,rez);
    cout<<"CORECTITUDINE   "<<log(10)<<endl;
    if(rez==0)
    {
        cleardevice();
        setbkcolor(RED);
        settextstyle(8,HORIZ_DIR,2);
        setcolor(WHITE);
        outtextxy(100,42,"EXPRESIE GRESITA !!! RECITITI REGULAMENTUL !");
        if(dc==1)
            outtextxy(100,200," nu putem avea operatori sau paranteze deschise la finalul exprsiei");
        else if(dc==2)
            outtextxy(100,200,"dupa numere putem avea doar operatori sau paranteza inchisa");
        else if(dc==3)
            outtextxy(100,200,"dupa functii matematice putem avea doar paranteza deschisa");
        else if(dc==4)
            outtextxy(100,200,"dupa paranteza deschisa putem avea doar un numar sau o functie matematica");
        else if(dc==5)
            outtextxy(100,200,"dupa paranteza inchisa putem avea doar operanzi sau alte paranteze inchise");
        else if(dc==6)
            outtextxy(100,200,"dupa operatori nu putem avea decat numere sau paranteza deschisa");
        else if(dc==7)
            outtextxy(100,200,"dupa x nu putem avea decat operatori sau paranteza inchisa");
        else if(dc==8 || dc==9)
            outtextxy(100,200,"parantezele nu sunt plasate corect");
        delay(10000);

    }
    else
    {
        infixtoPostfix(v,n,coada); // in coada am pastrat postfixata
        for(int i=0; i<n; i++)
        {
            s[i] = convertToString(coada[i]);
            cout<<s[i]<<' ';
        }
        root = construct(s,n);
        //cout << endl << calculinaltime(root);
        numarnoduri(root);
       // cout << endl << noduri;
        lungime = 600/noduri;// suprafata casuta
        int inaltime = calculinaltime(root);
        latime = 600/inaltime;// suprafata casuta
        imax=600/latime;//linii
        jmax=600/lungime;//coloane
        coordonatematrice();
        //cout<<"MATRICE "<<imax<< ' '<<jmax<<' ';
        parcurgere_linii(root,1);
        parcurgere_coloane(root);
        cleardevice();
        settextstyle(9,HORIZ_DIR, 2);
        while(isRunning)
        {
            cleardevice();
            variante();
            if(scene==3)
                afisare(root,false,false);
            else variante();
            delay(10);
            swapbuffers();
        }

        getch();
    }
}

