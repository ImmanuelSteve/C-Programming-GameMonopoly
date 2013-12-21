#include<stdio.h>
#include<conio.h>
#include <windows.h>
#include<time.h>
#include<string.h>

int jmlh,min;

typedef struct arena
{
   int kode;
   int urutan;
   arena *next;
   arena *prev;
   char nama[20];
   int harga;
   int punya;
   int level;
};

arena *head;
arena *tail;
arena *baru;
arena *bantu;
arena *bantu2;

struct kartu
{
       int head;
       int tail;
       int kode[35];
       char nama[25];
}antri;

void randomkartu()
{
     int jmlh,tampung=0;
     for(int i=0; i<35; i++)
     {
             antri.head=0;
             antri.tail++;
             do
             {
               jmlh=0;
               tampung=rand()%35;
               for(int j=0;j<i;j++)
               {                      
                       if(tampung==antri.kode[j])
                           jmlh++;
               }
             }while(jmlh!=0);
             if(jmlh==0)
             antri.kode[antri.tail]=tampung;
     }
}  
           
struct player
{
       arena *posisi;
       int putaran;
       char nama[50];
       int dadu;
       int urutan;
       int modal;
       int hak[20];
       int statusrutan;
       int statuspunyakartutahanan;
       int statusskip;
       int jumlahrumah;
}pemain[5];

int limassegitiga()
{
    int dadu[4]={1,2,3,4};
    return dadu[rand()%4];
}//random dadulimas

void tambah(int databaru,int urutan)
{
    baru = new arena;
    baru->kode = databaru;
    baru->urutan = urutan;
    baru->next = baru;
    baru->prev = baru;
    if(head == NULL)
    {
        head = tail = baru;
        head->next = head;
        head->prev = head;
        tail->next = tail;
    }
    else
    {   
        tail->next = baru;
        baru->prev = tail;
        tail = baru;
        tail -> next = head;
        head->prev = tail;   
    }
}

void cetak()
{
     bantu = head;
     for(int i=1;i<=20;i++)
     {
         if(bantu->kode==1)
         {
            strcpy(bantu->nama,"kotak start");
            bantu->punya=0;
         }
         else if(bantu->kode==6)
         {
            strcpy(bantu->nama,"masuk rumah tahanan");
            bantu->punya=0;
         }
         else if(bantu->kode==11)
         {
            strcpy(bantu->nama,"parkir bebas");
            bantu->punya=0;
         }
         else if(bantu->kode==16)
         {
            strcpy(bantu->nama,"Rumah Tahanan");
            bantu->punya=0;
         }
         if(bantu->kode>=2&&bantu->kode<=5)
         {
            strcpy(bantu->nama,"hitam ");
            bantu->punya=0;
            bantu->harga=1000;          
         }
         else if(bantu->kode>=7&&bantu->kode<=10)
         {
            strcpy(bantu->nama,"kuning");
            bantu->punya=0;
            bantu->harga=2000;
         }
         else if(bantu->kode>=12&&bantu->kode<=14)
         {
            strcpy(bantu->nama,"hijau ");
            bantu->punya=0;
            bantu->harga=4000;
         }
         else if(bantu->kode==15)
         {
            strcpy(bantu->nama,"merah ");
            bantu->punya=0;
            bantu->harga=10000;
         }
         else if(bantu->kode>=17&&bantu->kode<=18)
         {
            strcpy(bantu->nama,"KARTU ");
            bantu->punya=0;//kotak kartu
         }
         else if(bantu->kode>=19&&bantu->kode<=20)
         {
            strcpy(bantu->nama,"biru  ");
            bantu->punya=0;
            bantu->harga=7000;
         }
         bantu=bantu->next;
     }
} 
      
int cek(int databaru) //cek random warna kotak
{
    if(head == NULL)
         return 0;
    else 
    {
         int jmlh=0;
         bantu = head;
         if(bantu->kode==databaru)
                jmlh++;
         bantu=bantu->next;
         while( bantu != head)
         {
                if(bantu->kode==databaru)
                     jmlh++;
                bantu=bantu->next;
         }
         if(jmlh==1)
            return 1;
         else
            return 0;
     }
}

