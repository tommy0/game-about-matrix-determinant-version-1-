/*игра с растановкой цифр по матрице, если в конце детерминант >0 выиграл первый, <0 выиграл второй, =0 ничья */
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <cstring>
#define N 3                                                //размер матрицы игры

using namespace std;

int tabel[N][N];                                          //матрица игры
int tableTmp [N][N];
int explor[10];
int explorTmp[10];
bool coorThere[N][N];                                    //занята ли клетка
bool valThere[10];                                        //использонно ли значение
bool repeat[10];

void outputMatrix()                //вывод игрового поля
{
    int i,j;
    cout<<endl;
    cout<<"   1) 2) 3)"<<endl;
    for (i=0; i<N; ++i)
    {
        cout<<i+1<<") ";
        for (j=0; j<N; ++j)
        {
            cout<<tabel[i][j]<<"  ";
        }
        cout<<endl;
    }
    cout<<endl;
}

void checkInputCoor(int &x, int &y)
{
    cin>>x>>y;
    bool flag = true;
    while(flag)
    {
        if((x>4) || (y>4) || (x<1) || (y<1))
        {
            cout<<"Error input coordinates, please try again"<<endl;
            cin>>x>>y;
        }
        else
        {
            if(coorThere[x-1][y-1])
            {
                cout<<"Sector not clear, please try again"<<endl;
                cin>>x>>y;
            }
            else
            {
                coorThere[x-1][y-1]=true;
                flag = false;
            }
        }
    }
}

void checkInputVal(int &val)                        //проверка на дурака значения
{
    cin>>val;
    bool flag = true;
    while(flag)
    {
        if(val<1 || val>9)
        {
            cout<<"Error input value, please try again"<<endl;
            cin>>val;
        }
        else
        {
            if(valThere[val-1])
            {
                cout<<"Value used, please try again"<<endl;
                cin>>val;
            }
            else
            {
                valThere[val-1]=true;
                flag = false;
            }
        }
    }
}

bool endOfGame()                                     //проверка на конец игры
{
    int sch=0;
    for(int i=0; i<N; ++i)
        for(int j=0; j<N; ++j)
        {
            if(tabel[i][j]!=0) sch++;
        }
    if(sch==N*N) return false;
    else return true;
}

void outputWhoWin(int d)                 //вывод победителя
{
    if(d>0) cout<<"Winner player one"<<endl;
    else if (d<0) cout<<"Winner player two"<<endl;
    else cout<<"The draw"<<endl;
    cout<<"GAME OVER";
}

int det()   //нахождение определителя
{
    int l;
    int d;
    int sum11=1,sum12=0, sum21=1, sum22=0;
// находим детерминант
    for (int i=0; i<N; i++)
    {
        sum11=1;
        l=2*N-1-i;
        sum21=1;
        for (int j=0; j<N; j++)
        {
            sum21*=tabel[j][l%N];
            l--;
            sum11*=tabel[j][(j+i)%(N)];
        }
        sum22+=sum21;
        sum12+=sum11;
    }
    d=sum12-sum22;
    return d;
}

int detTmp()   //нахождение определителя
{
    int l;
    int d;
    int sum11=1,sum12=0, sum21=1, sum22=0;
// находим детерминант
    for (int i=0; i<N; i++)
    {
        sum11=1;
        l=2*N-1-i;
        sum21=1;
        for (int j=0; j<N; j++)
        {
            sum21*=tableTmp[j][l%N];
            l--;
            sum11*=tableTmp[j][(j+i)%(N)];
        }
        sum22+=sum21;
        sum12+=sum11;
    }
    d=sum12-sum22;
    return d;
}

void explorToTabel()
{
    int k=1;
    for(int i=0; i<N; ++i)
        for(int j=0; j<N; ++j)
        {
            tableTmp[i][j]=explor[k];
            ++k;
        }
}

void swapVal(int posSt, int posFn)
{
    int temp;
    temp=explor[posSt];
    explor[posSt]=explor[posFn];
    explor[posFn]=temp;
}

void cleanRepeat()
{
    for(int index=0; index<9; ++index)
        repeat[index+1]=valThere[index];
}

void tabelToExplor()
{
    int k=1;
    for(int i=0; i<N; ++i)
    {
        for(int j=0; j<N; ++j)
        {
            explor[k]=tabel[i][j];
            ++k;
        }
    }
    cleanRepeat();
}

void tabelToExplorTmp()
{
    int k=1;
    for(int i=0; i<N; ++i)
    {
        for(int j=0; j<N; ++j)
        {
            explorTmp[k]=tabel[i][j];
            ++k;
        }
    }
}

bool mark()
{
    if(detTmp>0) return true;
    else return false;
}

