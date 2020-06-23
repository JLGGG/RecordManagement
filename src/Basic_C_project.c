/*
  작성자: 임준수_2010111661_바이오환경과학과
  작성일: 2014.12.12
  주요내용: 기초프로그래밍 프로젝트
*/

#include<stdio.h>
#include<stdlib.h>//exit함수 헤더파일
#include<string.h>//문자열 함수 헤더파일
#define MAX 25//재고현황,거래내역서 구조체 배열 길이

char fname_stock[20]="albumdata.bin";//재고현황 파일이름
char fname_transaction[20]="transaction.bin";//거래내역서 파일이름
char sale_str[15]="판매";
char return_str[15]="반품";

//열거형 상수 구현
typedef enum __Menu {input=1,sale,stock_return,print,stock_exit} Menu;

typedef struct album_stock//재고현황 구조체 정의
{
	int stock_num;//재고현황 번호
	char stock_name[20];//앨범명
	char stock_singer[20];//가수 이름
	int stock_year;//년도
	int stock_unit_price;//단가
	int stock_amount;//수량
}album_stock;//자료형 album_stock 정의

typedef struct album_transaction//거래내역 구조체 정의
{
	char transaction_date[15];//거래일자
	char transaction_name[20];//앨범명
	char transaction_singer[20];//가수 이름
	char transaction_type[15];//거래유형
	int transaction_unit_price;//단가
	int transaction_amount;//수량
	int transaction_money;//거래액
	int transaction_money_turnover;//거래총액
	int transaction_surtax;//누적부가세
}album_transaction;//자료형 album_transaction 정의

int stock_count=0;//재고현황 개수를 세는 전역변수
int transaction_count=0;//거래내역 횟수를 세는 전역변수

int album_stock_start(album_stock stock_container[]);//재고현황 초기화 함수 선언
void album_stock_input(album_stock stock_container[]);//재고현황 입고 함수 선언
void album_stock_print(album_stock container[]);//재고현황 출력 함수 선언
void album_transaction_print(album_transaction container[]);//거래내역 출력 함수 선언
void album_stock_sale(album_stock stock_container[], album_transaction transaction_container[],FILE * transaction_f);//판매 함수 선언
void album_stock_return(album_stock stock_container[], album_transaction transaction_container[], FILE * transaction_f);//반품 함수 선언
int album_stock_exit(album_stock stock_container[], FILE * stock_f);//종료 함수 선언


