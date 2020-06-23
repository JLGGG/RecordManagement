/*
  �ۼ���: ���ؼ�_2010111661_���̿�ȯ����а�
  �ۼ���: 2014.12.12
  �ֿ䳻��: �������α׷��� ������Ʈ
*/

#include<stdio.h>
#include<stdlib.h>//exit�Լ� �������
#include<string.h>//���ڿ� �Լ� �������
#define MAX 25//�����Ȳ,�ŷ������� ����ü �迭 ����

char fname_stock[20]="albumdata.bin";//�����Ȳ �����̸�
char fname_transaction[20]="transaction.bin";//�ŷ������� �����̸�
char sale_str[15]="�Ǹ�";
char return_str[15]="��ǰ";

//������ ��� ����
typedef enum __Menu {input=1,sale,stock_return,print,stock_exit} Menu;

typedef struct album_stock//�����Ȳ ����ü ����
{
	int stock_num;//�����Ȳ ��ȣ
	char stock_name[20];//�ٹ���
	char stock_singer[20];//���� �̸�
	int stock_year;//�⵵
	int stock_unit_price;//�ܰ�
	int stock_amount;//����
}album_stock;//�ڷ��� album_stock ����

typedef struct album_transaction//�ŷ����� ����ü ����
{
	char transaction_date[15];//�ŷ�����
	char transaction_name[20];//�ٹ���
	char transaction_singer[20];//���� �̸�
	char transaction_type[15];//�ŷ�����
	int transaction_unit_price;//�ܰ�
	int transaction_amount;//����
	int transaction_money;//�ŷ���
	int transaction_money_turnover;//�ŷ��Ѿ�
	int transaction_surtax;//�����ΰ���
}album_transaction;//�ڷ��� album_transaction ����

int stock_count=0;//�����Ȳ ������ ���� ��������
int transaction_count=0;//�ŷ����� Ƚ���� ���� ��������

int album_stock_start(album_stock stock_container[]);//�����Ȳ �ʱ�ȭ �Լ� ����
void album_stock_input(album_stock stock_container[]);//�����Ȳ �԰� �Լ� ����
void album_stock_print(album_stock container[]);//�����Ȳ ��� �Լ� ����
void album_transaction_print(album_transaction container[]);//�ŷ����� ��� �Լ� ����
void album_stock_sale(album_stock stock_container[], album_transaction transaction_container[],FILE * transaction_f);//�Ǹ� �Լ� ����
void album_stock_return(album_stock stock_container[], album_transaction transaction_container[], FILE * transaction_f);//��ǰ �Լ� ����
int album_stock_exit(album_stock stock_container[], FILE * stock_f);//���� �Լ� ����


