// HHmodel.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <math.h>


FILE *ap;
double v,v0;
double t, dt,tau;
double ist;//stimulate current uA
double n=0.0, m=0.0, h=1.0;
double BCL = 30; //Basic Cycle Length(msec)
double beats = 3; //Number of Stimulation
double times = BCL*beats; // Total Duration(msec)
double tstim = 0.5; // Stimulation Duration(msec)

double ki();
double nai();
double leaki();
double ion();


int _tmain(int argc, _TCHAR* argv[])
{ 
	ap = fopen("ap", "w");
	double cm = 1.0;//膜电容，单位uF cm^-2.
	v = -62;
	dt = 0.01;

	for (t = 0; t < times;)
	{
		if (t - (floor(t/ BCL)*BCL) <= tstim)
			ist = -40; // Stimulation Current(uA)
		else
		ist = 0;
	
 	v = v -(1/cm)*(ist+ion()) * dt;
	t = t + dt;
	fprintf(ap, "%f  %f\n", t, v);

	}

	fclose(ap);
	return 0;
}

double ki()
{
	double an,  bn;
	double nn;
	double taun;
	double dn;
	double iki;//单位uA。
	double gk0=36.0;//单位m mho cm^-2.
	double vk=-77.0;//单位mv。


	an = 0.01*(v + 55) / (1-exp(-v / 10 -5.5));
	bn = 0.0555*exp(-v / 80);
	taun = 1 / (an + bn);
	nn = an / (an + bn);

	dn = (nn-n)/taun;
	n = n + dn*dt;
	iki = (n*n*n*n)*gk0*(v - vk);
	return iki;
}

double nai(){
	double am, bm,ah,bh;
	double mm;
	double hh;
	double taum, tauh;
	double dm,dh;
	double inai;//单位uA。
	double gna0 = 120.0;//单位m mho cm^-2.
	double vna = 50;//单位mv。


	am = 0.1*(v + 40) / (1-exp(-v / 10 -4));
	bm = 0.108*exp(-v / 18);
	ah = 0.0027*exp(-v / 20);
	bh = 1 / (exp(-v / 10 -3.5) + 1);
	taum = 1 / (am + bm);
	tauh = 1 / (ah + bh);
	mm = am / (am + bm);
	hh = ah / (ah + bh);

	dm = (mm-m)/taum;
	m = m + dm*dt;
	dh = (hh-h)/tauh;
	h = h+ dh*dt;
	inai = m*m*m*h*gna0*(v - vna);
	return inai;
}

double leaki()
{
	double gl0 = 0.3;//单位m mho cm^-2.
	double vl = -54.387;//单位mv,取值范围在-10~613
	
	return gl0*(v - vl);
}

double ion()
{
	double it;
	it = ki() + nai() + leaki();
	return it;
}