#include "GTN.h"
#include <queue>

using namespace std;

void GTN::CSDataOut(std::ofstream& fout)
{
    fout << Stations.size() << endl;
    for (auto it = Stations.begin(); it != Stations.end(); it++)
    {
        fout << it->first << endl;
        it->second.printCS(fout);
    }
}

void GTN::CSDataIn(std::ifstream& in)
{
    for (auto it = Stations.begin(); it != Stations.end(); it++)
    {
        Stations[-(-it->first + ID_max)] = it->second;
        Stations.erase(it);
    }

    in.ignore(10000, '\n');
    int CSCnt = 0;
    in >> CSCnt;
    for (int i = 0; i < CSCnt; i++)
    {
        int id;
        in >> id;
        CSs Station;
        Station.loadCS(in);
        Stations.insert({ id, Station });
    }

    ID_max = Stations.size() + pipes.size();
}

void GTN::PipesDataOut(std::ofstream& fout)
{
    fout << pipes.size() << endl;
    for (auto it = pipes.begin(); it != pipes.end(); it++)
    {
        fout << it->first << endl;
        it->second.printPipe(fout);
    }
}

void GTN::PipesDataIn(std::ifstream& in)
{
    for (auto it = pipes.begin(); it != pipes.end(); it++)
    {
        pipes[it->first + ID_max] = it->second;
        pipes.erase(it);
    }
    in.ignore(10000, '\n');
    int pipesCnt = 0;
    in >> pipesCnt;
    for (int i = 0; i < pipesCnt; i++)
    {
        int id;
        in >> id;
        Pipes pipe;
        pipe.loadPipe(in);
        pipes.insert({ id, pipe });
    }
}

void GTN::WaysDataOut(std::ofstream& fout)
{
    fout << connections.size() << endl;
    for (auto it = connections.begin(); it != connections.end(); it++)
    {
        fout << it->first.first << ' ' << it->first.second << ' ' << it->second << endl;
    }
}

void GTN::WaysDataIn(std::ifstream& in)
{
    in.ignore(10000, '\n');
    int waysCnt = 0;
    in >> waysCnt;
    for (int i = 0; i < waysCnt; i++)
    {
        int cs1, cs2, pipe;
        in >> cs1;
        in >> cs2;
        in >> pipe;
        connections.insert({ make_pair(cs1, cs2), pipe });
    }
}

void GTN::changeCS(const std::vector<int>& index)
{
    cout << "1. �������� ������ ��" << endl;
    cout << "2. ������� ��" << endl;
    int number = tryChoose(1, 2);
    if (number == 1)
    {
        cout << "1. ��������� ���" << endl;
        cout << "2. �������� ���";
        int inst = tryChoose(1, 2) == 1;
        cout << "�������?";
        int delta = inst == 1 ? -tryChoose(0, 999999) : tryChoose(0, 999999);
        for (int i = 0; i < index.size(); i++)
        {
            Stations[index[i]].editWS(delta);
            cout << endl;
        }
    }
    else
        for (int i = index.size() - 1; i >= 0; i--)
        {
            int t = -1;
            for (auto it = connections.begin(); it != connections.end(); it++)
            {
                if ((it->first.first == index[i] or it->first.second == index[i]) and (t == -1))
                {
                    cout << "�� ����� ������ ������� ��?" << endl;
                    cout << "1. ��" << endl;
                    cout << "2. ���" << endl;
                    Stations[index[i]].View();
                    cout << "��� ������� � ������ ��";
                    t = tryChoose(0, 1);
                    if (t == 0)
                        connections.erase(it->first);
                    else
                        break;
                }
                else if ((it->first.first == index[i] or it->first.second == index[i]) and (t == 0))
                {
                    connections.erase(it->first);
                }
            }
            if (t != 1)
                Stations.erase(index[i]);
        }
}

void GTN::changePipe(const std::vector<int>& index)
{
    cout << "1. �������� ������ �����" << endl;
    cout << "2. �������� �����" << endl;
    int number = tryChoose(1, 2);
    if (number == 1)
    {
        cout << "��������� �����:" << endl;
        cout << "\t1. � ��������� �������" << endl;
        cout << "\t2.��������" << endl;
        bool status = tryChoose(1, 2) == 1;
        for (int i = 0; i < index.size(); i++)
            pipes[index[i]].changeStatus(status);
    }
    else
        for (int i = index.size() - 1; i >= 0; i--)
        {
            pipes.erase(index[i]);
        }
}