void urutan(int jmlh) //membuat urutan bermain
{
    player tamp;
    for(int i=0; i<jmlh-1; i++)
    {
            for(int j=i+1; j<jmlh; j++)
            {
                    if(pemain[i].dadu<pemain[j].dadu)
                    {
                        tamp=pemain[i];
                        pemain[i]=pemain[j];
                        pemain[j]=tamp;
                    }
            }
    }
    printf("\nUrutan Permainan : \n");
    for(int i=0; i<jmlh; i++)
    {
            pemain[i].urutan=i+1;
            printf("Pemain urutan ke-%i : %s\n", pemain[i].urutan, pemain[i].nama);
    }
}

void beli(int i) //membeli kotak
{
     char beli;
     if(pemain[i].putaran>=1) //bisa beli jika putaran sudah 1 kali
     {
           printf("apakah anda ingin membeli kotak seharga %i ? [y/n] : ",pemain[i].posisi->harga);
           fflush(stdin);
           scanf("%c", &beli);
           if(beli=='y')
           {
                 if(pemain[i].modal<pemain[i].posisi->harga) //jika uang tidak cukup
                        printf("uang tidak cukup\n");
                 else if(pemain[i].modal>=pemain[i].posisi->harga) //jika uang cukup
                 {
                        pemain[i].posisi->punya=pemain[i].urutan;
                        pemain[i].jumlahrumah++;
                        pemain[i].modal=pemain[i].modal-pemain[i].posisi->harga;
                        pemain[i].posisi->level=1;                   
                 }              
           }
           printf("kotak milik pemain %i\n", pemain[i].posisi->punya); //cek kotak setelah dibeli
     }
     else //jika putaran masih 0 belum bisa beli
           printf("masih putaran 0, tidak bisa beli\n");
}

void upgrade(int i)
{
     char upgrade;
     if(pemain[i].posisi->level==1)
     {
           printf("ingin upgrade? [y/n] : ");
           fflush(stdin);
           scanf("%c", &upgrade);
           if(upgrade=='y')
           {
                  if(pemain[i].modal>=pemain[i].posisi->harga*2) //jika uang cukup
                  {
                        pemain[i].posisi->harga=pemain[i].posisi->harga*2;
                        pemain[i].modal=pemain[i].modal-pemain[i].posisi->harga;
                        pemain[i].posisi->level=2;
                        printf("berhasil diupgrade ");
                        printf("harga menjadi %i\n", pemain[i].posisi->harga);
                  }
                  else if(pemain[i].modal<pemain[i].posisi->harga*2) //ujika uang tidak cukup
                        printf("uang tidak cukup\n");
           }
     }
     else //jika kotak sudah pernah diupgrade
           printf("kotak sudah level 2, tidak dapat upgrade lagi\n");
}

