
#include"obsDateTime.h"
/*
void obsDateTime::set(int aMJDN, int aDS){
	pMJDN = aMJDN;
	pDaySecond = aDS;
}*/
void obsDateTime::set(int aY, int aM, int aD, int ah, int am, int as){
	pYear = aY;
	pMonth = aM;
	pDay = aD;
	pHour = ah;
	pMinute = am;
	pSecond = as;
	if((aY % 4 == 0 && aY % 100 != 0) || aY % 400 == 0){pLeapYearFlag = 1;}else{pLeapYearFlag = 0;}
}
obsDateTime::obsDateTime(int aY, int aM, int aD){
	set(aY, aM, aD, 0, 0, 0);
}
obsDateTime::obsDateTime(int aY, int aM, int aD, int ah, int am, int as){
	set(aY, aM, aD, ah, am, as);
}
obsDateTime::obsDateTime(void){
	set(0, 0, 0, 0, 0, 0);
}

void obsDateTime::settime(int ah, int am, int as){
	pHour = ah;
	pMinute = am;
	pSecond = as;
}

int obsDateTime::Year() const{
	return pYear;	
}
int obsDateTime::Month() const{
	return pMonth;
}
int obsDateTime::Date() const{
	return pDay;
}
int obsDateTime::Hour() const{
	return pHour;
}
int obsDateTime::Minute() const{
	return pMinute;
}
int obsDateTime::Second() const{
	return pSecond;
}

int obsDateTime::DOY() const{
	int doy = 0;
	switch(pMonth){
		case 12:
			doy += 30;
		case 11:
			doy += 31;
		case 10:
			doy += 30;
		case 9:
			doy += 31;
		case 8:
			doy += 31;
		case 7:
			doy += 30;
		case 6:
			doy += 31;
		case 5:
			doy += 30;
		case 4:
			doy += 31;
		case 3:
			doy += 28;
			if(pLeapYearFlag){ doy += 1; }
		case 2:
			doy += 31;
		case 1:
			doy += pDay;
	}	
	return doy;
}

int obsDateTime::DaySecond() const{
	return 60*60*pHour + 60*pMinute + pSecond;
}