float GTN::Dijkstra(vector<vector<int>>& a, int index_1, int index_2)
{
    int size = a.size();

    vector<vector<int>> d(size, vector<int>(size));
    vector<vector<int>> v(size, vector<int>(size));

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
        {
            d[i][j] = INF;
            v[i][j] = 0;
        }

    int temp, minindex, min;


    d[index_1][index_1] = 0;
    for (int count = 0; count < size; count++)
    {
        min = INF;
        for (int i = 0; i < size; i++)
        {
            if (v[index_1][i] == 0 && d[index_1][i] <= min)
            {
                min = d[index_1][i];
                minindex = i;
            }
        }

        v[index_1][minindex] = 1;

        for (int i = 0; i < size; i++)
        {
            if (!v[index_1][i] && a[minindex][i] != INF && d[index_1][minindex] != INF && d[index_1][minindex] + a[minindex][i] < d[index_1][i])
            {
                d[index_1][i] = d[index_1][minindex] + a[minindex][i];
            }
        }
    }

    return d[index_1][index_2];

}

float GTN::FordFulkerson(vector<vector<int>>& r_matrix, int source, int sink)
{
    int num_V = r_matrix.size();
    vector<int> parent(num_V, -1);
    int max_flow = 0;

    while (true) {

        parent.assign(num_V, -1);
        queue <pair<int, double>> q;
        q.push({ source, DBL_MAX });
        parent[source] = source;

        while (!q.empty()) {
            int u = q.front().first;
            double path_flow = q.front().second;
            q.pop();

            for (int v = 0; v < num_V; v++) {
                if (parent[v] == -1 && r_matrix[u][v] > 0) {
                    parent[v] = u;
                    int min_capacity = path_flow > r_matrix[u][v] ? r_matrix[u][v] : path_flow;
                    if (v == sink) {
                        while (v != source) {
                            u = parent[v];
                            r_matrix[u][v] -= min_capacity;
                            r_matrix[v][u] += min_capacity;
                            v = u;
                        }
                        max_flow += min_capacity;
                        break;
                    }
                    q.push({ v, min_capacity });
                }
            }
        }
        if (parent[sink] == -1)
            break;
    }
    return max_flow;
}

std::vector<int> GTN::filterCS()
{
    vector <int> index;
    string name = "";
    int workshops = -1;
    char p_way = '=';
    cout << "1. ������� ������ ��� ��" << endl;
    cout << "2. ������� ��� ��" << endl;
    if (tryChoose(1, 2) == 1)
    {
        cout << "1. ����� �� ��������" << endl;
        cout << "2. ��� ��������" << endl;
        if (tryChoose(1, 2) == 1)
        {
            cout << "��������: ";
            inputString(cin, name);
            cout << endl;
        }

        cout << "1. ����� �� ������� ������� �������" << endl;
        cout << "2. ����� ������" << endl;

        int number;
        if (tryChoose(1, 2) == 1)
        {
            cout << "�������� ������������������ �� (0..100)";
            workshops = tryChoose(0, 100);

            cout << "�������� ����� ������:" << endl
                << "1. ������, ��� ��������� ������" << endl
                << "2. ������, ��� ��������� �������" << endl
                << "3. � ����������� � ��������� ���������" << endl;
            number = tryChoose(1, 3);

            char p_way = number == 1 ? '>' : number == 2 ? '<' : '=';
        }

        for (auto it = Stations.begin(); it != Stations.end(); it++)
            if (it->second.filter(workshops, p_way, name))
                index.push_back(it->first);
    }
    else
        for (auto it = Stations.begin(); it != Stations.end(); it++)
            index.push_back(it->first);
    return index;
}

std::vector<int> GTN::filterPipes(int d, bool use)
{
    vector <int> index;
    string name = "";
    int status = -1;
    cout << "1. ������� ������ ��� �����" << endl;
    cout << "2. ������� ��� �����" << endl;
    if (tryChoose(1, 2) == 1)
    {
        cout << "1. ����� �� ��������" << endl;
        cout << "2. ��� ��������" << endl;
        if (tryChoose(1, 2) == 1)
        {
            cout << "��������: " << endl;
            inputString(cin, name);
            cout << endl;
        }

        cout << "1. ����� �� �������" << endl;
        cout << "2. ����� ������" << endl;
        if (tryChoose(1, 2) == 1)
        {
            cout << "1. ��������" << endl;
            cout << "2. � �������" << endl;
            status = tryChoose(1, 2) - 1;
        }
    }
    else
    {
        for (auto it = pipes.begin(); it != pipes.end(); it++)
            if (it->second.filter(status, name, d, use))
                index.push_back(it->first);
    }
    return index;
}