void denda(int i)
{
     int x=0;
     if(pemain[i].modal>=pemain[i].posisi->harga/2) //jika uang cukup
     {
         printf("kotak milik pemain %i anda terkena denda %i\n", pemain[i].posisi->punya, pemain[i].posisi->harga/2);
         pemain[i].modal=pemain[i].modal-(pemain[i].posisi->harga/2);
         x=pemain[i].posisi->punya;
         pemain[x-1].modal=pemain[x-1].modal+(pemain[i].posisi->harga/2);//x-1 krn punya diset mulai dari 1
     }
     else if(pemain[i].modal<pemain[i].posisi->harga/2) //jika uang tidak cukup
     {
          printf("kotak milik pemain %i anda terkena denda %i\n", pemain[i].posisi->punya, pemain[i].posisi->harga/2);
          printf("uang tidak cukup\n");
          bantu=head;
          for(int j=1; j<=20; j++)//menjual rumah
          {
                  if(bantu->punya==pemain[i].urutan)
                  {
                      printf("kotak %i %s milik anda harus dijual\n", bantu->urutan, bantu->nama);
                      pemain[i].modal=pemain[i].modal+bantu->harga;
                      bantu->punya=0;
                      pemain[i].jumlahrumah--;
                      printf("uang anda sekarang %i\n", pemain[i].modal);
                      printf("kotak %i %s lv %i sekarang jadi pemilik %i\n", bantu->urutan, bantu->nama, bantu->level, bantu->punya);//cek kotaak setelah dijual
                      if(pemain[i].modal>pemain[i].posisi->harga/2)break;
                  }
                  bantu=bantu->next;
          }    
          if(pemain[i].modal>pemain[i].posisi->harga/2)//jika uang cukupp setelah jual rumah
          {
              pemain[i].modal=pemain[i].modal-(pemain[i].posisi->harga/2);
              x=pemain[i].posisi->punya;
              pemain[x-1].modal=pemain[x-1].modal+(pemain[i].posisi->harga/2);
          }
          else if(pemain[i].modal<pemain[i].posisi->harga/2) //jika uang tidak cukupp setelah jual rumah(kalah)
          {
               printf("sudah tidak ada kotak untuk dijual, ANDA KALAH!!\n");
               for(int x=i; x<jmlh; x++) //menghapus pemain (kalah)
               {
                       if(i==jmlh-1) break;
                       pemain[x]=pemain[x+1];
                       pemain[x].urutan--;
               }
               jmlh--;
               bantu=head;
               for(int x=i;x<jmlh;x++)
               {
                       for(int j=0; j<20; j++)
                       {
                              if(bantu->punya>=pemain[x].urutan+1)
                              bantu->punya--;
                              bantu=bantu->next;       
                       }
               }
               min=1;
               for(int i=0; i<jmlh; i++)//cek pemain
                       printf("Pemain urutan ke-%i : %s\n", pemain[i].urutan, pemain[i].nama);
          }    
     }
}

void enqueuekartu(int input)//tumpuk kartu
{
     antri.head=0;
     antri.tail++;
     antri.kode[antri.tail]=input;
}
int dequeuekartu()//ambil kartu
{
     int tampung=antri.kode[antri.head];
     for(int i=antri.head;i<=antri.tail;i++)
         antri.kode[i]=antri.kode[i+1];
     antri.tail--;
     return tampung;
}

void jalan(int i);

void parkirbebas(int i)
{
     int tujuan;
     printf("silahkan pilih kode tempat dimana anda ingin pindah : \n");
     scanf("%i", &tujuan);
     do{
            pemain[i].posisi=pemain[i].posisi->next;
            if(pemain[i].posisi==head)
            {
                pemain[i].putaran++;
                pemain[i].modal=pemain[i].modal+1000;
            }
            if(pemain[i].posisi->urutan==tujuan) break;
        }while(true);
        printf("anda sekarang berada di kotak %i %s\n", pemain[i].posisi->urutan, pemain[i].posisi->nama);
        jalan(i);//cek kotak tujuan
}

void rumahtahanan(int i)
{
     pemain[i].statusskip=0;
     pemain[i].statusrutan=1;
     printf("anda masuk/pindah ke rumah tahanan\n");
     do{
           pemain[i].posisi=pemain[i].posisi->next;
           if(pemain[i].posisi->kode==16) break;
       }while(true);
     printf("%s sekarang berada di kotak %s kotak ke- %i \n", pemain[i].nama, pemain[i].posisi->nama, pemain[i].posisi->urutan);
     if(pemain[i].statusrutan==1)
     {
            int tanya=0,bayar=0;  
            if(pemain[i].statuspunyakartutahanan==1)
            {                                  
                    printf("mau pake kartu ? [masukan 1 untuk ya / 0 untuk no] = ");
                    scanf("%i",&tanya);
                    if(tanya==1)
                    {
                                pemain[i].statusrutan=0;
                                pemain[i].statuspunyakartutahanan=0;
                    }
            }
            if(pemain[i].modal>=2000&&tanya==0)
            {                                 
                    printf("mau bayar ga ? [masukan 1 untuk ya / 0 untuk no] = ");
                    scanf("%i",&bayar);
                    if(bayar==1)
                    {
                        pemain[i].modal=pemain[i].modal-2000;
                        pemain[i].statusrutan=0;
                    }
            }  
            if((tanya==0 && bayar==0)||(pemain[i].modal<2000 && pemain[i].statuspunyakartutahanan==0 && tanya==0 && bayar==0))
            {
                    printf("pemain menerima hukuman skip 1 giiran\n");
                    pemain[i].statusrutan=0;
                    pemain[i].statusskip=1;
            }
     }
}

