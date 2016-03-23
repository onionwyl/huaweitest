#include<iostream>
#include<vector>
#include<stdio.h>
#include<stdlib.h>
#include<algorithm>

using namespace std;
struct point{
	int id;
	int source;
	int destination;
	int cost;
};

struct yxdl{
	int point;
	int cc;
	int lcost;
	int path[60];
	int m;
};

int pm[601][601];

vector<point> points;
vector<yxdl> yx;
int matrix[601][601];
int v_[51];
int maxsource = 0, maxdest = 0, maxv_ = 0;
int source, destination;
int sign = 0;   //判断高斯迭代中是否发生边的替代，作为判断迭代结束的条件
char c = '_';

void out();		//输出邻接矩阵
void outpoint();	//输出路径矩阵
void GS_edge();		//高斯迭代
void iteration();	//多次进行高斯迭代
void line(int source, int dest);
void fenzhi(); //分支限界法
int lowcost(yxdl a);
bool compare(const yxdl &a, const yxdl &b);
bool checkexist(yxdl a, int n); //检查是否已经走过的点

int main()
{
	point p;
	for (int i = 0; i < 601; i++)		//初始化数组
		for (int j = 0; j < 601; j++)
		{
			if (i == j)
				matrix[i][j] = 0;
			else matrix[i][j] = 99999;
			pm[i][j] = -1;
		}
	FILE *fp = fopen("topo.csv", "r");

	while (!feof(fp))
	{
		fscanf(fp, "%d,%d,%d,%d", &p.id, &p.source, &p.destination, &p.cost);
		points.push_back(p);
	}
	fclose(fp);
	fp = fopen("demand.csv", "r");
	fscanf(fp, "%d,%d,", &source, &destination);
	while (!feof(fp))
	{
		fscanf(fp, "%d|", &v_[maxv_++]);
	}
	for (int i = 0; i < points.size(); i++)		//将读入数据赋值给邻接矩阵
	{
		if (points[i].cost < matrix[points[i].source][points[i].destination])
			matrix[points[i].source][points[i].destination] = points[i].cost;
		if (points[i].source > maxsource)
			maxsource = points[i].source;
		if (points[i].destination > maxdest)
			maxdest = points[i].destination;
	}


	iteration();
	out();
	cout<<"点阵为："<<endl;
	outpoint();
	fenzhi();

	yxdl tmp;
	tmp = yx.back();
	for (int i = 0; i < tmp.m - 1; i++)
	{
		cout << tmp.path[i] << "|";
		line(tmp.path[i], tmp.path[i + 1]);
	}
	cout << tmp.path[tmp.m - 1] << "|";
	line(tmp.path[tmp.m - 1], destination);
	cout << destination;


}

bool compare(const yxdl &a, const yxdl &b)
{
	if ((a.cc + a.lcost) > (b.cc + b.lcost))
		return true;
	else return false;
}

bool checkexist(yxdl a, int n)
{
	for (int i = 0; i < a.m; i++)
	{
		if (a.path[i] == n)
			return false;
	}
	return true;
}

void fenzhi()
{
	yxdl tmp;
	tmp.point = source;
	tmp.cc = 0;
	tmp.lcost = 99999;
	tmp.path[0] = source;
	tmp.m = 1;
	yx.push_back(tmp);

	while (1)
	{
		sort(yx.begin(), yx.end(), compare);
		for(int i=yx.size()-5;i<yx.size();i++)
	    cout<<yx[i].point<<" ";
	    cout<<endl;
	    for(int i=yx.size()-5;i<yx.size();i++)
	    cout<<yx[i].cc<<" ";
	    cout<<endl;
	    for(int i=yx.size()-5;i<yx.size();i++)
	    cout<<yx[i].lcost<<" ";
	    cout<<endl;

	    //system("pause");
		tmp = yx.back();
		if(yx.size()==0)
        {
            cout<<"没有路径"<<endl;
            break;
        }
		if (tmp.m == maxv_+1)
			break;
		yx.erase(yx.end()-1);
		for (int i = 0; i < maxv_; i++)
		{
			if (matrix[tmp.point][v_[i]] <99999 && checkexist(tmp, v_[i]))
			{
				yxdl tmpp;
				tmpp.point = v_[i];
				tmpp.cc = tmp.cc + matrix[tmp.point][v_[i]];
				tmpp.lcost = lowcost(tmp) ;
				for (int j = 0; j < tmp.m; j++)
					tmpp.path[j] = tmp.path[j];
				tmpp.path[tmp.m] = v_[i];
				tmpp.m = tmp.m + 1;
				yx.push_back(tmpp);
			}
		}
	}
	cout << endl;
	tmp = yx.back();
	cout << tmp.cc << endl;
	for (int i = 0; i < tmp.m; i++)
		cout << tmp.path[i] << "|";
	cout << endl;
}

int lowcost(yxdl a)
{
	int lcost = 0;
	int tmpcost;
	for (int i = 0; i < maxv_; i++)
	{   if(checkexist( a,v_[i])){

		tmpcost = 99999;
		for (int j = 0; j < maxv_; j++)
		{
			if (matrix[v_[i]][v_[j]] < tmpcost&&i != j)
				tmpcost = matrix[v_[i]][v_[j]];
		}
		lcost = lcost + tmpcost;
		}
	}
	return lcost;
}

void line(int source, int dest)
{
	if (pm[source][dest] == -1)
		return;
	line(source, pm[source][dest]);
	cout << pm[source][dest] << "|";
	line(pm[source][dest], dest);
}


void GS_edge(){
	sign = 0;      //邻接矩阵高斯迭代（一次）
	int mid;
	for (int i = 0; i < maxsource; i++)
		for (int j = 0; j < maxdest; j++)
		{
			for (int k = 0; k < maxsource; k++)
			{
				if ((matrix[i][k] < matrix[i][j]) && (matrix[k][j] < matrix[i][j]))
				{
					mid = matrix[i][k] + matrix[k][j];
					if (matrix[i][j] > mid)
					{
						matrix[i][j] = mid;
						pm[i][j] = k;
						sign = 1;
					}
				}
			}
		}
}

void out(){
	cout << "   ";
	for (int i = 0; i <= maxdest; i++)
		if (i < 10)
			cout << " " << i << " ";
		else cout << i << " ";
		cout << endl;
		for (int i = 0; i <= maxsource; i++)
		{
			if (i < 10)
				cout << " " << i << " ";
			else cout << i << " ";
			for (int j = 0; j <= maxdest; j++)
				if (matrix[i][j] == 99999)
					cout << " " << c << " ";
				else if (matrix[i][j] >= 10)
					cout << matrix[i][j] << " ";
				else cout << " " << matrix[i][j] << " ";
				cout << endl;
		}
		cout << "=====================================================================" << endl;
}

void outpoint()
{
	cout << "   ";
	for (int i = 0; i <= maxdest; i++)
		if (i < 10)
			cout << " " << i << " ";
		else cout << i << " ";
		cout << endl;
		for (int i = 0; i <= maxsource; i++)
		{
			if (i < 10)
				cout << " " << i << " ";
			else cout << i << " ";
			for (int j = 0; j <= maxdest; j++)
				if (pm[i][j] == -1)
					cout << " " << c << " ";
				else if (pm[i][j] >= 10)
					cout << pm[i][j] << " ";
				else cout << " " << pm[i][j] << " ";
				cout << endl;
		}
}

void iteration(){     //多次进行高斯迭代
	GS_edge();
	while (sign == 1)
	{
		GS_edge();
	}
}
