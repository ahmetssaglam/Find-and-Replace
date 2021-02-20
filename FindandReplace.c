/* 
@file
BLM2512 2019-2020 BAHAR ODEV-3
Bu programda Boyer-Moore Horspool Algoritmasý kullanýlarak bir Find and Replace uygulamasý tasarlanmýþtýr.

@author
Ýsim: Ahmet Said SAÐLAM
Öðrenci No: 17011501
Tarih: 24.04.2020
E-Mail: l1117501@std.yildiz.edu.tr
Compiler: TDM-GCC 4.9.2 64 bit-Release
IDE: DEV-C++ (version 5.11)
Ýþletim Sistemi: Windows 10 Pro 64 bit
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#define MAXCHAR 4000 //Hafizaya alinip ustunde arama ve degistirme yapilabilecek maksimum karakter sayisi.	
#define ASCII 256	//ascii karakter sayisi

//bad match table olusturan fonksiyon
int *createBadMatch(char *search, int *bad_match) {
	int i, j;
	int search_size = strlen(search);	//bad-match table'da bulunan maksimum deger / stringde aranan kelimenin uzunlugu
	int bad_line ;			//bad-match table olusturulurken kullanilan degisken
	
	//bad match table'daki tum karakter indislerine, standart olarak aranan kelimenin uzunlugunu atayan dongu
	for(i = 0; i < ASCII; i++){
		bad_match[i] = search_size;
	}
	
	//bad match table'da zaten aranan kelimedeki karakterlerin kaydirma sayilarini duzenleyen dongu
	for(i = 0; i < search_size-1; i++){
		j = (int) search[i];
		bad_line = search_size - 1 -i;
		bad_match[j] = bad_line;
	}
	return bad_match;
}

//horspool fonksiyonu case-sensitive durumda, text'te istenen pattern'i arar ve buldugu yerde pattern'i hedef ifade ile degistirir.
int horspool(char *text, char *pat, char *insert, int *badMatch) {
	int eslesme_sayac = 0;		//eger pattern stringin icinde aranirken (karakter karakter karsilastirilirken) bir kac eslesmeden sonra bozulma olursa bad match table'daki degerleri eslesme sayisina gore duzenleyecek olan degisken
	int temp, k, kaydir;
	int count = 0;						//pattern'e kac defa rastlandigini tutan degisken
	int patLength = strlen(pat);		//pattern'in uzunlugu
	int insertLength = strlen(insert);	//pattern'in yerine yazilacak ifadenin uzunlugu
	int fark, x;
	int i = strlen(pat) - 1;	//i degiskeni  pattern'in son karakterini gosterecek sekilde ayarlanir.Boylece textte aramaya hangi karakterden baslanacagi belirlenmis olur.
	int j = strlen(pat) - 1;	//j degiskeni  pattern'in son karakterini gosterecek sekilde ayarlanir.
	fark = patLength - insertLength;
	
	//text'in tum karakterlerini okumaya yardimci olan while dongusu
	while(i < strlen(text)) {
		temp = i;
		//karakter eslesmesi durumunda bir sonraki karakteri kontrol ettiren ve eslesme sayaci tutan, pattern uzunlugu kadar donen while dongusu
		while(text[temp] == pat[j] && j >= 0) {
			temp--;
			j--;
			eslesme_sayac++;
		}
		if(j >= 0) {		// eger pattern bulunamadiysa girilen if kontrolu
			k = text[temp];
			kaydir = badMatch[k];
			kaydir = kaydir - eslesme_sayac;
			if(kaydir <= 0){
				i++;			//kaydirma degiskeni 0 veya daha kucuk bir deger alirsa text'te  bir adim ileri gidilir.
			}
			else{				//badmatch table'a gore text'te kaydirma yapilir
				i = i+kaydir;
			}
			j = strlen(pat) - 1;	//j degiskeni yeniden pattern'in son karakterini gosterecek sekilde ayarlanir.
			eslesme_sayac = 0;		//eslesme sayaci 0'lanir.
		}
		else {			//pattern bulunduysa girilen else kolu.
			temp++;		//temp textte bulunan pattern'in ilk karakterinin indisini gosterecek sekilde ayarlanir.
			if(fark == 0) {		//pattern ile yerine konacak ifade esit uzunluktaysa degistirme islemini ona gore yapan kontrol
			
				//yeni ifadeyi ilgili gozlere yerlestiren dongu			
				for(x = 0; x < insertLength; x++) {
					text[temp] = insert[x];
					temp++;
				}
				//printf("0Yeni string: %s\n",text);
			}
			else if(fark > 0) {	//pattern'in uzunlugu yerine konacak ifadenin uzunlugundan fazla ise degistirme islemini ona gore yapan kontrol
				
				//yeni ifadeyi ilgili gozlere yerlestiren dongu
				for(x = 0; x < insertLength; x++){
					text[temp] = insert[x];
					temp++;
				}
				
				//yeni ifadeden sonra arta kalan fark kadar diziyi geri kaydiran dongu
				for(x = temp; x < strlen(text) - fark; x++){
					text[x] = text[x+fark];
				}
				
				//sondaki gozlere fark kadar null atanir.
				x = fark;
				while(x != 0){
					text[strlen(text) - 1] = 0;
					x--;
				}
				//printf("+Yeni string: %s\n",text);
			}
			else{		//pattern'in uzunlugu yerine konacak ifadenin uzunlugundan az ise degistirme islemini ona gore yapan kontrol
				x = strlen(text) - 1;
				//text'i pattern'den itibaren fark kadar ileri kaydiran dongu
				while(x >= temp) {
					text[x - fark] = text[x];
					x--;
				}
				//yeni ifadeyi ilgili gozlere yerlestiren dongu
				for(x = 0; x < insertLength; x++) {
					text[temp] = insert[x];
					temp++;
				}
				//printf("-Yeni string: %s\n",text);
			}
			count++;				//pattern bulundugu icin count 1 arttirilir.
			j = strlen(pat) - 1;	//j degiskeni yeniden pattern'in son karakterini gosterecek sekilde ayarlanir.
			i++;					//text'te bir sonraki karakterle arama devam edebilsin diye i degiskeni 1 arttirilir.
			eslesme_sayac = 0;		//eslesme sayaci 0'lanir.
		}
	}
	return count;					//count degeri disariya dondurulur ve fonksiyondan cikilir.
}

//caseHorspool fonksiyonu case-insensitive durumda, lower text'te, lower pattern'i arar ve buldugu yerlerin indislerini disari dondurur.
int *caseHorspool(char *text, char *pat, int *badMatch, int *location) {
	int eslesme_sayac = 0;		//eger lower pattern stringin icinde aranirken (karakter karakter karsilastirilirken) bir kac eslesmeden sonra bozulma olursa bad match table'daki degerleri eslesme sayisina gore duzenleyecek olan degisken
	int temp, k, kaydir;
	int count = 0;
	int patLength = strlen(pat);
	int i = strlen(pat) - 1;
	int j = strlen(pat) - 1;
	int x = 1;		//x degiskeni location dizisinin ikinci elemanini gosterecek sekilde ayarlanir.
	
	//lower text'in tum karakterlerini okumaya yardimci olan while dongusu
	while(i < strlen(text)) {
		temp = i;
		while(text[temp] == pat[j] && j >= 0) {
			temp--;
			j--;
			eslesme_sayac++;
		}
		if(j >= 0) {		// eger lower pattern bulunamadiysa girilen if kontrolu
			k = text[temp];
			kaydir = badMatch[k];
			kaydir = kaydir - eslesme_sayac;
			if(kaydir <= 0) {
				i++;			//kaydirma degiskeni 0 veya daha kucuk bir deger alirsa dizide bir adim ileri gidilir.
			}
			else {
				i = i+kaydir;
			}
			j = strlen(pat) - 1;
			eslesme_sayac = 0;
		}
		else {			//pattern bulunduysa girilen else kolu.
			temp++;
			location[x] = temp;		//location dizisinin ilgili gozune, lower text'te bulunan lower pattern'in text'te ki yerini gosteren indis atanir.
			x++;					//x degiskeni, location dizisinin bir sonraki gozunu gosterecek sekilde ayarlanir.
			count++;				//pattern bulundugu icin count 1 arttirilir.
			location[0] = count;	//location dizisinin 0. yani ilk gozune count degeri atilir.
			j = strlen(pat) - 1;
			i++;
			eslesme_sayac = 0;
		}
	}
	return location;		//location dizisi disariya dondurulur ve fonksiyondan cikilir.Boylece count degeri ve ilgili indisler programa verilmis olur.
}

//swap fonksiyonu case-insensitive durumda main'de cagirilir ve fonksiyona verilen text ve text'in ilgili gozundeki pattern'i degistirir.
//horsepool fonksiyonundaki degistirme blogunun yaptigi isin aynisini yapar.
void swap(int temp, int fark, int insertLength, char *text, char *pat, char *insert) {
	int x;
	if(fark == 0) {
		for(x = 0; x < insertLength; x++) {
			text[temp] = insert[x];
			temp++;
		}
		//printf("0Yeni string: %s\n",text);
	}
	else if(fark > 0) {
	
		//yeni ifadeyi ilgili gozlere yerlestiren dongu
		for(x = 0; x < insertLength; x++){
			text[temp] = insert[x];
			temp++;
		}
				
		//yeni ifadeden sonra arta kalan fark kadar diziyi geri kaydiran dongu
		for(x = temp; x < strlen(text) - fark; x++){
			text[x] = text[x+fark];
		}
				
		//sondaki gozlere fark kadar null atanir.
		x = fark;
		while(x != 0){
			text[strlen(text) - 1] = 0;
			x--;
		}
		//printf("+Yeni string: %s\n",text);
	}
	else{
		x = strlen(text) - 1;
		while(x >= temp) {
			text[x - fark] = text[x];
			x--;
		}
		for(x = 0; x < insertLength; x++) {
			text[temp] = insert[x];
			temp++;
		}
		//printf("-Yeni string: %s\n",text);
	}
}


//gets() fonksiyonu kullanýlmadan önce eger scanf kullanýldýysa gets() duzgun calismayacagi icin input buffer'i temizlemmek icin gerekli fonksiyon
int clear_input_buffer(void) {
    int ch;
    while (((ch = getchar()) != EOF) && (ch != '\n'));
    return ch;
}


int main(){
	FILE *fp;						//file pointer
	int i, j, z;					//dongulerde ve kontrollerde kullanilacak degiskenler
	int case_sensitive;				//case senstive durumunu kontrol eden degisken
	int bad_match[ASCII];			//tum ascii karakterlere gore deger tutan Bad-Match Table
	int *badMatch;					//Bad-Match Table'ý iþaret eden pointer
	int count;	   					//pattern'in kac kere bulunup (degistirildigini) tutan degisken.
	int *location;					//lokasyon dizisini isaret eden pointer
	int lokasyon[MAXCHAR];			//case insensitive durumunda count degerini ve text karakter dizisi uzerinde degisiklik yapilacak indis numaralarini tutan dizi
	char text[MAXCHAR];				//dosyadan alinan, kelimenin icinde aranacagi stringi tutan karakter dizisi
	char lowerText[MAXCHAR]; 		//dosyadan alinan text'in butun karakterlerini sirasiyla lowercase halde tutan karakter dizisi
	char filename[MAXCHAR];			//dosya ismini tutan karakter dizisi
	char search[MAXCHAR];			//hangi kelimenin aranacagini tutan karakter dizisi
	char lowerSearch[MAXCHAR];		//kullanicidan alinan pattern'in butun karakterlerini sirasiyla lowercase halde tutan karakter dizisi
	char insert[MAXCHAR];			//text'in icine uygun durumda hangi ifadenin yazilacagini tutan karakter dizisi
	int patLength;					//pattern'in uzunlugu
	int insertLength;				//pattern'in yerine eklenecek ifadenin uzunlugu.
	int fark,size;					//fark = pattern ve yeni ifadeinin uzunluk farki, size = dosyadan alinacak fadenin uzunlugu
	clock_t start, end;				//sure baslangic ve bitisi
	double time_spent;				//toplam gecen sure(ms)
	
	lokasyon[0] = 0;		//case-insensitive durumda count degeri ilk an icin 0 olarak ayarlanir.
	//kullanilacak diziler ilk durum icin 0'lanir.
	for(i = 0; i < MAXCHAR; i++){
		text[i] = 0;
		search[i] = 0;
		lowerText[i] = 0;
		lowerSearch[i] = 0;
		insert[i] = 0;
	}
	
	printf("Aramayi Case Insensitive (Buyuk-Kucuk harf duyarsiz) olarak yapmak isterseniz 1'e,\nCase Sensitive (Buyuk-Kucuk harf duyarli) yapmak isterseniz baska bir tusa basiniz.\n ");
	scanf("%d",&case_sensitive);
	printf("Aramak istediginiz ifadeyi giriniz.\n");
	clear_input_buffer();
	gets(search);
	printf("Yerine yazmak istediginiz ifadeyi giriniz.\n");
	gets(insert);
	patLength = strlen(search);
	insertLength = strlen(insert);
	fark = patLength - insertLength;

	printf("Lutfen acmak istediginiz dosyanin ismini uzantisiyla birlikte giriniz.\n");
	scanf("%s", filename);
	if((fp = fopen(filename,"r")) == NULL) {
		printf("Dosya okunmak icin acilamadi!\n");
		return 0;
	}
	else {
		if(case_sensitive == 1) {	//arama case-insensitive mi kontrol eden if statement
		
			//aranacak kelimenin tum karakterleri kucuk karakter olacak sekilde kelimeyi yeniden duzenleyen dongu
			for(i = 0; i < strlen(search); i++){
				lowerSearch[i] = tolower(search[i]);
			}
			badMatch = createBadMatch(lowerSearch,bad_match);   //badmatch table olusturulur
			fseek(fp, 0, SEEK_END); // seek to end of file
			size = ftell(fp); // get current file pointer
			fseek(fp, 0, SEEK_SET); // seek back to beginning of file
			fread(text,size,1,fp); //dosyadaki text text dizisine alinir.
			printf("Text: %s\n",text);
			
			//dosyadan alinan text'in tum karakterleri kucuk karakter olacak sekilde text'i duzenleyen dongu
			for(i = 0; i < strlen(text); i++){
				lowerText[i] = tolower(text[i]);
			}						
			//printf("Lower Text: %s\n",lowerText);
			//printf("Lower pattern: %s\n",lowerSearch);
				
			start = clock(); //sure hesaplama baslangici
			location = caseHorspool(lowerText,lowerSearch,badMatch,lokasyon); //pattern text'te aranir, count ve bulunan patternlerin yeri dondurulur.					
			if(location[0] == 0) {		//count 0 ise text'te aranan pattern mevcut degildir.
				printf("Eslesme yok!\n");
				return 0;
			}
			else {
				j = 0; 	//count-control
				i = 1;	//location dizisinin ilgili gozune erisen indis numarasi.Boylece text'te hangi indisde degisim yapilacagi ogrenilir.
				
				//count kadar donerek swap islemi yaptiran while dongusu
				while (j < location[0]) {
					if(j == 0){
						swap(location[i],fark,insertLength,text,search,insert);
						j++;
						i++;
					}
					else {
						//patternlerin baslangic indislerini guncelleyen for dongusu
						for(z = i; z <= location[0]; z++){
							location[z] = location[z] - fark;	//ilk degisimden sonra text dizisinde diger bulunan patternlerin yeri fark kadar kayacaktir. Bu islem sayesinde diger patternlerin yerleri dogru bir sekilde guncellenir.
						}
						swap(location[i],fark,insertLength,text,search,insert);
						j++;
						i++;
					}
				}
				end = clock(); //sure hesaplama bitisi.
				time_spent = (double) (end - start);
				printf("Son Text: %s\nDegisim sayisi: %d\nGecen zaman: %f ms\n",text,location[0],time_spent);
			}			
			fclose(fp); //dosyayi okumak icin acildigindan kapatir.
			
			//dosyayi yazmak icin yeniden acar ve yeni text'i icine yazar.
			if((fp = fopen(filename,"w")) == NULL) {
				printf("Dosya yazmak icin acilamadi!\n");
				return 0;
			}
			else {
				fputs(text,fp);
				fclose(fp);
				printf("Dosya Icerigi Guncellendi!\n");
			}	
		}
		else {	//case sensitive durum.
			badMatch = createBadMatch(search,bad_match); //bad-match table olusturulur
			fseek(fp, 0, SEEK_END); // seek to end of file
			size = ftell(fp); // get current file pointer
			fseek(fp, 0, SEEK_SET); // seek back to beginning of file
			fread(text,size,1,fp);
			printf("Text: %s\n",text);
			start = clock();	//sure hesaplama baslangici
			count = horspool(text, search, insert, badMatch);	//text'te pattern bulunduysa ilgili degisiklik yapilir ve count degeri disari donulur.
			if(count == 0){
				printf("Eslesme yok!\n");
				return 0;
			}
			end = clock();	//sure hesaplama bitisi
			time_spent = (double) (end - start);
			//printf("Count: %d\n",count);
			printf("Son Text: %s\nDegisim sayisi: %d\nGecen zaman: %f ms\n",text,count,time_spent);
			fclose(fp);	//dosyayi okumak icin acildigindan kapatir.
			
			//dosyayi yeniden acar ve yeni text'i icine yazar.
			if((fp = fopen(filename,"w")) == NULL) {
				printf("Dosya yazmak icin acilamadi!\n");
				return 0;
			}
			else {
				fputs(text,fp);
				fclose(fp);
				printf("Dosya Icerigi Guncellendi!\n");
			}	
		}
	}
	return 0;
}


