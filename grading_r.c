#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_TESTS 10

char *tests[] = {"ADD x4, x14, x19\n","SUBS x18, x15, x11\n","AND x0, x29, x29\n","EOR x10, x10, x1\n","ADD x0, x0, xzr\n","RORV x4, x20, x0\n","ADDS x4, x14, x19\n","ORR x5, x30, x23\n","ASRV x5, x5, x30\n","LSLV x4, x2, x1\n"};
char *answers[] = {"0x8b1301c4\n","0xeb0b01f2\n","0x8a1d03a0\n","0xca01014a\n","0x8b1f0000\n","0x9ac02e84\n","0xab1301c4\n","0xaa1703c5\n","0x9ade28a5\n","0x9ac12044\n"};

char* ignore_lines(char *buffer, int lines){
   if(lines < 0){
      return NULL;
   }
   while(1){
      char c = *(buffer++);
      if(c == '\n'){
         lines--;
      }
      if(lines <= 0){
         break;
      }
   }
   return buffer;
}

void writeTest(){
   FILE *f = fopen("grading_r.txt","w");
   int count;
   for(count = 0; count < NUM_TESTS; count++){
      fprintf(f,"%s",tests[count]);
   }
   fclose(f);
}

char* get_line(char *buffer, char **line){
   char c;
   int i = 0;
   while(1){
      c = *(buffer++);
      if(c == '\n' || c == 0){
         line[0][i++] = '\n';
         line[0][i] = 0;
         return buffer;
      }
      line[0][i++] = c;
   }
   return buffer;
}

int main(){
   char *buffer = (char*)malloc(4000*sizeof(char));
   writeTest();
   FILE *f = popen("./ensambladorV2 grading_r.txt","r");
   int i = 0;
   while(1){
      char c = fgetc(f);
      if(feof(f)){
         break;
      }
      buffer[i++]=c;
   }
   char *aux_buffer = ignore_lines(buffer,15);
   char *line = (char*)malloc(100*sizeof(char));

   i = 0;
   float total = 0.0;
   for(i = 0; i < NUM_TESTS;i++){
      aux_buffer = get_line(aux_buffer,&line);
      printf(".............................\nInst: %sObtenido: %sEsperado: %s",tests[i],line,answers[i]);
      if(!strcmp(line,answers[i])){
	  total+=100.0/NUM_TESTS;
          printf(" (+%.2f)\n",100.0/NUM_TESTS);
      }else{
         puts(" (+0.00)");
      }
   }
   printf(".............................\nTotal: %.2f/100.0\n",total);
   free(line);
   free(buffer);
   return 0;
}