void maju(int maju,int i)
{
      for(int j=0; j<maju; j++)
      {
              pemain[i].posisi=pemain[i].posisi->next;
              if(pemain[i].posisi==head)
              {
                   pemain[i].putaran++;
                   pemain[i].modal=pemain[i].modal+1000;
              }
      }
      printf("anda mendapat kartu %s\n", antri.nama);
      printf("%s sekarang berada di kotak %s kotak ke- %i \n", pemain[i].nama, pemain[i].posisi->nama, pemain[i].posisi->urutan);
      jalan(i);
}

void mundur(int mundur,int i)
{
      for(int j=0; j<mundur; j++)
      {
             pemain[i].posisi=pemain[i].posisi->prev;
             if(pemain[i].posisi==head&&pemain[i].putaran==0)break;
             else if(pemain[i].posisi==head&&pemain[i].putaran>0) pemain[i].putaran--;
      }
      printf("anda mendapat kartu %s\n", antri.nama);
      printf("%s sekarang berada di kotak %s kotak ke- %i \n", pemain[i].nama, pemain[i].posisi->nama, pemain[i].posisi->urutan);
      jalan(i);
}

void jualrumah( int i, int denda)
{
    bantu=head;
    for(int j=1; j<=20; j++)//menjual rumah
    {
            if(bantu->punya==pemain[i].urutan)
            {
                   printf("kotak %i %s milik anda harus dijual\n", bantu->urutan, bantu->nama);
                   pemain[i].modal=pemain[i].modal+bantu->harga;
                   bantu->punya=0;
                   pemain[i].jumlahrumah--;
                   printf("uang anda sekarang %i\n", pemain[i].modal);
                   printf("kotak %i %s lv %i sekarang jadi pemilik %i\n", bantu->urutan, bantu->nama, bantu->level, bantu->punya);//cek kotaak setelah dijual
                   if(pemain[i].modal>=denda)break;
            }
            bantu=bantu->next;
    }    
}     

