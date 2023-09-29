#include <iostream>
#include<string>
#include<windows.h>

using namespace std;

int print_menu() {
    system("cls");
    int num;
    cout << "1. Добавить трубу\n";
    cout << "2. Добавить КС\n";
    cout << "3. Просмотр всех объектов\n";
    cout << "4. Редактировать трубу\n";
    cout << "5. Редактировать КС\n";
    cout << "6. Сохранить\n";
    cout << "7. Загрузить\n";
    cout << "0. Выход\n";
    cout << "> ";
    cin >> num;
    return num;
}

struct pipe
{
    string name;
    int lenght;
    int diametr;
    bool remont;
};

struct ks 
{
    string name;
    int n_cex;
    int n_cex_v_rabote;
    int effectiv;
};

void out_pipe(pipe p)
{
    cout << "Название трубы: " << p.name << endl << "Длина трубы: " << p.lenght << endl << "Диаметр трубы: " << p.diametr << endl << "Состояние трубы: " << p.remont << endl;
}

void add_new_pipe()
{
    pipe p;
    cout << "Введите название трубы: ";
    cin >> p.name;
    cout << "Введите длину трубы: ";
    cin >> p.lenght;
    cout << "Введите диаметр трубы: ";
    cin >> p.diametr;
    cout << "Введите состояние трубы: ";
    cin >> p.remont;
    out_pipe(p);
}

int menu_num()
{
    int num = 1;
    do
    {
        int num = print_menu();

        switch (num)
        {
        case 1:
            add_new_pipe();
            break;
        case 2:
            //функция создания кс
            break;
        case 3:
            //функция просмотра всех объектов
            break;
        case 4:
            //функция редактирования трубы
            break;
        case 5:
            //функция редактирования кс
            break;
        case 6:
            //функция сохранения в файл
            break;
        case 7:
            //функция сохранения в файл
            break;
        case 0:
            exit(EXIT_SUCCESS);
            break;
        default:
            cout << "Введите пункт меню 0-7";
            break;
        }
    } while (num != 0);
    return 0;
}

int main()
{
    pipe p;
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    print_menu();
}