#include "Servere.h"
#include <stdio.h>
#include <winsock2.h>
#include <conio.h>
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")
#pragma hdrstop
#pragma argsused

char sendbuf[1000];
char sendbuf2[1000];
char sendString[1000];
int bytesSent;
int bytesSent2;

void addStringToString(int index, int sizeMas)
{
    int indexLast1 = 0, // обозначает конец для первого слова
    indexLast2 = 0, // обозначает конец для второго слова
    flag = 0;       // флаг для правильного поиска
    int bufDlina = 0; // для коректной записи завершающего символа

    for(int i = 0; i < sizeMas; i++) // находим индексы в массиве, которые обозначают конец слова - @
    {
        if (sendbuf[i]!='@' && flag == 0)
        {
            indexLast1++;
        }
        if (sendbuf[i]=='@')
        {
            if (flag==0) indexLast2 = indexLast1;
            flag++;
        }
        if (sendbuf[i]!='@' && flag == 1)
        {
            indexLast2++;
        }
        if (sendbuf[i]=='@' && flag == 2)
        {
            break;
        }
    }

    // теперь, когда мы знаем где конец двух слов, может реализовать вставку
    char mas1[100]="";
    int last = 0;
    if(index>indexLast1)
    {
        std::cout<<"Index error"<<std::endl;
        strcpy(sendbuf,"");
        char otvet [30] = "Net takogo indexa";
        int dlina = strlen(otvet);
        otvet[dlina] = '\0';
        strcpy(sendbuf,otvet);
        return;
    }
    if(index<indexLast1)
    {
        std::cout<<"Ot 0 do index: ";
        for (int i = 0; i < index; i++)
        {
            mas1[i]=sendbuf[i];
            printf("%c",mas1[i]);
            last++;
        }

        bufDlina = strlen(mas1);
        mas1[bufDlina] = '\0';

        char sendbufer[500] = "";
        int j = 0;
        for(int i = indexLast1+1; i < indexLast2+1; i++)
        {
            sendbufer[j] = sendbuf[i];
            j++;
        }
        bufDlina = strlen(sendbufer);
        sendbufer[bufDlina] = '\0';

        strcat(mas1,sendbufer);

        bufDlina = strlen(sendbufer);
        for (int i = last; i < indexLast1; i++)
        {
            mas1[bufDlina+last] =  sendbuf[i];
            bufDlina++;
        }

        bufDlina = strlen(mas1);
        mas1[bufDlina] = '\0';

        strcpy(sendbuf,"");
        strcat(sendbuf,mas1);
        // очищаем переменные для коректного дальнейшего использования

        strcpy(sendbufer,"");
        strcpy(mas1,"");

    }

}
void ravno(int a)
{
    int indexLast1 = 0, indexLast2 = 0, flag = 0;

    for(int i = 0; i < a; i++) // находим индексы в массиве, которые обозначают конец слова - @
    {
        if (sendbuf[i]!='@' && flag == 0)
        {
            indexLast1++;
        }
        if (sendbuf[i]=='@')
        {
            if (flag==0) indexLast2 = indexLast1;
            flag++;
        }
        if (sendbuf[i]!='@' && flag == 1)
        {
            indexLast2++;
        }
        if (sendbuf[i]=='@' && flag == 2)
        {
            break;
        }
    }
    std::cout<<"Probel stroka 1 = "<<indexLast1<<std::endl<<"Probel stroka 2 = "<<indexLast2<<std::endl;

    int j =  indexLast1+1;
    for (int i = 0; i <= indexLast1; i++)
    {
        if (sendbuf[i]!=sendbuf[j])
        {
            char otvet [100] = "Stroki ne ravni";
            strcpy(sendbuf,otvet);
            int dlina = strlen(otvet);
            otvet[dlina] = '\0';
            return;
        }
        j++;
    }
    char otvet [100] = "Stroki ravni";
    strcpy(sendbuf,otvet);
    int dlina = strlen(otvet);
    otvet[dlina] = '\0';
    return;
}

void conct()
{
    int dlina = strlen(sendbuf2);
    sendbuf2[dlina] = '\0';
    strcat(sendbuf,sendbuf2);
    dlina = strlen(sendbuf);
    sendbuf[dlina] = '\0';
    std::cout<<sendbuf<<std::endl;
}

void ravenstvo(int a)
{
    int leng1 = 0, leng2 = 0, flag = 0;

    for (int i = 0; i < a; i++)
    {
        if(sendbuf[i]!='@' && flag == 0)
        {
            leng1++;
        }
        if (sendbuf[i]=='@')
        {
            flag++;
            continue;
            if (flag==2)
            {
                break;
            }
        }
        if (sendbuf[i]!='@' && flag == 1)
        {
            leng2++;
        }
    }
    std::cout<<"Dlinna mes1 = "<<leng1<<std::endl<<"Dlinna mes2 = "<<leng2<<std::endl;
    if (leng1>leng2)
    {
        char otvet [100] = "1 stroka > 2 stoki";
        strcpy(sendbuf,otvet);
        int dlina = strlen(otvet);
        otvet[dlina] = '\0';
    }
    if (leng1<leng2)
    {
        char otvet [100] = "1 stroka < 2 stoki";
        strcpy(sendbuf,otvet);
        int dlina = strlen(otvet);
        otvet[dlina] = '\0';
    }
    if (leng1==leng2)
    {
        char otvet [100] = "1 stroka = 2 stoke";
        strcpy(sendbuf,otvet);
        int dlina = strlen(otvet);
        otvet[dlina] = '\0';
    }
}