int main(void)
{
	Menu menu;//자료형이 Menu인 열거형 변수 선언
	FILE *stock_f;//재고현황 파일을 가리키는 파일포인터 변수선언
	FILE *transaction_f;//거래내역 파일을 가리키는 파일포인터 변수선언
	album_stock stock_container[MAX];//재고현황 구조체 배열
	album_transaction transaction_container[MAX];//거래내역 구조체 배열
	int func_return_number, import_selection_number, append_stock_num;
	int print_selection_num, exit_returnNum, i=0, j=0;
	
	if(fopen_s(&stock_f,fname_stock,"r+b")!=0)//재고현황 읽고 쓰기 모드로 오픈
	{
		fprintf(stderr,"STOCK FILE OPEN ERROR\n");//에러처리
		exit(1);
	}
	
	if(fopen_s(&transaction_f,fname_transaction,"a+b") != 0)//거래내역 읽고 추가 모드로 오픈
	{
		fprintf(stderr,"TRANSACTION FILE OPEN ERROR\n");//에러처리
		exit(1);
	}

	printf("1번을 누르시면 저장된 파일을 불러옵니다>");
	scanf_s("%d",&import_selection_number);//파일 불러오기 선택 기능
	fflush(stdin);//입력버퍼 비우기

	if(import_selection_number==1)
	{
		//이진파일에 있는 재고현황 데이터 fread 함수로 읽어오기 성공 시 1반환, 실패or파일의 끝 도달 시 0반환
		while(fread(&stock_container[i],sizeof(stock_container[0]),1,stock_f)>0)
		{
			stock_count++;//파일에서 album_stock 구조체 한번 읽어올때 마다 1씩 증가
			i++;//제어변수 1증가
		}
		//이진파일에 있는 거래내역 데이터 fread 함수로 읽어오기 성공 시 1반환, 실패or파일의 끝 도달 시 0반환
		while(fread(&transaction_container[j],sizeof(transaction_container[0]),1,transaction_f)>0)
		{
			transaction_count++;//파일에서 album_transaction 구조체 한번 읽어올때 마다 1씩 증가
			j++;//제어변수 1증가
		}
	}
	
	
	printf("재고현황을 추가하실려면 1번을 누르세요>");
	scanf_s("%d",&append_stock_num);//재고현황 추가 선택기능
	fflush(stdin);//\n을 비워준다.

	if(append_stock_num==1)
	{
		while(1)//무한루프로 구성해서 종료 조건 입력시에만 루프 종료
		{
			func_return_number=album_stock_start(stock_container);//재고현황 입력함수 호출
			if(func_return_number==1)
				break;
		}
	}

	while(1)//무한루프안에 메뉴를 삽입했기 때문에 종료기능을 선택해야지만 프로그램 종료
	{
		//메뉴 출력
		puts("\n음반 판매 관리 프로그램");
		puts("1.입고");
		puts("2.판매");
		puts("3.반품");
		puts("4.출력(재고현황/거래내역)");
		puts("5.종료");

		printf("번호를 입력하고 ENTER를 누르세요> ");

		scanf_s("%d", &menu);//메뉴 선택

		switch(menu)//switch 조건에 열거형 변수 삽입
		{
		case input://레이블을 열거형 상수로 구현
			album_stock_print(stock_container);//재고현황 출력함수 호출
			album_stock_input(stock_container);//재고현황 입력함수 호출
			album_stock_print(stock_container);//재고현황 출력함수 호출
			break;//switch문 종료
		case sale://판매 레이블
			album_stock_print(stock_container);
			album_stock_sale(stock_container,transaction_container,transaction_f);//판매함수 호출
			break;
		case stock_return://반품 레이블
			album_stock_print(stock_container);
			album_stock_return(stock_container,transaction_container,transaction_f);//반품함수 호출
			break;
		case print://출력 레이블
			puts("\n출력할 항목을 선택하세요.");
			puts("1.재고 현황");
			puts("2.거래 내역");

			printf("번호를 입력하고 ENTER를 누르세요>");
			
			//사용자가 입력한 값에 따라 출력할 항목 선택
			scanf_s("%d", &print_selection_num);
			if(print_selection_num==1)
			{
				album_stock_print(stock_container);
				break;
			}
			else if(print_selection_num==2)
			{
				album_transaction_print(transaction_container);
				break;
			}
		case stock_exit:
			{
			exit_returnNum=album_stock_exit(stock_container, stock_f);//종료함수 호출

			if(exit_returnNum==1)//종료함수의 반환 값이 1이면 프로그램 종료
			{
				fclose(stock_f);//stock_f 파일 스트림 종료
				fclose(transaction_f);//transaction_f 파일 스트림 종료
				return 1;
			}

			}
		default://예외처리
			printf("잘못 입력하셨습니다.\n");
		}

	}

	return 0;
}
void album_stock_print(album_stock container[])//재고현황 출력 함수 정의
{
	int i=0;//제어변수

	printf("\n[[재고 현황]]\n");
	printf("No         앨범명          가수           년도 단가 수량\n");
	printf("----------------------------------------------------------\n");

	for(i=0; i<stock_count; i++)//재고현황 수만큼 출력
	{
		//fprintf를 사용해서 stdout으로 모니터 출력
		fprintf(stdout,"%d%17s%17s%11d%6d%3d\n",container[i].stock_num,container[i].stock_name,container[i].stock_singer,container[i].stock_year
			, container[i].stock_unit_price, container[i].stock_amount);
	}
	puts("");

	return;	
}

