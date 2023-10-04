#include <iostream>
#include<string>
#include<windows.h>
#include<fstream>

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
    string name = "";
    int lenght;
    int diametr;
    bool repair;
};

struct ks 
{
    string name = "";
    int n_workshop;
    int n_workshop_v_rabote;
    int effectiv;
};

bool check_repair()
{
    bool repair;
    while (true)
    {
        cout << "Введите состояние трубы ('0' - труба работает '1' - труба находится в ремонте): ";
        cin >> repair;
        if (!cin)
        {
            cout << "Некоректный ввыод. Введите '0' или '1'\n";
            cin.clear();
            while (cin.get() != '\n');
        }
        else break;
    }
    return repair;
}

int check_in()
{
    int a;
    while (true)
    {
        cin >> a;
        if (!cin || a < 0)
        {
            cout << "Некоректный ввод. Введите целое положительное число\n";
            cin.clear();
            while (cin.get() != '\n');
        }
        else break;
    }
    return a;
}

void out_pipe(pipe p)
{
    if (p.name != "")
    {
        cout << "Название трубы: " << p.name << endl 
        << "Длина трубы: " << p.lenght << endl 
        << "Диаметр трубы: " << p.diametr << endl 
        << "Состояние трубы: " << p.repair << endl;
    }
}

void out_ks(ks k)
{
    if (k.name != "")
    {
        cout << "Название КС: " << k.name << endl 
        << "Количество цехов на КС: " << k.n_workshop << endl 
        << "Количество цехов в работе на КС: " << k.n_workshop_v_rabote << endl 
        << "Эффективность КС: " << k.effectiv << endl;
    }
}

void all_out(pipe p, ks k)
{
    out_pipe(p);
    out_ks(k);
}

pipe edit_pipe(pipe p)
{
    cout << "Введите новое состояние трубы: " ;
    p.repair = check_repair();
    out_pipe(p);
    return p;
}

ks edit_ks(ks k)
{
    if (k.name != "");
    {
        cout << "Введите новое количество цехов в работе: ";
        k.n_workshop_v_rabote = check_in();
        while (true)
        {
            if (k.n_workshop_v_rabote > k.n_workshop)
            {
                cout << "Некоректный ввод. Количество цехов в работе не может превышать общее количество цехов" << endl
                    << "Введите количество цехов в работе на КС: ";
                k.n_workshop_v_rabote = check_in();
            }
            else
            {
                break;
            };
        }
        out_ks(k);
        return k;
    }
}

pipe add_new_pipe()
{
    pipe p;
    cout << "Введите название трубы: ";
    cin >> ws;
    getline(cin, p.name);
    cout << "Введите длину трубы: ";
    p.lenght = check_in();
    cout << "Введите диаметр трубы: ";
    p.diametr = check_in();
    cout << "Введите состояние трубы: ";
    p.repair = check_repair();
    return p;
}

ks add_new_ks()
{
    ks k;
    cout << "Введите название КС: ";
    cin >> ws;
    getline(cin, k.name);
    cout << "Введите количество цехов на КС: ";
    k.n_workshop = check_in();
    cout << "Введите количество цехов в работе на КС: ";
    k.n_workshop_v_rabote = check_in();
    while (true)
    {
        if (k.n_workshop_v_rabote > k.n_workshop)
        {
            cout << "Некоректный ввод. Количество цехов в работе не может превышать общее количество цехов" << endl 
                << "Введите количество цехов в работе на КС: ";
            k.n_workshop_v_rabote = check_in();
        }
        else
        {
            break;
        };
    }
    cout << "Введите эффективность КС: ";
    k.effectiv = check_in();
    return k;
}

void output_in_file(pipe p, ks k)
{
    ofstream out;
    out.open("output.txt");
    if (out.is_open())
    {
        if (p.name != "")
        {
            out << "Pipe" << endl;
            out << p.name << endl;
            out << p.lenght << endl;
            out << p.diametr << endl;
            out << p.repair << endl;
        }
        if (k.name != "")
        {
            out << "KS" << endl;
            out << k.name << endl;
            out << k.n_workshop << endl;
            out << k.n_workshop_v_rabote << endl;
            out << k.effectiv << endl;
        }
    }
    out.close();
    cout << "Запись в файл закончена\n";
}

pipe input_file_pipe(pipe p)
{
    string label;
    ifstream in;
    in.open("output.txt");
    if (in.is_open())
    {
        getline(in, label);
        while (!label.empty())
        {
            if (label == "Pipe")
            {
                getline(in, p.name);
                in >> p.lenght;
                in >> p.diametr;
                in >> p.repair;
            }
            getline(in, label);
        }
    }
    in.close();
    return p;
}

ks input_file_ks(ks k)
{
    string label;
    ifstream in;
    in.open("output.txt");
    if (in.is_open())
    {
        getline(in, label);
        while (!label.empty())
        {
            if (label == "KS")
            {
                getline(in, k.name);
                in >> k.n_workshop;
                in >> k.n_workshop_v_rabote;
                in >> k.effectiv;
            }
            getline(in, label);
        }
    }
    in.close();
    return k;
}

int main()
{
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    pipe p;
    ks k;
    int num = 1;
    do
    {
        int num = print_menu();

        switch (num)
        {
        case 1:
            p = add_new_pipe();
            break;
        case 2:
            k = add_new_ks();
            break;
        case 3:
            all_out(p, k);
            break;
        case 4:
            if (p.name != "")
            {
                edit_pipe(p);
            }
            else
            {
                cout << "Труба еще не создана\n";
            }
            break;
        case 5:
            if (k.name != "")
            {
                edit_ks(k);
            }
            else
            {
                cout << "КС еще не создана\n";
            }
            break;
        case 6:
            output_in_file(p, k);
            break;
        case 7:
            p = input_file_pipe(p);
            k = input_file_ks(k);
            cout << "Чтение завершено\n";
            break;
        case 0:
            exit(EXIT_SUCCESS);
            break;
        default:
            cout << "Введите пункт меню 0-7\n";
            break;
        }
        if (num != 0)
            system("pause");
    } while (num != 0);
}