void fungsikartu(int input,int i)//input=kartu,i=pemain ke
{
     if(input==0)
     {
            strcpy(antri.nama, "maju 1 langkah");
            maju(1,i);
     }
     else if(input==1)
     {
          strcpy(antri.nama, "maju 2 langkah");
          maju(2,i);
     }
     else if(input==2)
     {
          strcpy(antri.nama, "maju 3 langkah");
          maju(3,i);
     }
     else if (input==3)
     {
          strcpy(antri.nama, "maju 4 langkah");
          maju(4,i);
     }
     else if (input==4)
     {
          strcpy(antri.nama, "maju 5 langkah");
          maju(5,i);
     }
     else if(input==5)
     {
          strcpy(antri.nama, "mundur 1 langkah");
          mundur(1,i);
     }
     else if(input==6)
     {
          strcpy(antri.nama, "mundur 2 langkah");
          mundur(2,i);
     }
     else if(input==7)
     {
          strcpy(antri.nama, "mundur 3 langkah");
          mundur(3,i);
     }
     else if(input==8)
     {
          strcpy(antri.nama, "mundur 4 langkah");
          mundur(4,i);
     }
     else if(input==9)
     {
          strcpy(antri.nama, "mundur 5 langkah");
          mundur(5,i);
     }
     else if(input>=10&&input<=14)
     {
          strcpy(antri.nama, "hadiah 100");
          pemain[i].modal=pemain[i].modal+100;
     }
     else if(input>=15&&input<=17)
     {
          strcpy(antri.nama, "hadiah 500");
          pemain[i].modal=pemain[i].modal+500;
     }
     else if(input>=18&&input<=19)
     {
          strcpy(antri.nama, "hadiah 1000");
          pemain[i].modal=pemain[i].modal+1000;
     }
     else if(input>=20&&input<=24)
     {
          strcpy(antri.nama, "denda 100");
          printf("anda mendapat kartu %s\n", antri.nama);
          if(pemain[i].modal<100)jualrumah(i,100);
          pemain[i].modal=pemain[i].modal-100;
     }
     else if(input>=25&&input<=27)
     {
          strcpy(antri.nama, "denda 500");
          printf("anda mendapat kartu %s\n", antri.nama);
          if(pemain[i].modal<500)jualrumah(i,500);
          pemain[i].modal=pemain[i].modal-500;
     }
     else if(input>=28&&input<=29)
     {
          strcpy(antri.nama, "denda 1000");
          printf("anda mendapat kartu %s\n", antri.nama);
          if(pemain[i].modal<1000)jualrumah(i,1000);
          pemain[i].modal=pemain[i].modal-1000;
     }
     else if(input==30)
     {
          strcpy(antri.nama, "parkir bebas");
          printf("anda mendapat kartu %s\n", antri.nama);
          parkirbebas(i); 
     }
     else if(input==31)
     {
          strcpy(antri.nama, "bebas rumah tahanan");
          pemain[i].statuspunyakartutahanan=1;
     }
     else if(input==32)
     {
          strcpy(antri.nama, "masuk rumah tahanan");
          printf("anda mendapat kartu %s\n", antri.nama);
          rumahtahanan(i);
     }
     else if(input==33)
     {
          strcpy(antri.nama, "membayar 100 ke semua pemain");
          printf("anda mendapat kartu %s\n", antri.nama);
          if(pemain[i].modal<jmlh*100)jualrumah(i, jmlh*100);
          for(int j=0;j<jmlh;j++)
          {
                 if(j!=i)pemain[j].modal=pemain[j].modal+100;
          }
          pemain[i].modal=pemain[i].modal-(100*(jmlh-1));
     }
     else if(input==34)
     {
          strcpy(antri.nama, "dibayar 100 dari semua pemain");
          for(int j=0;j<jmlh;j++)
          {
                 if(j!=i)pemain[j].modal=pemain[j].modal-100;
          }
          pemain[i].modal=pemain[i].modal+(100*(jmlh-1));
     }
     if(input>=10 && input<=19 || input==31 || input ==34)
          printf("anda mendapat kartu %s\n", antri.nama);      
}

void cetakkartu()
{
     for(int i=1; i<=antri.tail+1; i++)
     {
         if(i%10==0&&i!=0)  
             printf("%i\n", antri.kode[i-1]); 
         else
             printf("%i\t", antri.kode[i-1]); 
     }
     printf("\n\n\n");
}

void jalan(int i)
{
            if(pemain[i].posisi->kode==17 || pemain[i].posisi->kode==18) //kotak kartu
            {
                 int tampung=dequeuekartu();//ambil kartu
                 fungsikartu(tampung,i);//fungsikartu
                 enqueuekartu(tampung);//tumpuk kartu
                 printf("\n");
            }
            else if(pemain[i].posisi->kode==6) //kotak masuk rutan
                 rumahtahanan(i);
            else if(pemain[i].posisi->kode==11) //kotak bebas parkir
                 parkirbebas(i);
            else if(pemain[i].posisi->kode==16 || pemain[i].posisi->kode==1) //kotak start & rutan
                 printf("anda berada di %s\n", pemain[i].posisi->nama);
            else //kotak yg bisa dibeli
            {
                if(pemain[i].posisi->punya==0)//kotak tidak ada yg punya
                     beli(i);
                else if(pemain[i].posisi->punya==pemain[i].urutan)//kotak punya sendiri
                     upgrade(i);
                else if(pemain[i].posisi->punya!=0 && pemain[i].posisi->punya!=pemain[i].urutan)//kotak milik musuh
                {     
                     denda(i);
                }
            }
}

