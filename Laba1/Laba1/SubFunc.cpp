#include "SubFunc.h"

using namespace std;

void inputString(istream& in, string& str)
{
    in.ignore(10000, '\n');
    getline(in, str);
}

bool inString(string str_where, string str_what)
{
    bool state = false;
    if (str_what.size() == 0)
        state = true;
    else
        for (int i = 0; i < str_where.size() - str_what.size() + 1; i++)
        {
            int cnt = 0;
            for (int j = 0; j < str_what.size(); j++)
                if (tolower(static_cast<unsigned char>(str_where[i + j])) == tolower(static_cast<unsigned char>(str_what[j])))
                    cnt++;
            if (cnt == str_what.size())
            {
                state = true;
                break;
            }
        }
    return state;
}

float tryInputNum(float min, float max) {
    float num;
    cout << "\n> ";
    while ((cin >> num).fail() || (num < min) || (num > max)) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "\n> ";
    }
    cout << endl;
    return num;
}

int tryChoose(int min, int max) {
    int num;
    cout << "\n> ";
    while ((cin >> num).fail() || (num < min) || (num > max)) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "\n> ";
    }
    cout << endl;
    return num;
}

void enterInterval(const vector <int>& index, vector <int>& index_ch) {
    cout << "Выберите индекс первого элемента";
    int f_index = tryChoose(1, index.size());
    cout << "Выберите индекс второго элемента";
    int l_index = tryChoose(1, index.size());

    if (f_index > l_index)
    {
        int t = f_index;
        f_index = l_index;
        l_index = t;
    }

    for (int i = f_index - 1; i < l_index; i++)
        index_ch.push_back(index[i]);
}

void enterPacket(const vector <int>& index, vector <int>& index_ch) {
    cout << "Введите количсество элементов";
    int pipes_cnt = tryChoose(1, index.size());

    for (int i = 0; i < pipes_cnt; i++)
    {
        cout << "Введите индексы труб";
        int index_sel = tryChoose(1, index.size()) - 1;
        bool t = false;
        for (int j = 0; j < index_ch.size(); j++)
            t = index_sel == index_ch[j] ? true || t : false || t;
        if (!t)
            index_ch.push_back(index_sel);
    }
}

void enterElement(const vector <int>& index, vector <int>& index_ch) {
    cout << "Введите индекс Трубы";
    index_ch.push_back(tryChoose(1, index.size()) - 1);
}

void choosingElements(const vector <int>& index, vector <int>& index_ch) {
    cout << "1. Выберите интервал элементов" << endl;
    cout << "2. Выберите несколько элементов" << endl;
    cout << "3. Выберите один элемент" << endl;
    int number = tryChoose(1, 3);
    switch (number)
    {
    case 1:
        enterInterval(index, index_ch);
        break;
    case 2:
        enterPacket(index, index_ch);
        break;
    case 3:
        enterElement(index, index_ch);
        break;
    default:
        break;
    }
}

void dfs(vector<vector<int>>& graph, int v, vector<int>& visited, vector<int>& order)
{
    visited[v] = 1;

    for (int i = 0; i < graph[v].size(); i++)
        if (visited[i] != 1)
            dfs(graph, i, visited, order);

    order.push_back(v);
}

bool loopcheck(vector<vector<int>>& graph) {
    for (int i = 0; i < graph.size(); i++) {
        for (int j = 0; j < graph[1].size(); j++) {
            if ((graph[i][j] != INF && graph[j][i] != INF) and (i != j)) {
                return true;
            }
        }
    }
    return false;
}

string chooseFiles(const std::string& path)
{
    cout << "Выберите файл:" << endl;
    for (const auto& entry : filesystem::directory_iterator(path))
        cout << entry.path().filename() << endl;
    string name;
    cout << "\n> ";
    inputString(cin, name);
    return name;
}