bool GTN::isExistCS()
{
    return Stations.size() > 0;
}

bool GTN::isExistPipes()
{
    return pipes.size() > 0;
}

void GTN::ViewStations()
{
    if (isExistCS())
    {
        vector <int> index = filterCS();
        cout << "�������:" << endl;
        for (int i = 0; i < index.size(); i++)
        {
            Stations[index[i]].View(index[i]);
        }
    }
}

void GTN::ViewPipes()
{
    if (isExistPipes())
    {
        vector <int> index = filterPipes();
        cout << "�����:" << endl;
        for (int i = 0; i < index.size(); i++)
            pipes[index[i]].View(index[i]);
    }
    cout << endl;
}

void GTN::addCS()
{
    int id = ++ID_max;
    CSs Station;
    Station.addCS();
    Stations.insert({ id, Station });
}

void GTN::addPipe(int d)
{
    int id = ++ID_max;
    Pipes pipe;
    pipe.addPipe(d);
    pipe.inUse = false;
    pipes.insert({ id, pipe });
}

void GTN::addConnect()
{
    while (true)
    {
        if (Stations.size() >= 2)
        {
            vector <int> index = filterCS();
            for (int i = 0; i < index.size(); i++)
            {
                cout << "������� " << i + 1 << endl;
                Stations[index[i]].View(index[i]);
            }

            int index_1;
            int index_2;

            while (1)
            {
                cout << "�������� ������ �� ��� ����������� ";
                index_1 = index[tryChoose(1, index.size()) - 1];
                cout << "�������� ������ �� ��� ����������� ";
                index_2 = index[tryChoose(1, index.size()) - 1];
                if (index_1 == index_2)
                    cout << "������ �� ������ ���� ������" << endl;
                else
                    break;
            }

            cout << "�������� �������:" << endl
                << "1. 500" << endl
                << "2. 700" << endl
                << "3. 1000" << endl
                << "4. 1400";
            int num = tryChoose(1, 4);
            int diameter = num == 1 ? 500 : num == 2 ? 700 : num == 3 ? 1000 : 1400;

            int pipe_cnt = 0;
            index.clear();

            index = filterPipes(diameter, false);

            if (index.size() == 0)
            {
                cout << "��� ���� � ����� ���������" << endl;
                cout << "������ �������� �����? " << endl
                    << "1. �������� �����" << endl
                    << "2. ��������� � ����";
                if (!(tryChoose(1, 2) - 1))
                {
                    addPipe(diameter);
                    index.push_back(ID_max);
                }
                else
                    break;
            }

            for (int i = 0; i < index.size(); i++)
            {
                cout << "����� " << i + 1 << endl;
                pipes[index[i]].View(index[i]);
            }


            cout << "������� �����";
            num = tryChoose(1, index.size()) - 1;
            int length = pipes[index[num]].getLength();
            pipes[index[num]].inUse = true;


            connections.insert({ make_pair(index_1, index_2) , index[num] });
            break;
        }
        else
        {
            cout << "������������ ��" << endl;
            cout << "������ �������� ��? " << endl
                << "1. �������� ��" << endl
                << "2. ��������� � ����";
            if (!(tryChoose(1, 2) - 1))
                addCS();
            else
                break;
        }
    }
}

void GTN::DataOut()
{
    string name = chooseFiles("Saves/");
    ofstream fout("Saves/" + name, ios_base::out | ios_base::trunc);// out - ������� ��� ������, trunc - �������� �����������, ios_base - ����� ��� ���� ��������� ������� �����-������
    if (fout.is_open())
    {
        fout << ID_max << endl;
        PipesDataOut(fout);
        CSDataOut(fout);
        WaysDataOut(fout);
        cout << "\n������� ���������" << endl;
        fout.close();
    }
    else
        cout << "������ ����������";
}

void GTN::DataIn()
{
    string name = chooseFiles("Saves/");
    ifstream fin("Saves/" + name);
    if (fin.is_open())
    {
        fin >> ID_max;
        PipesDataIn(fin);
        CSDataIn(fin);
        WaysDataIn(fin);
        fin.close();
        cout << "\n��������� �������" << endl;
    }
    else
        cout << "������ ��������" << endl;
    cout << endl;
}