void cover()
{
     printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t");
     for(int i=1;i<=61;i++)
     {
             printf("=");
             Sleep(10);
     }
     printf("\n\n\t\t\t     STRUKTUR DATA GRUP D\n");
     printf("\n\n\t\t\t       'GAME MONOPOYI'   \n");
     printf("\n\n\t\t\t             OLEH :      \n\n\n\t\t        ");
     char name1[68]={'J','E','F','F','R','I',' ','C','H','A','N','D','R','A',' ','P','R','E','K','A','S','A',' ','/',' ','7','1','1','1','0','0','1','7','S','T','E','V','E','N',' ','R','E','N','A','L','D','O',' ','A','N','T','O','N','Y',' ',' ','/',' ','7','1','1','1','0','0','5','4'};
     for(int i=0;i<68;i++)
     {
           if(i<32)printf("%c",name1[i]);
           else if(i==32)printf("%c\n\n\n\t\t        ",name1[i]);
           else printf("%c",name1[i]);
           Sleep(40);
     }printf("\n\t");
     for(int i=1;i<=61;i++)
     {
             printf("=");
             Sleep(10);
     }printf("\n\n\n\n\n\n\n\n\t\t\t         ");
     char load[10]={'L','O','A','D','I','N','G','.','.','.'};
     for(int i=0;i<3;i++)
     {
             for(int j=0;j<10;j++)
             {
                     printf("%c ",load[j]);//ada spasi
                     Sleep(200);
             }
             printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");//21 bzckspace krn ada spasi
     }system("cls");
}

void printarena()
{
     printf("\n\n\t========================GAME MONOPOYI========================\n\n\n");  
     printf("\t*************************************************************\n");
     printf("\t*         *         *         *         *         *         *\n");
     printf("\t*  parkir *         *         *         *         *  rumah  *\n");
     bantu = head;
     while(bantu->urutan!=12)bantu=bantu->next;
     printf("\t*  bebas  *  %.6s *  %.6s *  %.6s *  %.6s * tahanan *\n",bantu->nama,bantu->next->nama,bantu->next->next->nama,bantu->next->next->next->nama);
     printf("\t*         *         *         *         *         *         *\n");
     printf("\t*      11 *      12 *      13 *      14 *      15 *      16 *\n");
     printf("\t*************************************************************\n");
     bantu = head;bantu2 =head;
     while(bantu->urutan!=10)bantu=bantu->next;
     while(bantu2->urutan!=17)bantu2=bantu2->next;
     printf("\t*         *                                       *         *\n");
     printf("\t*         *                                       *         *\n");
     printf("\t*  %.6s *                                       *  %.6s *\n",bantu->nama,bantu2->nama);
     printf("\t*         *                                       *         *\n");
     printf("\t*      10 *                                       *      17 *\n");
     bantu = head;bantu2 =head;
     while(bantu->urutan!=9)bantu=bantu->next;
     while(bantu2->urutan!=18)bantu2=bantu2->next;
     printf("\t***********                                       ***********\n");
     printf("\t*         *                                       *         *\n");
     printf("\t*         *                                       *         *\n");
     printf("\t*  %.6s *            ***************            *  %.6s *\n",bantu->nama,bantu2->nama);
     printf("\t*         *            *             *            *         *\n");
     printf("\t*       9 *            *             *            *      18 *\n");
     bantu = head;bantu2 =head;
     while(bantu->urutan!=8)bantu=bantu->next;
     while(bantu2->urutan!=19)bantu2=bantu2->next;
     printf("\t***********            *    KARTU    *            ***********\n");
     printf("\t*         *            *             *            *         *\n");
     printf("\t*         *            *             *            *         *\n");
     printf("\t*  %.6s *            ***************            *  %.6s *\n",bantu->nama,bantu2->nama);
     printf("\t*         *                                       *         *\n");
     printf("\t*       8 *                                       *      19 *\n");
     bantu = head;bantu2 =head;
     while(bantu->urutan!=7)bantu=bantu->next;
     while(bantu2->urutan!=20)bantu2=bantu2->next;
     printf("\t***********                                       ***********\n");
     printf("\t*         *                                       *         *\n");
     printf("\t*         *                                       *         *\n");
     printf("\t*  %.6s *                                       *  %.6s *\n",bantu->nama,bantu2->nama);
     printf("\t*         *                                       *         *\n");
     printf("\t*       7 *                                       *      20 *\n");
     printf("\t*************************************************************\n");
     printf("\t*         *         *         *         *         *         *\n");
     printf("\t*  masuk  *         *         *         *         *  kotak  *\n");
     bantu = head;
     while(bantu->urutan!=5)bantu=bantu->next;
     printf("\t*  rumah  *  %.6s *  %.6s *  %.6s *  %.6s *  start  *\n",bantu->nama,bantu->prev->nama,bantu->prev->prev->nama,bantu->prev->prev->prev->nama);
     printf("\t* tahanan *         *         *         *         *         *\n");
     printf("\t*       6 *       5 *       4 *       3 *       2 *       1 *\n");
     printf("\t*************************************************************\n");
}

