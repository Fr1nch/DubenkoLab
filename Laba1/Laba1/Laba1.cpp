#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <format>
#include "CSs.h"
#include "Pipes.h"
#include "SubFunc.h"
#include "GTN.h"
#include "windows.h"

using namespace std;
using namespace chrono;

int Menu() {
    system("cls");
    cout << "Выберите действие" << endl;
    cout << "1.  Добавить трубу" << endl;
    cout << "2.  Добавить КС" << endl;
    cout << "3.  Добавить соединение" << endl;
    cout << "4.  Просмотр соединений" << endl;
    cout << "5.  Удаление соединений" << endl;
    cout << "6.  Топологическая сортировка" << endl;
    cout << "7.  Кратчайший путь" << endl;
    cout << "8.  Максимальный поток" << endl;
    cout << "9.  Просмотр труб" << endl;
    cout << "10. Просмотр КС" << endl;
    cout << "11. Изменить трубу" << endl;
    cout << "12. Изменить КС" << endl;
    cout << "13. Сохранить" << endl;
    cout << "14. Загрузить" << endl;
    cout << "15. Выход" << endl;
    int number = tryChoose(1, 15);
    return number;
}

int main()
{
    redirect_output_wrapper cerr_out(cerr);
    string time = format("{:%d_%m_%Y %H_%M_%OS}", system_clock::now() + hours(3));
    ofstream logfile("Logs/log_" + time + ".txt");
    if (logfile)
        cerr_out.redirect(logfile);
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    GTN gtn;
    while (true) {
        int number = Menu();
        switch (number)
        {
        case 1:
            gtn.addPipe();
            break;
        case 2:
            gtn.addCS();
            break;
        case 3:
            gtn.addConnect();
            system("pause");
            break;
        case 4:
            gtn.viewConnections();
            system("pause");
            break;
        case 5:
            gtn.editConnections();
            system("pause");
            break;
        case 6:
            gtn.topologicalSorting();
            system("pause");
            break;
        case 7:
            gtn.shortestPath();
            system("pause");
            break;
        case 8:
            gtn.maxStream();
            system("pause");
            break;
        case 9:
            gtn.ViewPipes();
            system("pause");
            break;
        case 10:
            gtn.ViewStations();
            system("pause");
            break;
        case 11:
            gtn.editPipes();
            system("pause");
            break;
        case 12:
            gtn.editCS();
            system("pause");
            break;
        case 13:
            gtn.DataOut();
            system("pause");
            break;
        case 14:
            gtn.DataIn();
            system("pause");
            break;
        case 15:
            return 0;
            break;
        default:
            break;
        }
    };
    return 0;
}