void GTN::editCS()
{
    if (isExistCS()) {
        vector <int> index = filterCS();
        for (int i = 0; i < index.size(); i++)
        {
            cout << "������� " << i + 1 << endl;
            Stations[index[i]].View(index[i]);
        }
        vector <int> index_ch;
        choosingElements(index, index_ch);
        changeCS(index_ch);
    }
}

void GTN::editPipes()
{
    if (isExistPipes()) {
        vector <int> index = filterPipes();
        for (int i = 0; i < index.size(); i++)
        {
            cout << "����� " << i + 1 << endl;
            pipes[index[i]].View(index[i]);
        }
        vector <int> index_ch;
        choosingElements(index, index_ch);
        changePipe(index_ch);
    }
}

void GTN::topologicalSorting()
{
    if (connections.size() > 0)
    {
        vector <int> vert;
        int n = 0;

        for (auto it = connections.begin(); it != connections.end(); it++)
        {
            bool t1 = true;
            bool t2 = true;
            for (int i = 0; i < vert.size(); i++)
            {
                if (vert[i] == it->first.first)
                    t1 = false;
                if (vert[i] == it->first.second)
                    t2 = false;
            }

            if (t1)
                vert.push_back(it->first.first);
            if (t2)
                vert.push_back(it->first.second);
            n += int(t1) + int(t2);
        }

        vector<vector<int>> matrix(n, vector<int>(n, INF));

        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
            {
                matrix[i][j] = i == j ? 0 : connections.contains(make_pair(vert[i], vert[j])) ? connections[make_pair(vert[i], vert[j])] : INF;
            }

        if (!loopcheck(matrix))
        {
            vector <int> visited(n);
            vector <int> order;
            for (int i = 0; i < n; i++)
            {
                if (visited[i] == false)
                {
                    dfs(matrix, i, visited, order);
                    reverse(order.begin(), order.end());
                    for (int j = 0; j < order.size(); j++)
                        cout << vert[order[j]] << ' ';
                }
            }
            cout << endl;
        }
        else
        {
            cout << "��������, ���� ��������" << endl;
        }
    }
    else
    {
        cout << "����� ��� �����" << endl;
        cout << "������ �������� �����? " << endl;
        cout << "1. �������� �����" << endl;
        cout << "2. ��������� � ����" << endl;
        if (-tryChoose(1, 2) + 2)
            addConnect();
    }
}

void GTN::shortestPath()
{
    if (connections.size() != 0)
    {
        vector <int> index = filterCS();

        for (int i = 0; i < index.size(); i++)
        {
            cout << "������� " << i + 1 << endl;
            Stations[index[i]].View(index[i]);
        }

        int index_1;
        int index_2;

        while (1)
        {
            cout << "������� ������ ��";
            index_1 = index[tryChoose(1, index.size()) - 1];
            cout << "������� ������ ��";
            index_2 = index[tryChoose(1, index.size()) - 1];
            if (index_1 == index_2)
                cout << "������ �� ������ ���� ������" << endl;
            else
                break;
        }

        vector <int> vert;
        int n = 0; bool temp1 = false; bool temp2 = false; int ind_1 = -1; int ind_2 = -1;

        for (auto it = connections.begin(); it != connections.end(); it++)
        {
            bool t1 = true;
            bool t2 = true;
            for (int i = 0; i < vert.size(); i++)
            {
                if (vert[i] == it->first.first)
                    t1 = false;
                if (vert[i] == it->first.second)
                    t2 = false;
            }

            if (t1)
            {
                vert.push_back(it->first.first);

                if (it->first.first == index_1)
                {
                    temp1 = true;
                    ind_1 = vert.size() - 1;
                }
                else if (it->first.first == index_2)
                {
                    temp2 = true;
                    ind_2 = vert.size() - 1;
                }
            }
            if (t2)
            {
                vert.push_back(it->first.second);

                if (it->first.second == index_1)
                {
                    temp1 = true;
                    ind_1 = vert.size() - 1;
                }
                else if (it->first.second == index_2)
                {
                    temp2 = true;
                    ind_2 = vert.size() - 1;
                }
            }

            n += int(t1) + int(t2);
        }

        vector<vector<int>> matrix(n, vector<int>(n, INF));

        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
            {
                matrix[i][j] = i == j ? 0 : connections.contains(make_pair(vert[i], vert[j])) ? pipes[connections[make_pair(vert[i], vert[j])]].getLength() : INF;
            }

        if (temp1 and temp2)
        {
            int path = Dijkstra(matrix, ind_1, ind_2);
            if (path < INF)
                cout << "���������� ���� �� �� � ID " << index_1 << " � �� � ID " << index_2 << " = " << path << endl << endl;
            else
                cout << "��� ���� �� �� � ID " << index_1 << " �� � ID " << index_2 << endl << endl;
        }
        else
            cout << "��� ���� �� �� � ID " << index_1 << " � �� � ID " << index_2 << endl << endl;

    }
    else
    {
        cout << "����� ��� �����" << endl;
        cout << "������ �������� �����?" << endl;
        cout << "1. �������� �����" << endl;
        cout << "2. ��������� � ����" << endl;
        if (-tryChoose(1, 2) + 2)
            addConnect();
    }

}

