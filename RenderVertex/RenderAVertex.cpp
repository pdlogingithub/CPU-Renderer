#include"stdafx.h"
#include <iostream>
#include <math.h>
#define PI 3.141592653
using namespace std;

class vec2int
{
public:
	int x;
	int y;
	vec2int operator+(vec2int v)
	{
		return {x+v.x,y+v.y};
	}
	vec2int operator/(int i)
	{
		return {x/i,y/i};
	}
};
class vec2
{
public:
	float x;
	float y;
	vec2 operator+(vec2 v)
	{
		return {x+v.x,y+v.y};
	}
	vec2 operator/(float f)
	{
		return {x/f,y/f};
	}
};
class vec3
{
public:
	float x;
	float y;
	float z;
	vec3 operator+(vec3 v)
	{
		return {x+v.x,y+v.y,z+v.z};
	}
	vec3 operator-()
	{
		return {-x,-y,-z};
	}
	vec3 rotate(vec3 trans)
	{
		vec3 ret;
		ret.x=x;
		ret.y=y*cos(trans.x)+z*sin(trans.x);
		ret.z=z*cos(trans.x)-y*sin(trans.x);
		x=ret.x*cos(trans.y)-ret.z*sin(trans.y);
		y=ret.y;
		z=ret.x*sin(trans.y)+ret.z*cos(trans.y);
		ret.x=x*cos(trans.z)+y*sin(trans.z);
		ret.y=y*cos(trans.z)-x*sin(trans.z);
		ret.z=z;
		return ret;
	}
};
class Mesh
{
public:
	vec3* LocalSpaceVertexBuffer;//ԭʼ�Ķ�������
	vec3* CameraSpaceVertexBuffer;//���任���Ķ�������
	int VertexCount;//������
	void Translate(vec3* from,vec3* to,vec3 trans)//����ƽ��
	{
		vec3* f=from;
		vec3* t=to;
		for(int i=0;i<VertexCount;i++)
		{
			*(t+i)=*(f+i)+trans;
		}
	}
	void Rotate(vec3* from,vec3* to,vec3 trans)//������ת
	{
		vec3* f=from;
		vec3* t=to;
		for(int i=0;i<VertexCount;i++)
		{
			*(t+i)=(f+i)->rotate(trans);
		}
	}
};
class RenderingPipeLine//��Ⱦ���߶���
{
public:
	vec3 CameraPosition;//�����λ��
	vec3 CameraRotation;//�������ת������
	vec2 CameraSize;//�������С��Ŀǰֻ��һ�������α�ʾ�������������Խ���ܿ����Ķ����Խ��
	vec2int Resolution;//�ֱ���
	bool* ViewPort=0;//�ӿڿ�����һ����ά���飬���������أ��������鳤�Ȳ��ܶ�̬������ֱ��ʸı���ı䣬������������Щ
	Mesh m;
	Mesh* mesh=&m;

	void SetCameraPosition(vec3 pos)
	{
		CameraPosition=pos;
	}
	void SetCameraRotation(vec3 rot)
	{
		CameraRotation=rot;
	}
	void SetCameraSize(vec2 cam)
	{
		CameraSize=cam;
	}
	void SetResolution(vec2int res)
	{
		Resolution=res;
		if (ViewPort) { delete ViewPort; }
		ViewPort = new bool[res.x*res.y]{0};//�ֱ��ʸı�ʱ�������µĵȴ�С�ӿ�
	}
	void SetMesh(vec3* res1,vec3* res2,int vertexcount)//��ʱ�������ö������ݣ���������ģ���ļ�����
	{
		mesh->LocalSpaceVertexBuffer=res1;
		mesh->CameraSpaceVertexBuffer=res2;
		mesh->VertexCount=vertexcount;
	}

	void WorldToCamera()//�������ǰһ���ߣ��Ͱ����������һ����
	{
		mesh->Rotate(mesh->LocalSpaceVertexBuffer,mesh->CameraSpaceVertexBuffer,-CameraRotation);
		mesh->Translate(mesh->CameraSpaceVertexBuffer,mesh->CameraSpaceVertexBuffer,-CameraPosition);
	}
	void DrawPrimitive()//�任��������ռ�Ķ���������������С��Χ�ڣ������ӿ����1
	{
		vec2 camh=CameraSize/2;
		vec3* ver=mesh->CameraSpaceVertexBuffer;	
		for(int i=0;i<mesh->VertexCount;i++)
		{
			if (abs((ver + i)->x)>camh.x || abs((ver + i)->y)>camh.y) {continue;}
			int w=((ver+i)->x/CameraSize.x+0.5)*Resolution.x+0.5;
			int h=((ver+i)->y/CameraSize.y+0.5)*Resolution.y+0.5;
			*(ViewPort+h*Resolution.x+w)=true;
		}	
	}
	void Render()//���ڵ���Ⱦ����ֻ������
	{
		WorldToCamera();
		DrawPrimitive();
	}
};

RenderingPipeLine* CreatePipeLine()
{
	RenderingPipeLine* RPL=new RenderingPipeLine;
	return RPL;
}
void InitilizePipeLine(RenderingPipeLine* RPL)
{
	RPL->SetCameraPosition({0,0,0});
	RPL->SetCameraRotation({(float)-PI/2,0,0});
	RPL->SetCameraSize({50,25});
	RPL->SetResolution({20,10});
	vec3* res1 = new vec3{2,2,4};//����һ��λ��2,2,4�Ķ���
	vec3* res2 = new vec3{2,2,4};
	RPL->SetMesh(res1,res2,1);
}
void Render(RenderingPipeLine* RPL)
{
	RPL->Render();
}
void Present(RenderingPipeLine* RPL)
{
	for(int h=RPL->Resolution.y-1;h>=0;h--)
	{
		for(int w=0;w<RPL->Resolution.x;w++)
		{
			bool* pixel=RPL->ViewPort+h*RPL->Resolution.x+w;
			cout<<*pixel;
		}
		cout<<endl;
	}
}
void DestroyPipeLine(RenderingPipeLine* RPL)
{
	delete RPL;
}
int main()
{
	RenderingPipeLine* RPL=CreatePipeLine();
	InitilizePipeLine(RPL);	
	Render(RPL);
	Present(RPL);
	DestroyPipeLine(RPL);
	while (1) {}
}