void album_transaction_print(album_transaction container[])//거래내역 출력 함수 정의
{
	int i=0;
	char function_exit_condition;//종료조건을 입력받기 위한 변수

	printf("\n[[ 거래내역서 ]]\n");
	printf("거래일자        앨범명     가수      유형 단가 수량 거래액  거래총액 누적부가세\n");
	printf("-------------------------------------------------------------------------------\n\n");

	for(i=0;i<transaction_count; i++)//거래내역 수만큼 출력
	{
		fprintf(stdout,"%s%15s%11s%7s%6d%3d%8d%8d%8d\n",container[i].transaction_date,container[i].transaction_name,container[i].transaction_singer,
			container[i].transaction_type,container[i].transaction_unit_price,container[i].transaction_amount,container[i].transaction_money,
			container[i].transaction_money_turnover,container[i].transaction_surtax);
	}
	puts("");

	fflush(stdin);//입력 버퍼 비우기
	printf("\nq를 누르시면 메인 화면으로 돌아갑니다>");
	scanf_s("%c",&function_exit_condition);

	if(function_exit_condition=='q')
		return;

}
int album_stock_exit(album_stock stock_container[], FILE * stock_f)//종료 함수 정의
{
	fflush(stock_f);//main함수에서 입력스트림으로 값을 읽어왔으므로 버퍼를 비워준다
	fseek(stock_f,0,SEEK_SET);//파일지시자를 파일의 시작으로 이동
	//stock_container 구조체 배열 시작 주소부터 stock_container 구조체 변수 사이즈 * stock_count 만큼 파일에 데이터 저장
	fwrite(stock_container,sizeof(stock_container[0]),stock_count,stock_f);//출력스트림을 형성해서 파일에 데이터를 저장한다.
	printf("\n재고현황을 파일에 저장했습니다.\n");
	puts("프로그램을 종료합니다.\n");

	return 1;
}
int album_stock_start(album_stock stock_container[])//재고현황 입력 함수 정의
{
	static int i=0;//변수 i는 static변수로 선언해서 스택영역이 아니라 데이터영역에 올린다 그러므로 함수가 종료되도 값이 유지된다.
	char function_exit_condition;

	i=stock_count;//재고현황 개수 i에 대입, 만약 파일로부터 3개를 입력 받아오면 배열 인덱스는 0부터 시작하므로 0,1,2가 된다.
				  //현 데이터의 마지막에 추가하기 위해서 i에 값 대입
	printf("\n재고 현황을 입력합니다.\n");
	printf("앨범명과 가수는 엔터로 구분 년도, 단가, 수량은 스페이스로 구분 입력\n");

	fflush(stdin);//입력버퍼 비우기

	fgets(stock_container[i].stock_name,sizeof(stock_container[i].stock_name),stdin);
	//fgets함수는 \n까지 입력받으므로 \n을 \0로 변경해준다.
	stock_container[i].stock_name[strlen(stock_container[i].stock_name)-1]='\0';
	
	fgets(stock_container[i].stock_singer,sizeof(stock_container[i].stock_singer),stdin);
	stock_container[i].stock_singer[strlen(stock_container[i].stock_singer)-1]='\0';
	
	fscanf_s(stdin,"%d %d %d",&stock_container[i].stock_year,&stock_container[i].stock_unit_price,&stock_container[i].stock_amount);
	fflush(stdin);//fscanf는 \n이 입력되어야지 버퍼에 있던 데이터가 변수로 저장된다. 근데 \n은 계속 버퍼에 있으므로 버퍼를 비워줘야 한다.

	stock_container[i].stock_num=i;//재고현황 번호 지정
	
	stock_count++;//재고현황을 추가하였으므로 stock_count 1증가

	printf("\nq를 입력하시면 재고현황 입력이 종료됩니다>");

	scanf_s("%c",&function_exit_condition);
	if(function_exit_condition=='q')
		return 1;
	else
		return 0;

}
void album_stock_input(album_stock stock_container[])//재고현황 입고 함수 정의
{
	int i,input_amount;//제어변수 i와 사용자로부터 수량을 입력받을 변수 input_amount선언
	char album_name[20];//사용자로부터 입력받은 앨범명을 저장하기 위한 char형 배열

	printf("\t입고할 앨범의 정보를 입력하세요.\n");
	printf("\t1.앨범명을 입력하고 ENTER를 누루세요>");

	fflush(stdin);//입력버퍼를 비워준다
	fgets(album_name,sizeof(album_name),stdin);//공백을 포함한 앨범명을 입력받는다.
	album_name[strlen(album_name)-1]='\0';//fgets는 \n도 문자로 취급하므로 \n을 찾아서 \0로 바꿔준다.

	for(i=0; i<stock_count; i++)//재고현황 개수-1 만큼 반복문 실행
	{
		//strncmp함수로 재고현황 앨범명과 입력받은 앨범명을 strlen(album_name) 길이만큼 비교한다. 참이면 0을 반환해서 if문 실행
		if(strncmp(album_name,stock_container[i].stock_name,strlen(album_name))==0)
		{
			printf("\n%d번 앨범은 재고가 있습니다.\n",stock_container[i].stock_num);
			printf("%d번 앨범에 입고할 수량 입력>",stock_container[i].stock_num);
			scanf_s("%d",&input_amount);//수량 입력
			stock_container[i].stock_amount=stock_container[i].stock_amount+input_amount;//입고할 수량하고 기존 수량하고 합산
			printf("%d번 앨범의 입고된 수량과 합산한 재고수량 %d개\n",stock_container[i].stock_num,
				stock_container[i].stock_amount);
			
			return;			
		}

	}

		{
			printf("\n신규 입고입니다.\n");
			printf("앨범명과 가수는 엔터로 구분 년도, 단가, 수량은 스페이스로 구분 입력.\n");
			
			fflush(stdin);//입력버퍼 비우기

			fgets(stock_container[stock_count].stock_name,sizeof(stock_container[stock_count].stock_name),stdin);
			stock_container[stock_count].stock_name[strlen(stock_container[stock_count].stock_name)-1]='\0';
	
			fgets(stock_container[stock_count].stock_singer,sizeof(stock_container[stock_count].stock_singer),stdin);
			stock_container[stock_count].stock_singer[strlen(stock_container[stock_count].stock_singer)-1]='\0';
	
			fscanf_s(stdin,"%d %d %d",&stock_container[stock_count].stock_year,&stock_container[stock_count].stock_unit_price,&stock_container[stock_count].stock_amount);
			fflush(stdin);

			stock_container[stock_count].stock_num=stock_count;//현 재고현황 마지막에 추가하였으므로 stock_count 값 대입

			stock_count++;//stock_count 1증가
			
			return;	
		}
}
//판매함수 정의, 매개변수는 album_stock 구조체 배열, album_transaction 구조체 배열, 거래내역 파일을 가리키는 포인터 변수
void album_stock_sale(album_stock stock_container[], album_transaction transaction_container[],FILE * transaction_f)
{
	int num1, sell_num,turnover=0;
	int i,j;
	char function_exit_condition;

	printf("판매할 앨범 번호를 입력하고 ENTER를 누르세요>");
	scanf_s("%d",&num1);//사용자가 앨범 번호 입력
	printf("판매할 앨범의 수량을 입력하고 ENTER를 누르세요>");
	scanf_s("%d",&sell_num);//사용자가 수량 입력

	for(i=0; i<stock_count; i++)//재고현황 개수-1 만큼 반복문 실행
	{
		if(num1==stock_container[i].stock_num)//입력받은 앨범번호와 기존에 있던 앨범번호가 같으면 if문 실행
		{
			stock_container[i].stock_amount=stock_container[i].stock_amount - sell_num;//입력받은 수량을 뺀다

			if(stock_container[i].stock_amount < 0)//수량이 음수로 가면 0을 대입해서 수량 0으로 화면 출력
				stock_container[i].stock_amount=0;

			printf("\n[[판매 내역]]\n");//변경된 수량과 판매액 화면 출력
			printf("No         앨범명          가수           년도 단가 수량 판매액\n");
			printf("-----------------------------------------------------------------\n");
			fprintf(stdout,"%d%17s%17s%11d%6d%3d%8d\n\n",stock_container[i].stock_num,stock_container[i].stock_name,stock_container[i].stock_singer,stock_container[i].stock_year
			, stock_container[i].stock_unit_price, sell_num,stock_container[i].stock_unit_price*sell_num);

			printf("거래내역 파일에 새로운 거래 내역을 추가합니다\n");
			printf("거래일자를 입력해주세요.\n");

			
			for(j=transaction_count-1; j>=0; j--)//거래내역 데이터 마지막에서 부터 1씩 빼면서 처음 거래내역까지 for문 실행
			{
				//기존의 거래내역이 있는지 strncmp 함수로 확인
				if(strncmp(stock_container[i].stock_name,transaction_container[j].transaction_name,strlen(stock_container[i].stock_name))==0)
				{
					//기존 거래내역이 있는 영역
					scanf_s("%s",transaction_container[transaction_count].transaction_date,sizeof(transaction_container[transaction_count].transaction_date));
					fflush(stdin);
					
					strcpy_s(transaction_container[transaction_count].transaction_type,sizeof(transaction_container[transaction_count].transaction_type),sale_str);//판매유형 복사
					strcpy_s(transaction_container[transaction_count].transaction_name,sizeof(stock_container[i].stock_name),stock_container[i].stock_name);//이름 복사
					strcpy_s(transaction_container[transaction_count].transaction_singer,sizeof(stock_container[i].stock_singer),stock_container[i].stock_singer);//가수 이름 복사
					transaction_container[transaction_count].transaction_unit_price=stock_container[i].stock_unit_price;//재고현황 단가 거래내역 단가에 대입
					transaction_container[transaction_count].transaction_amount=sell_num;//판매된 수량 재고현황 수량에 대입
					transaction_container[transaction_count].transaction_money=transaction_container[transaction_count].transaction_unit_price*sell_num;//신규거래액 대입
					turnover+=transaction_container[j].transaction_money_turnover;//기존 거래액 가져오기
					transaction_container[transaction_count].transaction_money_turnover=turnover+transaction_container[transaction_count].transaction_money;//기존 거래액과 새로운 거래액 합산
					transaction_container[transaction_count].transaction_surtax=transaction_container[transaction_count].transaction_money_turnover/10;//누적부가세는 거래총액에서 10을 나눈다
					

					fflush(transaction_f);//transaction_f 입력스트림을 비운다.
					fseek(transaction_f, 0, SEEK_END);//파일지시자를 transaction_f 파일의 끝으로 이동한다.
					//현재 추가된 거래내역 구조체 변수의 주소에서 거래내역 구조체 변수 사이즈만큼 한번만 파일에 저장
					fwrite(&transaction_container[transaction_count],sizeof(transaction_container[0]),1,transaction_f);
					puts("거래 내역 파일에 거래 내역이 추가 되었습니다.");

					transaction_count++;//거래내역 횟수 증가

					fflush(stdin);
					printf("\nq를 누르시면 메인 화면으로 돌아갑니다>");
					scanf_s("%c",&function_exit_condition);

					if(function_exit_condition=='q')
						return;
				}
			}

			{
				//신규 거래내역 추가 영역
				scanf_s("%s",transaction_container[transaction_count].transaction_date,sizeof(transaction_container[transaction_count].transaction_date));
				fflush(stdin);

				strcpy_s(transaction_container[transaction_count].transaction_type,sizeof(transaction_container[transaction_count].transaction_type),sale_str);
				strcpy_s(transaction_container[transaction_count].transaction_name,sizeof(stock_container[i].stock_name),stock_container[i].stock_name);
				strcpy_s(transaction_container[transaction_count].transaction_singer,sizeof(stock_container[i].stock_singer),stock_container[i].stock_singer);
				transaction_container[transaction_count].transaction_unit_price=stock_container[i].stock_unit_price;
				transaction_container[transaction_count].transaction_amount=sell_num;
				transaction_container[transaction_count].transaction_money=transaction_container[transaction_count].transaction_unit_price*sell_num;
				turnover+=transaction_container[transaction_count].transaction_money;//신규 거래내역은 기존 거래액이 없으므로 새로운 거래액만 총액에 합산
				transaction_container[transaction_count].transaction_money_turnover=turnover;
				transaction_container[transaction_count].transaction_surtax=transaction_container[transaction_count].transaction_money_turnover/10;
				

				fflush(transaction_f);
				fseek(transaction_f, 0, SEEK_END);
				fwrite(&transaction_container[transaction_count],sizeof(transaction_container[0]),1,transaction_f);
				puts("거래 내역 파일에 거래 내역이 추가 되었습니다.");

				transaction_count++;

				fflush(stdin);
				printf("\nq를 누르시면 메인 화면으로 돌아갑니다>");
				scanf_s("%c",&function_exit_condition);

				if(function_exit_condition=='q')
					return;
			}
		}
	}
}
//반품함수 정의, 매개변수는 album_stock 구조체 배열, album_transaction 구조체 배열, 거래내역 파일을 가리키는 포인터 변수
void album_stock_return(album_stock stock_container[], album_transaction transaction_container[],FILE * transaction_f)
{
	int i,j;
	int return_Album,return_Amount,turnover=0;
	char function_exit_condition;

	printf("반품할 앨범 번호를 입력하고 ENTER를 누르세요>");
	scanf_s("%d",&return_Album);//반품할 앨범 번호 입력
	printf("반품할 앨범의 수량을 입력하고 ENTER를 누르세요>");
	scanf_s("%d",&return_Amount);//앨범 수량 입력

	for(i=0; i<stock_count; i++)//재고현황 개수-1 만큼 반복문 실행
	{
		if(return_Album==stock_container[i].stock_num)//입력받은 앨범번호와 기존에 있던 앨범번호가 같으면 if문 실행
		{
			stock_container[i].stock_amount=stock_container[i].stock_amount + return_Amount;//입력받은 수량을 기존 수량에 더한다.

			printf("\n[[반품 내역]]\n");
			printf("No         앨범명          가수           년도 단가 수량 반환액\n");
			printf("----------------------------------------------------------------\n");
			fprintf(stdout,"%d%17s%17s%11d%6d%3d%8d\n\n",stock_container[i].stock_num,stock_container[i].stock_name,stock_container[i].stock_singer,stock_container[i].stock_year
			, stock_container[i].stock_unit_price, return_Amount,stock_container[i].stock_unit_price*return_Amount);

			printf("거래내역 파일에 새로운 거래 내역을 추가합니다\n");
			printf("거래일자를 입력해주세요.\n");

			for(j=transaction_count-1; j>=0; j--)//거래내역 데이터 마지막에서 부터 1씩 빼면서 처음 거래내역까지 for문 실행
			{
				//기존의 거래내역이 있는지 strncmp 함수로 확인
				if(strncmp(stock_container[i].stock_name,transaction_container[j].transaction_name,strlen(stock_container[i].stock_name))==0)
				{
					//기존 거래내역이 있는 영역
					scanf_s("%s",transaction_container[transaction_count].transaction_date,sizeof(transaction_container[transaction_count].transaction_date));//거래일자 입력
					fflush(stdin);
					
					strcpy_s(transaction_container[transaction_count].transaction_type,sizeof(transaction_container[transaction_count].transaction_type),return_str);
					strcpy_s(transaction_container[transaction_count].transaction_name,sizeof(stock_container[i].stock_name),stock_container[i].stock_name);
					strcpy_s(transaction_container[transaction_count].transaction_singer,sizeof(stock_container[i].stock_singer),stock_container[i].stock_singer);
					transaction_container[transaction_count].transaction_unit_price=stock_container[i].stock_unit_price;
					transaction_container[transaction_count].transaction_amount=return_Amount;//반품한 앨범수량 거래내역 수량의 대입
					transaction_container[transaction_count].transaction_money=transaction_container[transaction_count].transaction_unit_price*return_Amount;
					turnover+=transaction_container[j].transaction_money_turnover;//기존 거래액을 변수 turnover에 저장
					transaction_container[transaction_count].transaction_money_turnover=turnover+transaction_container[transaction_count].transaction_money;//기존 거래액과 새로운 거래액 합산
					transaction_container[transaction_count].transaction_surtax=transaction_container[transaction_count].transaction_money_turnover/10;//누적부가세 
					
					fflush(transaction_f);
					fseek(transaction_f, 0, SEEK_END);
					fwrite(&transaction_container[transaction_count],sizeof(transaction_container[0]),1,transaction_f);
					puts("거래 내역 파일에 거래 내역이 추가 되었습니다.");

					transaction_count++;

					fflush(stdin);
					printf("\nq를 누르시면 메인 화면으로 돌아갑니다>");
					scanf_s("%c",&function_exit_condition);

					if(function_exit_condition=='q')
						return;
				}
			}

			{
				//신규 거래내역 추가 영역
				scanf_s("%s",transaction_container[transaction_count].transaction_date,sizeof(transaction_container[transaction_count].transaction_date));
				fflush(stdin);

				strcpy_s(transaction_container[transaction_count].transaction_type,sizeof(transaction_container[transaction_count].transaction_type),return_str);
				strcpy_s(transaction_container[transaction_count].transaction_name,sizeof(stock_container[i].stock_name),stock_container[i].stock_name);
				strcpy_s(transaction_container[transaction_count].transaction_singer,sizeof(stock_container[i].stock_singer),stock_container[i].stock_singer);
				transaction_container[transaction_count].transaction_unit_price=stock_container[i].stock_unit_price;
				transaction_container[transaction_count].transaction_amount=return_Amount;
				transaction_container[transaction_count].transaction_money=transaction_container[transaction_count].transaction_unit_price*return_Amount;
				turnover+=transaction_container[transaction_count].transaction_money;//신규 거래내역은 기존 거래액이 없으므로 새로운 거래액만 총액에 합산
				transaction_container[transaction_count].transaction_money_turnover=turnover;
				transaction_container[transaction_count].transaction_surtax=transaction_container[transaction_count].transaction_money_turnover/10;
				
				fflush(transaction_f);
				fseek(transaction_f, 0, SEEK_END);
				fwrite(&transaction_container[transaction_count],sizeof(transaction_container[0]),1,transaction_f);
				puts("거래 내역 파일에 거래 내역이 추가 되었습니다.");

				transaction_count++;

				fflush(stdin);
				printf("\nq를 누르시면 메인 화면으로 돌아갑니다>");
				scanf_s("%c",&function_exit_condition);

				if(function_exit_condition=='q')
					return;
			}	
		}
	}
}