void turn(bool* flg, bool* flagTurn)
{
    bool flag = false;
    explorToTabel();
    if(mark())
    {
        for(int i=0; i<N; ++i)
        {
            if(flag) break;
            for (int j=0; j<N; ++j)
            {
                if(tabel[i][j]==0)
                {
                    if(not repeat[tableTmp[i][j]])
                    {
                        tabel[i][j]=tableTmp[i][j];
                        valThere[tableTmp[i][j]-1]=true;
                        repeat[tableTmp[i][j]]=true;
                        flag=true;
                        *flg=false;
                        *flagTurn=true;
                        break;
                    }
                }
            }
        }
    }
    else cleanRepeat();
}

void foolExplor()
{
    int k=1;
    for(int i=1; i<=9; ++i)
    {
        if(explor[i]==0)
        {
            while(repeat[k])
            {
                k++;
            }
            explor[i]=k;
            k++;
        }
    }
}

void bust(int pos, bool* flg, bool* flagTurn)
{
    if(*flg)
    {
        if(pos<=9)
        {
            for(int i=pos; i<=9; ++i)
            {
                if(explorTmp[pos]==0)
                {
                    swapVal(pos,1);
                    bust(pos+1, flg, flagTurn);
                    swapVal(pos,i);
                }
                else
                {
                    bust(pos+1, flg, flagTurn);
                }
            }
        }
        else
        {
            turn(flg,flagTurn);
        }
    }
    else if(*flagTurn==false)
    {
        bool flag=false;
        for(int i=0; i<N; ++i)
        {
            if(flag) break;
            for(int j=0; j<N; ++j)
            {
                if(tabel[i][j]==0)
                {
                    if(not repeat[tableTmp[i][j]])
                    {
                        tabel[i][j]=tableTmp[i][j];
                        valThere[tableTmp[i][j]-1]=true;
                        repeat[tableTmp[i][j]]=true;
                        flag=true;
                        break;
                    }
                }
            }
        }
    }
}

int chooseGame()
{
    cout<<"Hello, this game about determinant"<<endl;
    cout<<"Choose game edition"<<endl;
    cout<<"1) Player vs Player"<<endl;
    cout<<"2) Player vs Computer"<<endl;
    int option;
    bool flag=true;
    while(flag)
    {
        cin>>option;
        if(option<1 || option>2)
        {
            cout<<"Wrong choose, please try again"<<endl;
            cin>>option;
        }
        else
        {
            flag=false;
            return option;
        }
    }
}

void menuPvP()                             //меню игры
{
    int x,y;                                        //координаты хода
    int i,j;                                        //счетчики
    int val;                                        //значение в клетку
    for(i=0; i<9; ++i)
        valThere[i]=false;
    for(i=0; i<N; ++i)
        for(j=0; j<N; ++j)
            coorThere[i][j]=false;
    string numPl;
    bool flag = false;
    cout<<"You play with player"<<endl;
    outputMatrix();
    while(endOfGame())
    {
        flag==false?numPl="one":numPl="two";
        cout<<"Turn player "+numPl<<endl;
        cout<<"Please enter coordinates of you turn: ";  //input запихнуть
        checkInputCoor(x,y);                        //проверка на дурака координат
        cout<<"Enter the value: ";
        checkInputVal(val);                        //проверка на дурака значения
        tabel[x-1][y-1]=val;                                //растановк хода
        system("cls");
        outputMatrix();                                //вывод игрвого поля
        cout<<endl;
        //system("cls");
        flag=not flag;                                  //смена игрока
    }
    int d=det();                                        //расчет детерминанта
    outputWhoWin(d);                                     //вывод победителя
}

void menuPvC()
{
    int x,y;                                        //координаты хода
    int i,j;                                        //счетчики
    int val;                                        //значение в клетку
    for(i=0; i<9; ++i)
        valThere[i]=false;
    for(i=0; i<N; ++i)
        for(j=0; j<N; ++j)
            coorThere[i][j]=false;
    cout<<"You play with computer"<<endl;
    outputMatrix();
    while(endOfGame())
    {
        system("cls");
        cout<<"Computer was turned"<<endl;
        tabelToExplor();
        tabelToExplorTmp();
        foolExplor();
        bool flg;
        flg=true;
        bool flagTurn;
        flagTurn=false;
        bust(1,&flg,&flagTurn);
        outputMatrix();
        if(not endOfGame()) break;
        cout<<"You turn player"<<endl;
        cout<<"Please enter coordinates of you turn: ";  //input запихнуть
        checkInputCoor(x,y);                        //проверка на дурака координат
        cout<<"Enter the value: ";
        checkInputVal(val);                        //проверка на дурака значения
        tabel[x-1][y-1]=val;                                //растановк хода
        outputMatrix();
        cout<<endl;         //вывод игрвого поля
    }
    int d=det();                                        //расчет детерминанта
    outputWhoWin(d);

}

int main()
{
    for (int i=0; i<N; ++i)                 //заполним матрицу нулями для проверки заполнения цифрами
        for(int j=0; j<N; j++)
        {
            tabel[i][j]=0;
            tableTmp[i][j]=0;
        }
    for(int index=0; index<9; ++index)
    {
        valThere[index]=false;
        repeat[index]=false;
    }

    int choose = chooseGame();
    if(choose==1) menuPvP();
    else menuPvC();
    return 0;
}