void GTN::maxStream()
{
    if (connections.size() != 0)
    {
        vector <int> index = filterCS();

        for (int i = 0; i < index.size(); i++)
        {
            cout << "������� " << i + 1 << endl;
            Stations[index[i]].View(index[i]);
        }

        int index_1;
        int index_2;

        while (1)
        {
            cout << "������� ������. ��";
            index_1 = index[tryChoose(1, index.size()) - 1];
            cout << "������� ������ ��";
            index_2 = index[tryChoose(1, index.size()) - 1];
            if (index_1 == index_2)
                cout << "������ �� ������ ���� ������" << endl;
            else
                break;
        }

        vector <int> vert;
        int n = 0; bool temp1 = false; bool temp2 = false; int ind_1 = -1; int ind_2 = -1;

        for (auto it = connections.begin(); it != connections.end(); it++)
        {
            bool t1 = true;
            bool t2 = true;
            for (int i = 0; i < vert.size(); i++)
            {
                if (vert[i] == it->first.first)
                    t1 = false;
                if (vert[i] == it->first.second)
                    t2 = false;
            }

            if (t1)
            {
                vert.push_back(it->first.first);

                if (it->first.first == index_1)
                {
                    temp1 = true;
                    ind_1 = vert.size() - 1;
                }
                else if (it->first.first == index_2)
                {
                    temp2 = true;
                    ind_2 = vert.size() - 1;
                }
            }
            if (t2)
            {
                vert.push_back(it->first.second);

                if (it->first.second == index_1)
                {
                    temp1 = true;
                    ind_1 = vert.size() - 1;
                }
                else if (it->first.second == index_2)
                {
                    temp2 = true;
                    ind_2 = vert.size() - 1;
                }
            }

            n += int(t1) + int(t2);
        }

        vector<vector<int>> matrix(n, vector<int>(n, INF));

        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
            {
                matrix[i][j] = i == j ? 0 : connections.contains(make_pair(vert[i], vert[j])) ? pipes[connections[make_pair(vert[i], vert[j])]].getLength() : INF;
            }

        if (temp1 and temp2)
        {
            int path = FordFulkerson(matrix, ind_1, ind_2);
            if (path != 0)
                cout << "������������ �����: " << path << endl << endl;
            else
                cout << "��� ���� �� �� � ID " << index_1 << " � �� � ID " << index_2 << endl << endl;
        }
        else
            cout << "��� ���� �� �� � ID " << index_1 << " � �� � ID " << index_2 << endl << endl;
    }
    else
    {
        cout << "����� ��� �����" << endl;
        cout << "������ �������� ����� ?" << endl;
        cout << "1. �������� �����" << endl;
        cout << "2. ��������� � ����" << endl;
        if (-tryChoose(1, 2) + 2)
            addConnect();
    }
}

void GTN::viewConnections()
{
    int i = 1;
    for (auto it = connections.begin(); it != connections.end(); it++)
    {
        cout << "���������� " << i << "    ������: �� " << it->first.first << " �����d: �� " << it->first.second << " �����: " << it->second << endl;
        cout << endl;
        i++;
    }
}

void GTN::editConnections()
{
    int i = 0;
    for (auto it = connections.begin(); it != connections.end(); it++)
    {
        i++;
        cout << "���������� " << it->first.first << ' ' << it->first.second << "    ������: �� " << it->first.first << " ����� : �� " << it->first.second << " ����� : " << it->second << endl;
        cout << endl;
    }
    cout << "����� ���������� (pair): ";
    int index_1 = tryChoose(1, 99999);
    int index_2 = tryChoose(1, 99999);
    if (connections.contains({ index_1, index_2 }))
    {
        cout << "������� ����������?" << endl;
        cout << "1. ��" << endl;
        cout << "2.���" << endl;
        if (-tryChoose(1, 2) + 2)
        {
            connections.erase({ index_1, index_2 });
        }
    }
    else
        cout << "��� �� �� ���������" << endl;

}