int main()
{
    system("color f0");
    srand(time(NULL));
    head=NULL;
    min=0;
    int counter,tampung=0,dadu1,dadu2;
    int kotak[16]={2,3,4,5,7,8,9,10,12,13,14,15,17,18,19,20};
    antri.tail=-1;
    randomkartu();
    for(int i=0;i<20;i++)//randome urutan kotak pada arena
    {
        if(i==0||i==5||i==10||i==15)
            tampung=i+1;
        else
        {
            do
            {
                tampung=kotak[rand()%16];
                counter=cek(tampung);
            }while(counter==1);
        }
        tambah(tampung,i+1);
    }
    cetak();
    cover();
    do{
        printarena();
        printf("\n\n\nminimal pemain 3 maksimal pemain 5\n");
        printf("Ada berapa pemain ?");
        scanf("%d",&jmlh);
        if(jmlh>=3&&jmlh<=5) //error handling jumlah pemain min3 max 5
           break;
        else
        {
            printf("\n\n\nminimal pemain 3 maksimal pemain 5\n");
            getch();
            system("cls");
        }
    }while(true);
    printf("\n");
    for(int i=0;i<jmlh;i++) //masukkin data pemain sejumlah yg diminta
    {
            printf("Pemain ke - %d\n",i+1);
            printf("Masukan nama : ");
            fflush(stdin);
            gets(pemain[i].nama); 
            pemain[i].dadu=limassegitiga()+limassegitiga();
            printf("kocokan dadu : %d\n\n",pemain[i].dadu);
            pemain[i].modal=2000;
            //pemain[i].modal=49000;//cheat menang
            pemain[i].putaran=0;
            pemain[i].posisi=head;
            pemain[i].statuspunyakartutahanan=0;
            pemain[i].statusrutan=0;
            pemain[i].statusskip=0;
            pemain[i].jumlahrumah=0;
    }
    urutan(jmlh);
    system("pause");
    system("cls");
    do{
        int a,win=0,rumah;      
        for(int i=0; i<jmlh; i++)
        {       
                a=i;
                printarena();
                printf("\n\n\n");
                //printf("urutan kartu dari head - tail : \n");cetakkartu();
                 /*bantu=head;
                 printf("\n\n");
                 printf("%i\t %s\t\t\t",bantu->punya, bantu->nama);
                 if(bantu->kode==1||bantu->kode==6||bantu->kode==11||bantu->kode==16||bantu->kode==17||bantu->kode==18)
                 printf("\n");
                 else
                 printf(" %i\n",bantu->harga);
                 bantu = bantu->next;
                 while(bantu != head)
                 {
                     printf("%i\t %s\t\t\t",bantu->punya, bantu->nama);
                     if(bantu->kode==1||bantu->kode==6||bantu->kode==11||bantu->kode==16||bantu->kode==17||bantu->kode==18)
                     printf("\n");
                     else
                     printf("%i\n",bantu->harga);
                     bantu = bantu->next; 
                 }*/ 
                printf("urutan\t nama\t\t uang\t jmlh rumah    putaran\t posisi\t punya kartu tahanan\n");
                printf("------------------------------------------------------------------------------------\n"); 
                for(int x=0; x<jmlh; x++)
                        printf("   %i\t %s\t\t %i\t    %i\t\t %i\t   %i\t        %i\n", pemain[x].urutan, pemain[x].nama, pemain[x].modal, pemain[x].jumlahrumah, pemain[x].putaran, pemain[x].posisi->urutan,pemain[x].statuspunyakartutahanan);
                if(pemain[i].statusskip==1)
                {
                     printf("\nPemain ke -%i masih di dalam rumah tahanan [skip 1 giliran]\n", pemain[i].urutan);
                     pemain[i].statusskip=0;
                     system("pause");
                     system("cls");
                     continue;
                }
                do{ //giliran pemain ekstra
                       dadu1=limassegitiga();
                       dadu2=limassegitiga();
                       if(dadu1==dadu2)pemain[i].dadu=pemain[i].dadu+(dadu1+dadu2);
                       else pemain[i].dadu=dadu1+dadu2;
                }while(dadu1==dadu2);
                printf("\ndadu1 = %i,dadu2 = %i\n",dadu1,dadu2);
                printf("\nkocokan dadu %s = %i\n", pemain[i].nama, pemain[i].dadu);
                for(int j=0; j<pemain[i].dadu; j++)
                {
                       pemain[i].posisi=pemain[i].posisi->next;
                       if(pemain[i].posisi==head)
                       {
                           pemain[i].putaran++;
                           pemain[i].modal=pemain[i].modal+1000;
                           //pemain[i].modal=pemain[i].modal-2100;//cheat kalah
                       }
                }
                printf("%s sekarang berada di kotak %s kotak ke- %i \n", pemain[i].nama, pemain[i].posisi->nama, pemain[i].posisi->urutan);
                jalan(i);
                if(min==1)//jika ada pemain di kick
                {
                    if(jmlh>1)
                    {
                        i--;
                        min=0;         
                    }
                }
                system("pause");
                system("cls");
                if(pemain[i].modal<0)
                {
                    printarena();
                    printf("\n\n\npemain ke %i bernama %s KALAH karena uang <0!!\n",pemain[i].urutan, pemain[i].nama);
                    for(int x=i; x<jmlh; x++) //menghapus pemain (kalah)
                    {
                           if(i==jmlh-1) break;
                           pemain[x]=pemain[x+1];
                           pemain[x].urutan--;
                    }
                    jmlh--;
                    bantu=head;
                    for(int x=i;x<jmlh;x++)
                    {
                       for(int j=0; j<20; j++)
                       {
                              if(bantu->punya>=pemain[x].urutan+1)
                              bantu->punya--;
                              bantu=bantu->next;       
                       }
                    }
                    for(int q=0; q<jmlh; q++)//cek pemain
                               printf("Pemain urutan ke-%i : %s\n", q+1, pemain[q].nama);
                    getch();system("cls");
                    if(jmlh>1)
                    i--;
                }
                if(jmlh==1 || pemain[i].modal>=50000) 
                {
                    if(jmlh==1)i=0;
                    printarena();
                    printf("\n\n\npemain urutan %i bernama %s menang.\n\nPERMAINAN BERAKHIR !!", pemain[i].urutan, pemain[i].nama);
                    win=1;
                    break;
                }     
        }   
        if(win==1)break;    
    }while(true);
    getch();
    return 0;
}
