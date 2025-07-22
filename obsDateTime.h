/*
 * obsDateTime.h
 *
 * 参考:
 * https://eco.mtk.nao.ac.jp/koyomi/topics/html/topics2023_1.html
 *https://eco.mtk.nao.ac.jp/koyomi/topics/html/topics2023_1.html
 *
 */

class obsDateTime{
private:
/*
	int pMJDN; 修正ユリウス日（日数のみ） 
	int pDaySecond; 00:00:00 からの経過秒数。閏秒のせいで引き算したときにずれる可能性がある。 
*/
	int pYear;
	int pMonth;
	int pDate;
	int pHour;/* UTC */
	int pMinute;
	int pSecond;
public:
	/* obsDateTime.cpp */
/* 	void set(int aMJDN, int aDS); */
	void set(int aY, int aM, int aD, int ah, int am, int as);

	int Year();
	int Month();
	int Date();
	int Hour();
	int Minute();
	int Second();

	int DOY();



};