int main(void)
{
	Menu menu;//�ڷ����� Menu�� ������ ���� ����
	FILE *stock_f;//�����Ȳ ������ ����Ű�� ���������� ��������
	FILE *transaction_f;//�ŷ����� ������ ����Ű�� ���������� ��������
	album_stock stock_container[MAX];//�����Ȳ ����ü �迭
	album_transaction transaction_container[MAX];//�ŷ����� ����ü �迭
	int func_return_number, import_selection_number, append_stock_num;
	int print_selection_num, exit_returnNum, i=0, j=0;
	
	if(fopen_s(&stock_f,fname_stock,"r+b")!=0)//�����Ȳ �а� ���� ���� ����
	{
		fprintf(stderr,"STOCK FILE OPEN ERROR\n");//����ó��
		exit(1);
	}
	
	if(fopen_s(&transaction_f,fname_transaction,"a+b") != 0)//�ŷ����� �а� �߰� ���� ����
	{
		fprintf(stderr,"TRANSACTION FILE OPEN ERROR\n");//����ó��
		exit(1);
	}

	printf("1���� �����ø� ����� ������ �ҷ��ɴϴ�>");
	scanf_s("%d",&import_selection_number);//���� �ҷ����� ���� ���
	fflush(stdin);//�Է¹��� ����

	if(import_selection_number==1)
	{
		//�������Ͽ� �ִ� �����Ȳ ������ fread �Լ��� �о���� ���� �� 1��ȯ, ����or������ �� ���� �� 0��ȯ
		while(fread(&stock_container[i],sizeof(stock_container[0]),1,stock_f)>0)
		{
			stock_count++;//���Ͽ��� album_stock ����ü �ѹ� �о�ö� ���� 1�� ����
			i++;//����� 1����
		}
		//�������Ͽ� �ִ� �ŷ����� ������ fread �Լ��� �о���� ���� �� 1��ȯ, ����or������ �� ���� �� 0��ȯ
		while(fread(&transaction_container[j],sizeof(transaction_container[0]),1,transaction_f)>0)
		{
			transaction_count++;//���Ͽ��� album_transaction ����ü �ѹ� �о�ö� ���� 1�� ����
			j++;//����� 1����
		}
	}
	
	
	printf("�����Ȳ�� �߰��ϽǷ��� 1���� ��������>");
	scanf_s("%d",&append_stock_num);//�����Ȳ �߰� ���ñ��
	fflush(stdin);//\n�� ����ش�.

	if(append_stock_num==1)
	{
		while(1)//���ѷ����� �����ؼ� ���� ���� �Է½ÿ��� ���� ����
		{
			func_return_number=album_stock_start(stock_container);//�����Ȳ �Է��Լ� ȣ��
			if(func_return_number==1)
				break;
		}
	}

	while(1)//���ѷ����ȿ� �޴��� �����߱� ������ �������� �����ؾ����� ���α׷� ����
	{
		//�޴� ���
		puts("\n���� �Ǹ� ���� ���α׷�");
		puts("1.�԰�");
		puts("2.�Ǹ�");
		puts("3.��ǰ");
		puts("4.���(�����Ȳ/�ŷ�����)");
		puts("5.����");

		printf("��ȣ�� �Է��ϰ� ENTER�� ��������> ");

		scanf_s("%d", &menu);//�޴� ����

		switch(menu)//switch ���ǿ� ������ ���� ����
		{
		case input://���̺��� ������ ����� ����
			album_stock_print(stock_container);//�����Ȳ ����Լ� ȣ��
			album_stock_input(stock_container);//�����Ȳ �Է��Լ� ȣ��
			album_stock_print(stock_container);//�����Ȳ ����Լ� ȣ��
			break;//switch�� ����
		case sale://�Ǹ� ���̺�
			album_stock_print(stock_container);
			album_stock_sale(stock_container,transaction_container,transaction_f);//�Ǹ��Լ� ȣ��
			break;
		case stock_return://��ǰ ���̺�
			album_stock_print(stock_container);
			album_stock_return(stock_container,transaction_container,transaction_f);//��ǰ�Լ� ȣ��
			break;
		case print://��� ���̺�
			puts("\n����� �׸��� �����ϼ���.");
			puts("1.��� ��Ȳ");
			puts("2.�ŷ� ����");

			printf("��ȣ�� �Է��ϰ� ENTER�� ��������>");
			
			//����ڰ� �Է��� ���� ���� ����� �׸� ����
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
			exit_returnNum=album_stock_exit(stock_container, stock_f);//�����Լ� ȣ��

			if(exit_returnNum==1)//�����Լ��� ��ȯ ���� 1�̸� ���α׷� ����
			{
				fclose(stock_f);//stock_f ���� ��Ʈ�� ����
				fclose(transaction_f);//transaction_f ���� ��Ʈ�� ����
				return 1;
			}

			}
		default://����ó��
			printf("�߸� �Է��ϼ̽��ϴ�.\n");
		}

	}

	return 0;
}
void album_stock_print(album_stock container[])//�����Ȳ ��� �Լ� ����
{
	int i=0;//�����

	printf("\n[[��� ��Ȳ]]\n");
	printf("No         �ٹ���          ����           �⵵ �ܰ� ����\n");
	printf("----------------------------------------------------------\n");

	for(i=0; i<stock_count; i++)//�����Ȳ ����ŭ ���
	{
		//fprintf�� ����ؼ� stdout���� ����� ���
		fprintf(stdout,"%d%17s%17s%11d%6d%3d\n",container[i].stock_num,container[i].stock_name,container[i].stock_singer,container[i].stock_year
			, container[i].stock_unit_price, container[i].stock_amount);
	}
	puts("");

	return;	
}

