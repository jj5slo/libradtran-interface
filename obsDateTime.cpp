
#include"obsDateTime.h"
/*
void obsDateTime::set(int aMJDN, int aDS){
	pMJDN = aMJDN;
	pDaySecond = aDS;
}*/
void obsDateTime::set(int aY, int aM, int aD, int ah, int am, int as){
	pYear = aY;
	pMonth = aM;
	pDate = aD;
	pHour = ah;
	pMinute = am;
	pSecond = as;
}
obsDateTime::obsDateTime(int aY, int aM, int aD){
	set(aY, aM, aD, 0, 0, 0)
}

int obsDateTime::Year(){
	return pYear;	
}
int obsDateTime::Month(){
	return pMonth;
}
int obsDateTime::Date(){
	return pDate;
}
int obsDateTime::Hour(){
	return pHour;
}
int obsDateTime::Minute(){
	return pMinute;
}
int obsDateTime::Second(){
	return pSecond;
}

int obsDateTime::DOY(){
	return 0;/* TODO */
}

