/*
 * obsDateTime.h
 *
 * 参考:
 * https://eco.mtk.nao.ac.jp/koyomi/topics/html/topics2023_1.html
 *https://eco.mtk.nao.ac.jp/koyomi/topics/html/topics2023_1.html
 *
 */

#ifndef __OBSDATETIME_H__
#define __OBSDATETIME_H__


class obsDateTime{
private:
/*
	int pMJDN; 修正ユリウス日（日数のみ） 
	int pDaySecond; 00:00:00 からの経過秒数。閏秒のせいで引き算したときにずれる可能性がある。 
*/
	int pYear;
	int pMonth;
	int pDay;
	int pHour;/* UTC */
	int pMinute;
	int pSecond;

	int pLeapYearFlag;
public:
	/* obsDateTime.cpp */
/* 	void set(int aMJDN, int aDS); */
	void set(int aY, int aM, int aD, int ah, int am, int as);
	obsDateTime(int aY, int aM, int aD);
	obsDateTime(int aY, int aM, int aD, int ah, int am, int as);
	obsDateTime(void);
	void settime(int ah, int am, int as);

	int Year() const;
	int Month() const;
	int Date() const;
	int Hour() const;
	int Minute() const;
	int Second() const;

	int DOY() const;
	int DaySecond() const;


};


#endif