void album_transaction_print(album_transaction container[])//�ŷ����� ��� �Լ� ����
{
	int i=0;
	char function_exit_condition;//���������� �Է¹ޱ� ���� ����

	printf("\n[[ �ŷ������� ]]\n");
	printf("�ŷ�����        �ٹ���     ����      ���� �ܰ� ���� �ŷ���  �ŷ��Ѿ� �����ΰ���\n");
	printf("-------------------------------------------------------------------------------\n\n");

	for(i=0;i<transaction_count; i++)//�ŷ����� ����ŭ ���
	{
		fprintf(stdout,"%s%15s%11s%7s%6d%3d%8d%8d%8d\n",container[i].transaction_date,container[i].transaction_name,container[i].transaction_singer,
			container[i].transaction_type,container[i].transaction_unit_price,container[i].transaction_amount,container[i].transaction_money,
			container[i].transaction_money_turnover,container[i].transaction_surtax);
	}
	puts("");

	fflush(stdin);//�Է� ���� ����
	printf("\nq�� �����ø� ���� ȭ������ ���ư��ϴ�>");
	scanf_s("%c",&function_exit_condition);

	if(function_exit_condition=='q')
		return;

}
int album_stock_exit(album_stock stock_container[], FILE * stock_f)//���� �Լ� ����
{
	fflush(stock_f);//main�Լ����� �Է½�Ʈ������ ���� �о�����Ƿ� ���۸� ����ش�
	fseek(stock_f,0,SEEK_SET);//���������ڸ� ������ �������� �̵�
	//stock_container ����ü �迭 ���� �ּҺ��� stock_container ����ü ���� ������ * stock_count ��ŭ ���Ͽ� ������ ����
	fwrite(stock_container,sizeof(stock_container[0]),stock_count,stock_f);//��½�Ʈ���� �����ؼ� ���Ͽ� �����͸� �����Ѵ�.
	printf("\n�����Ȳ�� ���Ͽ� �����߽��ϴ�.\n");
	puts("���α׷��� �����մϴ�.\n");

	return 1;
}
int album_stock_start(album_stock stock_container[])//�����Ȳ �Է� �Լ� ����
{
	static int i=0;//���� i�� static������ �����ؼ� ���ÿ����� �ƴ϶� �����Ϳ����� �ø��� �׷��Ƿ� �Լ��� ����ǵ� ���� �����ȴ�.
	char function_exit_condition;

	i=stock_count;//�����Ȳ ���� i�� ����, ���� ���Ϸκ��� 3���� �Է� �޾ƿ��� �迭 �ε����� 0���� �����ϹǷ� 0,1,2�� �ȴ�.
				  //�� �������� �������� �߰��ϱ� ���ؼ� i�� �� ����
	printf("\n��� ��Ȳ�� �Է��մϴ�.\n");
	printf("�ٹ���� ������ ���ͷ� ���� �⵵, �ܰ�, ������ �����̽��� ���� �Է�\n");

	fflush(stdin);//�Է¹��� ����

	fgets(stock_container[i].stock_name,sizeof(stock_container[i].stock_name),stdin);
	//fgets�Լ��� \n���� �Է¹����Ƿ� \n�� \0�� �������ش�.
	stock_container[i].stock_name[strlen(stock_container[i].stock_name)-1]='\0';
	
	fgets(stock_container[i].stock_singer,sizeof(stock_container[i].stock_singer),stdin);
	stock_container[i].stock_singer[strlen(stock_container[i].stock_singer)-1]='\0';
	
	fscanf_s(stdin,"%d %d %d",&stock_container[i].stock_year,&stock_container[i].stock_unit_price,&stock_container[i].stock_amount);
	fflush(stdin);//fscanf�� \n�� �ԷµǾ���� ���ۿ� �ִ� �����Ͱ� ������ ����ȴ�. �ٵ� \n�� ��� ���ۿ� �����Ƿ� ���۸� ������ �Ѵ�.

	stock_container[i].stock_num=i;//�����Ȳ ��ȣ ����
	
	stock_count++;//�����Ȳ�� �߰��Ͽ����Ƿ� stock_count 1����

	printf("\nq�� �Է��Ͻø� �����Ȳ �Է��� ����˴ϴ�>");

	scanf_s("%c",&function_exit_condition);
	if(function_exit_condition=='q')
		return 1;
	else
		return 0;

}
void album_stock_input(album_stock stock_container[])//�����Ȳ �԰� �Լ� ����
{
	int i,input_amount;//����� i�� ����ڷκ��� ������ �Է¹��� ���� input_amount����
	char album_name[20];//����ڷκ��� �Է¹��� �ٹ����� �����ϱ� ���� char�� �迭

	printf("\t�԰��� �ٹ��� ������ �Է��ϼ���.\n");
	printf("\t1.�ٹ����� �Է��ϰ� ENTER�� ���缼��>");

	fflush(stdin);//�Է¹��۸� ����ش�
	fgets(album_name,sizeof(album_name),stdin);//������ ������ �ٹ����� �Է¹޴´�.
	album_name[strlen(album_name)-1]='\0';//fgets�� \n�� ���ڷ� ����ϹǷ� \n�� ã�Ƽ� \0�� �ٲ��ش�.

	for(i=0; i<stock_count; i++)//�����Ȳ ����-1 ��ŭ �ݺ��� ����
	{
		//strncmp�Լ��� �����Ȳ �ٹ���� �Է¹��� �ٹ����� strlen(album_name) ���̸�ŭ ���Ѵ�. ���̸� 0�� ��ȯ�ؼ� if�� ����
		if(strncmp(album_name,stock_container[i].stock_name,strlen(album_name))==0)
		{
			printf("\n%d�� �ٹ��� ��� �ֽ��ϴ�.\n",stock_container[i].stock_num);
			printf("%d�� �ٹ��� �԰��� ���� �Է�>",stock_container[i].stock_num);
			scanf_s("%d",&input_amount);//���� �Է�
			stock_container[i].stock_amount=stock_container[i].stock_amount+input_amount;//�԰��� �����ϰ� ���� �����ϰ� �ջ�
			printf("%d�� �ٹ��� �԰�� ������ �ջ��� ������ %d��\n",stock_container[i].stock_num,
				stock_container[i].stock_amount);
			
			return;			
		}

	}

		{
			printf("\n�ű� �԰��Դϴ�.\n");
			printf("�ٹ���� ������ ���ͷ� ���� �⵵, �ܰ�, ������ �����̽��� ���� �Է�.\n");
			
			fflush(stdin);//�Է¹��� ����

			fgets(stock_container[stock_count].stock_name,sizeof(stock_container[stock_count].stock_name),stdin);
			stock_container[stock_count].stock_name[strlen(stock_container[stock_count].stock_name)-1]='\0';
	
			fgets(stock_container[stock_count].stock_singer,sizeof(stock_container[stock_count].stock_singer),stdin);
			stock_container[stock_count].stock_singer[strlen(stock_container[stock_count].stock_singer)-1]='\0';
	
			fscanf_s(stdin,"%d %d %d",&stock_container[stock_count].stock_year,&stock_container[stock_count].stock_unit_price,&stock_container[stock_count].stock_amount);
			fflush(stdin);

			stock_container[stock_count].stock_num=stock_count;//�� �����Ȳ �������� �߰��Ͽ����Ƿ� stock_count �� ����

			stock_count++;//stock_count 1����
			
			return;	
		}
}
//�Ǹ��Լ� ����, �Ű������� album_stock ����ü �迭, album_transaction ����ü �迭, �ŷ����� ������ ����Ű�� ������ ����
void album_stock_sale(album_stock stock_container[], album_transaction transaction_container[],FILE * transaction_f)
{
	int num1, sell_num,turnover=0;
	int i,j;
	char function_exit_condition;

	printf("�Ǹ��� �ٹ� ��ȣ�� �Է��ϰ� ENTER�� ��������>");
	scanf_s("%d",&num1);//����ڰ� �ٹ� ��ȣ �Է�
	printf("�Ǹ��� �ٹ��� ������ �Է��ϰ� ENTER�� ��������>");
	scanf_s("%d",&sell_num);//����ڰ� ���� �Է�

	for(i=0; i<stock_count; i++)//�����Ȳ ����-1 ��ŭ �ݺ��� ����
	{
		if(num1==stock_container[i].stock_num)//�Է¹��� �ٹ���ȣ�� ������ �ִ� �ٹ���ȣ�� ������ if�� ����
		{
			stock_container[i].stock_amount=stock_container[i].stock_amount - sell_num;//�Է¹��� ������ ����

			if(stock_container[i].stock_amount < 0)//������ ������ ���� 0�� �����ؼ� ���� 0���� ȭ�� ���
				stock_container[i].stock_amount=0;

			printf("\n[[�Ǹ� ����]]\n");//����� ������ �Ǹž� ȭ�� ���
			printf("No         �ٹ���          ����           �⵵ �ܰ� ���� �Ǹž�\n");
			printf("-----------------------------------------------------------------\n");
			fprintf(stdout,"%d%17s%17s%11d%6d%3d%8d\n\n",stock_container[i].stock_num,stock_container[i].stock_name,stock_container[i].stock_singer,stock_container[i].stock_year
			, stock_container[i].stock_unit_price, sell_num,stock_container[i].stock_unit_price*sell_num);

			printf("�ŷ����� ���Ͽ� ���ο� �ŷ� ������ �߰��մϴ�\n");
			printf("�ŷ����ڸ� �Է����ּ���.\n");

			
			for(j=transaction_count-1; j>=0; j--)//�ŷ����� ������ ���������� ���� 1�� ���鼭 ó�� �ŷ��������� for�� ����
			{
				//������ �ŷ������� �ִ��� strncmp �Լ��� Ȯ��
				if(strncmp(stock_container[i].stock_name,transaction_container[j].transaction_name,strlen(stock_container[i].stock_name))==0)
				{
					//���� �ŷ������� �ִ� ����
					scanf_s("%s",transaction_container[transaction_count].transaction_date,sizeof(transaction_container[transaction_count].transaction_date));
					fflush(stdin);
					
					strcpy_s(transaction_container[transaction_count].transaction_type,sizeof(transaction_container[transaction_count].transaction_type),sale_str);//�Ǹ����� ����
					strcpy_s(transaction_container[transaction_count].transaction_name,sizeof(stock_container[i].stock_name),stock_container[i].stock_name);//�̸� ����
					strcpy_s(transaction_container[transaction_count].transaction_singer,sizeof(stock_container[i].stock_singer),stock_container[i].stock_singer);//���� �̸� ����
					transaction_container[transaction_count].transaction_unit_price=stock_container[i].stock_unit_price;//�����Ȳ �ܰ� �ŷ����� �ܰ��� ����
					transaction_container[transaction_count].transaction_amount=sell_num;//�Ǹŵ� ���� �����Ȳ ������ ����
					transaction_container[transaction_count].transaction_money=transaction_container[transaction_count].transaction_unit_price*sell_num;//�ű԰ŷ��� ����
					turnover+=transaction_container[j].transaction_money_turnover;//���� �ŷ��� ��������
					transaction_container[transaction_count].transaction_money_turnover=turnover+transaction_container[transaction_count].transaction_money;//���� �ŷ��װ� ���ο� �ŷ��� �ջ�
					transaction_container[transaction_count].transaction_surtax=transaction_container[transaction_count].transaction_money_turnover/10;//�����ΰ����� �ŷ��Ѿ׿��� 10�� ������
					

					fflush(transaction_f);//transaction_f �Է½�Ʈ���� ����.
					fseek(transaction_f, 0, SEEK_END);//���������ڸ� transaction_f ������ ������ �̵��Ѵ�.
					//���� �߰��� �ŷ����� ����ü ������ �ּҿ��� �ŷ����� ����ü ���� �����ŭ �ѹ��� ���Ͽ� ����
					fwrite(&transaction_container[transaction_count],sizeof(transaction_container[0]),1,transaction_f);
					puts("�ŷ� ���� ���Ͽ� �ŷ� ������ �߰� �Ǿ����ϴ�.");

					transaction_count++;//�ŷ����� Ƚ�� ����

					fflush(stdin);
					printf("\nq�� �����ø� ���� ȭ������ ���ư��ϴ�>");
					scanf_s("%c",&function_exit_condition);

					if(function_exit_condition=='q')
						return;
				}
			}

			{
				//�ű� �ŷ����� �߰� ����
				scanf_s("%s",transaction_container[transaction_count].transaction_date,sizeof(transaction_container[transaction_count].transaction_date));
				fflush(stdin);

				strcpy_s(transaction_container[transaction_count].transaction_type,sizeof(transaction_container[transaction_count].transaction_type),sale_str);
				strcpy_s(transaction_container[transaction_count].transaction_name,sizeof(stock_container[i].stock_name),stock_container[i].stock_name);
				strcpy_s(transaction_container[transaction_count].transaction_singer,sizeof(stock_container[i].stock_singer),stock_container[i].stock_singer);
				transaction_container[transaction_count].transaction_unit_price=stock_container[i].stock_unit_price;
				transaction_container[transaction_count].transaction_amount=sell_num;
				transaction_container[transaction_count].transaction_money=transaction_container[transaction_count].transaction_unit_price*sell_num;
				turnover+=transaction_container[transaction_count].transaction_money;//�ű� �ŷ������� ���� �ŷ����� �����Ƿ� ���ο� �ŷ��׸� �Ѿ׿� �ջ�
				transaction_container[transaction_count].transaction_money_turnover=turnover;
				transaction_container[transaction_count].transaction_surtax=transaction_container[transaction_count].transaction_money_turnover/10;
				

				fflush(transaction_f);
				fseek(transaction_f, 0, SEEK_END);
				fwrite(&transaction_container[transaction_count],sizeof(transaction_container[0]),1,transaction_f);
				puts("�ŷ� ���� ���Ͽ� �ŷ� ������ �߰� �Ǿ����ϴ�.");

				transaction_count++;

				fflush(stdin);
				printf("\nq�� �����ø� ���� ȭ������ ���ư��ϴ�>");
				scanf_s("%c",&function_exit_condition);

				if(function_exit_condition=='q')
					return;
			}
		}
	}
}
//��ǰ�Լ� ����, �Ű������� album_stock ����ü �迭, album_transaction ����ü �迭, �ŷ����� ������ ����Ű�� ������ ����
void album_stock_return(album_stock stock_container[], album_transaction transaction_container[],FILE * transaction_f)
{
	int i,j;
	int return_Album,return_Amount,turnover=0;
	char function_exit_condition;

	printf("��ǰ�� �ٹ� ��ȣ�� �Է��ϰ� ENTER�� ��������>");
	scanf_s("%d",&return_Album);//��ǰ�� �ٹ� ��ȣ �Է�
	printf("��ǰ�� �ٹ��� ������ �Է��ϰ� ENTER�� ��������>");
	scanf_s("%d",&return_Amount);//�ٹ� ���� �Է�

	for(i=0; i<stock_count; i++)//�����Ȳ ����-1 ��ŭ �ݺ��� ����
	{
		if(return_Album==stock_container[i].stock_num)//�Է¹��� �ٹ���ȣ�� ������ �ִ� �ٹ���ȣ�� ������ if�� ����
		{
			stock_container[i].stock_amount=stock_container[i].stock_amount + return_Amount;//�Է¹��� ������ ���� ������ ���Ѵ�.

			printf("\n[[��ǰ ����]]\n");
			printf("No         �ٹ���          ����           �⵵ �ܰ� ���� ��ȯ��\n");
			printf("----------------------------------------------------------------\n");
			fprintf(stdout,"%d%17s%17s%11d%6d%3d%8d\n\n",stock_container[i].stock_num,stock_container[i].stock_name,stock_container[i].stock_singer,stock_container[i].stock_year
			, stock_container[i].stock_unit_price, return_Amount,stock_container[i].stock_unit_price*return_Amount);

			printf("�ŷ����� ���Ͽ� ���ο� �ŷ� ������ �߰��մϴ�\n");
			printf("�ŷ����ڸ� �Է����ּ���.\n");

			for(j=transaction_count-1; j>=0; j--)//�ŷ����� ������ ���������� ���� 1�� ���鼭 ó�� �ŷ��������� for�� ����
			{
				//������ �ŷ������� �ִ��� strncmp �Լ��� Ȯ��
				if(strncmp(stock_container[i].stock_name,transaction_container[j].transaction_name,strlen(stock_container[i].stock_name))==0)
				{
					//���� �ŷ������� �ִ� ����
					scanf_s("%s",transaction_container[transaction_count].transaction_date,sizeof(transaction_container[transaction_count].transaction_date));//�ŷ����� �Է�
					fflush(stdin);
					
					strcpy_s(transaction_container[transaction_count].transaction_type,sizeof(transaction_container[transaction_count].transaction_type),return_str);
					strcpy_s(transaction_container[transaction_count].transaction_name,sizeof(stock_container[i].stock_name),stock_container[i].stock_name);
					strcpy_s(transaction_container[transaction_count].transaction_singer,sizeof(stock_container[i].stock_singer),stock_container[i].stock_singer);
					transaction_container[transaction_count].transaction_unit_price=stock_container[i].stock_unit_price;
					transaction_container[transaction_count].transaction_amount=return_Amount;//��ǰ�� �ٹ����� �ŷ����� ������ ����
					transaction_container[transaction_count].transaction_money=transaction_container[transaction_count].transaction_unit_price*return_Amount;
					turnover+=transaction_container[j].transaction_money_turnover;//���� �ŷ����� ���� turnover�� ����
					transaction_container[transaction_count].transaction_money_turnover=turnover+transaction_container[transaction_count].transaction_money;//���� �ŷ��װ� ���ο� �ŷ��� �ջ�
					transaction_container[transaction_count].transaction_surtax=transaction_container[transaction_count].transaction_money_turnover/10;//�����ΰ��� 
					
					fflush(transaction_f);
					fseek(transaction_f, 0, SEEK_END);
					fwrite(&transaction_container[transaction_count],sizeof(transaction_container[0]),1,transaction_f);
					puts("�ŷ� ���� ���Ͽ� �ŷ� ������ �߰� �Ǿ����ϴ�.");

					transaction_count++;

					fflush(stdin);
					printf("\nq�� �����ø� ���� ȭ������ ���ư��ϴ�>");
					scanf_s("%c",&function_exit_condition);

					if(function_exit_condition=='q')
						return;
				}
			}

			{
				//�ű� �ŷ����� �߰� ����
				scanf_s("%s",transaction_container[transaction_count].transaction_date,sizeof(transaction_container[transaction_count].transaction_date));
				fflush(stdin);

				strcpy_s(transaction_container[transaction_count].transaction_type,sizeof(transaction_container[transaction_count].transaction_type),return_str);
				strcpy_s(transaction_container[transaction_count].transaction_name,sizeof(stock_container[i].stock_name),stock_container[i].stock_name);
				strcpy_s(transaction_container[transaction_count].transaction_singer,sizeof(stock_container[i].stock_singer),stock_container[i].stock_singer);
				transaction_container[transaction_count].transaction_unit_price=stock_container[i].stock_unit_price;
				transaction_container[transaction_count].transaction_amount=return_Amount;
				transaction_container[transaction_count].transaction_money=transaction_container[transaction_count].transaction_unit_price*return_Amount;
				turnover+=transaction_container[transaction_count].transaction_money;//�ű� �ŷ������� ���� �ŷ����� �����Ƿ� ���ο� �ŷ��׸� �Ѿ׿� �ջ�
				transaction_container[transaction_count].transaction_money_turnover=turnover;
				transaction_container[transaction_count].transaction_surtax=transaction_container[transaction_count].transaction_money_turnover/10;
				
				fflush(transaction_f);
				fseek(transaction_f, 0, SEEK_END);
				fwrite(&transaction_container[transaction_count],sizeof(transaction_container[0]),1,transaction_f);
				puts("�ŷ� ���� ���Ͽ� �ŷ� ������ �߰� �Ǿ����ϴ�.");

				transaction_count++;

				fflush(stdin);
				printf("\nq�� �����ø� ���� ȭ������ ���ư��ϴ�>");
				scanf_s("%c",&function_exit_condition);

				if(function_exit_condition=='q')
					return;
			}	
		}
	}
}