int nclients =0;

// прототип функции, обслуживающий подключившихся пользователей
DWORD WINAPI SexToClient(LPVOID client_socket);

void couttf(const char *string, int nclients);

int main(int argc, char* argv[])
{


    WSADATA wsaData;  //создание сервера

    /* 1. Параметр MAKEWORD(2, 2) - На современных платформах Win32 используется версия 2.2.
          Для получения значения параметра wVersionRequested можно использовать
          макрос MAKEWORD(2, 2) либо значение 0х0202.
      2.  Параметр &wsaData - указатель на структуру LPWSADATA, которая при
          вызове функции WSAStartup заполняется сведениями о версии загружаемой
          библиотеки. */

    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR)
    {
        printf("- Error WSAStartup().\n");
        return 0;
    }
    else
        printf("WSAStartup()... OK.\n");


    SOCKET LSocket;                        //создание сокета

    /*    1. Параметр AF_INET - определяет семейство адресов
          протокола и ограничивает применение второго и третьего параметров.
          Он может принимать значения AF_UNIX, AF_INET, AF_OSI, AF_NS,
          AF_NETBIOS, AF_APPLETALK, AF_ATM и т.д.. Значение AF_INET позволяет
          Взаимодействовать через объединенную сеть по IP-адресам.
          2. Параметр  SOCK_STREAM это тип сокета для данного протокола. Для протокола TCP/IP он может принимать одно из следующих значений: SOCK_STREAM  (транспортный протокол TCP, ориентированная на соединение надежная связь), SOCK_DGRAM (транспортный протокол UDP, ненадежная дейтаграммная связь), SOCK_RAW (простые сокеты).
          3. Параметр IPPROTO_TCP - указывает конкретный протокол. Если этот
          параметр равен 0, это означает, что задействуется протокол по умолчанию для выбранных значений семейства адресов и типа. Для протокола TCP задается значений  IPPROTO_IP, для протокола UDP - IPPROTO_UDP, для простых сокетов - IPPROTO_RAW или IPPROTO_ICMP. */

    LSocket=WSASocket(  AF_INET,
                        SOCK_STREAM,
                        IPPROTO_TCP,
                        0,
                        WSA_FLAG_OVERLAPPED,
                        0);

    if (LSocket == SOCKET_ERROR)
    {
        printf("- Возникла ошибка при создании сокета(): %d\n",
               WSAGetLastError());  //проверка на ошибки
        /* По завершении работы с библиотекой Winsock необходимо вызвать функцию
           WSACleanup для выгрузки библиотеки и освобождения ресурсов. */
        WSACleanup();
        getch();
        return 0;
    }
    printf("Socket()... OK\n");


    sockaddr_in serv;          //выполняем привязку сокета, инициализация
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = htonl(INADDR_ANY);
    serv.sin_port = htons(2222);


    /* 1.  Параметр LSocket - задает дескриптор локального сокета, создаваемый
           функцией socket(), на котором ожидаются соединения клиентов.
       2.  Параметр (SOCKADDR*) &serv - указатель на структуру, в которой
           хранится адрес сокета, соответствующий стандартам используемого протокола. Его нужно привести к типу struct sockaddr.В заголовочном файле Winsock определен тип SOCKADDR, соответствующий структуре struct sockaddr.
       3.  Параметр sizeof(serv)) == SOCKET_ERROR задает размер переданной
            адресной структуры, зависящий от протокола. */
    if (bind(LSocket, (SOCKADDR*) &serv, sizeof(serv)) == SOCKET_ERROR)
    {
        printf("- Error Bind().");
        /* По окончании работы с сокетом необходимо закрыть соединение и
           освободить все ресурсы, связанные с описателем сокета. */
        closesocket(LSocket);
        WSACleanup();
        getch();
        return 0;    }
    /* В случае успеха функция bind() возвращает 0.
       В случае ошибки - значение SOCKET_ERROR (т.е. -1). */
    printf("Bind()... OK.\n");


    //Для перевода сокета в состояние ожидания входящих соединений

    /* 1.  Параметр LSocket - дескриптор сокета.
       2.  Параметр 10 - максимальное число входящих запросов на
           установление соединения, которые могут ждать в очереди на
           обработку пока сервер занят. */
    if (listen(LSocket, 10) == SOCKET_ERROR)
    {
        printf("Error listening socket.\n");
        closesocket(LSocket);
        WSACleanup();
        getch();
        return 0;
    }
    else
        printf("Listen()... OK\n");


    SOCKET client_socket;        //сокет для клиента
    sockaddr_in client_addr;    // адрес клиента (заполняется системой)

    //   функции accept необходимо передать размер структуры
    int client_addr_size = sizeof(client_addr);

    printf("...Jdem Vhod9s4ego soedinenia...\n");

    // цикл извлечения запросов на подключение из очереди
    while ((client_socket = accept(LSocket, (sockaddr *)&client_addr, &client_addr_size)))
    {
        nclients++; // увеличиваем счетчик подключившихся клиентов

        // пытаемся получить имя хоста
        HOSTENT *hst;
        hst = gethostbyaddr((char *)&client_addr. sin_addr.s_addr, 4, AF_INET);

        // вывод сведений о клиенте

        std::cout <<std::endl << "********************************************" << std::endl;
        printf("+ New connect %s [%s]\n",(hst) ? hst->h_name:"", inet_ntoa(client_addr.sin_addr));


//прием данных

        /* 1.  Параметр AcceptSocket - определяет сокет для приема данных.
           2.  Параметр (char *) sendbuf - является символьным буфером и предназначен для полученных данных.
           3.  Параметр sizeof (sendbuf) - указывает число принимаемых байт или размер буфера buf.
           4.  Параметр 0 - может принимать значения 0, MSG_PEEK, MSG_OOB или результат логического ИЛИ над любыми
               из этих параметров. Разумеется, 0 означает отсутствие особых действий. Флаг MSG_PEEK указывает, что доступные данные
               должны копироваться в ринимающий буфер и при этом оставаться в системном буфере. Его
               использовать не рекомендуется.*/

        // Принимает 1ое сообщение
        strcpy(sendbuf,"");
        strcpy(sendbuf2,"");

        char choice;
        if ( (bytesSent = recv(client_socket, (char *) sendbuf, sizeof (sendbuf), 0)) == -1 )
        {
            printf("Recv error massege");
            closesocket(client_socket);
            WSACleanup();
            getch();
            return 0;
        }
        else {
            printf("\nPeredanni tekst ot clienta: ");
            for(int i = 0; i < bytesSent-1; i++)
            {
                if (sendbuf[i]=='@')
                {
                    continue;
                }
                printf("%c", sendbuf[i]);
                choice = sendbuf[bytesSent-1];
            }
            printf("\nPeredano byte: %d \n", bytesSent);
        }
        std::cout<<"choice = "<<choice<<std::endl;
        switch(choice)
        {
            case '1' :
            {
                conct();
                bytesSent = send(client_socket, sendbuf, strlen(sendbuf), 0);
                break;
            }
            case '2' :
            {
                ravenstvo(bytesSent);
                bytesSent = send(client_socket, sendbuf, strlen(sendbuf), 0);
                break;
            }
            case '3' :
            {
                ravno(bytesSent);
                bytesSent = send(client_socket, sendbuf, strlen(sendbuf), 0);
                break;
            }
            case '4' :
            {
                char indexMes[100] = "Vvedite index vstavki: ";
                bytesSent = send(client_socket, indexMes, strlen(indexMes), 0);
                char index[3];
                int bytesSent3 = recv(client_socket, (char *) index, sizeof (index), 0);

                std::cout << "Peredan index ot klienta: " << index << std::endl;
                int indexChar4;
                indexChar4 = atoi(&index[0]);
                addStringToString(indexChar4,bytesSent);
                std::cout<<"sendbuf = "<<sendbuf<<std::endl;
                bytesSent = send(client_socket, sendbuf, sizeof(sendbuf), 0);

                strcpy(indexMes,"");
                strcpy(index,"");
                bytesSent = 0;
                bytesSent3 = 0;
                indexChar4 = '\0';
                choice =     '\0';
                break;
            }
        }
        // отправляем пробел
        //cout<<"Vvedite otvet: ";
        //gets(sendbuf);
        //bytesSent = send(client_socket, sendbuf, strlen(sendbuf), 0);
        // Принимает 2ое сообщение
        // Вызов нового потока для обслужвания клиента
        // Да, для этого рекомендуется использовать _beginthreadex
        // но, поскольку никаких вызовов функций стандартной Си библиотеки
        // поток не делает, можно обойтись и CreateThread
        // Функция CreateThread создает поток, который выполняется в пределах //виртуального адресного пространства вызывающего процесса.

        DWORD thID;
        CreateThread(NULL, NULL, SexToClient, &client_socket, NULL, &thID);
    }
    return 0;
}

// Эта функция создается в отдельном потоке
// и обсуживает очередного подключившегося клиента независимо от остальных
DWORD WINAPI SexToClient(LPVOID client_socket) {
    SOCKET my_sock;
    my_sock = ((SOCKET *) client_socket)[0];
    char buff[1000];
    int nsize;

    if (nsize = recv(my_sock, (char *) buff, sizeof(buff), 0)) {
        couttf("Client [%d] disconnect\n", nclients);
//printf("%s\n", buff);
        std::cout << "********************************************" << std::endl;
    }

    nclients--; // уменьшаем счетчик активных клиентов
    return 0;
}
void couttf(const char *string, int nclients) {
}

